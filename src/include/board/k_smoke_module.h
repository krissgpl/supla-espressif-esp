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

#ifndef K_SMOKE_MODULE_H_
#define K_SMOKE_MODULE_H_

#define ESP8266_SUPLA_PROTO_VERSION 12

#define SUPLA_ESP_SOFTVER "2.8.0.0"

#define BOARD_CFG_HTML_TEMPLATE

#define BOARD_ON_CONNECT

#ifdef __BOARD_k_smoke_module_ds18b20
	#define DS18B20
	#define TEMPERATURE_CHANNEL 1
	#define AP_SSID "SMOKE-MODULE_DS18B20"

    	#define W1_GPIO5

#endif

#ifdef __BOARD_k_smoke_module_DHT22
	#define DHTSENSOR
   	#define TEMPERATURE_HUMIDITY_CHANNEL 1
	#define AP_SSID "SMOKE-MODULE_DHT22"

        #define W1_GPIO5

#endif

#ifdef __BOARD_k_smoke_module
	#define AP_SSID "SMOKE-MODULE"
#endif

#define ESP_HOSTNAME "SUPLA-SMOKE-MODULE"

#define LED_RED_PORT      2
#define B_CFG_PORT        0
#define B_SENSOR_PORT1    12
#define B_UPD_PORT		   20

#define BOARD_GPIO_OUTPUT_SET_HI if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);
	
char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif
