/**
 * @file i2c.c
 * @author ztnel (christian911@sympatico.ca)
 * @brief
 * @version 0.1
 * @date 2022-11
 *
 * @copyright Copyright © 2022 Christian Sargusingh
 *
 */

#include "i2c.h"
#include "logger.h"
#include <avr/io.h>

void i2c_wake() {
  // clear power reduction register entry for twi
  PRR0 &= ~(1 << PRTWI);
}

void i2c_sleep() {
  // set power reduction register entry for twi
  PRR0 |= (1 << PRTWI);
}

i2c_status_t i2c_init(uint32_t f_i2c, enum I2CPrescaler prescaler) {
  // set prescaler register
  switch (prescaler) {
    case PRESCALER_1:
      TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
      break;
    case PRESCALER_4:
      TWSR = (TWSR & ~(1 << TWPS1)) | (1 << TWPS0);
      break;
    case PRESCALER_16:
      TWSR = (TWSR & ~(1 << TWPS0)) | (1 << TWPS1);
      break;
    case PRESCALER_64:
      TWSR |= (1 << TWPS1) | (1 << TWPS0);
      break;
    default:
      error("Unrecognized prescaler: %i", prescaler);
      return I2C_INIT_FAILURE;
  }
  // configure twi bit rate
  uint8_t baud = ((F_CPU / f_i2c) - 16) / 2 * prescaler;
  TWBR = baud;
  info("Configured i2c bitrate: %u for transaction frequency: %u", baud, f_i2c);
  return I2C_SUCCESS;
}
