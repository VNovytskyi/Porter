#include "porter.h"

static void increase_circular_value(uint8_t *value, const uint8_t amount, const uint8_t max_value)
{
    uint16_t i;
    for (i = 0; i < amount; ++i) {
        *value = (*value + 1) % max_value;
    }
}

static porter_frame_t handle_sync(porter_t *porter)
{
    porter->tx_pack_id = 0;
    porter->rx_pack_id = 0;

    porter->tx_pack.data = NULL;
    porter->tx_pack.length = 0;

    porter->rx_pack.data = NULL;
    porter->rx_pack.length = 0;

    porter->system_pack.data[0] = PORTER_ACK_SYNC_PACK_ID;
    porter->system_pack.length = 1;
    return porter->system_pack;
}

static porter_frame_t handle_received_data(porter_t *porter, uint8_t *received_data, uint8_t received_data_length)
{
    porter_frame_t frame = {NULL, 0};

    if (received_data[1] == porter->rx_pack_id) {
        increase_circular_value(&porter->rx_pack_id, 1, PORTER_MAX_ID);
        porter->rx_pack.data = received_data + PORTER_SERVICE_BYTES_COUNT;
        porter->rx_pack.length = received_data_length - PORTER_SERVICE_BYTES_COUNT;
        porter->system_pack.data[0] = PORTER_ACK_PACK_ID;
        porter->system_pack.length = 1;
        return porter->system_pack;
    }

    return frame;
}

/*!
 * \brief porter_init Set initial values
 * \param porter - address of sheller descriptor
 * \param timeout_ms - time for which acknowledgement should come
 * \return result of init porter
 */
uint8_t porter_init(porter_t *porter, porter_time_t timeout_ms)
{
    if (porter == NULL)
        return PORTER_ERROR;

    porter->system_pack.data = porter->service;
    porter->system_pack.length = 1;

    porter->tx_pack_id = 0;
    porter->tx_pack.data = NULL;
    porter->tx_pack.length = 0;

    porter->rx_pack_id = 0;
    porter->rx_pack.data = NULL;
    porter->rx_pack.length = 0;

    porter->first_send = false;
    porter->send_time = 0;
    porter->timeout_ms = timeout_ms;

    return PORTER_OK;
}

/*!
 * \brief porter_send Push data to an internal buffer to send later
 * \param porter - address of porter descriptor
 * \param package_buff - address of buffer for porter package
 * \param data - pointer to user data which need to send
 * \param data_length - length of user data
 * \return result of pushing data to an internal buffer
 */
uint8_t porter_send(porter_t *porter, uint8_t *package_buff, const uint8_t *data, const uint8_t data_length)
{
    if (porter == NULL || data == NULL || package_buff == NULL)
        return PORTER_ERROR;

    if (porter->tx_pack.data == NULL) {
        porter->first_send = true;
        porter->tx_pack.data = package_buff;
        porter->tx_pack.length = data_length + PORTER_SERVICE_BYTES_COUNT;
        package_buff[0] = PORTER_DATA_PACK_ID;
        package_buff[1] = porter->tx_pack_id;
        increase_circular_value(&porter->tx_pack_id, 1, PORTER_MAX_ID);
        memcpy((package_buff + PORTER_SERVICE_BYTES_COUNT), data, data_length);
        return PORTER_OK;

    }

    return PORTER_ERROR;
}

porter_frame_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time)
{
    porter_frame_t frame = {NULL, 0};

    if(porter == NULL)
        return frame;

    //Handle TX
    if (porter->tx_pack.data != NULL) {
        if ((current_time - porter->send_time) >= porter->timeout_ms || porter->first_send) {
            porter->first_send = false;
            porter->send_time = current_time;
            return porter->tx_pack;
        }
    }

    //Handle RX
    if ((received_data != NULL) && (received_data_length > 0)) {
        if (received_data[0] == PORTER_ACK_PACK_ID) { //recv ack on data
            porter->tx_pack.data = NULL;
            porter->tx_pack.length = 0;
        } else if (received_data[0] == PORTER_DATA_PACK_ID) { //recv data
            return handle_received_data(porter, received_data, received_data_length);
        } else if (received_data[0] == PORTER_SYNC_PACK_ID) { //recv sync cmd
            return handle_sync(porter);
        } else if (received_data[0] == PORTER_ACK_SYNC_PACK_ID) {//recv ask on sync
            handle_sync(porter);
        }
    }

    return frame;
}

porter_frame_t porter_get_data(porter_t *porter)
{
    porter_frame_t frame = {NULL, 0};

    if ((porter != NULL) && (porter->rx_pack.data != NULL)) {
        frame.data = porter->rx_pack.data;
        frame.length = porter->rx_pack.length;
        porter->rx_pack.data = NULL;
        porter->rx_pack.length = 0;
    }

    return frame;
}

uint8_t porter_get_tx_status(porter_t *porter)
{
    if (porter == NULL) {
        return PORTER_ERROR;
    }

    if (porter->tx_pack.data == NULL) {
        return PORTER_TX_FREE;
    }

    return PORTER_TX_BUSY;
}

uint8_t porter_sync(porter_t *porter)
{
    porter->system_pack.data[0] = PORTER_SYNC_PACK_ID;
    porter->system_pack.length = 1;
    porter->tx_pack = porter->system_pack;
    porter->first_send = true;
    return 0;
}
