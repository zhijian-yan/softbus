#include "swi2c.h"

#if SWBUS_USE_SWI2C

void swi2c_init(swi2c_t *swi2c, uint32_t scl, uint32_t sda) {
    swi2c_write_scl(swi2c->scl, 1);
    swi2c_write_sda(swi2c->sda, 1);
    swi2c->scl = scl;
    swi2c->sda = sda;
}

uint32_t swi2c_send_7bit_addr(swi2c_t *swi2c, uint8_t addr, swi2c_dir_t dir) {
    uint32_t i, ack;
    if (dir == swi2c_dir_receiver)
        addr |= 0x01;
    for (i = 0; i < 8; i++) {
        swi2c_write_sda(swi2c->sda, addr & (0x80 >> i));
        swi2c_write_scl(swi2c->scl, 1);
        swi2c_write_scl(swi2c->scl, 0);
    }
    swi2c_receive_ack(swi2c, ack);
    return ack;
}

uint32_t swi2c_send_data(swi2c_t *swi2c, uint8_t *data, uint32_t size) {
    uint32_t ack = 0;
    uint32_t i, j, ack_temp;
    for (j = 0; j < size; j++) {
        for (i = 0; i < 8; i++) {
            swi2c_write_sda(swi2c->sda, data[j] & (0x80 >> i));
            swi2c_write_scl(swi2c->scl, 1);
            swi2c_write_scl(swi2c->scl, 0);
        }
        swi2c_receive_ack(swi2c, ack_temp);
        ack |= ack_temp;
    }
    return ack;
}

void swi2c_receive_data(swi2c_t *swi2c, uint8_t *data, uint32_t size) {
    uint8_t recv_data;
    uint32_t i, j;
    for (j = 0; j < size; j++) {
        recv_data = 0x00;
        swi2c_write_sda(swi2c->sda, 1);
        for (i = 0; i < 8; i++) {
            swi2c_write_scl(swi2c->scl, 1);
            if (swi2c_read_sda(swi2c->sda)) {
                recv_data |= (0x80 >> i);
            }
            swi2c_write_scl(swi2c->scl, 0);
        }
        if (j + 1 != size) {
            swi2c_send_ack(swi2c, SWI2C_ACK);
        }
        data[j] = recv_data;
    }
    swi2c_send_ack(swi2c, SWI2C_NACK);
}

uint32_t swi2c_write_cmd(swi2c_t *swi2c, uint8_t addr, uint8_t reg,
                         uint8_t cmd) {
    uint32_t ack = SWI2C_ACK;
    swi2c_start(swi2c);
    ack |= swi2c_send_7bit_addr(swi2c, addr, swi2c_dir_transmitter);
    ack |= swi2c_send_data(swi2c, &reg, 1);
    ack |= swi2c_send_data(swi2c, &cmd, 1);
    swi2c_stop(swi2c);
    return ack;
}

uint32_t swi2c_write_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                          uint8_t *data, uint32_t size) {
    uint32_t ack = SWI2C_ACK;
    swi2c_start(swi2c);
    ack |= swi2c_send_7bit_addr(swi2c, addr, swi2c_dir_transmitter);
    ack |= swi2c_send_data(swi2c, &cmd, 1);
    ack |= swi2c_send_data(swi2c, data, size);
    swi2c_stop(swi2c);
    return ack;
}

uint32_t swi2c_read_data(swi2c_t *swi2c, uint8_t addr, uint8_t cmd,
                         uint8_t *data, uint32_t size) {
    uint32_t ack = SWI2C_ACK;
    swi2c_start(swi2c);
    ack |= swi2c_send_7bit_addr(swi2c, addr, swi2c_dir_transmitter);
    ack |= swi2c_send_data(swi2c, &cmd, 1);
    swi2c_start(swi2c);
    ack |= swi2c_send_7bit_addr(swi2c, addr, swi2c_dir_receiver);
    swi2c_receive_data(swi2c, data, size);
    swi2c_stop(swi2c);
    return ack;
}

#endif