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

#define ESP8266_SUPLA_PROTO_VERSION 16
#define RETREIVE_CHANNEL_CONFIG 0b1100

#define SUPLA_ESP_SOFTVER "2.8.48.1"

#define BOARD_CFG_HTML_TEMPLATE

#define BOARD_ON_CONNECT

#define RELAY_MAX_COUNT		8

#define TEMP_SELECT

//#define BTN_MODE_PUBLISH_AT

#define AP_SSID "SWITCH_DUAL"
#define ESP_HOSTNAME "SUPLA-SWITCH-DUAL"
#define CFGMODE_SSID_LIMIT_MACLEN

#define DS18B20
#define TEMPERATURE_CHANNEL 7

#define DHTSENSOR
#define TEMPERATURE_HUMIDITY_CHANNEL 7

#define USE_GPIO16_OUTPUT

#define LED_RED_BLOCK    0x1
#define LED_GREEN_BLOCK  0x2

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
	if ( port == B_RELAY1_PORT && supla_esp_state.Relay[5] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO5 !!!");	\
																	supla_block_channel(LED_RED_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY1_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO5 !!! po");	\
																	return;	} 	}	\
	if ( port == B_RELAY2_PORT && supla_esp_state.Relay[6] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO13 !!!");	\
																	supla_block_channel(LED_GREEN_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY2_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO13 !!! po");	\
																	return;	}	}	\
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
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 5 = %i", supla_esp_state.Relay[5]);	\
									return supla_esp_state.Relay[5] == 1 ? 1 : 0;	}	\
				if ( port == 22)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 6 = %i", supla_esp_state.Relay[6]);	\
									return supla_esp_state.Relay[6] == 1 ? 1 : 0;	}
				
#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 4 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };
										
//#define BOARD_ON_COUNTDOWN_START	supla_log(LOG_DEBUG, "COUNTDOWN_START, time=%d, gpio=%d, ch=%d", time_ms, gpio_id, channel_number);

#define BOARD_SEND_AT supla_send_at(input_cfg->gpio_id, action);

void supla_send_at(uint8 gpio, int action);
				
void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(int port, char hi);

void supla_block_channel(int ledblock);
	
char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif
