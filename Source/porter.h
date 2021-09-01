#ifndef PORTER_H
#define PORTER_H

#ifdef STM8S103
    #include "stm8s.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PORTER_ACK_PACK_ID 2
#define PORTER_DATA_PACK_ID 1

#define PORTER_SERVICE_BYTES_COUNT 1

#define PORTER_OK 0
#define PORTER_NULL_DESCRIPTOR  10
#define PORTER_TX_OVERFLOW      11
#define PORTER_MEMORY_ERROR     12
#define PORTER_LONG_DATA        13
#define PORTER_NULL_DATA        14

typedef  uint32_t porter_time_t;

typedef struct {
    uint8_t *tx_buff;
    uint8_t tx_buff_max_length;
    uint8_t tx_empty;
    uint8_t current_message_length;

    uint8_t ack[1];

    porter_time_t send_time;
    porter_time_t timeout_ms;
    void (*send_callback)(uint8_t *data, uint8_t data_length);
    void (*recv_callback)(uint8_t *data, uint8_t data_length);

} porter_t;

uint8_t porter_init(porter_t *porter,
                    uint8_t max_data_length,
                    void (*send_callback)(uint8_t *data, uint8_t data_length),
                    void (*recv_callback)(uint8_t *data, uint8_t data_length),
                    porter_time_t timeout_ms);

uint8_t porter_send(porter_t *porter, uint8_t *data, uint8_t data_length);
uint8_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time);


#endif // PORTER_H
