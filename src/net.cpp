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
 * server communication
 *
 */

#define	TRACE

#include	"config.h"
#include	"utility.h"

#ifdef	USE_ARDUINO
#include 	<ESP8266WiFi.h>
#include	<Time.h>
#include	<TimeLib.h>
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include	<stdio.h>
#include	<espressif/esp_common.h>
#include	<lwip/err.h>
#include	<lwip/sockets.h>
#include	<lwip/sys.h>
#include	<lwip/netdb.h>
#include	<lwip/dns.h>
#include	"Time.h"
#endif
#include 	"os_wifi.h"
#include 	"os_ntp.h"
#include	"SenseBuffer.h"
#include	"SensorInfo.h"
#include	"Sensors.h"
#include	"os_time.h"
#include	"net.h"
#include	"debug.h"

SenseBuffer	sense_buff;
static	char	message_buff[SIZE_OF_MESSAGE_BUFF];
static	char	*message_pointer;

static	void
make_one_sensor_message(
	time_t	n,
	SensorInfo	*info,
	SenseBuffer	buff)
{
	char	*q = message_pointer;
ENTER_FUNC;
	message_pointer += sprintf(message_pointer, "{\"sensor_name\":\"%s\",", info->name());
	message_pointer += sprintf(message_pointer, "\"data_class_name\":\"%s\",", info->data_class_name());
	message_pointer += sprintf(message_pointer, "\"data\":{");
	message_pointer += sprintf(message_pointer, "\"at\":\"%d-%d-%d %02d:%02d:%02d +0000\",",
							   year(n), month(n), day(n), hour(n), minute(n), second(n));
	message_pointer += info->build(message_pointer, buff);
	message_pointer += sprintf(message_pointer, "}}");
	dbgprintf("size = %d", strlen(q));
LEAVE_FUNC;
}

#ifdef	USE_ESP_OPEN_RTOS
static	size_t
net_print(
	int		s,
	char	*str)
{
	return	(write(s, str, strlen(str)));
}
#define	net_printf(s, fmt, ...)					\
{	\
	char	buf[128];	\
	sprintf(buf, fmt, __VA_ARGS__);	\
	net_print(s, buf);	\
}
#endif

static	void
post_http()
{
	time_t	n;
	uint8_t	*pp;
	size_t	size;
#ifdef	USE_ARDUINO
	WiFiClient	client;
#endif
#ifdef	USE_ESP_OPEN_RTOS
	struct	addrinfo	hints;
	struct	addrinfo	*res;
	struct	in_addr	*addr;
	int		s;
	char	rbuf[80];
#endif
	SensorInfo	*info;

ENTER_FUNC;

	OS_show_time(now());

	sense_buff.rewind_read();
	dbgprintf("buff %d bytes use", sense_buff.used_size());
	while	(  !sense_buff.is_end() )	{
		pp = sense_buff.mark_read();
		sense_buff.get(&n, sizeof(time_t));
		OS_show_time(n);
		size = 0;
		size ++;	//	[
		while	( sense_buff.current_value() != 0xFF )	{
			message_pointer = message_buff;
			info = Sensors::item(sense_buff.get_value());
			make_one_sensor_message(n, info, sense_buff);
			size += strlen(message_buff);
			if	( sense_buff.current_value() != 0xFF )	{
				size ++;	//	,
			}
		}
		size ++;	//	]

		sense_buff.rewind_read(pp);
#ifdef	USE_ARDUINO
		if	( client.connect(HOST, PORT) )	{
			client.print("POST /device_data/");
			client.print(UUID);
			client.print(" HTTP/1.1\r\n");
			client.print("Host: ");
			client.print(HOST);
			client.print(":");
			client.print(PORT);
			client.print("\r\n");
			client.print("Connection: close\r\n");
			client.print("Content-Type: application/json\r\n");
			client.print("Content-Length: ");
			client.print(size);
			client.print("\r\n\r\n");

			sense_buff.get(&n, sizeof(time_t));
			OS_show_time(n);
			client.print('[');
			while	( sense_buff.current_value() != 0xFF )	{
				message_pointer = message_buff;
				info = Sensors::item(sense_buff.get_value());
				make_one_sensor_message(n, info, sense_buff);
				DEBUG_PRINTLN(message_buff);
				client.print(message_buff);
				if	( sense_buff.current_value() != 0xFF )	{
					client.print(',');
				}
			}
			(void)sense_buff.get_value();
			client.print(']');
			client.flush();
			delay(100);
			while	( client.available() )	{
				String	line = client.readStringUntil('\r');
				DEBUG_PRINTS(line);
			}
			client.stop();
		}
#endif
#ifdef	USE_ESP_OPEN_RTOS
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		if	( getaddrinfo(HOST, PORT, &hints, &res) == 0 )	{
			addr = &((struct sockaddr_in *)res->ai_addr)->sin_addr;
			dbgprintf("DNS lookup succeeded. IP=%s\r\n", inet_ntoa(*addr));
			s = socket(res->ai_family, res->ai_socktype, 0);
			if	( s > 0 )	{
				if	( connect(s, res->ai_addr, res->ai_addrlen) == 0 )	{
					net_printf(s, "POST /device_data/%s HTTP/1.1\r\n", UUID);
					net_printf(s, "Host: %s:%d\r\n", HOST, PORT);
					net_printf(s, "Connection: %s\r\n", "close");
					net_printf(s, "Content-Type: %s\r\n", "application/json");
					net_printf(s, "Content-Length: %d\r\n\r\n", size);
					sense_buff.get(&n, sizeof(time_t));
					OS_show_time(n);
					net_print(s, "[");
					while	( sense_buff.current_value() != 0xFF )	{
						message_pointer = message_buff;
						info = Sensors::item(sense_buff.get_value());
						make_one_sensor_message(n, info, sense_buff);
						DEBUG_PRINTLN(message_buff);
						net_print(s, message_buff);
						if	( sense_buff.current_value() != 0xFF )	{
							net_print(s, ",");
						}
					}
					(void)sense_buff.get_value();
					net_print(s, "]");
					delay(100);
					while	( read(s, rbuf, 80 ) > 0 )	{
						DEBUG_PRINTLN(rbuf);
					}
				}
				close(s);
			}
		}
		if	( res != NULL )	{
			freeaddrinfo(res);
		}
#endif
	}
LEAVE_FUNC;
}

extern	void
send_server(void)
{
ENTER_FUNC;
	WLAN_monitor();
	WLAN_connect_wifi();
	if	( WLAN_wait_connect() )	{
		post_http();
		sense_buff.rewind_write();
		NTP_start();
	}
LEAVE_FUNC;
}

