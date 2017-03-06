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

#ifndef	_INC_POWER_H_
#define	_INC_POWER_H_

class	Power	{
  public:
	static	void	wifi_off(void);
	static	void	wifi_on(void);
	static	void	sleep(time_t next, int sec);
	static	void	control(bool power);
	static	void	save_ram(time_t next);
	static	void	clear_ram(void);
	static	bool	load_ram(void);
	static	bool	restore_status(void);
	static	time_t	next_send;
	static	time_t	next_sense;
	static	bool	rf_on;
};

#endif
