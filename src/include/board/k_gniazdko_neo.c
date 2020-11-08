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

#include "public_key_in_c_code"		// moj klucz

ETSTimer value_timer1;
int UPD_channel;

void supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {

	ets_snprintf(buffer, buffer_size, "GNIAZDKO NEO");

}

void supla_esp_baord_value_timer1_cb(void *timer_arg) {
	
	supla_log(LOG_DEBUG, "TIMER update - restart");
	supla_system_restart();
	
}

void supla_esp_board_gpio_init(void) {
		
	supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
	supla_input_cfg[0].gpio_id = B_CFG_PORT;
	supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
	supla_input_cfg[0].relay_gpio_id = B_RELAY1_PORT;
	supla_input_cfg[0].channel = 0;

	// ---------------------------------------
	// ---------------------------------------

    supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
    supla_relay_cfg[0].flags = RELAY_FLAG_RESTORE_FORCE;
    supla_relay_cfg[0].channel = 0;

	//---------------------------------------

	supla_relay_cfg[1].gpio_id = B_UPD_PORT;
	supla_relay_cfg[1].flags = RELAY_FLAG_VIRTUAL_GPIO;
	supla_relay_cfg[1].channel = 1;
    
}



void supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels, unsigned char *channel_count) {

	*channel_count = 2;

	channels[0].Number = 0;
	channels[0].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[0].FuncList = SUPLA_BIT_FUNC_POWERSWITCH \
								| SUPLA_BIT_FUNC_LIGHTSWITCH;
	channels[0].Flags = SUPLA_CHANNEL_FLAG_CHANNELSTATE;
	channels[0].Default = SUPLA_CHANNELFNC_POWERSWITCH;
	channels[0].value[0] = supla_esp_gpio_relay_on(B_RELAY1_PORT);

	channels[1].Number = 1;
	channels[1].Type = SUPLA_CHANNELTYPE_RELAY;
	channels[1].FuncList = SUPLA_BIT_FUNC_POWERSWITCH;
	channels[1].Default = 0;
	channels[1].value[0] = supla_esp_gpio_relay_on(B_UPD_PORT);

}

void ICACHE_FLASH_ATTR supla_esp_board_on_connect(void) {
  
  if ( supla_esp_gpio_output_is_hi(12) == 1 ) { supla_esp_gpio_set_led(0, 0, 0); }
  if ( supla_esp_gpio_output_is_hi(12) == 0 ) { supla_esp_gpio_set_led(1, 0, 0); }
	  
}

void supla_esp_board_send_channel_values_with_delay(void *srpc) {

	supla_esp_channel_value_changed(0, supla_esp_gpio_relay_on(B_RELAY1_PORT));
	supla_esp_channel_value_changed(1, supla_esp_gpio_relay_on(B_UPD_PORT));

}

void ICACHE_FLASH_ATTR supla_esp_board_gpio_relay_switch(void* _input_cfg,
    char hi)
{

    supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

    if (input_cfg->relay_gpio_id != 255) {

        // supla_log(LOG_DEBUG, "RELAY");

        supla_esp_gpio_relay_hi(input_cfg->relay_gpio_id, hi, 0);

        if (input_cfg->channel != 255)
            supla_esp_channel_value_changed(
                input_cfg->channel,
                supla_esp_gpio_relay_is_hi(input_cfg->relay_gpio_id));
    }
}

void ICACHE_FLASH_ATTR supla_esp_board_gpio_on_input_active(void* _input_cfg)
{

    supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

  if ( input_cfg->type == INPUT_TYPE_BTN_MONOSTABLE 	//wlaczanie przy zboczu narastajacym
		|| input_cfg->type == INPUT_TYPE_BTN_BISTABLE ) {

		supla_log(LOG_DEBUG, "RELAY");
		supla_esp_board_gpio_relay_switch(input_cfg, 255);
		
		} else if ( input_cfg->type == INPUT_TYPE_SENSOR
				&&  input_cfg->channel != 255 ) {

		supla_esp_channel_value_changed(input_cfg->channel, 1);

	}

    input_cfg->last_state = 1;
}

void ICACHE_FLASH_ATTR
supla_esp_board_gpio_on_input_inactive(void* _input_cfg)
{

    supla_input_cfg_t* input_cfg = (supla_input_cfg_t*)_input_cfg;

    if ( input_cfg->type == INPUT_TYPE_BTN_BISTABLE ) {		//wlaczanie przy zboczu narastajacym

		supla_esp_board_gpio_relay_switch(input_cfg, 255);

    } else if ( input_cfg->type == INPUT_TYPE_SENSOR
			    &&  input_cfg->channel != 255 ) {
		supla_esp_channel_value_changed(input_cfg->channel, 0);

	}

    input_cfg->last_state = 0;
}

void supla_esp_board_gpiooutput_set_hi(uint8 port, uint8 hi) {
	
	if ( port == B_UPD_PORT ) { 
	
		UPD_channel = 1;
	
		if ( hi == 1 ) {
	
			supla_log(LOG_DEBUG, "update, port = %i", port);
		
			if ( supla_esp_cfg.FirmwareUpdate == 1 ) {
			
				supla_esp_state.Relay[UPD_channel] = 1;
				supla_log(LOG_DEBUG, "value_changed upd - 1");
				supla_esp_save_state(SAVE_STATE_DELAY);
				supla_esp_channel_value_changed(UPD_channel, supla_esp_state.Relay[UPD_channel]);
				os_timer_disarm(&value_timer1);
				os_timer_setfn(&value_timer1, (os_timer_func_t *)supla_esp_baord_value_timer1_cb, NULL);
				os_timer_arm(&value_timer1, 4000, 0);
				
			};
		
			if ( supla_esp_cfg.FirmwareUpdate == 0 ) {
				
				supla_esp_cfg.FirmwareUpdate = 1; 
				supla_esp_cfg_save(&supla_esp_cfg);
				supla_esp_channel_value_changed(UPD_channel, 1);
				supla_log(LOG_DEBUG, "value_changed upd - 0");
				
			};
		};
	};
	
	if ( port == B_RELAY1_PORT ) {
		
		hi =!supla_esp_gpio_output_is_hi(B_RELAY1_PORT);
		supla_esp_gpio_set_led(hi, 1, 1);
		
	};
}