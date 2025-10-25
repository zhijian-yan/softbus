#ifndef __SOFTBUS_CONFIG_H
#define __SOFTBUS_CONFIG_H

#define SWBUS_USE_SWI2C 1
#define SWBUS_USE_SWSPI 1

#define swbus_delay()
#define swbus_write_bit(pin, bit_value)
#define swbus_read_bit(pin) (0)
#define SWSPI_USE_MISO 0

#endif