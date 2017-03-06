/*
 *  EveryStamp for ESP8266 - a sensor data collector
 *
 *  Copyright (C) 2017  Masami Ogoshi (ogochan@wasp.co.jp)
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 *
 * This code is made available on the understanding that it will not be
 * used in safety-critical situations without a full and competent review.
 *
 * Power control class
 *
 */

#include	"config.h"

#define	DEBUG
#define	TRACE

#ifdef	USE_ARDUINO
#include	<Arduino.h>
extern	"C"	{
#include	<user_interface.h>
};
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include	<espressif/esp_common.h>
#include	"Time.h"
#define	system_rtc_mem_write	sdk_system_rtc_mem_write
#define	system_rtc_mem_read		sdk_system_rtc_mem_read
#include	"ESP.h"
#endif

#include	"os_wifi.h"
#include	"os_time.h"
#include	"Power.h"
#include	"SenseBuffer.h"
#include	"Sensors.h"
#include	"GPIO.h"
#include	"debug.h"

extern	SenseBuffer	sense_buff;
SensorInfo	Sensors[NUMBER_OF_SENSORS];
int	nSensors;

static	bool	fPowerControl = false;
static	bool	fWiFi_ON = false;
static	bool	fRestore = false;

time_t	Power::next_send;
time_t	Power::next_sense;
bool	Power::rf_on;

/*
  save area map

  0-63		sytem data
  64-127	gap
  128-195	sensor data save
*/
static	const	uint32_t	NVRAM_SAVE_AREA = 128;

static	void
UI_reboot(
	char	*name)
{
	dbgmsg("rebooting...");
	ESP.restart();
}

void
Power::wifi_off(void)
{
	int		wifi_timeout = WIFI_DISCONNECT_TIMEOUT;

ENTER_FUNC;
	if		( fPowerControl )	{
		if	( fWiFi_ON )	{
			if		( WLAN_check_connection() )	{
				dbgmsg("WiFi off");
				WLAN_disconnect_wifi();
				while	( WLAN_check_connection() )	{ 
					dbgmsg("sleep 1000ms");
					sleep_ms(1000);
					wifi_timeout -= 1;
					if		(  wifi_timeout == 0 )	{
						UI_reboot(NULL);
					}
				}
			}
			fWiFi_ON = false;
		}
	} else {
		dbgmsg("not power controlled");
	}
LEAVE_FUNC;
}

void
Power::wifi_on(void)
{
	if		( fPowerControl )	{
		if	( !fWiFi_ON )	{
			dbgmsg("WiFi on");
			WLAN_monitor();
			fWiFi_ON = true;
		}
	}
}

void
Power::clear_ram(void)
{
	uint32_t	addr
		,		zero;
	int			i;

	dbgmsg("POWER_clear_ram");
	zero = 0;
	for	( addr = NVRAM_SAVE_AREA, i = 0 ; i < SIZE_OF_SENSOR_BUFF ; addr ++, i += 4 )	{
		system_rtc_mem_write(addr, &zero, sizeof(uint32_t));
	}
}

static	uint32_t
fnv1_hash32(
	uint8_t	*bytes,
	size_t	length)
{
	size_t	i;
	static	const	uint32_t	FNV_OFFSET_BASIS_32 = 2166136261U;
	static	const	uint32_t	FNV_PRIME_32 = 16777619U;
	uint32_t hash = FNV_OFFSET_BASIS_32;;

	for	( i = 0 ; i < length ; i ++ )	{
		hash = (FNV_PRIME_32 * hash) ^ (bytes[i]);
	}
	return hash;
}

#define	SAVE_SIZE		(NVRAM_SIZE - sizeof(uint32_t))
void
Power::save_ram(
	time_t	next)
{
	uint32_t	sum;
	uint32_t	pointer;
	uint8_t		buff[SAVE_SIZE]
		,		*p;

	dbgmsg("save_ram");
	pointer = (uint32_t)sense_buff.pointer();
	next -= EALIER_TIME + SLEEP_TIMER_SKEW;

	p = buff;
	memclear(buff, SAVE_SIZE);
	memcpy(p, &pointer, sizeof(uint8_t *));			p += sizeof(uint8_t *);
	memcpy(p, &next, sizeof(time_t));				p += sizeof(time_t);
	memcpy(p, &Power::next_send, sizeof(time_t));	p += sizeof(time_t);
	memcpy(p, &Power::next_sense, sizeof(time_t));	p += sizeof(time_t);
	memcpy(p, &Power::rf_on, sizeof(bool));			p += sizeof(bool);
	memcpy(p, sense_buff.buff(), SAVE_SIZE - ( p - buff));

	sum = fnv1_hash32(buff , SAVE_SIZE);
	dbgprintf("sum = %X:%X", sum, pointer);
	system_rtc_mem_write(NVRAM_SAVE_AREA + 0, &sum, sizeof(uint32_t));
	system_rtc_mem_write(NVRAM_SAVE_AREA + 1, buff, SAVE_SIZE);
}

bool
Power::load_ram(void)
{
	uint32_t	sum
		,		hsum;
	uint32_t	pointer;
	time_t		next;
	uint8_t		buff[SAVE_SIZE]
		,		*p;

ENTER_FUNC;
	memclear(buff, SAVE_SIZE);
	system_rtc_mem_read(NVRAM_SAVE_AREA + 0, &sum, sizeof(uint32_t));
	system_rtc_mem_read(NVRAM_SAVE_AREA + 1, buff, SAVE_SIZE);
	Power::clear_ram();

	p = buff;
	hsum = fnv1_hash32(p , SAVE_SIZE);
	if	( hsum == sum )	{
		memcpy(&pointer, p, sizeof(uint8_t *));			p += sizeof(uint8_t *);
		memcpy(&next, p, sizeof(time_t));				p += sizeof(time_t);
		memcpy(&Power::next_send, p, sizeof(time_t));	p += sizeof(time_t);
		memcpy(&Power::next_sense, p, sizeof(time_t));	p += sizeof(time_t);
		memcpy(&Power::rf_on, p, sizeof(bool));			p += sizeof(bool);
		memcpy(sense_buff.buff(), p, SAVE_SIZE - (p - buff));
		sense_buff.set_pointer(pointer);
		setTime(next);
	} else {
		Power::next_send = 0;
		Power::next_sense = 0;
		Power::rf_on = false;
	}
LEAVE_FUNC;
	return	(hsum == sum);
}

bool
Power::restore_status(void)
{
	return	(fRestore);
}

void
Power::sleep(
	time_t	next,
	int		sec)
{
	OS_show_time("wakeup", next);
	dbgprintf("sleep %d sec", sec);
#ifdef	POWER_SAVE_NONE
	sleep(sec);
#else
	if		( fPowerControl )	{
		if	( sec >= WIFI_SLEEP_TIMEOUT )	{
			Power::wifi_off();
		}
		OS_show_time(now());
#ifdef	USE_POWER_SAVE_DEEPSLEEP
		if	( sec >= DEEPSLEEP_TIMEOUT )	{
			Power::rf_on = false;
			if	( sec > 3600 )	{
				Power::clear_ram();
				sec = DEEPSLEEP_TIMEOUT;
			} else {
				save_ram(next);
			}
			Sensors::stop(sec);
			sec -= EALIER_TIME + SLEEP_TIMER_SKEW;
			dbgmsg("deep sleep");
			ESP.deepSleep(sec * 1000 * 1000, WAKE_RF_DISABLED);
			sleep_ms(1000);
		} else
#endif
		{
			dbgmsg("Enter sleep");
			sleep_ms(sec*1000);
			dbgmsg("Leave sleep");
		}
	} else {
		sleep_ms(sec*1000);
	}
#endif
}

void
Power::control(
	bool	power)
{
	fPowerControl = power;
	if	( fPowerControl )	{
#ifndef	POWER_SAVE_NONE
#endif
	}
	fWiFi_ON = false;
}
