/**
 * @file       sys_transmiter.c
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      System module to handle transmit UART data
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------------- */
#include "sys_transmiter.h"
#include "bsp.h"
#include "sys_aws_config.h"
#include "sys_nvs.h"

#include "aws_iot_json_utils.h"
#include "jsmn.h"
#include "frozen.h"
#include "aws_iot_config.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
static const char *TAG = "sys/transmiter";

#define TRANSMITER_TASK_STACK_SIZE   (8192 / sizeof(StackType_t))
#define TRANSMITER_TASK_PRIORITY     (3)
#define RX_BUFFER_LEN                (2000)

/* Private function prototypes ---------------------------------------------- */
static void m_sys_transmiter_task(void *params);
static void m_sys_json_parse_data(char *p_data, uint32_t len);
static bool m_sys_aws_save_certificates(char *p_path, char *p_data, uint16_t data_len);

/* Private variables -------------------------------------------------------- */
static char m_rx_buffer[RX_BUFFER_LEN];
static jsmn_parser  m_json_parser;
static jsmntok_t    m_json_token_struct[MAX_JSON_TOKEN_EXPECTED];

/* Function definitions ----------------------------------------------------- */
void sys_transmiter_init(void)
{
  xTaskCreate(m_sys_transmiter_task,
              "transmiter_task",
              TRANSMITER_TASK_STACK_SIZE,
              NULL,
              TRANSMITER_TASK_PRIORITY,
              NULL);
}

/* Private function --------------------------------------------------------- */
/**
 * @brief         Transmiter task
 * 
 * @param[in]     params    Params
 * 
 * @attention     None
 * 
 * @return        None
 */
static void m_sys_transmiter_task(void *params)
{
  ESP_LOGI(TAG, "Start transmit task");

  while (1)
  {
    if (bsp_uart_read(m_rx_buffer) > 0)
    {
      m_sys_json_parse_data(m_rx_buffer, strlen(m_rx_buffer));
      memset(m_rx_buffer, 0, RX_BUFFER_LEN);
    }
  }
}

/**
 * @brief         Json parse data
 * 
 * @param[in]     p_data  Pointer to data
 *                len     Data lenght
 * 
 * @attention     None
 * 
 * @return        None
 */
static void m_sys_json_parse_data(char *p_data, uint32_t len)
{
  jsmntok_t *json_obs;
  char buf[200];

  // Json parse data that contains certificate data
  jsmn_init(&m_json_parser);
  jsmn_parse(&m_json_parser,
             (char *)p_data,
             len,
             m_json_token_struct,
             sizeof(m_json_token_struct) / sizeof(m_json_token_struct[0]));

  ESP_LOGI(TAG, "Data parse: %s", p_data);

  // A response has been recieved from the service that contains certificate data.
  json_obs = findToken("operation", p_data, m_json_token_struct);
  if (json_obs)
  {
    ESP_LOGI(TAG, "operation");
    if (0 == strncmp("wifi", p_data + json_obs->start, json_obs->end - json_obs->start))
    {
      ESP_LOGI(TAG, "wifi");
      json_obs = findToken("ssid", p_data, m_json_token_struct);
      if (json_obs)
      {
        memset(buf, 0, 200);
        strncpy(buf,  p_data + json_obs->start, json_obs->end - json_obs->start);
        ESP_LOGI(TAG, "ssid: %s", buf);
        sys_nvs_set_wifi_ssid(buf);
      }

      json_obs = findToken("password", p_data, m_json_token_struct);
      if (json_obs)
      {
        memset(buf, 0, 200);
        strncpy(buf,  p_data + json_obs->start, json_obs->end - json_obs->start);
        ESP_LOGI(TAG, "password: %s", buf);
        sys_nvs_set_wifi_password(buf);
      }
    }
    else if (0 == strncmp("certs", p_data + json_obs->start, json_obs->end - json_obs->start))
    {
      ESP_LOGI(TAG, "certs");
      json_obs = findToken("certs_type", p_data, m_json_token_struct);
      if (json_obs)
      {
        if (0 == strncmp("certificate.pem.crt", p_data + json_obs->start, json_obs->end - json_obs->start))
        {
          ESP_LOGI(TAG, "certificatePem");
          json_obs = findToken("data", p_data, m_json_token_struct);
          if (json_obs)
          {
            ESP_LOGI(TAG, "data");
            m_sys_aws_save_certificates(AWS_OFFICIAL_CERTIFICATE_PATH,
                                        p_data + json_obs->start,
                                        json_obs->end - json_obs->start);
          }
        }
        else if (0 == strncmp("private.pem.key", p_data + json_obs->start, json_obs->end - json_obs->start))
        {
          ESP_LOGI(TAG, "privateKey");
          json_obs = findToken("data", p_data, m_json_token_struct);
          if (json_obs)
          {
            ESP_LOGI(TAG, "data");
            m_sys_aws_save_certificates(AWS_OFFICIAL_PRIVATE_KEY_PATH,
                                        p_data + json_obs->start,
                                        json_obs->end - json_obs->start);
          }
        }
        else if (0 == strncmp("root.ca.pem", p_data + json_obs->start, json_obs->end - json_obs->start))
        {
          ESP_LOGI(TAG, "rootCa");
          json_obs = findToken("data", p_data, m_json_token_struct);
          if (json_obs)
          {
            ESP_LOGI(TAG, "data");
            m_sys_aws_save_certificates(AWS_OFFICIAL_ROOT_CA_PATH,
                                        p_data + json_obs->start,
                                        json_obs->end - json_obs->start);
          }
        }
        else
        {
          // Do notthing
        }
      }
    }
    else if (0 == strncmp("aws_endpoint", p_data + json_obs->start, json_obs->end - json_obs->start))
    {
      json_obs = findToken("aws_endpoint", p_data, m_json_token_struct);
      if (json_obs)
      {
        memset(buf, 0, 200);
        strncpy(buf,  p_data + json_obs->start, json_obs->end - json_obs->start);
        ESP_LOGI(TAG, "aws_endpoint: %s", buf);
        sys_nvs_set_aws_endpoint(buf);
      }
    }
  }
}

/**
 * @brief         Save certificates to SPIFF
 *
 * @param[in]     p_path        Pointer to save data destination
 * @param[in]     p_data        Pointer data
 * @param[in]     data_len      Data length
 *
 * @attention     None
 *
 * @return
 *    - true:   Save data success
 *    - failed: Save data fail
 */
static bool m_sys_aws_save_certificates(char *p_path, char *p_data, uint16_t data_len)
{
  FILE        *fp = NULL;
  struct stat file_stat;
  uint8_t     index = 1;
  uint16_t    data_start_pos[100];

  unlink(p_path);
  if (0 == stat(p_path, &file_stat))
  {
    fp = fopen(p_path, "r+");
    ESP_LOGI(TAG, "%s: Tag_file existed", __FUNCTION__);
  }
  else
  {
    fp = fopen(p_path, "w+");
    ESP_LOGW(TAG, "%s: Tag_file not existed", __FUNCTION__);
  }

  if (NULL == fp)
  {
    ESP_LOGE(TAG, "%s: Open file failed", __FUNCTION__);
    return false;
  }

  data_start_pos[0] = 0;
  for (int i = 0; i < data_len; i++)
  {
    // Analyze data, remove '\r\n'
    if ((p_data[i] == '\\') && (p_data[i + 1] == 'r') && (p_data[i + 2] == '\\') && (p_data[i + 3] == 'n'))
    {
      data_start_pos[index] = i + 4;
      p_data[i] = 0;
      p_data[i + 1] = 0;
      p_data[i + 2] = 0;
      p_data[i + 3] = 0;
      index++;
    }
    else
    {
      // Analyze data, remove '\n'
      if ((p_data[i] == '\\') && (p_data[i + 1] == 'n'))
      {
        data_start_pos[index] = i + 2;
        p_data[i] = 0;
        index++;
      }
    }
  }

  // Write data to file system
  for (int i = 0; i < (index - 1); i++)
  {
    fprintf(fp, "%s\n", &p_data[data_start_pos[i]]);
    ESP_LOGW(TAG, "Print data: %s", &p_data[data_start_pos[i]]);
  }

  fclose(fp);

  return true;
}


/* End of file -------------------------------------------------------------- */
