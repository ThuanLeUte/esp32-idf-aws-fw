/**
 * @file       sys_ota.h
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      System module to handle OTA
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------------- */
#include "sys_ota.h"

#include "esp_system.h"
#include "esp_event.h"
#include "esp_ota_ops.h"
#include "esp_http_client.h"
#include "esp_https_ota.h"
#include "sys_aws_config.h"
#include "esp_tls.h"

#include "nvs.h"
#include "nvs_flash.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
static const char *TAG      = "sys/ota";

static const uint8_t server_cert_pem_start[] asm("_binary_server_cert_ota_pem_start");
static const uint8_t server_cert_pem_end[]   asm("_binary_server_cert_ota_pem_end");

/* Private function prototypes ---------------------------------------------- */
static esp_err_t m_http_event_handler(esp_http_client_event_t *evt);

/* Private variables -------------------------------------------------------- */
/* Function definitions ----------------------------------------------------- */
bool sys_ota_start(const char *http_url)
{
  ESP_LOGI(TAG, "Starting OTA");

  esp_http_client_config_t config =
  {
    .url           = http_url,
    .event_handler = m_http_event_handler,
    // .cert_pem      = (char *)server_cert_pem_start,
  };

  esp_err_t ret = esp_https_ota(&config);
  if (ESP_OK == ret)
  {
    return true;
  }
  ESP_LOGE(TAG, "Firmware upgrade failed");

  return false;
}

/* Private function --------------------------------------------------------- */
/**
 * @brief         Http event handler
 * 
 * @param[in]     event    Pointer to event
 * 
 * @attention     None
 * 
 * @return
 *    - ESP_OK
 *    - ESP_FAIL
 */
static esp_err_t m_http_event_handler(esp_http_client_event_t *event)
{
  static int curr_len        = 0;
  static int total_length    = 0;
  static int tmp_percentage  = 0;
  static int prev_percentage = 0;

  switch (event->event_id)
  {
  case HTTP_EVENT_ERROR:
    ESP_LOGI(TAG, "HTTP_EVENT_ERROR");
    break;
  case HTTP_EVENT_ON_CONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_CONNECTED");
    break;
  case HTTP_EVENT_HEADER_SENT:
    ESP_LOGI(TAG, "HTTP_EVENT_HEADER_SENT");
    break;
  case HTTP_EVENT_ON_HEADER:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_HEADER, key=%s, value=%s", event->header_key, event->header_value);
    if (0 == strcmp(event->header_key, "Content-Length"))
    {
      total_length = atoi(event->header_value);
      ESP_LOGI(TAG, "total length: %d", total_length);
    }
    break;
  case HTTP_EVENT_ON_DATA:
    curr_len += event->data_len;
    tmp_percentage = curr_len * 100 / total_length;
    if (tmp_percentage - prev_percentage >= 5)
    {
      prev_percentage = tmp_percentage;
      ESP_LOGI(TAG, "Downloading...%d%%", tmp_percentage);
    }
    break;
  case HTTP_EVENT_ON_FINISH:
    ESP_LOGI(TAG, "HTTP_EVENT_ON_FINISH");
    break;
  case HTTP_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "HTTP_EVENT_DISCONNECTED");
    break;
  }
  return ESP_OK;
}

/* End of file -------------------------------------------------------------- */
