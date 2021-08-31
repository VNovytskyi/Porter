#include "porter.h"

/*
uint8_t porter_init(porter_t *desc, uint8_t package_length, uint8_t buff_length) //dynamic
{
    if (desc != NULL) {
        desc->buff_length = buff_length;
        desc->package_length = package_length;

        desc->tx_buff = (uint8_t**)malloc(buff_length);
        if (desc->tx_buff != NULL) {
            for (uint8_t i = 0; i < buff_length; ++i) {
                desc->tx_buff[i] = (uint8_t*)malloc(package_length);
                if (desc->tx_buff[i] == NULL) {
                    return PORTER_ERROR;
                }
                for (uint8_t j = 0; j < package_length; ++j) {
                    desc->tx_buff[i][j] = i;
                }
            }
        } else {
            return PORTER_ERROR;
        }
    }

    return PORTER_OK;
}
*/

uint8_t porter_init(porter_t *desc)
{
    if (desc != NULL) {
        for (uint8_t i = 0; i < PORTER_BUFFER_LENGTH; ++i) {
            for (uint8_t j = 0; j < PORTER_PACKAGE_LENGTH; ++j) {
                desc->tx_buff[i][j] = i;
            }
        }
    }

    return PORTER_OK;
}
