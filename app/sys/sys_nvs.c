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

/* Includes ----------------------------------------------------------------- */
#include "sys_nvs.h"
#include "nvs.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
static nvs_handle m_nvs_handle;

/* Private function prototypes ---------------------------------------------- */
/* Private variables -------------------------------------------------------- */
/* Function definitions ----------------------------------------------------- */
void sys_nvs_set_wifi_ssid(const char *wifi_ssid)
{
  nvs_open("wifi_ssid", NVS_READWRITE, &m_nvs_handle);
  nvs_set_str(m_nvs_handle, "wifi_ssid", wifi_ssid);
  nvs_close(m_nvs_handle);
}

void sys_nvs_get_wifi_ssid(char *wifi_ssid)
{
  char log[100];
  size_t log_size = sizeof(log);
  nvs_open("wifi_ssid", NVS_READONLY, &m_nvs_handle);
  nvs_get_str(m_nvs_handle, "wifi_ssid", log, &log_size);
  strcpy(wifi_ssid, (const char *) &log);
  nvs_close(m_nvs_handle);
}

void sys_nvs_set_wifi_password(const char *wifi_password)
{
  nvs_open("wifi_password", NVS_READWRITE, &m_nvs_handle);
  nvs_set_str(m_nvs_handle, "wifi_password", wifi_password);
  nvs_close(m_nvs_handle);
}

void sys_nvs_get_wifi_password(char *wifi_password)
{
  char log[100];
  size_t log_size = sizeof(log);
  nvs_open("wifi_password", NVS_READONLY, &m_nvs_handle);
  nvs_get_str(m_nvs_handle, "wifi_password", log, &log_size);
  strcpy(wifi_password, (const char *) &log);
  nvs_close(m_nvs_handle);
}

void sys_nvs_set_aws_endpoint(const char *aws_endpoint)
{
  nvs_open("aws_endpoint", NVS_READWRITE, &m_nvs_handle);
  nvs_set_str(m_nvs_handle, "aws_endpoint", aws_endpoint);
  nvs_close(m_nvs_handle);
}

void sys_nvs_get_aws_endpoint(char *aws_endpoint)
{
  char log[100];
  size_t log_size = sizeof(log);
  nvs_open("aws_endpoint", NVS_READONLY, &m_nvs_handle);
  nvs_get_str(m_nvs_handle, "aws_endpoint", log, &log_size);
  strcpy(aws_endpoint, (const char *) &log);
  nvs_close(m_nvs_handle);
}

/* End of file -------------------------------------------------------------- */
