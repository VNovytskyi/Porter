#include "porter.h"

uint8_t porter_init(porter_t *porter,
                    uint8_t max_data_length,
                    void (*send_callback)(uint8_t *, uint8_t),
                    void (*recv_callback)(uint8_t *, uint8_t),
                    porter_time_t timeout_ms)
{
    if (porter == NULL)
        return PORTER_ERROR;

    porter->tx_buff_max_length = (max_data_length + 1);
    porter->tx_buff = (uint8_t*)malloc(porter->tx_buff_max_length * sizeof(uint8_t));
    if (porter->tx_buff == NULL) {
        return PORTER_ERROR;
    }

    porter->tx_status = PORTER_TX_FREE;
    porter->timeout_ms = timeout_ms;
    porter->send_callback = send_callback;
    porter->recv_callback = recv_callback;

    porter->ack[0] = PORTER_ACK_PACK_ID;

    return PORTER_OK;
}

uint8_t porter_deinit(porter_t *porter)
{
    if (porter == NULL)
        return PORTER_ERROR;

    free(porter->tx_buff);
    return PORTER_OK;
}

uint8_t porter_send(porter_t *porter, uint8_t *data, uint8_t data_length)
{
    if (porter == NULL)
        return PORTER_ERROR;

    if (porter->tx_status == PORTER_TX_BUSY)
        return PORTER_ERROR;

    if (data == NULL)
        return PORTER_ERROR;

    if (data_length > (porter->tx_buff_max_length - 1))
        return PORTER_ERROR;

    porter->tx_status = PORTER_TX_BUSY;
    porter->tx_buff[0] = PORTER_DATA_PACK_ID;
    porter->current_tx_data_length = data_length + 1;
    memcpy((porter->tx_buff + 1), data, data_length);

    return PORTER_OK;
}

uint8_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time)
{
    if (porter == NULL)
        return PORTER_ERROR;

    //Handle RX
    if ((received_data != NULL) && (received_data_length > 0)) {
        if (received_data[0] == PORTER_ACK_PACK_ID) {
            porter->tx_status = PORTER_TX_FREE;
            if (porter->tx_free_callback != NULL) { // написать макрос
                porter->tx_free_callback();
            }
        } else if (received_data[1] == PORTER_DATA_PACK_ID) {
            if (porter->send_callback != NULL) {// написать макрос
                porter->send_callback(porter->ack, 1);
            }
            if (porter->recv_callback != NULL) {// написать макрос
                porter->recv_callback((received_data + 1), (received_data_length - 1));
            }
        }
    }

    //Handle TX
    if (porter->tx_status == PORTER_TX_BUSY) {
        if ((current_time - porter->send_time) >= porter->timeout_ms) {
            porter->send_time = current_time;
            if (porter->send_callback != NULL) {// написать макрос
                porter->send_callback(porter->tx_buff, porter->current_tx_data_length);
            }
        }
    }

    return PORTER_OK;
}

uint8_t porter_is_tx_free(porter_t *porter)
{
    if (porter == NULL)
        return PORTER_ERROR;

    return porter->tx_status;
}

uint8_t porter_set_tx_free_callback(porter_t *porter, void (*tx_free_callback)())
{
    if (porter == NULL)
        return PORTER_ERROR;

    porter->tx_free_callback = tx_free_callback;
    return PORTER_OK;
}
