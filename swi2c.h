#ifndef __SWI2C_H
#define __SWI2C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "softbus_config.h"
#include <stdint.h>

#if SWBUS_USE_SWI2C

#define swi2c_delay() swbus_delay()
#define swi2c_write_bit(pin, bit_value) swbus_write_bit(pin, bit_value)
#define swi2c_read_bit(pin) swbus_read_bit(pin)

#define SWI2C_ACK 0
#define SWI2C_NACK 1

#define swi2c_write_scl(scl, bit_value)                                        \
    do {                                                                       \
        swi2c_delay();                                                         \
        swi2c_write_bit(scl, bit_value);                                       \
    } while (0)
#define swi2c_write_sda(sda, bit_value) swi2c_write_bit(sda, bit_value)
#define swi2c_read_sda(sda) swi2c_read_bit(sda)

typedef struct {
    uint32_t scl;
    uint32_t sda;
} swi2c_t;

typedef enum {
    swi2c_dir_transmitter,
    swi2c_dir_receiver,
} swi2c_dir_t;

// 上层封装函数
void swi2c_init(swi2c_t *swi2c, uint32_t scl, uint32_t sda);
uint32_t swi2c_write_cmd(swi2c_t *swi2c, uint8_t addr, uint8_t reg,
                         uint8_t cmd);
uint32_t swi2c_write_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                          uint8_t *data, uint32_t size);
uint32_t swi2c_read_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                         uint8_t *data, uint32_t size);

// 底层时序函数和宏
#define swi2c_start(swi2c)                                                     \
    do {                                                                       \
        swi2c_write_sda((swi2c)->sda, 1);                                      \
        swi2c_write_scl((swi2c)->scl, 1);                                      \
        swi2c_write_sda((swi2c)->sda, 0);                                      \
        swi2c_write_scl((swi2c)->scl, 0);                                      \
    } while (0)

#define swi2c_stop(swi2c)                                                      \
    do {                                                                       \
        swi2c_write_sda((swi2c)->sda, 0);                                      \
        swi2c_write_scl((swi2c)->scl, 1);                                      \
        swi2c_write_sda((swi2c)->sda, 1);                                      \
    } while (0)

#define swi2c_send_ack(swi2c, ack)                                             \
    do {                                                                       \
        swi2c_write_sda((swi2c)->sda, (ack));                                  \
        swi2c_write_scl((swi2c)->scl, 1);                                      \
        swi2c_write_scl((swi2c)->scl, 0);                                      \
    } while (0)

#define swi2c_receive_ack(swi2c, ack)                                          \
    do {                                                                       \
        swi2c_write_sda((swi2c)->sda, 1);                                      \
        swi2c_write_scl((swi2c)->scl, 1);                                      \
        (ack) = swi2c_read_sda((swi2c)->sda);                                  \
        swi2c_write_scl((swi2c)->scl, 0);                                      \
    } while (0)

uint32_t swi2c_send_7bit_addr(swi2c_t *swi2c, uint8_t addr, swi2c_dir_t dir);
uint32_t swi2c_send_data(swi2c_t *swi2c, uint8_t *data, uint32_t size);
void swi2c_receive_data(swi2c_t *swi2c, uint8_t *data, uint32_t size);

#endif

#ifdef __cplusplus
}
#endif

#endif