/**
 * @file       sys_nvs.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-07-30 
 * @author     Thuan Le
 * @brief      System module to handle Non-Volatile Storage (NVS)
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_NVS_H
#define __SYS_NVS_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         NVS set wifi ssid
 * 
 * @param[in]     wifi_ssid      Pointer to wifi ssid
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_set_wifi_ssid(const char *wifi_ssid);

/**
 * @brief         NVS get wifi ssid
 * 
 * @param[in]     wifi_ssid      Pointer to wifi ssid
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_get_wifi_ssid(char *wifi_ssid);

/**
 * @brief         NVS set wifi password
 * 
 * @param[in]     wifi_password  Pointer to wifi password
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_set_wifi_password(const char *wifi_password);

/**
 * @brief         NVS get wifi password
 * 
 * @param[in]     wifi_password  Pointer to wifi password
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_get_wifi_password(char *wifi_password);

/**
 * @brief         NVS set aws endpoint
 * 
 * @param[in]     aws_endpoint  Pointer to aws endpoint
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_set_aws_endpoint(const char *aws_endpoint);

/**
 * @brief         NVS get aws endpoint
 * 
 * @param[in]     aws_endpoint  Pointer to aws endpoint
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_nvs_get_aws_endpoint(char *aws_endpoint);

#endif /* __SYS_NVS_H */

/* End of file -------------------------------------------------------- */
