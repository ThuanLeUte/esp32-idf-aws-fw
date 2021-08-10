/**
 * @file       sys_damos_ram.c
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2021-03-18
 * @author     Thuan Le
 * @brief      System damos ram
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------- */
#include "sys_damos_ram.h"

/* Private defines ---------------------------------------------------- */
/* Private enumerate/structure ---------------------------------------- */
/* Private macros ----------------------------------------------------- */
/* Public variables --------------------------------------------------- */
// Can Rx message structe
sys_can_msg_rx_set_device_id_confirm_t g_can_msg_device_id_confirm;
sys_can_msg_rx_encoder_value_t g_can_msg_encoder_value;

// Can Tx message structe
sys_can_msg_tx_set_device_id_t g_can_msg_set_device_id;
sys_can_msg_tx_command_t g_can_msg_command;
sys_can_msg_tx_request_sensor_value_t g_can_msg_request_sensor_value;

/* Private variables -------------------------------------------------- */
/* Private function prototypes ---------------------------------------- */
/* Function definitions ----------------------------------------------- */
/* Private function definitions---------------------------------------- */
/* End of file -------------------------------------------------------- */
