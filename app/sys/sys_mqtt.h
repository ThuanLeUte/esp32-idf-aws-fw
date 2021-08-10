/**
 * @file       sys_mqtt.h
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      System module to handle MQTT
 * @note       None
 * @example    None
 */

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_MQTT_H
#define __SYS_MQTT_H

/* Includes ----------------------------------------------------------- */
#include "platform_common.h"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */
/**
 * @brief         System MQTT start
 * 
 * @param[in]     uri   MQTT uri
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_mqtt_start(const char *uri);

/**
 * @brief         System MQTT publish
 * 
 * @param[in]     topic   Pointer to topic
 * @param[in]     data    Pointer to data
 * @param[in]     len     Data lenght
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_mqtt_publish(const char *topic, const char *data, int len);

/**
 * @brief         System MQTT subscribe
 * 
 * @param[in]     topic   Pointer to topic
 * 
 * @attention     None
 * 
 * @return        None
 */
void sys_mqtt_subscribe(const char *topic);

#endif /* __SYS_MQTT_H */

/* End of file -------------------------------------------------------- */