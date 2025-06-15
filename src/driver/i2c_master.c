#include "i2c_master.h"

void i2c_master_gpio_init(void) {
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO2_U, FUNC_GPIO2);
    PIN_FUNC_SELECT(PERIPHS_IO_MUX_GPIO0_U, FUNC_GPIO0);
}

void i2c_master_init(void) {
    i2c_master_gpio_init();
    gpio_output_set(0, (1 << I2C_MASTER_SDA_GPIO) | (1 << I2C_MASTER_SCL_GPIO), 0, 0);
}

void i2c_master_start(void) {
    gpio_output_set(0, (1 << I2C_MASTER_SDA_GPIO), 0, 0);
    gpio_output_set(0, (1 << I2C_MASTER_SCL_GPIO), 0, 0);
}

void i2c_master_stop(void) {
    gpio_output_set(0, (1 << I2C_MASTER_SCL_GPIO), 0, 0);
    gpio_output_set(0, (1 << I2C_MASTER_SDA_GPIO), 0, 0);
}

void i2c_master_writeByte(uint8_t data) {
    for (int i = 0; i < 8; i++) {
        if (data & (1 << (7 - i))) {
            gpio_output_set(1 << I2C_MASTER_SDA_GPIO, 0, 0, 0);
        } else {
            gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO, 0, 0);
        }
        gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 0, 0);
        gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 0, 0);
    }
}

uint8_t i2c_master_readByte(void) {
    uint8_t data = 0;
    for (int i = 0; i < 8; i++) {
        gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO, 0, 0);
        gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 0, 0);
        if (gpio_input_get() & (1 << I2C_MASTER_SDA_GPIO)) {
            data |= (1 << (7 - i));
        }
        gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 0, 0);
    }
    return data;
}

bool i2c_master_checkAck(void) {
    gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO, 0, 0);
    gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 0, 0);
    bool ack = !(gpio_input_get() & (1 << I2C_MASTER_SDA_GPIO));
    gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 0, 0);
    return ack;
}

void i2c_master_send_ack(void) {
    gpio_output_set(0, 1 << I2C_MASTER_SDA_GPIO, 0, 0);
    gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 0, 0);
    gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 0, 0);
}

void i2c_master_send_nack(void) {
    gpio_output_set(1 << I2C_MASTER_SDA_GPIO, 0, 0, 0);
    gpio_output_set(1 << I2C_MASTER_SCL_GPIO, 0, 0, 0);
    gpio_output_set(0, 1 << I2C_MASTER_SCL_GPIO, 0, 0);
}