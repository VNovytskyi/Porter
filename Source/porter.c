#include "porter.h"

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

    porter->tx_pack.data = NULL;
    porter->tx_pack.length = 0;

    porter->rx_pack.data = NULL;
    porter->rx_pack.length = 0;

    porter->ack[0] = PORTER_ACK_PACK_ID;
    porter->ack_pack.data = porter->ack;
    porter->ack_pack.length = 1;

    porter->tx_status = PORTER_TX_FREE;
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

    if (porter->tx_status == PORTER_TX_BUSY)
        return PORTER_ERROR;

    porter->tx_status = PORTER_TX_BUSY;
    porter->tx_pack.data = package_buff;
    porter->tx_pack.length = data_length + 1;
    package_buff[0] = PORTER_DATA_PACK_ID;
    memcpy((package_buff + 1), data, data_length);
    return PORTER_OK;
}

porter_frame_t porter_process(porter_t *porter, uint8_t *received_data, uint8_t received_data_length, porter_time_t current_time)
{
    porter_frame_t frame = {NULL, 0};

    if(porter == NULL)
        return frame;

    //Handle TX
    if (porter->tx_status == PORTER_TX_BUSY) {
        if ((current_time - porter->send_time) >= porter->timeout_ms) {
            porter->send_time = current_time;
            return porter->tx_pack;
        }
    }

    //Handle RX
    if ((received_data != NULL) && (received_data_length > 0)) {
        if (received_data[0] == PORTER_ACK_PACK_ID) {
            porter->tx_status = PORTER_TX_FREE;
        } else if (received_data[0] == PORTER_DATA_PACK_ID) {
            porter->rx_pack.data = received_data + 1;
            porter->rx_pack.length = received_data_length - 1;
            return porter->ack_pack;
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

/*!
 * \brief porter_is_tx_free
 * \param porter - address of sheller descriptor
 * \return tx buffer status
 * \details compare the return value with PORTER_TX_BUSY or PORTER_TX_FREE
 */
uint8_t porter_is_tx_free(porter_t *porter)
{
    if (porter == NULL)
        return PORTER_ERROR;

    return porter->tx_status;
}
