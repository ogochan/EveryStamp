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
 * Wi-Fi functions
 *
 */

#define	TRACE
#include	"config.h"
#include	<string.h>
#include	"debug.h"
#include	"os_wifi.h"
#include	"os_ntp.h"
#include	"os_time.h"
#include	"Power.h"

#ifdef	USE_ARDUINO
#include <ESP8266WiFi.h>
#include <ESP8266WiFiAP.h>
#include <ESP8266WiFiGeneric.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WiFiScan.h>
#include <ESP8266WiFiSTA.h>
#include <ESP8266WiFiType.h>
#include <WiFiClient.h>
#include <WiFiClientSecure.h>
#include <WiFiServer.h>
#include <WiFiUdp.h>
#include <Time.h>
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include	<time.h>
#include	<espressif/esp_common.h>
#include	<espressif/user_interface.h>
#define	now()	(sdk_system_get_time() / 1000)
#endif

extern	bool
WLAN_check_connection(void)
{
	bool	ret;

#ifdef	USE_ARDUINO
	ret = WiFi.status() == WL_CONNECTED ? true : false;
#endif
#ifdef	USE_ESP_OPEN_RTOS
    ret = ( sdk_wifi_station_get_connect_status() == STATION_GOT_IP ) ? true : false;
#endif
	return	(ret);
}

extern	bool
WLAN_wait_connect(void)
{
	int		i;
	bool	ret;

	dbgmsg("now waiting");

ENTER_FUNC;
	if	( !WLAN_check_connection() )	{
		i = 0;
		while	(	( i < CONNECTION_TIMEOUT )
				&&	( !WLAN_check_connection() ) )	{
			sleep_ms(1000);
			i ++;
			dbgprintf("wait %d",i);
		}
		if	( i == CONNECTION_TIMEOUT )	{
			ret = false;
		} else {
#ifdef	USE_ARDUINO
			Serial.println("connected!");
			WiFi.printDiag(Serial);
			Serial.print("IPAddress: ");
			Serial.println(WiFi.localIP());
#endif
#ifdef	USE_ESP_OPEN_RTOS
			struct ip_info ip;
			sdk_wifi_get_ip_info(STATION_IF, &ip);
#endif
			ret = true;
		}
	} else {
		ret = true;
	}
LEAVE_FUNC;
	return	(ret);
}

extern	void
WLAN_monitor(void)
{
ENTER_FUNC;
	dbgmsg( Power::rf_on ? "RF ON" : "RF OFF");
	if	( !Power::rf_on )	{
		Power::rf_on = true;
		Power::save_ram(now() + EALIER_TIME - SLEEP_TIMER_SKEW);
#ifdef	USE_ARDUINO
		ESP.deepSleep(1, WAKE_RF_DEFAULT);
#endif
		sleep_ms(1000);
	}
#ifdef	USE_ARDUINO
	DEBUG_PRINTLN(ESP.getResetReason());
	WiFi.mode(WIFI_STA);
#endif
#ifdef	USE_ESP_OPEN_RTOS
    sdk_wifi_set_opmode(STATION_MODE);
#endif
LEAVE_FUNC;
}

extern	void
WLAN_connect_wifi(void)
{
#ifdef	USE_ESP_OPEN_RTOS
    struct sdk_station_config config;

	strcpy((char *)config.ssid, MY_SSID);
	strcpy((char *)config.password, MY_PASS);
#endif
ENTER_FUNC;
	dbgmsg("Client");
	if	( !WLAN_check_connection() )	{
		dbgmsg("disconnected");
#ifdef	USE_ARDUINO
		WiFi.begin(MY_SSID, MY_PASS);
#endif
#ifdef	USE_ESP_OPEN_RTOS
		sdk_wifi_station_set_config(&config);
		sdk_wifi_station_connect();
        //sdk_wifi_station_dhcpc_start();
#endif
	} else {
		dbgmsg("connected");
	}
LEAVE_FUNC;
}
extern	void
WLAN_disconnect_wifi(void)
{
	if	( WLAN_check_connection() )	{
#ifdef	USE_ARDUINO
		WiFi.mode(WIFI_OFF);
#endif
#ifdef	USE_ESP_OPEN_RTOS
		sdk_wifi_set_opmode(STATION_MODE);
#endif
	}
}

