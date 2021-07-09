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

#ifndef K_SWITCH_DUAL_H_
#define K_SWITCH_DUAL_H_

#define ESP8266_SUPLA_PROTO_VERSION 12

#define SUPLA_ESP_SOFTVER "2.8.11.0"

#define BOARD_CFG_HTML_TEMPLATE

#define BOARD_ON_CONNECT

#define RELAY_MAX_COUNT		6

#define TEMP_SELECT

#define AP_SSID "SWITCH_DUAL"
#define ESP_HOSTNAME "SUPLA-SWITCH-DUAL"

#define DS18B20
#define TEMPERATURE_CHANNEL 5

#define DHTSENSOR
#define TEMPERATURE_HUMIDITY_CHANNEL 5

#define USE_GPIO16_OUTPUT

#define LED_RED_PORT     16
#define LED_GREEN_PORT    4
#define B_RELAY1_PORT     5
#define B_RELAY2_PORT    13
#define B_BTN1_PORT      14
#define B_BTN2_PORT      12
#define B_UPD_PORT		 20
#define B_RELAY1_DIS	 21
#define B_RELAY2_DIS	 22

#define BOARD_GPIO_OUTPUT_SET_HI	\
	if ( port == B_RELAY1_PORT && supla_esp_state.Relay[3] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO5 !!!");	\
																	GPIO_OUTPUT_SET(5, 0);	\
																	return;	}	\
	if ( port == B_RELAY2_PORT && supla_esp_state.Relay[4] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO13 !!!");	\
																	GPIO_OUTPUT_SET(13, 0);	\
																	return;	}	\
	if (supla_last_state == STATE_CONNECTED) { \
		if ( supla_esp_cfg.StatusLedOff == 0 || supla_esp_cfg.StatusLedOff == 1 ) {	\
			supla_log(LOG_DEBUG, "STATUS LED OFF ON");	\
		} else if ( supla_esp_cfg.StatusLedOff == 2 ) {	\
			char hi1;	\
			char hi2;	\
			hi1 = supla_esp_gpio_output_is_hi(B_RELAY1_PORT);	\
			hi2 = supla_esp_gpio_output_is_hi(B_RELAY2_PORT);	\
			if (port == LED_RED_PORT) {hi1 = supla_esp_gpio_output_is_hi(B_RELAY1_PORT);	\
									   hi2 = supla_esp_gpio_output_is_hi(B_RELAY2_PORT);	\
			} else if (port == B_RELAY1_PORT) {supla_esp_gpio_set_led(hi1, hi2, 1); \
			} else if (port == B_RELAY2_PORT) {supla_esp_gpio_set_led(hi1, hi2, 1); }; }	\
		if (port >= 20) {supla_esp_board_gpiooutput_set_hi(port, hi); 	\
						supla_log(LOG_DEBUG, "PORT 20 MAKRO");	\
						return;  };	\
}

#define BOARD_GPIO_OUTPUT_IS_HI	\
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 3 = %i", supla_esp_state.Relay[3]);	\
									return supla_esp_state.Relay[3] == 1 ? 1 : 0;	}	\
				if ( port == 22)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
									return supla_esp_state.Relay[4] == 1 ? 1 : 0;	}	\
				
#define BOARD_ON_CHANNEL_STATE_PREPARE	state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
										state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;
				
void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(int port, char hi);

void ICACHE_FLASH_ATTR supla_esp_board_block_channel(char red, char blue);
	
char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

#define BOARD_ON_INPUT_ACTIVE       				 \
    supla_esp_board_gpio_on_input_active(input_cfg); \
    return; 
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg);

#define BOARD_ON_INPUT_INACTIVE                        \
    supla_esp_board_gpio_on_input_inactive(input_cfg); \
    return;
void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_inactive(void* _input_cfg);

#endif
