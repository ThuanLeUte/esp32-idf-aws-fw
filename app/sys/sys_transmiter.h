/**
 * @file       sys_transmiter.h
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      System module to handle transmit UART
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_TRANSMITER_H
#define __SYS_TRANSMITER_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         System transmiter init
 * 
 * @param[in]     None
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_transmiter_init(void);

#endif /* __SYS_TRANSMITER_H */

/* End of file -------------------------------------------------------- */