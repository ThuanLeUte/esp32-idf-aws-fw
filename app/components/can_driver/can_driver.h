/**
 * @file       can_driver.h
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      CAN bus driver
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __CAN_DRIVER_H
#define __CAN_DRIVER_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         Can driver init
 * 
 * @param[in]     None
 * 
 * @attention     None
 * 
 * @return        None
 */
void can_driver_init(void);

#endif /* __CAN_DRIVER_H */

/* End of file -------------------------------------------------------- */