/**
* @file       sys_aws_pub_sub.h
* @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
* @license    This project is released under the ThuanLe License.
* @version    01.00.00
* @date       2020-12-17
* @author     Thuan Le
* @brief      System module to handle Amazon Web Services publish and subcribe (AWS)
* @note       None
* @example    None
*/

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_AWS_PUB_SUB_H
#define __SYS_AWS_PUB_SUB_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         AWS start pub sub
 *
 * @param[in]     None
 *
 * @attention     None
 *
 * @return        None
 */
void sys_aws_pub_sub_start(void);

/**
 * @brief         AWS enable monitor
 *
 * @param[in]     None
 *
 * @attention     None
 *
 * @return
 *  - true:   Enable monitor success
 *  - false:  Enable monitor failed
 */
bool m_sys_aws_subscribe(void);

/**
 * @brief         AWS register thing
 *
 * @param[in]     None
 *
 * @attention     None
 *
 * @return
 *  - true:   AWS register thing success
 *  - false:  AWS register thing failed
 */
bool m_sys_aws_publish(void);

#endif /* __SYS_AWS_PUB_SUB_H */

/* End of file -------------------------------------------------------- */