#ifndef __EEPROM_H__
#define __EEPROM_H__

#include <stdint.h>

typedef int eeprom_status_t;

#define EEPROM_SUCCESS (eeprom_status_t)0;
#define EEPROM_FAILURE (eeprom_status_t)1;

eeprom_status_t eeprom_read_byte(uint16_t addr, uint8_t *data);
eeprom_status_t eeprom_write_byte(uint16_t addr, uint8_t data);

#endif // __EEPROM_H__ 