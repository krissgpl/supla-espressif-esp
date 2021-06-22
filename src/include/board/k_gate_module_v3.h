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

#ifndef K_SUPLA_GATE_MODULE_V3_H_
#define K_SUPLA_GATE_MODULE_V3_H_

#define ESP8266_SUPLA_PROTO_VERSION 12

#define SUPLA_ESP_SOFTVER "2.8.11.0"

#define BOARD_CFG_HTML_TEMPLATE

#define BOARD_ON_CONNECT

#define RELAY_MAX_COUNT		6

#define TEMP_SELECT

#define ESP_HOSTNAME "SUPLA-NICE_V3"
#define AP_SSID "SUPLA-NICE-V3"

#define DS18B20
#define TEMPERATURE_CHANNEL 5

#define DHTSENSOR
#define TEMPERATURE_HUMIDITY_CHANNEL 5

#define USE_GPIO16_OUTPUT

#define B_CFG_PORT          0
#define LED_RED_PORT  		16
#define B_RELAY1_PORT       13

#define B_SENSOR_PORT1      12
#define B_SENSOR_PORT2      14

#define B_UPD_PORT		    20
#define B_RELAY1_DIS	 	21

#define BOARD_GPIO_OUTPUT_SET_HI	\
	if ( port == B_RELAY1_PORT && supla_esp_state.Relay[4] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO13 !!!");	\
																	GPIO_OUTPUT_SET(13, 0);	\
																	return;	}	\
	if (supla_last_state == STATE_CONNECTED) { \
		if ( supla_esp_cfg.StatusLedOff == 0 || supla_esp_cfg.StatusLedOff == 1 ) {	\
			supla_log(LOG_DEBUG, "STATUS LED OFF ON");	\
		} else if ( supla_esp_cfg.StatusLedOff == 2 ) {	\
			char hi1;	\
			hi1 = supla_esp_gpio_output_is_hi(B_RELAY1_PORT);	\
			if (port == LED_RED_PORT) {hi1 = supla_esp_gpio_output_is_hi(B_RELAY1_PORT);	\
			} else if (port == B_RELAY1_PORT) {supla_esp_gpio_set_led(hi1 ,1 , 1); }; }\
		if (port >= 20) {supla_esp_board_gpiooutput_set_hi(port, hi); 	\
						supla_log(LOG_DEBUG, "PORT 20 MAKRO");	\
						return;  };	\
	}

#define BOARD_GPIO_OUTPUT_IS_HI	\
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
									return supla_esp_state.Relay[4] == 1 ? 1 : 0;	}
									
void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);
		
void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

char* ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);

#endif
