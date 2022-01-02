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

#ifndef K_DIMMER_H_
#define K_DIMMER_H_

#define SUPLA_PWM_COUNT    2
//#define DIMMER_CHANNEL  0
#define RGBWW_CONTROLLER_CHANNEL  0

#define RELAY_MAX_COUNT	   9

#define ESP8266_SUPLA_PROTO_VERSION 16

#define SUPLA_ESP_SOFTVER "2.8.42.1"
#define AP_SSID "STAIRCASE_DIMMER"
#define ESP_HOSTNAME "STAIRCASE_DIMMER"

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_0_OUT_IO_NUM 13
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO13

#define B_CFG_PORT         0

#define B_SENSOR_PORT1     12
#define B_SENSOR_PORT2     14

#define B_UPD_PORT		   20
#define B_HARMONOGRAM	   21
#define B_SWITCH		   22
#define B_SENSOR_BLOCK1    23
#define B_SENSOR_BLOCK2    24

#define LED_RED_PORT       16

#define USE_GPIO16_OUTPUT

#define BOARD_GPIO_OUTPUT_SET_HI	\
		if (port >= 20) {supla_esp_board_gpiooutput_set_hi(port, hi); 	\
						supla_log(LOG_DEBUG, "PORT 20 MAKRO");	\
						return;  };	\
						
#define BOARD_GPIO_OUTPUT_IS_HI	\
		if ( port == B_HARMONOGRAM)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
											return supla_esp_state.Relay[4] == 1 ? 1 : 0;	};	\
		if ( port == B_SWITCH)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 5 = %i", supla_esp_state.Relay[5]);	\
												return supla_esp_state.Relay[5] == 1 ? 1 : 0;	};	\
		if ( port == B_UPD_PORT)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 6 = %i", supla_esp_state.Relay[6]);	\
												return supla_esp_state.Relay[6] == 1 ? 1 : 0;	};	\
		if ( port == B_SENSOR_BLOCK1)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 7 = %i", supla_esp_state.Relay[7]);	\
												return supla_esp_state.Relay[7] == 1 ? 1 : 0;	};	\
		if ( port == B_SENSOR_BLOCK2)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 8 = %i", supla_esp_state.Relay[8]);	\
												return supla_esp_state.Relay[8] == 1 ? 1 : 0;	};	\
								
#define BOARD_ON_INPUT_INACTIVE if (supla_last_state == STATE_CONNECTED) { \
									if ( input_cfg->gpio_id == 12 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) == 1 ) {	\
										supla_log(LOG_DEBUG, "CHANNEL inactive = %i", input_cfg->channel);	\
										supla_dimmer_smooth(1); };	\
									if ( input_cfg->gpio_id == 14 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1 ) {	\
										supla_log(LOG_DEBUG, "CHANNEL inactive = %i", input_cfg->channel);	\
										supla_dimmer_smooth(1); }; };

#define BOARD_ON_INPUT_ACTIVE if (supla_last_state == STATE_CONNECTED) { \
								if ( input_cfg->gpio_id == 12 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) == 1 ) {	\
									supla_log(LOG_DEBUG, "CHANNEL active = %i", input_cfg->channel);	\
									supla_dimmer_smooth(0); };	\
								if ( input_cfg->gpio_id == 14 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) == 1 ) {	\
									supla_log(LOG_DEBUG, "CHANNEL active = %i", input_cfg->channel);	\
									supla_dimmer_smooth(0); };	};
							  
#define BOARD_INTR_HANDLER	if (supla_last_state == STATE_CONNECTED) { \
							supla_log(LOG_DEBUG, "INTR gpio_status = %i", gpio_status);	\
							if ( gpio_status > 1 && gpio_status < 14 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK1) ) {	\
							supla_log(LOG_DEBUG, "Input intr test");	\
							supla_dimmer_smooth(2);	\
							} else if ( gpio_status > 12 && supla_esp_gpio_output_is_hi(B_SENSOR_BLOCK2) ) {	\
							supla_log(LOG_DEBUG, "Input intr test");	\
							supla_dimmer_smooth(2);	\
							} else { supla_dimmer_smooth(3); }; };
							
#define BOARD_ON_CHANNEL_STATE_PREPARE	state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
										state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;

void ICACHE_FLASH_ATTR supla_esp_board_pwm_init(void);
char ICACHE_FLASH_ATTR supla_esp_board_set_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_get_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

void supla_dimmer_smooth(int hi);

#endif
