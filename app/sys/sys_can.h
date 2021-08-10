/**
 * @file       sys_can.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2021-03-20
 * @author     Thuan Le
 * @brief      System module to handle system CAN bus
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_CAN_H
#define __SYS_CAN_H

/* Includes ----------------------------------------------------------- */
#include <stdbool.h>
#include <stdint.h>
#include "can_driver.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/**
 * @brief Can message send handler enum
 */
typedef enum
{
  TX_MSG_SET_DEVICE_ID,
  TX_MSG_WRITE_COMMAND,
  TX_MSG_REQUEST_READ_SENSOR,
  TX_MSG_COUNT
}
msg_handle_send_t;

/**
 * @brief Can message receive handler enum
 */
typedef enum
{
  RX_MSG_DEVICE_ID_CONFIRM,
  RX_MSG_ENCODER_VALUE,
  RX_MSG_COUNT
}
msg_handle_receive_t;

/**
 * @brief Can message msg command handler enum
 */
typedef enum
{
  MOTOR_FORWARD,
  MOTOR_BACKWARD,
  MOTOR_STOP,
  MOTOR_SPEED,
  MOTOR_CM_COUNT
}
msg_command_t;

/**
 * @brief Can message set device id confirm
 */
typedef struct
{
  uint8_t device_id;
  uint8_t data_1;
  uint8_t data_2;
  uint8_t data_3;
  uint8_t data_4;
  uint8_t data_5;
  uint8_t data_6;
  uint8_t data_7;
}
sys_can_msg_rx_set_device_id_confirm_t;

/**
 * @brief Can message encoder value
 */
typedef struct
{
  uint8_t  device_id;
  uint32_t motor_total_rotation;
  uint16_t motor_speed;
  uint8_t  rotation;
}
sys_can_msg_rx_encoder_value_t;

/**
 * @brief Can message set device id
 */
typedef struct
{
  uint8_t device_id;
  uint8_t data_1;
  uint8_t data_2;
  uint8_t data_3;
  uint8_t data_4;
  uint8_t data_5;
  uint8_t data_6;
  uint8_t data_7;
}
sys_can_msg_tx_set_device_id_t;

/**
 * @brief Can message write command
 */
typedef struct
{
  uint8_t device_id;
  uint8_t command;
  uint8_t motor_speed;
  uint8_t data_3;
  uint8_t data_4;
  uint8_t data_5;
  uint8_t data_6;
  uint8_t data_7;
}
sys_can_msg_tx_command_t;

/**
 * @brief Can message request sensor value
 */
typedef struct
{
  uint8_t device_id;
  uint8_t data_1;
  uint8_t data_2;
  uint8_t data_3;
  uint8_t data_4;
  uint8_t data_5;
  uint8_t data_6;
  uint8_t data_7;
}
sys_can_msg_tx_request_sensor_value_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         System boot
 *
 * @param[in]     None
 * 
 * @attention     None
 *
 * @return        None
 */
void sys_can_send_msg(msg_handle_send_t msg_handle);

/**
 * @brief         System run
 *
 * @param[in]     None
 * 
 * @attention     None
 *
 * @return        None
 */
void sys_can_receive_msg(void);

#endif // __SYS_CAN_H

/* End of file -------------------------------------------------------- */
