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
 TEST
 */

#include "public_key_in_c_code"
 
#include "k_dimmer.h"
#include "supla_esp_devconn.h"

int HRM_channel;
int UPD_channel;
int BLK_channel;

uint8 dimmer_brightness = 0;

uint8 Licznik = 0;
uint8 Jasnosc = 0;

unsigned int Wlacznik = 0;
unsigned int Start = 0;

ETSTimer dimmer_timer;
ETSTimer value_timer1;
ETSTimer work_timer;

void ICACHE_FLASH_ATTR supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
	ets_snprintf(buffer, buffer_size, "DIMMER");
}


void ICACHE_FLASH_ATTR supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	
	supla_input_cfg[1].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[1].gpio_id = B_SENSOR_PORT1;
	supla_input_cfg[1].channel = 1;
	
	supla_input_cfg[2].type = INPUT_TYPE_SENSOR;
	supla_input_cfg[2].gpio_id = B_SENSOR_PORT2;
	supla_input_cfg[2].channel = 2;
	
	supla_input_cfg[3].type = INPUT_TYPE_BTN_MONOSTABLE;
    supla_input_cfg[3].gpio_id = 4;
	
	supla_relay_cfg[0].gpio_id = B_HARMONOGRAM;	// harmonogram channel
    supla_relay_cfg[0].channel = 3;
	
	supla_relay_cfg[1].gpio_id = B_UPD_PORT;	// update init channel
    supla_relay_cfg[1].channel = 4;
	
	supla_relay_cfg[2].gpio_id = B_BLOKADA;		// blokada channel
    supla_relay_cfg[2].channel = 5;
	
	// ---------------------------------------
	
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_SD_DATA3_U, FUNC_GPIO10);//uzycie GPIO10
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTDI_U, FUNC_GPIO12);	//uzycie GPIO12
	PIN_FUNC_SELECT(PERIPHS_IO_MUX_MTMS_U, FUNC_GPIO14);	//uzycie GPIO14
	
	PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);			// pullup gpio 0
	PIN_PULLUP_EN(PERIPHS_IO_MUX_SD_DATA3_U);		// pullup gpio 10
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTDI_U);			// pullup gpio 12
	PIN_PULLUP_EN(PERIPHS_IO_MUX_MTMS_U);			// pullup gpio 14

	//----------------------------------------	wlaczenie zasilania dht z opoznieniem (zaklocenia)
	
	if ( supla_esp_cfg.ThermometerType == 1 || supla_esp_cfg.ThermometerType == 2 ) {
	
		supla_esp_gpio_set_hi(10, 0);	// ustaw gpio10 low wyl zasilania DHT
		supla_log(LOG_DEBUG, "ustaw gpio10 low wyl zasilania DHT");
		os_delay_us(500000);						// poczekaj 0,3s
		supla_esp_gpio_set_hi(10, 1);	// ustaw gpio10 high wl zasilania DHT
		supla_log(LOG_DEBUG, "ustaw gpio10 high wl zasilania DH");
	};
}

void supla_esp_baord_value_timer1_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "TIMER update - restart");
	supla_system_restart();
	
}

void dimmer_timer_ON_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Dimmer Timer ON start");
	
	Licznik = Licznik + 1;
	supla_log(LOG_DEBUG, "Licznik : %i", Licznik);
	supla_esp_pwm_set_percent_duty(Licznik, 100, 0);
	
	if ( supla_esp_gpio_output_is_hi(B_HARMONOGRAM) == 1 ) { Jasnosc = 100;
	} else { Jasnosc = supla_esp_state.brightness[0]; };
	
	 if ( Licznik == Jasnosc ) { 
	 supla_log(LOG_DEBUG, "Dimmer Timer ON stop");
	 os_timer_disarm(&dimmer_timer); }
	
}

void dimmer_timer_OFF_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Dimmer Timer OFF start");
	
	Licznik = Licznik - 1;
	supla_log(LOG_DEBUG, "Licznik : %i", Licznik);
	supla_esp_pwm_set_percent_duty(Licznik, 100, 0);
	
	 if ( Licznik == 0 ) { 
	 supla_log(LOG_DEBUG, "Dimmer Timer OFF stop");
	 Wlacznik = 0;
	 os_timer_disarm(&dimmer_timer); }
	
}

void work_timer_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "Set dimmer 0");
	Licznik = supla_esp_state.brightness[0];
	//os_timer_disarm(&dimmer_timer);
	//os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_OFF_cb, NULL);
	//os_timer_arm(&dimmer_timer, 20, 1);
}

void supla_dimmer_smooth(int hi) {

if ( supla_esp_gpio_output_is_hi(B_BLOKADA) == 0 ) {
	
	Wlacznik = Wlacznik + 1;
	supla_log(LOG_DEBUG, "Wlacznik = %i", Wlacznik);
	
	if ( Wlacznik == 1 && hi == 1 ) { supla_log(LOG_DEBUG, "Set dimmer 1 hi=1");
					Licznik = 0;
					//os_timer_disarm(&dimmer_timer);
					//os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
					//os_timer_arm(&dimmer_timer, 20, 1);
					//os_timer_disarm(&work_timer);
					//os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					//os_timer_arm(&work_timer, 30000, 0);
					};
		
	
	
	if ( hi == 2 && Start == 0 ) { supla_log(LOG_DEBUG, "Set dimmer 1 hi=2");
					Licznik = 0;
					Start = 1;
					//os_timer_disarm(&dimmer_timer);
					//os_timer_setfn(&dimmer_timer, (os_timer_func_t *)dimmer_timer_ON_cb, NULL);
					//os_timer_arm(&dimmer_timer, 20, 1);
					//os_timer_disarm(&work_timer);
					//os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					//os_timer_arm(&work_timer, 30000, 0);
					};
	if ( hi == 0 ) { Wlacznik = 0;
					os_timer_disarm(&work_timer);
					os_timer_setfn(&work_timer, (os_timer_func_t *)work_timer_cb, NULL);
					os_timer_arm(&work_timer, 30000, 0); };
};
}

void ICACHE_FLASH_ATTR supla_esp_board_pwm_init(void) {
	//supla_esp_channel_set_rgbw_value(0, 0, 0, supla_esp_state.brightness[0], 0, 0);
	Jasnosc = supla_esp_state.brightness[0];
	supla_log(LOG_DEBUG, "Jasnosc init = %i", Jasnosc);
}

void ICACHE_FLASH_ATTR supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {

	*channel_count = 6;

	channels[0].Type = SUPLA_CHANNELTYPE_DIMMER;
	channels[0].Number = 0;
	channels[0].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	supla_esp_channel_rgbw_to_value(channels[0].value, 0, 0, supla_esp_state.brightness[0]);
	
	channels[1].Number = 1;
	channels[1].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[1].FuncList = 0;
	channels[1].Default = 0;
	channels[1].value[0] = 0;
	
	channels[2].Number = 2;
	channels[2].Type = SUPLA_CHANNELTYPE_SENSORNO;
	channels[2].FuncList = 0;
	channels[2].Default = 0;
	channels[2].value[0] = 0;

	channels[3].Number = 3;
	channels[3].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[3].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[3].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[3].Default = SUPLA_CHANNELFNC_POWERSWITCH;
	channels[3].value[0] = supla_esp_gpio_relay_on(B_HARMONOGRAM);
	
	channels[4].Number = 4;
	channels[4].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[4].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[4].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[4].Default = 0;
	channels[4].value[0] = supla_esp_gpio_relay_on(B_UPD_PORT);
	
	channels[5].Number = 5;
	channels[5].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[5].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[5].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[5].Default = 0;
	channels[5].value[0] = supla_esp_gpio_relay_on(B_BLOKADA);
}

char ICACHE_FLASH_ATTR supla_esp_board_set_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness ) {

	dimmer_brightness = *Brightness;
	
	if ( dimmer_brightness > 100 )
		dimmer_brightness = 100;
		
	//supla_esp_pwm_set_percent_duty(dimmer_brightness, 100, 0);
	supla_log(LOG_DEBUG, "Set dimmer : %i", dimmer_brightness);
	
	return 1;
}


void ICACHE_FLASH_ATTR supla_esp_board_get_rgbw_value(int ChannelNumber, int *Color, float *ColorBrightness, float *Brightness) {

	if ( Brightness != NULL ) {
			*Brightness = dimmer_brightness;
			supla_log(LOG_DEBUG, "Get dimmer : %i", dimmer_brightness);
	}

}

void ICACHE_FLASH_ATTR supla_esp_board_send_channel_values_with_delay(void *srpc) {
	
	supla_esp_channel_value_changed(3, supla_esp_gpio_relay_on(B_HARMONOGRAM));
	supla_esp_channel_value_changed(4, supla_esp_gpio_relay_on(B_UPD_PORT));
	supla_esp_channel_value_changed(5, supla_esp_gpio_relay_on(B_BLOKADA));
	
}

void ICACHE_FLASH_ATTR supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi) {
			
		supla_log(LOG_DEBUG, "supla_esp_board_gpiooutput_set_hi port = %i, hi = %i", port, hi);
		
		HRM_channel = 3;
		UPD_channel = 4;
		BLK_channel = 5;
				
if ( port == B_UPD_PORT ) {	

	supla_esp_state.Relay[UPD_channel] = hi;
	supla_esp_save_state(SAVE_STATE_DELAY);
	supla_esp_channel_value_changed(UPD_channel, supla_esp_state.Relay[UPD_channel]);
	supla_esp_cfg_save(&supla_esp_cfg);
	supla_esp_channel_value_changed(UPD_channel, hi);

	if ( hi == 1 ) {
	
		supla_log(LOG_DEBUG, "update, port = %i", port);
		
		supla_esp_cfg.FirmwareUpdate = 1; 
		supla_esp_cfg_save(&supla_esp_cfg);

		os_timer_disarm(&value_timer1);
		os_timer_setfn(&value_timer1, (os_timer_func_t *)supla_esp_baord_value_timer1_cb, NULL);
		os_timer_arm(&value_timer1, 4000, 0);
	};
		

}; 

		
if ( port == B_HARMONOGRAM ) {	
			
	supla_esp_state.Relay[HRM_channel] = hi;
	supla_esp_save_state(SAVE_STATE_DELAY);
	supla_esp_channel_value_changed(HRM_channel, supla_esp_state.Relay[HRM_channel]);
	supla_esp_cfg_save(&supla_esp_cfg);
	supla_esp_channel_value_changed(HRM_channel, hi);
				
};
	
if ( port == B_BLOKADA ) {	
			
		supla_esp_state.Relay[BLK_channel] = hi;
		supla_esp_save_state(SAVE_STATE_DELAY);
		supla_esp_channel_value_changed(BLK_channel, supla_esp_state.Relay[BLK_channel]);
		supla_esp_cfg_save(&supla_esp_cfg);
		supla_esp_channel_value_changed(BLK_channel, hi);
	}
}
