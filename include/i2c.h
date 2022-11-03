/**
 * @file i2c.h
 * @author ztnel (christian911@sympatico.ca)
 * @brief
 * @version 0.1
 * @date 2022-10
 *
 * @copyright Copyright © 2022 Christian Sargusingh
 *
 */

#ifndef __I2C_H__
#define __I2C_H__

#include <stdint.h>

typedef int i2c_status_t;

#define I2C_SUCCESS (i2c_status_t)0
#define I2C_INIT_FAILURE (i2c_status_t)1
#define I2C_TIMEOUT (i2c_status_t)2

enum I2CPrescaler {
  PRESCALER_1 = 1,
  PRESCALER_4 = 4,
  PRESCALER_16 = 16,
  PRESCALER_64 = 64
};

void i2c_wake(void);
i2c_status_t i2c_init(uint32_t f_i2c, enum I2CPrescaler prescaler);
void i2c_start(uint8_t i2c_addr);
void i2c_stop(void);
void i2c_write(uint8_t byte);
void i2c_sleep(void);

#endif // __I2C_H__
