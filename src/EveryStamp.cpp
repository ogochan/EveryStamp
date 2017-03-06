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
 * entry point
 *
 */

#define	TRACE

#include	"config.h"
#include	"debug.h"
#include	"utility.h"

#ifdef	USE_ARDUINO
#include	<Time.h>
#include	<TimeLib.h>
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include	"espressif/esp_common.h"
#include	"FreeRTOS.h"
#include	"task.h"
#include	"esp8266.h"
#include	"Time.h"
#include	"ESP.h"
EspClass ESP;
#endif
#include 	"os_wifi.h"
#include 	"os_ntp.h"
#include	"SenseBuffer.h"
#include	"SensorInfo.h"
#include	"Sensors.h"
#include	"GPIO.h"
#include	"os_time.h"
#include	"Power.h"
#include	"net.h"

extern	SenseBuffer	sense_buff;

#define ROUND_UP(val, unit)	(( ( (val) / (unit) ) + 1 ) * (unit))
#define	NEXT_SEND(now)		ROUND_UP(now, (time_t)SEND_INTERVAL)
#define	NEXT_SENSE(now)		ROUND_UP(now, (time_t)SENSE_INTERVAL)


#ifdef	USE_ARDUINO
extern	void
setup(void)
{
	time_t	n;

	GPIO_init();

	sleep_ms(1000);
	if	(( !Power::load_ram() ) ||
		 ( year(Power::next_send) < 2010 ))	{
		Power::clear_ram();
		dbgmsg("RAM clean");
		WLAN_monitor();
		NTP_start();
		sense_buff.rewind_write();
#if	0
		if	( Serial.available() > 0 )	{
			Serial.print(">");
			String	str = Serial.readStringUntil('\r');
			Serial.println();
		}
#endif
	}
	n = now();
	OS_show_time(n);

	Sensors::init();
	if	( year(Power::next_send) < 2010 )	{
		Power::next_send = NEXT_SEND(n);
		Power::next_sense = NEXT_SENSE(n);
	}
	Power::control(true);
}

void loop() {
	time_t	n
		,	sleep_sec
		,	next;

	n = now();
	OS_show_time(n);
	if	( n >= Power::next_sense )	{
		Sensors::collect(n, sense_buff);
		Power::next_sense = NEXT_SENSE(now());
	}
	if ( n >= Power::next_send )	{
		send_server();
		Power::next_send = NEXT_SEND(now());
	}
	next = ( Power::next_send > Power::next_sense ) ? Power::next_sense : Power::next_send;
	sleep_sec = next - now();
	dbgmsg("---");
	OS_show_time(Power::next_send);
	OS_show_time(Power::next_sense);
	Power::sleep(next, sleep_sec);
}
#endif
#ifdef	USE_ESP_OPEN_RTOS
void
main_task(
	void	*para)
{
	time_t	n
		,	sleep_sec
		,	next;

	if	(( !Power::load_ram() ) ||
		 ( year(Power::next_send) < 2010 ))	{
		dbgprintf("next_send year = %d", year(Power::next_send));
		Power::clear_ram();
		dbgmsg("RAM clean");
		WLAN_monitor();
		WLAN_wait_connect();
		NTP_start();
		sense_buff.rewind_write();
	}
	while	(true)	{
		dbgprintf("next_send year = %d", year(Power::next_send));
		n = now();
		OS_show_time(n);

		Sensors::init();
		if	( year(Power::next_send) < 2010 )	{
			Power::next_send = NEXT_SEND(n);
			Power::next_sense = NEXT_SENSE(n);
		}
		Power::control(true);
		if	( n >= Power::next_sense )	{
			Sensors::collect(n, sense_buff);
			Power::next_sense = NEXT_SENSE(now());
		}
		if ( n >= Power::next_send )	{
			send_server();
			Power::next_send = NEXT_SEND(now());
		}
		next = ( Power::next_send > Power::next_sense ) ? Power::next_sense : Power::next_send;
		sleep_sec = next - now();
		if	( sleep_sec < 0 )	{
			sleep_sec = 10;
		}
		dbgmsg("---");
		OS_show_time("next send ", Power::next_send);
		OS_show_time("next sense", Power::next_sense);
		Power::sleep(next, sleep_sec);
	}
}
extern "C" void
user_init(void)
{
	GPIO_init();
	sleep_ms(1000);
	start_time();
    xTaskCreate(main_task, "main", 2048, NULL, 1, NULL);
}
#endif
