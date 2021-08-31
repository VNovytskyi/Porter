#include "porter.h"

uint8_t porter_init(porter_t *porter, const uint8_t package_length, const uint8_t buff_length) //dynamic
{
    if (porter != NULL) {
        if (package_length > 0 && buff_length > 0) {
            porter->buff_length = buff_length;
            porter->package_length = package_length;

            porter->tx_buff = (uint8_t**)malloc(buff_length * sizeof(uint8_t*));
            if (porter->tx_buff == NULL) {
                return PORTER_ERROR;
            }

            for (uint8_t i = 0; i < buff_length; ++i) {
                porter->tx_buff[i] = (uint8_t*)malloc(package_length * sizeof(uint8_t));

                if (porter->tx_buff[i] == NULL) {
                    return PORTER_ERROR;
                }

                for (uint8_t j = 0; j < package_length; ++j) {
                    porter->tx_buff[i][j] = i;
                }
            }

            porter->rx_buff = (uint8_t**)malloc(buff_length * sizeof(uint8_t*));
            if (porter->rx_buff == NULL) {
                return PORTER_ERROR;
            }

            for (uint8_t i = 0; i < buff_length; ++i) {
                porter->rx_buff[i] = (uint8_t*)malloc(package_length * sizeof(uint8_t));

                if (porter->rx_buff[i] == NULL) {
                    return PORTER_ERROR;
                }

                for (uint8_t j = 0; j < package_length; ++j) {
                    porter->rx_buff[i][j] = i;
                }
            }

            return PORTER_OK;
        }
    }

    return PORTER_ERROR;
}

uint8_t porter_deinit(porter_t *porter)
{
    if (porter != NULL) {
        if (porter->tx_buff != NULL) {
            for (uint8_t i = 0; i < porter->buff_length; ++i) {
                free(porter->tx_buff[i]);
            }
            free(porter->tx_buff);
        }

        if (porter->rx_buff != NULL) {
            for (uint8_t i = 0; i < porter->buff_length; ++i) {
                free(porter->rx_buff[i]);
            }
            free(porter->rx_buff);
        }

        return PORTER_OK;
    }

    return PORTER_ERROR;
}

uint8_t porter_write(porter_t *porter, const uint8_t *data, const uint8_t length)
{
    if (porter != NULL) {
        if (data != NULL && length > 0) {
            if (length <= porter->package_length) {
                //влезит в одну ячейку
            } else {
                //нужно разбивать по ячейкам
            }
        }
    }
}
