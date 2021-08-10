/**
* @file       sys_aws_config.h
* @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
* @license    This project is released under the ThuanLe License.
* @version    01.00.00
* @date       2020-12-24
* @author     Thuan Le
* @brief      AWS file config
* @note       None
* @example    None
*/

/* Define to prevent recursive inclusion ------------------------------ */
#ifndef __SYS_AWS_CONFIG_H
#define __SYS_AWS_CONFIG_H

/* Includes ----------------------------------------------------------- */
#define AWS_HOST_ADDRESS          "a1dijy20fc5xqe-ats.iot.us-east-2.amazonaws.com"

#define AWS_PORT                  (8883)
#define AWS_CLIENT_ID             "soncabox"

#define AWS_OFFICIAL_CERTIFICATE_PATH    "/spiffs/certificate.pem.crt"
#define AWS_OFFICIAL_PRIVATE_KEY_PATH    "/spiffs/private.pem.key"
#define AWS_OFFICIAL_ROOT_CA_PATH        "/spiffs/root.ca.pem"

/* Public defines ----------------------------------------------------- */
/* Public enumerate/structure ----------------------------------------- */
/* Public macros ------------------------------------------------------ */
/* Public variables --------------------------------------------------- */
/* Public function prototypes ----------------------------------------- */

#endif /* __SYS_AWS_CONFIG_H */

/* End of file -------------------------------------------------------- */