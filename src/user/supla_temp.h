/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#ifndef SUPLA_TEMP_H_
#define SUPLA_TEMP_H_

#include "supla_esp.h"
#include "supla-dev/proto.h"

extern ETSTimer supla_ds18b20_timer1;
extern ETSTimer supla_ds18b20_timer2;
extern ETSTimer supla_dht_timer1;

void ICACHE_FLASH_ATTR supla_set_temp_channel( uint8 channel );
void ICACHE_FLASH_ATTR supla_ds18b20_init(void);
void supla_get_temperature(char value[SUPLA_CHANNELVALUE_SIZE]);
void supla_ds18b20_start(void);

void DHT_ICACHE_FLASH supla_dht_init(void);
void DHT_ICACHE_FLASH supla_get_temp_and_humidity(char value[SUPLA_CHANNELVALUE_SIZE]);
void DHT_ICACHE_FLASH supla_dht_start(void);

#endif