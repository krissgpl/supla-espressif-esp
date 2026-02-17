/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 */

#include <gpio.h>

#include "supla_esp.h"
#include "supla_hlw8012.h"

static supla_hlw8012_t hlw;
static ETSTimer hlw_mode_timer;

#define SP111_HLW_CURRENT_MULTIPLIER 0.0025f
#define SP111_HLW_VOLTAGE_MULTIPLIER 0.50f
#define SP111_HLW_POWER_MULTIPLIER 1.0f

static void ICACHE_FLASH_ATTR supla_esp_board_hlw_apply_sel(void) {
  if (hlw.cf1_mode == SUPLA_HLW8012_CF1_MODE_CURRENT) {
    supla_esp_gpio_set_hi(B_HLW_SEL_PORT, 1);
  } else {
    supla_esp_gpio_set_hi(B_HLW_SEL_PORT, 0);
  }
}

static void ICACHE_FLASH_ATTR supla_esp_board_hlw_mode_timer_cb(void *arg) {
  (void)arg;
  if (hlw.cf1_mode == SUPLA_HLW8012_CF1_MODE_CURRENT) {
    supla_hlw8012_set_cf1_mode(&hlw, SUPLA_HLW8012_CF1_MODE_VOLTAGE);
  } else {
    supla_hlw8012_set_cf1_mode(&hlw, SUPLA_HLW8012_CF1_MODE_CURRENT);
  }

  supla_esp_board_hlw_apply_sel();
}

void ICACHE_FLASH_ATTR supla_esp_board_starting(void) {
  supla_hlw8012_config_t cfg;
  memset(&cfg, 0, sizeof(cfg));

  float voltage_cal = supla_esp_cfg.HlwVoltageCalibration == 0
                          ? 1.0f
                          : supla_esp_cfg.HlwVoltageCalibration / 1000.0f;
  float energy_cal = supla_esp_cfg.HlwEnergyCalibration == 0
                         ? 1.0f
                         : supla_esp_cfg.HlwEnergyCalibration / 1000.0f;

  cfg.current_multiplier = SP111_HLW_CURRENT_MULTIPLIER;
  cfg.voltage_multiplier = SP111_HLW_VOLTAGE_MULTIPLIER * voltage_cal;
  cfg.power_multiplier = SP111_HLW_POWER_MULTIPLIER * energy_cal;
  cfg.stale_time_us = 3000000;

  supla_hlw8012_init(&hlw, &cfg);
  supla_hlw8012_set_cf1_mode(&hlw, SUPLA_HLW8012_CF1_MODE_VOLTAGE);
  supla_esp_board_hlw_apply_sel();

  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(B_HLW_CF_PORT));
  GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, BIT(B_HLW_CF1_PORT));
  gpio_pin_intr_state_set(GPIO_ID_PIN(B_HLW_CF_PORT), GPIO_PIN_INTR_ANYEDGE);
  gpio_pin_intr_state_set(GPIO_ID_PIN(B_HLW_CF1_PORT), GPIO_PIN_INTR_ANYEDGE);

  os_timer_disarm(&hlw_mode_timer);
  os_timer_setfn(&hlw_mode_timer,
                 (os_timer_func_t *)supla_esp_board_hlw_mode_timer_cb, NULL);
  os_timer_arm(&hlw_mode_timer, 2000, 1);
}

void supla_esp_board_set_device_name(char *buffer, uint8 buffer_size) {
  ets_snprintf(buffer, buffer_size, "SP111");
}

void supla_esp_board_gpio_init(void) {
  supla_input_cfg[0].type = INPUT_TYPE_BTN_MONOSTABLE;
  supla_input_cfg[0].gpio_id = B_CFG_PORT;
  supla_input_cfg[0].flags = INPUT_FLAG_PULLUP | INPUT_FLAG_CFG_BTN;
  supla_input_cfg[0].relay_gpio_id = B_RELAY1_PORT;
  supla_input_cfg[0].channel = 0;

  supla_relay_cfg[0].gpio_id = B_RELAY1_PORT;
  supla_relay_cfg[0].flags = RELAY_FLAG_RESTORE_FORCE;
  supla_relay_cfg[0].channel = 0;

  supla_input_cfg[1].type = INPUT_TYPE_CUSTOM;
  supla_input_cfg[1].gpio_id = B_HLW_CF_PORT;
  supla_input_cfg[1].flags = INPUT_FLAG_DISABLE_INTR;

  supla_input_cfg[2].type = INPUT_TYPE_CUSTOM;
  supla_input_cfg[2].gpio_id = B_HLW_CF1_PORT;
  supla_input_cfg[2].flags = INPUT_FLAG_DISABLE_INTR;

  PIN_PULLUP_EN(PERIPHS_IO_MUX_GPIO0_U);
}

uint8 supla_esp_board_intr_handler(uint32 gpio_status) {
  uint8 result = 0;

  if (gpio_status & BIT(B_HLW_CF_PORT)) {
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS, gpio_status & BIT(B_HLW_CF_PORT));
    if (gpio__input_get(B_HLW_CF_PORT)) {
      supla_hlw8012_on_cf_pulse(&hlw, system_get_time());
    }
    gpio_pin_intr_state_set(GPIO_ID_PIN(B_HLW_CF_PORT), GPIO_PIN_INTR_ANYEDGE);
    result = 1;
  }

  if (gpio_status & BIT(B_HLW_CF1_PORT)) {
    GPIO_REG_WRITE(GPIO_STATUS_W1TC_ADDRESS,
                   gpio_status & BIT(B_HLW_CF1_PORT));
    if (gpio__input_get(B_HLW_CF1_PORT)) {
      supla_hlw8012_on_cf1_pulse(&hlw, system_get_time());
    }
    gpio_pin_intr_state_set(GPIO_ID_PIN(B_HLW_CF1_PORT),
                            GPIO_PIN_INTR_ANYEDGE);
    result = 1;
  }

  return result;
}

uint8 ICACHE_FLASH_ATTR supla_esp_board_get_measurements(
    unsigned char channel_number, TElectricityMeter_ExtendedValue_V2 *ev) {
  supla_hlw8012_values_t values;

  if (channel_number != ELECTRICITY_METER_CHANNEL_OFFSET || ev == NULL) {
    return 0;
  }

  memset(ev, 0, sizeof(TElectricityMeter_ExtendedValue_V2));
  supla_hlw8012_get_values(&hlw, system_get_time(), &values);

  ev->measured_values = EM_VAR_FORWARD_ACTIVE_ENERGY;
  ev->total_forward_active_energy[0] = (_supla_int64_t)(values.energy_wh * 100);

  if (values.power_w > 0.01f) {
    ev->measured_values |= EM_VAR_POWER_ACTIVE;
    ev->m[0].power_active[0] = (_supla_int_t)(values.power_w * 100000);
  }

  if (values.voltage_v > 0.01f) {
    ev->measured_values |= EM_VAR_VOLTAGE;
    ev->m[0].voltage[0] = (_supla_int16_t)(values.voltage_v * 100);
  }

  if (values.current_a > 0.001f) {
    ev->measured_values |= EM_VAR_CURRENT;
    ev->m[0].current[0] = (_supla_int16_t)(values.current_a * 1000);
  }

  if (values.power_factor > 0.001f) {
    ev->measured_values |= EM_VAR_POWER_FACTOR;
    ev->m[0].power_factor[0] = (_supla_int16_t)(values.power_factor * 1000);
  }

  if (values.voltage_v > 0.01f && values.current_a > 0.001f) {
    ev->measured_values |= EM_VAR_POWER_APPARENT;
    ev->m[0].power_apparent[0] =
        (_supla_int_t)(values.voltage_v * values.current_a * 100000);
  }

  ev->m_count = 1;
  return 1;
}

void supla_esp_board_set_channels(TDS_SuplaDeviceChannel_C *channels,
                                  unsigned char *channel_count) {
  *channel_count = 2;

  channels[0].Number = 0;
  channels[0].Type = SUPLA_CHANNELTYPE_RELAY;
  channels[0].FuncList = SUPLA_BIT_FUNC_POWERSWITCH | SUPLA_BIT_FUNC_LIGHTSWITCH;
  channels[0].Default = SUPLA_CHANNELFNC_POWERSWITCH;
  channels[0].value[0] = supla_esp_gpio_relay_on(B_RELAY1_PORT);

  channels[1].Number = ELECTRICITY_METER_CHANNEL_OFFSET;
  channels[1].Type = SUPLA_CHANNELTYPE_IMPULSE_COUNTER;
  channels[1].Default = SUPLA_CHANNELFNC_ELECTRICITY_METER;
}

void supla_esp_board_send_channel_values_with_delay(void *srpc) {
  (void)srpc;
  supla_esp_channel_value_changed(0, supla_esp_gpio_relay_on(B_RELAY1_PORT));
}

char *ICACHE_FLASH_ATTR supla_esp_board_cfg_html_template(
    char dev_name[25], const char mac[6], const char data_saved) {
  (void)mac;
  static char html[] =
      "<!DOCTYPE html><meta charset=\"UTF-8\"><html><body>"
      "<h2>%s</h2>%s"
      "<form method=\"post\">"
      "SSID:<input name=\"sid\" value=\"%s\"><br>"
      "Haslo WiFi:<input name=\"wpw\"><br>"
      "Serwer:<input name=\"svr\" value=\"%s\"><br>"
      "Email:<input name=\"eml\" value=\"%s\"><br>"
      "Kalibracja napiecia (promile):<input name=\"hvc\" value=\"%u\"><br>"
      "Kalibracja energii (promile):<input name=\"hec\" value=\"%u\"><br>"
      "<button type=\"submit\">SAVE</button>"
      "</form></body></html>";

  int bufflen = strlen(html) + strlen(dev_name) + strlen(supla_esp_cfg.WIFI_SSID) +
                strlen(supla_esp_cfg.Server) + strlen(supla_esp_cfg.Email) + 80;
  char *buffer = (char *)malloc(bufflen);

  ets_snprintf(buffer, bufflen, html, dev_name,
               data_saved == 1 ? "<p>Data saved</p>" : "", supla_esp_cfg.WIFI_SSID,
               supla_esp_cfg.Server, supla_esp_cfg.Email,
               supla_esp_cfg.HlwVoltageCalibration == 0
                   ? 1000
                   : supla_esp_cfg.HlwVoltageCalibration,
               supla_esp_cfg.HlwEnergyCalibration == 0
                   ? 1000
                   : supla_esp_cfg.HlwEnergyCalibration);

  return buffer;
}
