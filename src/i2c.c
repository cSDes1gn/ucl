#include "i2c.h"

#include <assert.h>
#include <avr/interrupt.h>
#include <avr/io.h>

#include "logger.h"

#define TWSR_PRESCALER_MASK 0xf8

/**
 * @brief General status codes for master mode
 *
 */
#define M_START 0x08   // A START condition has been transmitted
#define M_RSTART 0x10  // A repeated START condition has been transmitted
#define M_DISCON 0x38  // Arbitration lost in SLA+W or data bytes
/**
 * @brief Table 24-2. Status codes for Master Transmitter Mode
 * SLAW: Slave address write
 * DTX: Data transmit byte
 */
#define MTX_SLAW_ACK 0x18   // SLA+W has been transmitted; ACK has been received
#define MTX_SLAW_NACK 0x20  // SLA+W has been transmitted; NOT ACK has been received
#define MTX_DTX_ACK 0x28    // Data byte has been transmitted; ACK has been received
#define MTX_DTX_NACK 0x30   // Data byte has been transmitted; NACK has been received

/**
 * @brief Table 24-3. Status codes for Master Receiver Mode
 * SLAR: Slave address read
 * DRX: Data receive byte
 */
#define MRX_SLAR_ACK 0x40   // SLA+R has been transmitted; ACK has been received
#define MRX_SLAR_NACK 0x48  // SLA+R has been transmitted; NACK has been received
#define MRX_DRX_ACK 0x50    // Data byte has been received; ACK has been returned
#define MRX_DRX_NACK 0x20   // Data byte has been received; NACK has been returned

static void handle_start(void) {
  // rx and tx context kw
  if (1) {
    TWDR =
  } else {
    TWDR = ()
  }
}

ISR(TWI_vect) {
  // classify bus event
  if ((TWSR & TWSR_PRESCALER_MASK) == M_START) {
    handle_start();
  }
}

i2c_status_t i2c_init(i2c_device_t *device) {
  // ensure the slowest slaves max clock frequency is 16x the target scl freq
  assert(device->slave_fcpu >= device->scl_freq * 16);
  switch (device->prescaler) {
    case I2C_PRESCALE_1:
      TWSR &= ~((1 << TWPS1) | (1 << TWPS0));
      break;
    case I2C_PRESCALE_4:
      TWSR = (TWSR & ~(1 << TWPS1)) | (1 << TWPS0);
      break;
    case I2C_PRESCALE_16:
      TWSR = (1 << TWPS1) | (TWSR & ~(1 << TWPS0));
      break;
    case I2C_PRESCALE_64:
      TWSR |= (1 << TWPS1) | (1 << TWPS0);
      break;
    default:
      assert("Invalid prescaler");
      break;
  }
  // configure bit rate generator unit
  uint8_t brgu = ((device->slave_fcpu / device->scl_freq) - 16) / (2 * device->prescaler);
  TWBR = brgu;
  trace("Configured TWBR: %u", brgu);
  // setup bus interrupt
  cli();
  TWCR |= (1 << TWIE);
  sei();
}

i2c_status_t i2c_block_read_byte(uint8_t addr, uint8_t data) {
  // send start condition
  TWCR = (1 << TWEN) | (1 << TWSTA);
}

i2c_status_t i2c_close(void) {
  // TODO: check for any currently running transactions?
  // clearing the enable bit stops all transactions and relinquishes control of TWI hw
  TWCR &= ~(1 << TWEN);
}