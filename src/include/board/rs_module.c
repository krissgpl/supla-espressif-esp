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

#include "supla_ds18b20.h"
#include "supla_esp.h"

#define B_CFG_PORT 0
#define B_RELAY1_PORT 4
#define B_RELAY2_PORT 5
#define B_BTN1_PORT 13
#define B_BTN2_PORT 14
#define B_SENSOR_PORT1 12

const uint8_t rsa_public_key_bytes[512] = {
    0xcb, 0x5e, 0xdd, 0x04, 0x2e, 0xd9, 0x7d, 0xf1, 0x13, 0xb9, 0x0f, 0xf7,
    0x43, 0x85, 0xe5, 0x9e, 0xda, 0xef, 0xf9, 0xdd, 0xec, 0x78, 0x39, 0x06,
    0x69, 0xa4, 0x9a, 0x97, 0x70, 0x8b, 0xde, 0x82, 0x65, 0xc3, 0x03, 0xb2,
    0x6f, 0x32, 0x57, 0xad, 0x90, 0xc1, 0xa9, 0x2f, 0x5a, 0x80, 0xe8, 0xf0,
    0x73, 0x7d, 0xcd, 0x45, 0x80, 0x18, 0x0c, 0x3a, 0xd1, 0x75, 0x0d, 0x19,
    0x27, 0x71, 0x42, 0xf6, 0x1b, 0xda, 0xaa, 0x82, 0xca, 0x48, 0x5c, 0x25,
    0xc2, 0xdc, 0xeb, 0x1e, 0x02, 0x5f, 0xde, 0x01, 0x77, 0xf7, 0x31, 0x54,
    0xd8, 0x3e, 0x75, 0xcd, 0xd7, 0x8a, 0x3c, 0xdf, 0xd0, 0xfb, 0xde, 0x04,
    0x2b, 0xa0, 0x29, 0xa4, 0xa3, 0x39, 0xf2, 0xa3, 0xd6, 0xc5, 0xee, 0xf0,
    0xba, 0x2d, 0xe8, 0xd7, 0x30, 0x84, 0x82, 0x2e, 0xcb, 0x4f, 0xcf, 0x72,
    0x38, 0x59, 0x19, 0x9b, 0x75, 0xc4, 0x1b, 0x8a, 0x4e, 0x08, 0x35, 0xba,
    0x05, 0x11, 0xe7, 0x7b, 0x78, 0x2b, 0x32, 0x18, 0xc6, 0xb5, 0x5e, 0xa6,
    0xf6, 0x90, 0x9f, 0x98, 0xd7, 0x1e, 0x39, 0xe4, 0xc4, 0xef, 0x11, 0x68,
    0xf5, 0x9a, 0x37, 0xbc, 0xa9, 0x97, 0xdb, 0x8a, 0x4f, 0x7e, 0xd2, 0x8f,
    0x23, 0x95, 0xda, 0x9f, 0x73, 0x99, 0x78, 0x6f, 0x37, 0x26, 0x50, 0x8a,
    0xdc, 0x21, 0x8c, 0x7b, 0x67, 0x34, 0x7c, 0x42, 0x00, 0x27, 0xf4, 0xb8,
    0xcb, 0x35, 0x49, 0x15, 0x7f, 0x2c, 0xc8, 0x1b, 0x6c, 0x03, 0x09, 0xd8,
    0xdc, 0x54, 0xb8, 0x2a, 0xa9, 0x1f, 0x4d, 0x89, 0x85, 0xd5, 0x97, 0xf6,
    0xc0, 0x4e, 0xd6, 0x13, 0xc6, 0xa5, 0x0b, 0xed, 0xaa, 0x91, 0x13, 0x55,
    0x6a, 0x52, 0x51, 0x79, 0xfa, 0x0c, 0x8d, 0x73, 0xc1, 0xaf, 0xcd, 0x64,
    0x98, 0x91, 0x44, 0x6a, 0xd5, 0xe2, 0x0d, 0x72, 0xac, 0x9d, 0xce, 0xc4,
    0xff, 0x4f, 0x21, 0x11, 0xc5, 0xb6, 0x86, 0x39, 0x8f, 0x22, 0xc8, 0x64,
    0x95, 0xfc, 0x35, 0xbe, 0xdc, 0x82, 0x5a, 0x1f, 0xbf, 0xea, 0x8e, 0xa1,
    0x55, 0x1f, 0x4f, 0x10, 0x9c, 0x2b, 0xde, 0x35, 0xc4, 0x1e, 0x60, 0xc3,
    0x1b, 0x40, 0x0e, 0x29, 0xaa, 0x64, 0x1b, 0x1d, 0xf8, 0xea, 0xaf, 0x0b,
    0x22, 0x6a, 0xfb, 0x95, 0x18, 0x57, 0x42, 0x41, 0x7d, 0x4a, 0x4f, 0xb0,
    0xc0, 0xb7, 0x39, 0xc8, 0x2a, 0x0a, 0x5a, 0x44, 0x2a, 0x47, 0x34, 0x6f,
    0x27, 0x21, 0x12, 0x30, 0x33, 0x38, 0x88, 0x1e, 0x7a, 0x6e, 0xb9, 0x01,
    0xdc, 0xdc, 0xb5, 0x37, 0xb9, 0xbc, 0x76, 0x7f, 0x3c, 0x07, 0x1d, 0x55,
    0x4d, 0xc1, 0x70, 0x52, 0xe0, 0xb8, 0x72, 0xbe, 0x4e, 0x7a, 0xfe, 0x1b,
    0xf6, 0x6c, 0x12, 0xf3, 0x87, 0xcd, 0xe2, 0x7f, 0x1b, 0xc4, 0x29, 0x2d,
    0x62, 0x80, 0xfb, 0xee, 0x61, 0x21, 0x03, 0x4a, 0xe2, 0xbc, 0x11, 0xf0,
    0xc5, 0x7f, 0x9a, 0xf9, 0x45, 0xd5, 0x79, 0xe8, 0x72, 0x6e, 0xbb, 0x92,
    0x59, 0x24, 0xd2, 0xe6, 0xbf, 0x8f, 0xe5, 0xfd, 0x83, 0x46, 0xcb, 0xb2,
    0x46, 0x21, 0x5f, 0xe8, 0x6b, 0x51, 0xf8, 0xf8, 0xbc, 0xad, 0x94, 0x51,
    0xc6, 0x70, 0x55, 0xb6, 0xd4, 0xdb, 0x76, 0xd5, 0xf2, 0x33, 0xc0, 0x57,
    0xb4, 0xd4, 0x57, 0x2a, 0x62, 0xa2, 0x43, 0xa1, 0x14, 0x28, 0xcc, 0x92,
    0xf3, 0xdc, 0x2c, 0x4f, 0xe2, 0xa3, 0x6c, 0x43, 0x06, 0x18, 0x95, 0x02,
    0x9d, 0x98, 0xdd, 0xdf, 0x65, 0x3e, 0x58, 0x17, 0xd3, 0xc4, 0x29, 0xa8,
    0x1f, 0xae, 0x1d, 0x44, 0xeb, 0x1e, 0xee, 0x51, 0x30, 0x86, 0xc2, 0xab,
    0x08, 0x3e, 0x1a, 0xc2, 0xa0, 0xdb, 0x5a, 0x7f, 0x49, 0x69, 0x6d, 0xb0,
    0x1b, 0x72, 0xfd, 0x1f, 0x9e, 0x3a, 0x2b, 0xb1, 0xca, 0x61, 0x79, 0x0a,
    0xd8, 0x60, 0x61, 0xd1, 0x53, 0x85, 0x31, 0x43};

void ICACHE_FLASH_ATTR supla_esp_board_set_device_name(char *buffer,
                                                       uint8 buffer_size) {
  ets_snprintf(buffer, buffer_size, "SUPLA-RS-MODULE");
}

void ICACHE_FLASH_ATTR supla_esp_board_gpio_init(void) {
  supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
  supla_input_cfg[0].gpio_id = B_CFG_PORT;
  supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;

  supla_input_cfg[1].type = INPUT_TYPE_SENSOR;
  supla_input_cfg[1].gpio_id = B_SENSOR_PORT1;
  supla_input_cfg[1].channel = 1;

  supla_input_cfg[2].type = INPUT_TYPE_BTN_MONOSTABLE_RS;
  supla_input_cfg[2].gpio_id = B_BTN1_PORT;
  supla_input_cfg[2].relay_gpio_id = B_RELAY1_PORT;

  supla_input_cfg[3].type = INPUT_TYPE_BTN_MONOSTABLE_RS;
  supla_input_cfg[3].gpio_id = B_BTN2_PORT;
  supla_input_cfg[3].relay_gpio_id = B_RELAY2_PORT;

  // ---------------------------------------
  // ---------------------------------------

  supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
  supla_relay_cfg[0].channel = 0;

  supla_relay_cfg[1].gpio_id = B_RELAY2_PORT;
  supla_relay_cfg[1].channel = 0;

  supla_rs_cfg[0].up = &supla_relay_cfg[0];
  supla_rs_cfg[0].down = &supla_relay_cfg[1];
}

void ICACHE_FLASH_ATTR supla_esp_board_set_channels(
    TDS_SuplaDeviceChannel_B *channels, unsigned char *channel_count) {
  *channel_count = 3;

  channels[0].Number = 0;
  channels[0].Type = SUPLA_CHANNELTYPE_RELAY;
  channels[0].FuncList = SUPLA_BIT_RELAYFUNC_CONTROLLINGTHEROLLERSHUTTER;
  channels[0].Default = SUPLA_CHANNELFNC_CONTROLLINGTHEROLLERSHUTTER;
  channels[0].value[0] = (*supla_rs_cfg[0].position) - 1;

  channels[1].Number = 1;
  channels[1].Type = SUPLA_CHANNELTYPE_SENSORNO;

  channels[2].Number = 2;
  channels[2].Type = SUPLA_CHANNELTYPE_THERMOMETERDS18B20;
  channels[2].FuncList = 0;
  channels[2].Default = 0;

  supla_get_temperature(channels[2].value);
}

void ICACHE_FLASH_ATTR
supla_esp_board_send_channel_values_with_delay(void *srpc) {}
