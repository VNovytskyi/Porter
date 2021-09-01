#ifndef PORTER_H
#define PORTER_H

#ifdef STM8S103
    #include "stm8s.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PORTER_DATA_PACK_ID 1
#define PORTER_ACK_PACK_ID  2

#define PORTER_OK       0
#define PORTER_ERROR    1
#define PORTER_TX_BUSY  2
#define PORTER_TX_FREE  3

typedef  uint32_t porter_time_t;

typedef struct {
    uint8_t *tx_buff;
    uint8_t tx_buff_max_length;

    uint8_t tx_status;
    uint8_t current_tx_data_length;

    uint8_t ack[1];

    porter_time_t send_time;
    porter_time_t timeout_ms;

    void (*send_callback)(uint8_t *data, uint8_t data_length);
    void (*recv_callback)(uint8_t *data, uint8_t data_length);
    void (*tx_free_callback)();

} porter_t;

uint8_t porter_init(porter_t *porter,
                    uint8_t max_data_length,
                    void (*send_callback)(uint8_t *data, uint8_t data_length),
                    void (*recv_callback)(uint8_t *data, uint8_t data_length),
                    porter_time_t timeout_ms);

uint8_t porter_deinit(porter_t *porter);

uint8_t porter_is_tx_free(porter_t *porter);
uint8_t porter_set_tx_free_callback(porter_t *porter, void (*tx_free_callback)());

uint8_t porter_send(porter_t *porter, uint8_t *data, uint8_t data_length);
uint8_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time);


#endif // PORTER_H
