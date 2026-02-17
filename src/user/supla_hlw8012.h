/*
 Copyright (C) AC SOFTWARE SP. Z O.O.

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 2
 of the License, or (at your option) any later version.
 */

#ifndef SUPLA_HLW8012_H_
#define SUPLA_HLW8012_H_

#include "supla_esp.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
  SUPLA_HLW8012_CF1_MODE_VOLTAGE = 0,
  SUPLA_HLW8012_CF1_MODE_CURRENT = 1
} supla_hlw8012_cf1_mode_t;

typedef struct {
  float power_multiplier;
  float voltage_multiplier;
  float current_multiplier;
  uint32 stale_time_us;
} supla_hlw8012_config_t;

typedef struct {
  float power_w;
  float voltage_v;
  float current_a;
  float energy_wh;
  float power_factor;
} supla_hlw8012_values_t;

typedef struct {
  supla_hlw8012_config_t cfg;
  uint8 cf1_mode;
  uint32 last_cf_ts;
  uint32 last_cf1_ts;
  uint32 cf_period_us;
  uint32 cf1_period_us;
  float cached_voltage_v;
  float cached_current_a;
  float energy_wh;
} supla_hlw8012_t;

void ICACHE_FLASH_ATTR supla_hlw8012_init(supla_hlw8012_t *hlw,
                                          const supla_hlw8012_config_t *cfg);
void ICACHE_FLASH_ATTR supla_hlw8012_set_cf1_mode(supla_hlw8012_t *hlw,
                                                  uint8 cf1_mode);
void ICACHE_FLASH_ATTR supla_hlw8012_on_cf_pulse(supla_hlw8012_t *hlw,
                                                 uint32 timestamp_us);
void ICACHE_FLASH_ATTR supla_hlw8012_on_cf1_pulse(supla_hlw8012_t *hlw,
                                                  uint32 timestamp_us);
void ICACHE_FLASH_ATTR supla_hlw8012_reset_energy(supla_hlw8012_t *hlw);
void ICACHE_FLASH_ATTR supla_hlw8012_get_values(supla_hlw8012_t *hlw,
                                                uint32 timestamp_us,
                                                supla_hlw8012_values_t *values);

#ifdef __cplusplus
}
#endif

#endif /* SUPLA_HLW8012_H_ */
