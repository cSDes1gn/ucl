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
#include <stdlib.h>

typedef int i2c_status_t;

#define I2C_SUCCESS (i2c_status_t)0
#define I2C_INIT_FAILURE (i2c_status_t)1
#define I2C_TIMEOUT (i2c_status_t)2

enum I2CPrescaler {
  I2C_PRESCALE_1 = 1,
  I2C_PRESCALE_4 = 4,
  I2C_PRESCALE_16 = 16,
  I2C_PRESCALE_64 = 64,
};

typedef struct i2c_device_t {
  enum I2CPrescaler prescaler; // clock prescaler.
  uint32_t scl_freq; // i2c clock drive frequency. NOTE: the slave CPU clock
                     // frequency must be at least 16 times higher than the SCL
                     // frequency
  uint32_t slave_fcpu; // slave max clock frequency NOTE: consider all slaves on
                       // the bus and use the slowest clock speed here.
} i2c_device_t;

i2c_status_t i2c_init(i2c_device_t *device);
i2c_status_t i2c_block_read_byte(uint8_t addr, uint8_t data);
i2c_status_t i2c_block_read_block(uint8_t addr, uint8_t *buffer, size_t size);
i2c_status_t i2c_block_write_byte(uint8_t addr, uint8_t data);
i2c_status_t i2c_block_write_block(uint8_t addr, uint8_t *buffer, size_t size);
i2c_status_t i2c_close(void);
i2c_status_t i2c_sleep(void);
i2c_status_t i2c_wake(void);

#endif // __I2C_H__
