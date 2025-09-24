#ifndef __SWSPI_H
#define __SWSPI_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// 使用前需要将下面的宏函数补充完整
#define swspi_write_pin(pin, level)
#define swspi_read_pin(pin)

typedef struct {
    uint32_t cs;
    uint32_t miso;
    uint32_t mosi;
    uint32_t sclk;
} swspi_t;

// 上层封装函数
void swspi_init(swspi_t *swspi, uint32_t cs, uint32_t sclk, uint32_t miso,
                uint32_t mosi);
void swspi_write_cmd(swspi_t *swspi, uint8_t cmd);
void swspi_write_data(swspi_t *swspi, uint8_t *data, uint32_t size);
void swspi_read_data(swspi_t *swspi, uint8_t *data, uint32_t size);

// 底层时序函数和宏
#define swspi_start(swspi)                                                     \
    do {                                                                       \
        swspi_write_pin((swspi)->cs, 0);                                       \
    } while (0)

#define swspi_stop(swspi)                                                      \
    do {                                                                       \
        swspi_write_pin((swspi)->cs, 1);                                       \
    } while (0)

void swspi_swap_data(swspi_t *swspi, uint8_t *send_data, uint8_t *recv_data,
                     uint32_t size);
void swspi_send_data(swspi_t *swspi, uint8_t *data, uint32_t size);
void swspi_receive_data(swspi_t *swspi, uint8_t *data, uint32_t size);

#ifdef __cplusplus
}
#endif

#endif