#include "porter.h"

uint8_t porter_init(porter_t *porter, uint8_t data_length, void (*send_callback)(uint8_t *, uint8_t), porter_time_t timeout_ms)
{
    if (porter == NULL)
        return PORTER_NULL_DESCRIPTOR;

    porter->tx_buff_length = (data_length + PORTER_SERVICE_BYTES_COUNT);
    porter->tx_buff = (uint8_t*)malloc(porter->tx_buff_length * sizeof(uint8_t));
    if (porter->tx_buff == NULL) {
        return PORTER_MEMORY_ERROR;
    }

    porter->tx_empty = 1;
    porter->send_callback = send_callback;
    porter->timeout_ms = timeout_ms;

    return PORTER_OK;
}

uint8_t porter_send(porter_t *porter, uint8_t *data, uint8_t data_length)
{
    if (porter == NULL)
        return PORTER_NULL_DESCRIPTOR;

    if (porter->tx_empty == 0)
        return PORTER_TX_OVERFLOW;

    //Copy data to tx_buff[1]! tx_buff[0] = packType

    return PORTER_OK;
}

uint8_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time)
{
    if (porter == NULL)
        return PORTER_NULL_DESCRIPTOR;

    //Handle RX
    if ((received_data != NULL) && (received_data_length > 0)) {

    }

    //Handle TX
    if (porter->tx_empty == 0) {
        if ((current_time - porter->send_time) >= porter->timeout_ms) {
            porter->send_time = current_time;
            if (porter->send_callback != NULL) {
                porter->send_callback(porter->tx_buff, porter->tx_buff_length);
            }
        }
    }

    return PORTER_OK;
}
