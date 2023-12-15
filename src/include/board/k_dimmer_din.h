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

/*------------------------------------------------------------STAIRCASE-DIMMER-DIN-------------------------------------------------------------------------

	CHANNEL 0 - SET BRIGHTNESS IN NIGHT MODE	( virtual dimmer )  --FUNCTIONS--- DIMMER (DEFAULT)
																									  	  
	--------------------------------------------------------------------------------													  
												  
	CHANNEL 1 - SET LIGHTNING TIME				( virtual dimmer ) 	--FUNCTIONS--- DIMMER (DEFAULT)
																		 
	--------------------------------------------------------------------------------		
	
	CHANNEL 2 - IN				   ( gpio 12 ) 						--FUNCTIONS---|-- NONE (DEFAULT)
																				  |-- SENSORNO  -- FOR TURN ON LED STRIP
																				  
	--------------------------------------------------------------------------------		
	
	CHANNEL 3 - HARMONOGRAM	   	   ( virtual gpio 21 )				--FUNCTIONS--- POWERSWITCH (DEFAULT) --|-- ON  - LED BRIGHTNESS FULL
																										   |-- OFF - LED BRIGHTNESS SET TO VALUE OF CHANNEL 0
																								   
	--------------------------------------------------------------------------------															  

	CHANNEL 4 - ENABLE LED STRIP   ( virtual gpio 22 )				--FUNCTIONS---|-- NONE (DEFAULT)
																				  |-- POWERSWITCH  	--|-- ON  - LED STRIP ENABLE
																									  |-- OFF - LED STRIP DISABLE
																								 
	--------------------------------------------------------------------------------															  

	CHANNEL 5 - UPDATE INIT		   ( gpio 20 virtual ) 				--FUNCTIONS---|-- NONE (DEFAULT)
																				  |-- POWERSWITCH
																								 
-------------------------------------------------------------------------------------------------------------------------------------------------------------*/

#ifndef K_DIMMER_DIN_H_
#define K_DIMMER_DIN_H_

#define SUPLA_PWM_COUNT    2
#define RGBWW_CONTROLLER_CHANNEL  0

#define RELAY_MAX_COUNT	   9

#define ESP8266_SUPLA_PROTO_VERSION 16

#define BOARD_CFG_HTML_TEMPLATE

#define BOARD_ON_CONNECT

#define SUPLA_ESP_SOFTVER "2.8.55.1"
#define AP_SSID "STAIRCASE_DIMMER_DIN"
#define CFGMODE_SSID_LIMIT_MACLEN
#define ESP_HOSTNAME "STAIRCASE_DIMMER_DIN"

#define PWM_0_OUT_IO_MUX PERIPHS_IO_MUX_MTCK_U
#define PWM_0_OUT_IO_NUM 13
#define PWM_0_OUT_IO_FUNC  FUNC_GPIO13

#define TEMP_SELECT
//#define TEMP_DEBUG

#define B_CFG_PORT         0
#define B_SENSOR_PORT1     12

#define B_UPD_PORT		   20
#define B_HARMONOGRAM	   21
#define B_SWITCH		   22

#define LED_RED_PORT       16

#define USE_GPIO16_OUTPUT

#define BOARD_GPIO_OUTPUT_SET_HI if (supla_last_state == STATE_CONNECTED) {\
									if ( port == LED_RED_PORT ) { hi = supla_esp_gpio_output_is_hi(B_SWITCH);\
									} else if ( port==B_SWITCH ) { supla_esp_gpio_set_led(hi, 1, 1); } };	\
									if ( port >= 20 ) {	\
										supla_esp_board_gpiooutput_set_hi(port, hi); 	\
										return; };
						
#define BOARD_GPIO_OUTPUT_IS_HI	\
		if ( port == B_HARMONOGRAM)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 3 = %i", supla_esp_state.Relay[3]);	\
											return supla_esp_state.Relay[3] == 1 ? 1 : 0;	};	\
		if ( port == B_SWITCH)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 4 = %i", supla_esp_state.Relay[4]);	\
												return supla_esp_state.Relay[4] == 1 ? 1 : 0;	};	\
		if ( port == B_UPD_PORT)  {  supla_log(LOG_DEBUG, "BOARD_GPIO_OUTPUT_IS_HI 5 = %i", supla_esp_state.Relay[5]);	\
												return supla_esp_state.Relay[5] == 1 ? 1 : 0;	};	\
												
#define BOARD_INTR_HANDLER	if ( supla_last_state == STATE_CONNECTED && gpio_status > 1 ) { \
							supla_log(LOG_DEBUG, "INTR gpio_status = %i", gpio_status);	\
							supla_dimmer_smooth(!gpio__input_get(B_SENSOR_PORT1)); }
							
							
#define BOARD_ON_CHANNEL_STATE_PREPARE	if ( ChannelNumber == 5 ) {	\
											state->Fields |= SUPLA_CHANNELSTATE_FIELD_LASTCONNECTIONRESETCAUSE;	\
											state->LastConnectionResetCause = supla_esp_cfg.UpdateStatus;	\
										    state->IPv4 = ipaddr_addr(SUPLA_ESP_SOFTVER);	\
											supla_log(LOG_DEBUG, "IP FIELD = %i", ipaddr_addr(SUPLA_ESP_SOFTVER)); };
											
void ICACHE_FLASH_ATTR supla_esp_board_pwm_init(void);
char ICACHE_FLASH_ATTR supla_esp_board_set_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_get_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness);
void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc);

void ICACHE_FLASH_ATTR board_esp_pwm_set_percent_duty(uint8 percent, uint8 channel);

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi);

void supla_dimmer_smooth(int in1);

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void);

char* ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved);

#endif
