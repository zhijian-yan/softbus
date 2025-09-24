#include "swspi.h"

void swspi_init(swspi_t *swspi, uint32_t cs, uint32_t sclk, uint32_t miso,
                uint32_t mosi) {
    swspi_write_pin(swspi->cs, 1);
    swspi_write_pin(swspi->sclk, 0);
    swspi->cs = cs;
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
            swspi_write_pin(swspi->mosi, *(send_data + j) & (0x80 >> i));
            swspi_write_pin(swspi->sclk, 1);
            if (swspi_read_pin(swspi->miso)) {
                *(recv_data + j) |= (0x80 >> i);
            }
            swspi_write_pin(swspi->sclk, 0);
        }
    }
}

void swspi_send_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        for (i = 0; i < 8; i++) {
            swspi_write_pin(swspi->mosi, *(data + j) & (0x80 >> i));
            swspi_write_pin(swspi->sclk, 1);
            swspi_write_pin(swspi->sclk, 0);
        }
    }
}

void swspi_receive_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        *(data + j) = 0x00;
        for (i = 0; i < 8; i++) {
            swspi_write_pin(swspi->mosi, 0);
            swspi_write_pin(swspi->sclk, 1);
            if (swspi_read_pin(swspi->miso)) {
                *(data + j) |= (0x80 >> i);
            }
            swspi_write_pin(swspi->sclk, 0);
        }
    }
}

void swspi_write_cmd(swspi_t *swspi, uint8_t cmd) {
    swspi_start(swspi);
    swspi_send_data(swspi, &cmd, 1);
    swspi_stop(swspi);
}

void swspi_write_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    swspi_start(swspi);
    swspi_send_data(swspi, data, size);
    swspi_stop(swspi);
}

void swspi_read_data(swspi_t *swspi, uint8_t *data, uint32_t size) {
    swspi_start(swspi);
    swspi_receive_data(swspi, data, size);
    swspi_stop(swspi);
}