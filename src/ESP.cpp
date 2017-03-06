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
 * ESP class(for compatibilities)
 *
 */

#define	TRACE
#include	"config.h"
#ifdef	USE_ESP_OPEN_RTOS
extern "C" {
#include	<espressif/esp_common.h>
#include	<espressif/esp_system.h>
#include	<string.h>
}
#include	"ESP.h"
#include	"debug.h"

void
EspClass::restart(void)
{
	sdk_system_restart();
}

void
EspClass::deepSleep(
	uint32_t	time_us,
	WakeMode	mode)
{
#if	0
	sdk_system_deep_sleep_set_option(static_cast<int>(mode));	//	not support yet
#endif
	sdk_system_deep_sleep(time_us);
}
#endif
