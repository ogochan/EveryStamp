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
 * Time functions(for compatibilities)
 *
 */

#ifndef	__INC_TIME_H__
#define	__INC_TIME_H__

extern	int		hour(void);
extern	int		hour(time_t t);
extern	int		minute(void);
extern	int		minute(time_t t);
extern	int		second(void);
extern	int		second(time_t t);
extern	int		day(void);
extern	int		day(time_t t);
extern	int		weekday(void);
extern	int		weekday(time_t t);
extern	int		month(void);
extern	int		month(time_t t);
extern	int		year(void);
extern	int		year(time_t t);
extern	time_t	now(void);
extern	void	setTime(time_t t);
extern	void	start_time(void);

#endif
