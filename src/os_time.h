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
 * time functions
 *
 */

#ifndef	__INC_OS_TIME_H_
#define	__INC_OS_TIME_H_

#ifdef	USE_ARDUINO
#include	<Arduino.h>
#include	<Time.h>
#include	<TimeLib.h>
#endif
#ifdef	USE_ESP_OPEN_RTOS
#include	<FreeRTOS.h>
#include	<task.h>
#define	delay(ms)		vTaskDelay((ms) / portTICK_PERIOD_MS)
#endif

#define	sleep_ms(ms)	delay(ms)

extern	void	OS_show_time(time_t n);
extern	void	OS_show_time(char *title, time_t n);

#endif
