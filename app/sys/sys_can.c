/**
 * @file       sys.c
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2021-03-20
 * @author     Thuan Le
 * @brief      System module to handle system CAN bus
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_can.h"
#include "sys_damos_ram.h"
#include "sys_aws_pub_sub.h"

/* Private defines ---------------------------------------------------- */
// RX messages ID
#define CAN_MSG_RX_DEVICE_SET_DEVICE_ID_CONFIRM   (0x31)
#define CAN_MSG_RX_ENCODER_VALUE_ID               (0x32)

// TX messages ID
#define CAN_MSG_TX_SET_DEVICE_ID             (0x01)
#define CAN_MSG_TX_WRITE_COMMAND_ID          (0x02)
#define CAN_MSG_TX_REQUEST_READ_SENSOR_ID    (0x03)

static const char *TAG = "sys/can";

/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
/* Private variables -------------------------------------------------- */
static can_message_t m_can_tx_msg;
static can_message_t m_can_rx_msg;

/* Private function prototypes ---------------------------------------- */
static void m_sys_can_update_tx_header(can_message_t *p_tx_buff, uint8_t msg_id);

static void m_sys_can_unpack_msg_device_id_confirm(can_message_t *p_tx_buff);
static void m_sys_can_unpack_msg_encoder_value(can_message_t *p_tx_buff);

static void m_sys_can_pack_msg_set_device_id(can_message_t *p_tx_data);
static void m_sys_can_pack_msg_command(can_message_t *p_tx_data);
static void m_sys_can_pack_msg_request_sensor_value(can_message_t *p_tx_data);

/* Function definitions ----------------------------------------------- */
void sys_can_send_msg(msg_handle_send_t msg_handle)
{
  switch (msg_handle)
  {
  case TX_MSG_SET_DEVICE_ID:
    m_sys_can_pack_msg_set_device_id(&m_can_tx_msg);
    m_sys_can_update_tx_header(&m_can_tx_msg, CAN_MSG_TX_SET_DEVICE_ID);
    break;

  case TX_MSG_WRITE_COMMAND:
    m_sys_can_pack_msg_command(&m_can_tx_msg);
    m_sys_can_update_tx_header(&m_can_tx_msg, CAN_MSG_TX_WRITE_COMMAND_ID);
    break;

  case TX_MSG_REQUEST_READ_SENSOR:
    m_sys_can_pack_msg_request_sensor_value(&m_can_tx_msg);
    m_sys_can_update_tx_header(&m_can_tx_msg, CAN_MSG_TX_REQUEST_READ_SENSOR_ID);
    break;

  default:
    break;
  }

  // Can send out
  can_transmit(&m_can_tx_msg, 100);
}

void sys_can_receive_msg(void)
{
  can_receive(&m_can_rx_msg, portMAX_DELAY);
  switch (m_can_rx_msg.identifier)
  {
  case CAN_MSG_RX_DEVICE_SET_DEVICE_ID_CONFIRM:
    ESP_LOGI(TAG, "CAN_MSG_RX_DEVICE_SET_DEVICE_ID_CONFIRM");
    m_sys_can_unpack_msg_device_id_confirm(&m_can_rx_msg);
    break;

  case CAN_MSG_RX_ENCODER_VALUE_ID:
    ESP_LOGI(TAG, "CAN_MSG_RX_ENCODER_VALUE_ID");
    m_sys_can_unpack_msg_encoder_value(&m_can_rx_msg);

    // m_sys_aws_publish();
    break;

  default:
    break;
  }
}

/* Private function definitions---------------------------------------- */
/**
 * @brief         Can unpack message device id confirm
 *
 * @param[in]     p_tx_buff     Pointer to tx data
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_unpack_msg_device_id_confirm(can_message_t *p_rx_buff)
{
  g_can_msg_device_id_confirm.device_id = p_rx_buff->data[0];
}

/**
 * @brief         Can unpack message encoder value
 *
 * @param[in]     p_tx_buff     Pointer to tx data
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_unpack_msg_encoder_value(can_message_t *p_rx_buff)
{
  g_can_msg_encoder_value.device_id            = p_rx_buff->data[0];
  g_can_msg_encoder_value.motor_total_rotation = (uint32_t) p_rx_buff->data[4] << 24 |
                                                 (uint32_t) p_rx_buff->data[3] << 16 |
                                                 (uint32_t) p_rx_buff->data[2] << 8  |
                                                 (uint32_t) p_rx_buff->data[1];
  g_can_msg_encoder_value.motor_speed          = (uint16_t) p_rx_buff->data[6] << 8 |
                                                 (uint16_t) p_rx_buff->data[5];
}

/**
 * @brief         Can pack message set device id
 *
 * @param[in]     p_tx_data     Pointer to tx data
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_pack_msg_set_device_id(can_message_t *p_tx_data)
{
  p_tx_data->data[0] =  g_can_msg_set_device_id.device_id;
}

/**
 * @brief         Can pack message command
 *
 * @param[in]     p_tx_data     Pointer to tx data
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_pack_msg_command(can_message_t *p_tx_data)
{
  p_tx_data->data[0] = g_can_msg_command.device_id;
  p_tx_data->data[1] = g_can_msg_command.command;
  p_tx_data->data[2] = g_can_msg_command.motor_speed;
}

/**
 * @brief         Can pack message request sensor value
 *
 * @param[in]     p_tx_data     Pointer to tx data
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_pack_msg_request_sensor_value(can_message_t *p_tx_data)
{
  p_tx_data->data[0] = g_can_msg_request_sensor_value.device_id;
}

/**
 * @brief         Can update tx header
 *
 * @param[in]     p_tx_data     Pointer to tx data
 * @param[in]     msg_id        Message id
 * 
 * @attention     None
 *
 * @return        None
 */
static void m_sys_can_update_tx_header(can_message_t *p_tx_data, uint8_t msg_id)
{
  p_tx_data->identifier       = msg_id;
  p_tx_data->data_length_code = 8;
}

/* End of file -------------------------------------------------------- */
