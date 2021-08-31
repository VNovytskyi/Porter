#ifndef PORTER_H
#define PORTER_H

#ifdef STM8S103
    #include "stm8s.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define PORTER_OK 1
#define PORTER_ERROR 0

typedef struct {
    uint8_t **tx_buff;
    uint8_t tx_buff_begin;
    uint8_t tx_buff_end;

    uint8_t **rx_buff;
    uint8_t rx_buff_begin;
    uint8_t rx_buff_end;


    uint8_t package_length;
    uint8_t buff_length;

} porter_t;

uint8_t porter_init(porter_t *porter, const uint8_t package_length, const uint8_t buff_length);
uint8_t porter_deinit(porter_t *porter);

uint8_t porter_write(porter_t *porter, const uint8_t *data, const uint8_t length);

#endif // PORTER_H
