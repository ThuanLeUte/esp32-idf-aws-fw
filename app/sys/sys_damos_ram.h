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

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_DAMOS_RAM_H
#define __SYS_DAMOS_RAM_H

/* Includes ----------------------------------------------------------- */
#include <stdint.h>
#include "sys_can.h"

/* Public defines ----------------------------------------------------- */
// Can Rx message structe
extern sys_can_msg_rx_set_device_id_confirm_t g_can_msg_device_id_confirm;
extern sys_can_msg_rx_encoder_value_t g_can_msg_encoder_value;

// Can Tx message structe
extern sys_can_msg_tx_set_device_id_t g_can_msg_set_device_id;
extern sys_can_msg_tx_command_t g_can_msg_command;
extern sys_can_msg_tx_request_sensor_value_t g_can_msg_request_sensor_value;

/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */

#endif // __SYS_DAMOS_RAM_H

/* End of file -------------------------------------------------------- */
