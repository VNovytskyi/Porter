#ifndef PORTER_H
#define PORTER_H

#ifdef STM8S103
    #include "stm8s.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define PORTER_MAX_ID 64

#define PORTER_SERVICE_BYTES_COUNT 2

#define PORTER_DATA_PACK_ID     1
#define PORTER_ACK_PACK_ID      2
#define PORTER_SYNC_PACK_ID     3
#define PORTER_ACK_SYNC_PACK_ID 4

#define PORTER_OK       0
#define PORTER_ERROR    1
#define PORTER_TX_BUSY  2
#define PORTER_TX_FREE  3

typedef  uint32_t porter_time_t;

typedef struct {
    uint8_t *data;
    uint8_t length;
} porter_frame_t ;

typedef struct {
    uint8_t tx_pack_id;     // Циклическое значение, отправляется с data pack
    porter_frame_t tx_pack; // Пакет, который сейчас передается и должен быть принят приемником

    uint8_t rx_pack_id;     // Циклическое значение, ожидается сообщение с таким id
    porter_frame_t rx_pack; // Пакет, который сейчас принят и ф-цией get_data будет передан в BS

    uint8_t service[1];
    porter_frame_t ack_pack;

    bool first_send;
    porter_time_t send_time; // Время отправки tx_pack
    porter_time_t timeout_ms; // Время за которое должен прийти ack на tx_pack
} porter_t;

uint8_t porter_init(porter_t *porter, porter_time_t timeout_ms);
uint8_t porter_send(porter_t *porter, uint8_t *package_buff, const uint8_t *data, const uint8_t data_length);
porter_frame_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time);
porter_frame_t porter_get_data(porter_t *porter);

bool porter_is_tx_free(porter_t *porter);

#endif // PORTER_H
