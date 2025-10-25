#include "swspi.h"

#if SWBUS_USE_SWSPI

#define swspi_write_sclk(sclk, bit_value)                                      \
    do {                                                                       \
        swspi_delay();                                                         \
        swspi_write_bit(sclk, bit_value);                                      \
    } while (0)
#define swspi_write_mosi(mosi, bit_value) swspi_write_bit(mosi, bit_value)
#define swspi_read_miso(miso) swspi_read_bit(miso)

void swspi_init(swspi_t *swspi, uint32_t sclk, uint32_t miso, uint32_t mosi) {
    swspi_write_sclk(swspi->sclk, 0);
    swspi->miso = miso;
    swspi->mosi = mosi;
    swspi->sclk = sclk;
}

void swspi_swap_data(swspi_t *swspi, uint8_t *send_data, uint8_t *recv_data,
                     uint32_t size) {
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        *(recv_data + j) = 0x00;
        for (i = 0; i < 8; i++) {
            swspi_write_mosi(swspi->mosi, *(send_data + j) & (0x80 >> i));
            swspi_write_sclk(swspi->sclk, 1);
            if (swspi_read_miso(swspi->miso)) {
                *(recv_data + j) |= (0x80 >> i);
            }
            swspi_write_sclk(swspi->sclk, 0);
        }
    }
}

void swspi_send_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        for (i = 0; i < 8; i++) {
            swspi_write_mosi(swspi->mosi, *(data + j) & (0x80 >> i));
            swspi_write_sclk(swspi->sclk, 1);
            swspi_write_sclk(swspi->sclk, 0);
        }
    }
}

void swspi_receive_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        *(data + j) = 0x00;
        for (i = 0; i < 8; i++) {
            swspi_write_mosi(swspi->mosi, 0);
            swspi_write_sclk(swspi->sclk, 1);
            if (swspi_read_miso(swspi->miso)) {
                *(data + j) |= (0x80 >> i);
            }
            swspi_write_sclk(swspi->sclk, 0);
        }
    }
}

void swspi_write_cmd(swspi_t *swspi, uint8_t cmd) {
    swspi_send_data(swspi, &cmd, 1);
}

void swspi_write_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    swspi_send_data(swspi, data, size);
}

void swspi_read_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    swspi_receive_data(swspi, data, size);
}

#endif