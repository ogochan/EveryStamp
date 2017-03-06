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
 * NTP class
 *
 */

#define	TRACE
#include	"config.h"
#include	"debug.h"
#include	"os_wifi.h"
#include	"os_time.h"
#include	"os_ntp.h"
#include	"Time.h"
#ifdef	USE_ESP_OPEN_RTOS
extern "C" {
#include	<sntp.h>
}
#endif
#ifdef	USE_ARDUINO
#include	<NTP.h>
#endif

static	bool	fInit = false;

#ifdef	USE_ARDUINO
extern	void
NTP_start(void)
{
	if	( !fInit )	{
		WLAN_connect_wifi();
		if	( WLAN_wait_connect() )	{
			setTimeServer(NTP_SERVER);
			ntp_begin(2390);
			fInit = true;
		}
	}
}
#endif

#ifdef	USE_ESP_OPEN_RTOS
extern	void
NTP_start(void)
{
	char	*servers[] = { NTP_SERVER };

ENTER_FUNC;
	if	( !fInit )	{
		sntp_set_update_delay(5*60*1000);
		sntp_initialize(NULL);
		sntp_set_servers(servers, 1);
		sleep_ms(1000);
		setTime(time(NULL));
		fInit = true;
	}
LEAVE_FUNC;
}
#endif
