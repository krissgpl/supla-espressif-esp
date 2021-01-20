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

#ifndef K_SONOFF_TOUCH_H_
#define K_SONOFF_TOUCH_H_

#define ESP8285

#define BOARD_CFG_HTML_TEMPLATE
#define BOARD_ON_CONNECT

#define ESP8266_SUPLA_PROTO_VERSION 12

#define SUPLA_ESP_SOFTVER "3.0.0.0"

#ifdef __BOARD_k_sonoff_touch
	#define AP_SSID "SONOFF-TOUCH"
	#define ESP_HOSTNAME "SONOFF-TOUCH"
#endif

#ifdef __BOARD_k_sonoff_touch_dual
	#define AP_SSID "TOUCH-DUAL"
	#define ESP_HOSTNAME "SONOFF-TOUCH-DUAL"
#endif

#ifdef __BOARD_k_sonoff_touch_triple
	#define AP_SSID "TOUCH-TRIPLE"
	#define ESP_HOSTNAME "SONOFF-TOUCH-TRIPLE"
#endif

#define LED_RED_PORT     13
#define B_RELAY1_PORT    12
#define B_RELAY2_PORT     5
#define B_RELAY3_PORT     4

#define B_CFG_PORT       14
#define B_INPUT2_PORT     9
#define B_INPUT3_PORT    10

#define B_UPD_PORT		 20

#define BOARD_GPIO_OUTPUT_SET_HI if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };
		
void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

#define BOARD_ON_INPUT_ACTIVE                        \
    supla_esp_board_gpio_on_input_active(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg);

#define BOARD_ON_INPUT_INACTIVE                        \
    supla_esp_board_gpio_on_input_inactive(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_inactive(void* _input_cfg);

#endif