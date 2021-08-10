/**
 * @file       sys_json.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-07-30 
 * @author     Thuan Le
 * @brief      System module to parse and create json data
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_JSON_H
#define __SYS_JSON_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
typedef enum
{
  SYS_JSON_SEND_ENCODE_DATA,
  SYS_JSON_RECEIVE_DEVICE_ID,
  SYS_JSON_RECEIVE_MOTOR_CMD,
  SYS_JSON_COUNT
}
sys_json_type_t;

/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         Json create
 * 
 * @param[in]     p_data    Pointer to data
 * @param[in]     type      Json type
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_json_create(char *p_data, sys_json_type_t type);

/**
 * @brief         Json parse
 * 
 * @param[in]     p_data    Pointer to data
 * @param[in]     len       Data length
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_json_parse(char *p_data, uint32_t len);

#endif /* __SYS_JSON_H */

/* End of file -------------------------------------------------------- */
