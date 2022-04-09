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

#define ESP8266_SUPLA_PROTO_VERSION 16

#define RELAY_MAX_COUNT		11

#define SUPLA_ESP_SOFTVER "2.8.49.0"

#ifdef __BOARD_k_sonoff_touch
	#define AP_SSID "SONOFF-TOUCH"
	#define ESP_HOSTNAME "SONOFF-TOUCH"
	#define RETREIVE_CHANNEL_CONFIG 0b1000
#endif

#ifdef __BOARD_k_sonoff_touch_dual
	#define AP_SSID "TOUCH-DUAL"
	#define ESP_HOSTNAME "SONOFF-TOUCH-DUAL"
	#define RETREIVE_CHANNEL_CONFIG 0b1100000
#endif

#ifdef __BOARD_k_sonoff_touch_triple
	#define AP_SSID "TOUCH-TRIPLE"
	#define ESP_HOSTNAME "SONOFF-TOUCH-TRIPLE"
	#define RETREIVE_CHANNEL_CONFIG 0b1110000000
#endif

#define CFGMODE_SSID_LIMIT_MACLEN

#define LED_RED_BLOCK    0x1
#define LED_GREEN_BLOCK  0x2
#define LED_BLUE_BLOCK   0x3

#define LED_RED_PORT     13
#define B_RELAY1_PORT    12
#define B_RELAY2_PORT     5
#define B_RELAY3_PORT     4

#define B_CFG_PORT       14
#define B_INPUT2_PORT     9
#define B_INPUT3_PORT    10

#define B_UPD_PORT		 20
#define B_RELAY1_DIS	 21
#define B_RELAY2_DIS	 22
#define B_RELAY3_DIS	 23

#ifdef __BOARD_k_sonoff_touch_triple
	#define BOARD_GPIO_OUTPUT_SET_HI	\
				if ( port == B_RELAY1_PORT && supla_esp_state.Relay[4] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO12 !!!");	\
																	supla_block_channel(LED_RED_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY1_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO12 !!! po");	\
																	return;	} 	}	\
				if ( port == B_RELAY2_PORT && supla_esp_state.Relay[5] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO5 !!!");	\
																	supla_block_channel(LED_GREEN_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY2_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO5 !!! po");	\
																	return;	}	}	\
				if ( port == B_RELAY3_PORT && supla_esp_state.Relay[6] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO4 !!!");	\
																	supla_block_channel(LED_BLUE_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY2_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO4 !!! po");	\
																	return;	}	}	\
				if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };
	#define BOARD_GPIO_OUTPUT_IS_HI	\
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
									return supla_esp_state.Relay[4] == 1 ? 1 : 0;	}	\
				if ( port == 22)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 5 = %i", supla_esp_state.Relay[5]);	\
									return supla_esp_state.Relay[5] == 1 ? 1 : 0;	}	\
				if ( port == 23)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 6 = %i", supla_esp_state.Relay[6]);	\
									return supla_esp_state.Relay[6] == 1 ? 1 : 0;	}	

	#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 3 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };

#endif
									
#ifdef __BOARD_k_sonoff_touch_dual
	#define BOARD_GPIO_OUTPUT_SET_HI	\
				if ( port == B_RELAY1_PORT && supla_esp_state.Relay[3] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO12 !!!");	\
																	supla_block_channel(LED_RED_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY1_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO12 !!! po");	\
																	return;	} 	}	\
				if ( port == B_RELAY2_PORT && supla_esp_state.Relay[4] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO5 !!!");	\
																	supla_block_channel(LED_GREEN_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY2_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO5 !!! po");	\
																	return;	}	}	\
				if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };
	#define BOARD_GPIO_OUTPUT_IS_HI	\
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 3 = %i", supla_esp_state.Relay[3]);	\
									return supla_esp_state.Relay[3] == 1 ? 1 : 0;	}	\
				if ( port == 22)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
									return supla_esp_state.Relay[4] == 1 ? 1 : 0;	}	

	#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 2 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };

#endif

#ifdef __BOARD_k_sonoff_touch
	#define BOARD_GPIO_OUTPUT_SET_HI	\
				if ( port == B_RELAY1_PORT && supla_esp_state.Relay[2] == 1 ) { supla_log(LOG_DEBUG, "Blokada GPIO12 !!!");	\
																	supla_block_channel(LED_RED_BLOCK);	\
																	if (supla_esp_gpio_output_is_hi(B_RELAY1_PORT) == 0) {\
																	supla_log(LOG_DEBUG, "Blokada GPIO12 !!! po");	\
																	return;	} 	}	\
				if ( port >= 20 ) { supla_esp_board_gpiooutput_set_hi(port, hi); return; };
	#define BOARD_GPIO_OUTPUT_IS_HI	\
				if ( port == 21)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 2 = %i", supla_esp_state.Relay[2]);	\
									return supla_esp_state.Relay[2] == 1 ? 1 : 0;	}	

	#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 1 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };

#endif



#define BOARD_SEND_AT supla_send_at(input_cfg->gpio_id, action);

void supla_send_at(uint8 gpio, int action);
		
void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

void supla_block_channel(int ledblock);

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);
	
void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

#endif