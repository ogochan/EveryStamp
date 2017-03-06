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
 * GPIO pin assign
 *
 */

#ifndef	__INC_GPIO_H__
#define	__INC_GPIO_H__

#ifdef	USE_ESP_OPEN_RTOS
#include	<esp8266.h>

#define	OUTPUT			GPIO_OUTPUT
#define	INPUT			GPIO_INPUT
#define	HIGH			1
#define	LOW				0
#define	pinMode(p, m)		gpio_enable((p), (m))
#define	digitalWrite(p, v)	gpio_write((p), (v))
#endif

extern	void	GPIO_init(void);
extern	void	GPIO_reset_sensors(void);

#endif
