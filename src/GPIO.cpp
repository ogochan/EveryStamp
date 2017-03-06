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

#define	TRACE
#include	"config.h"
#ifdef	USE_ESP_OPEN_RTOS
#include <esp/uart.h>
#endif
#include	"GPIO.h"
#include	"debug.h"
/*
 *	GPIO	usage
 *	0		boot select
 *	1		TXD
 *	2		I2C	data		PIN_SDA
 *	3		RXD
 *	4		1wire			PIN_1WIRE
 *	5		NC
 *	12		NC(PWM0)
 *	13		LED
 *	14		I2C clock		PIN_SCL
 *	15		pull up
 *	16		RTC
 *	
 */

static	void
setOutputAndValue(
	int		port,
	int		initialVal)
{
	digitalWrite(port, initialVal);
	pinMode(port, OUTPUT);
}

extern	void
GPIO_init(void)
{
	// General config : BOOT MODE (GPIO0,2,15) related
	pinMode(0, INPUT);
#ifdef	USE_ARDUINO
	pinMode(PIN_SDA, INPUT_PULLUP);   		// for I2C
	pinMode(PIN_SCL, INPUT_PULLUP);  		// for I2C
#endif
#ifdef	USE_ESP_OPEN_RTOS
    gpio_set_pullup(PIN_1WIRE, true, true);	//	1 wire
#endif
	pinMode(15, INPUT);
	pinMode(13, OUTPUT);		//	LED

	// If pin is NC, we should set {output / High} or {input / pull up enabled} on the pin.
	setOutputAndValue(5, HIGH);
	setOutputAndValue(12, HIGH);
	setOutputAndValue(13, LOW);
	setOutputAndValue(16, HIGH);
#ifdef	USE_ARDUINO
	Serial.begin(115200);
#endif
#ifdef	USE_ESP_OPEN_RTOS
	uart_set_baud(0, 115200);
#endif
}

extern	void
GPIO_reset_sensors(void)
{
}
