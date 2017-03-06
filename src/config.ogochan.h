#ifndef __INC_CONFIG_H__
#define __INC_CONFIG_H__

#define	USE_ESP_OPEN_RTOS
//#define	USE_ARDUINO

#define	HOST							"www.folloger.com"
#ifdef	USE_ESP_OPEN_RTOS
#define	PORT							"7001"
#endif
#ifdef	USE_ARDUINO
#define	PORT							7001
#endif
#define	UUID							"fe9032ff-5d7f-473a-868e-a33b365cd050"		//	desktop
//#define	UUID							"5241d24e-fcf0-40f1-90c3-5e1f4f591fb3"	//	freezer
#define	MY_SSID							"LANLANLAN24"
#define	MY_PASS							"8309f815520d2"

#include	"config.common.h"

#endif // __CONFIG_H__
