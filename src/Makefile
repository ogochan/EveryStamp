#
#  EveryStamp for ESP8266 - a sensor data collector
# 
#   Copyright (C) 2017  Masami Ogoshi (ogochan@wasp.co.jp)
# 
#   This program is free software: you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation, either version 3 of the License, or
#   (at your option) any later version.
# 
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
# 
#   You should have received a copy of the GNU General Public License
#   along with this program.  If not, see <http://www.gnu.org/licenses/>.
# 
# 
#  This code is made available on the understanding that it will not be
#  used in safety-critical situations without a full and competent review.
# 
#  Makefile for ESP OPEN RTOS (see https://github.com/SuperHouse/esp-open-rtos )

ESPPORT=/dev/ttyUSB0
CROSS = ~/ESP8266/esp-open-sdk/xtensa-lx106-elf/bin/xtensa-lx106-elf-
PROGRAM=EveryStamp
EXTRA_COMPONENTS = extras/sntp extras/i2c extras/onewire extras/ds18b20
include ~/ESP8266/esp-open-rtos/common.mk
