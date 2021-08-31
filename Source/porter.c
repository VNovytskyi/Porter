#include "porter.h"

uint8_t porter_init(porter_t *desc, const uint8_t package_length, const uint8_t buff_length) //dynamic
{
    if (desc != NULL) {
        if (package_length > 0 && buff_length > 0) {
            desc->buff_length = buff_length;
            desc->package_length = package_length;

            desc->tx_buff = (uint8_t**)malloc(buff_length * sizeof(uint8_t*));
            if (desc->tx_buff == NULL) {
                return PORTER_ERROR;
            }

            for (uint8_t i = 0; i < buff_length; ++i) {
                desc->tx_buff[i] = (uint8_t*)malloc(package_length * sizeof(uint8_t));

                if (desc->tx_buff[i] == NULL) {
                    return PORTER_ERROR;
                }

                for (uint8_t j = 0; j < package_length; ++j) {
                    desc->tx_buff[i][j] = i;
                }
            }

            desc->rx_buff = (uint8_t**)malloc(buff_length * sizeof(uint8_t*));
            if (desc->rx_buff == NULL) {
                return PORTER_ERROR;
            }

            for (uint8_t i = 0; i < buff_length; ++i) {
                desc->rx_buff[i] = (uint8_t*)malloc(package_length * sizeof(uint8_t));

                if (desc->rx_buff[i] == NULL) {
                    return PORTER_ERROR;
                }

                for (uint8_t j = 0; j < package_length; ++j) {
                    desc->rx_buff[i][j] = i;
                }
            }

            return PORTER_OK;
        }
    }

    return PORTER_ERROR;
}

uint8_t porter_deinit(porter_t *desc)
{
    if (desc != NULL) {
        if (desc->tx_buff != NULL) {
            for (uint8_t i = 0; i < desc->buff_length; ++i) {
                free(desc->tx_buff[i]);
            }
            free(desc->tx_buff);
        }

        if (desc->rx_buff != NULL) {
            for (uint8_t i = 0; i < desc->buff_length; ++i) {
                free(desc->rx_buff[i]);
            }
            free(desc->rx_buff);
        }

        return PORTER_OK;
    }

    return PORTER_ERROR;
}
