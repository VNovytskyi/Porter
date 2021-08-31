#ifndef PORTER_H
#define PORTER_H

#ifdef STM8S103
    #include "stm8s.h"
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "assert.h"

#define PORTER_OK 1
#define PORTER_ERROR 0

#define PORTER_PACKAGE_LENGTH 11
#define PORTER_BUFFER_LENGTH 12

typedef struct {
    uint8_t tx_buff[PORTER_BUFFER_LENGTH][PORTER_PACKAGE_LENGTH];
} porter_t;

//uint8_t porter_init(porter_t *desc, uint8_t package_length, uint8_t buff_length); //dynamic

uint8_t porter_init(porter_t *desc);

#endif // PORTER_H
