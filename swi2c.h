#ifndef __SWI2C_H
#define __SWI2C_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 使用前需要将下面的宏函数补充完整
#define swi2c_write_pin(pin, level)
#define swi2c_read_pin(pin)
#define swi2c_delay(us)

typedef struct {
    uint32_t delay_us;
    uint32_t scl;
    uint32_t sda;
} swi2c_t;

typedef enum {
    swi2c_dir_transmitter,
    swi2c_dir_receiver,
} swi2c_dir_t;

// 上层封装函数
void swi2c_init(swi2c_t *swi2c, uint32_t scl, uint32_t sda, uint32_t delay_us);
uint32_t swi2c_write_cmd(swi2c_t *swi2c, uint8_t addr, uint8_t reg,
                         uint8_t cmd);
uint32_t swi2c_write_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                          uint8_t *data, uint32_t size);
uint32_t swi2c_read_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                         uint8_t *data, uint32_t size);

// 底层时序函数和宏
#define swi2c_start(swi2c)                                                     \
    do {                                                                       \
        swi2c_write_pin((swi2c)->sda, 1);                                      \
        swi2c_write_pin((swi2c)->scl, 1);                                      \
        swi2c_delay((swi2c)->delay_us);                                        \
        swi2c_write_pin((swi2c)->sda, 0);                                      \
        swi2c_write_pin((swi2c)->scl, 0);                                      \
    } while (0)

#define swi2c_stop(swi2c)                                                      \
    do {                                                                       \
        swi2c_write_pin((swi2c)->sda, 0);                                      \
        swi2c_write_pin((swi2c)->scl, 1);                                      \
        swi2c_write_pin((swi2c)->sda, 1);                                      \
    } while (0)

#define swi2c_send_ack(swi2c, ack)                                             \
    do {                                                                       \
        swi2c_write_pin((swi2c)->sda, (ack));                                  \
        swi2c_write_pin((swi2c)->scl, 1);                                      \
        swi2c_delay((swi2c)->delay_us);                                        \
        swi2c_write_pin((swi2c)->scl, 0);                                      \
    } while (0)

#define swi2c_receive_ack(swi2c, ack)                                          \
    do {                                                                       \
        swi2c_write_pin((swi2c)->sda, 1);                                      \
        swi2c_write_pin((swi2c)->scl, 1);                                      \
        swi2c_delay((swi2c)->delay_us);                                        \
        ack = swi2c_read_pin((swi2c)->sda);                                    \
        swi2c_write_pin((swi2c)->scl, 0);                                      \
    } while (0)

uint32_t swi2c_send_7bit_addr(swi2c_t *swi2c, uint8_t addr, swi2c_dir_t dir);
uint32_t swi2c_send_data(swi2c_t *swi2c, uint8_t *data, uint32_t size);
void swi2c_receive_data(swi2c_t *swi2c, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif