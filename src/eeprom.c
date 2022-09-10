
#include <avr/io.h>
#include <avr/interrupt.h>

#include "eeprom.h"

/**
 * @brief Reads a byte from EEPROM at specified address and stores it in the 
 * passed data buffer. Disables global interrupt mask during transaction.
 * 
 * @param addr EEPROM read addres. Must be in the range [0, EEPROM_MAX_ADDR]
 * @param data data buffer for EEPROM byte at read address
 * @return eeprom_status_t 
 */
eeprom_status_t eeprom_read_byte(uint16_t addr, uint8_t *data) {
  if (addr > 4096) {
    return EEPROM_FAILURE;
  }
  /* Wait for previous EEPROM transaction
     NOTE: EEPE is self-cleared by hardware after EEPROM write transactions */
  while (EECR & (1 << EEPE));
  cli();
  EEAR = addr;
  // start read transaction
  EECR |= (1 << EERE);
  // save contents in buffer
  *data = EEDR;
  sei();
  return EEPROM_SUCCESS;
}

/**
 * @brief Writes a byte to EEPROM at specified address. Disables global
 * interrupt mask during transaction.
 * 
 * @param addr eeprom write address. Must be in the range [0, EEPROM_MAX_ADDR]
 * @param data eeprom write byte
 * @return eeprom_status_t 
 */
eeprom_status_t eeprom_write_byte(uint16_t addr, uint8_t data) {
  if (addr > 4096) {
    return EEPROM_FAILURE;
  }
  /* Wait for previous EEPROM transaction
     NOTE: EEPE is self-cleared by hardware after EEPROM write transactions */
  while (EECR & (1 << EEPE));
  /* relevant if the software contains a Boot Loader allowing the CPU to program the Flash
     wait for store program memory instruction */
  while (SPMCSR & (1 << SPMEN));
  cli();
  // write to EEPROM address and data registers 
  EEAR = addr;
  EEDR = data;
  // start transaction
  EECR |= (1 << EEMPE);
  EECR |= (1 << EEPE);
  sei();
  return EEPROM_SUCCESS;
}