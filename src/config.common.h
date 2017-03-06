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
 * common configurations
 *
 */

#ifndef __INC_CONFIG_COMMON_H__
#define __INC_CONFIG_COMMON_H__

#include	<time.h>

#define NTP_SERVER						"ntp.nict.jp"

#define	PIN_SDA							2
#define	PIN_SCL							14
#define	PIN_1WIRE						4

//#define	POWER_SAVE_NONE
#define	USE_POWER_SAVE_DEEPSLEEP
#define	DEEPSLEEP_TIMEOUT				20
#define	EALIER_TIME						2
#define	SLEEP_TIMER_SKEW				5
#define	WIFI_SLEEP_TIMEOUT				60		//	1min
#define	WIFI_DISCONNECT_TIMEOUT			30
#define	CONNECTION_TIMEOUT				30

#define	SIZE_OF_SSID					32
#define	SIZE_OF_UUID					36
#define	SIZE_OF_IP						15
#define	SIZE_OF_PASSPHRASE				63
#define	SIZE_OF_HOST					255
#define	SIZE_OF_URL						255
#define	SERVER_TIMEOUT					30
#define	NUMBER_OF_SENSORS				10

#define	SEND_INTERVAL					180
#define	SENSE_INTERVAL					60

#define	NVRAM_SIZE						256
#define	SIZE_OF_MESSAGE_BUFF			160
#define	SIZE_OF_SENSOR_BUFF				(NVRAM_SIZE - ( \
	sizeof(uint32_t) + \
	sizeof(uint8_t *) + \
	sizeof(time_t) * 3 + \
	sizeof(bool)))

#define	memclear(v,s)					memset((v),0,(s))

#endif // __CONFIG_COMMON_H__

