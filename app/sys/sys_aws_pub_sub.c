/**
* @file       sys_aws_provision.c
* @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
* @license    This project is released under the ThuanLe License.
* @version    01.00.00
* @date       2020-12-17
* @author     Thuan Le
* @brief      System module to handle Amazon Web Services publish and subcribe (AWS)
* @note       None
* @example    None
*/

/* Includes ----------------------------------------------------------------- */
#include "sys_aws_pub_sub.h"
#include "sys_aws_config.h"
#include "sys_json.h"
#include "sys_nvs.h"

#include "platform_common.h"
#include "aws_iot_config.h"
#include "aws_iot_version.h"
#include "aws_iot_mqtt_client_interface.h"
#include "jsmn.h"
#include "aws_iot_json_utils.h"
#include "frozen.h"
#include "esp_spiffs.h"
#include <sys/stat.h>

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
#define AWS_PUB_SUB_TASK_STACK_SIZE       (8192 / sizeof(StackType_t))
#define AWS_PUB_SUB_TASK_PRIORITY         (3)
#define AWS_PUB_SUB_NUM_SUBSCRIBE_TOPIC   (1)

/* Private Constants -------------------------------------------------------- */
static const char *TAG = "sys/aws_pub_sub";
static const char *AWS_SUB_TOPIC[] =
{
  "$aws/certificates/create/json/accepted",
  "$aws/device/test"
};

static const char *AWS_PUB_TOPIC[] =
{
  "sdkTest/sub"
};

/* Private variables -------------------------------------------------------- */
static char m_aws_endpoint[100];
static AWS_IoT_Client             m_aws_client;
static IoT_Publish_Message_Params m_params_publish_msg;

/* Public variables --------------------------------------------------------- */
static const uint8_t aws_root_ca_pem_start[]      asm("_binary_aws_root_ca_pem_start");
static const uint8_t aws_root_ca_pem_end[]        asm("_binary_aws_root_ca_pem_end");
static const uint8_t certificate_pem_crt_start[]  asm("_binary_certificate_pem_crt_start");
static const uint8_t certificate_pem_crt_end[]    asm("_binary_certificate_pem_crt_end");
static const uint8_t private_pem_key_start[]      asm("_binary_private_pem_key_start");
static const uint8_t private_pem_key_end[]        asm("_binary_private_pem_key_end");

/* Private function prototypes ---------------------------------------------- */
static void m_sys_aws_pub_sub_task(void *params);
static bool m_sys_aws_connect(void);
static void m_sys_aws_disconnect_callback_handler(AWS_IoT_Client *p_client, void *data);
static void m_sys_aws_subscribe_callback_handler(AWS_IoT_Client             *p_client,
                                                 char                       *topic_name,
                                                 uint16_t                   topic_name_len,
                                                 IoT_Publish_Message_Params *params,
                                                 void                       *p_data);

/* Function definitions ----------------------------------------------------- */
void  sys_aws_pub_sub_start(void)
{
  xTaskCreate(m_sys_aws_pub_sub_task,
              "aws_provision_task",
              AWS_PUB_SUB_TASK_STACK_SIZE,
              NULL,
              AWS_PUB_SUB_TASK_PRIORITY,
              NULL);
}

bool m_sys_aws_subscribe(void)
{
  IoT_Error_t rc;

  ESP_LOGI(TAG, "Subscribing...: %s", AWS_SUB_TOPIC[0]);

  CHECK(SUCCESS == aws_iot_mqtt_subscribe(&m_aws_client, AWS_SUB_TOPIC[0],
                                          strlen(AWS_SUB_TOPIC[0]),
                                          QOS0,
                                          m_sys_aws_subscribe_callback_handler,
                                          NULL), false);

  rc = aws_iot_mqtt_subscribe(&m_aws_client, AWS_SUB_TOPIC[1],
                                          strlen(AWS_SUB_TOPIC[1]),
                                          QOS1,
                                          m_sys_aws_subscribe_callback_handler,
                                          NULL);
  ESP_LOGI(TAG, "Error: %d", rc);

  CHECK(SUCCESS == aws_iot_mqtt_subscribe(&m_aws_client, "sdkTest/sub", 11, QOS0,
                                          m_sys_aws_subscribe_callback_handler,
                                          NULL), false);

  return true;
}

bool m_sys_aws_publish(void)
{
  char buf[1000] = "";

  // Create json format
  sys_json_create(buf, SYS_JSON_SEND_ENCODE_DATA);

  m_params_publish_msg.payload    = (void *)buf;
  m_params_publish_msg.payloadLen = strlen(buf);

  ESP_LOGI(TAG, "Publishing...: %s", AWS_PUB_TOPIC[0]);
  CHECK(SUCCESS == aws_iot_mqtt_publish(&m_aws_client,
                                        AWS_PUB_TOPIC[0],
                                        strlen(AWS_PUB_TOPIC[0]),
                                        &m_params_publish_msg), false);

  return true;
}

/* Private function definitions --------------------------------------------- */
/**
 * @brief         AWS pub sub task
 *
 * @param[in]     params    Pointer to params
 *
 * @attention     None
 *
 * @return        None
 */
static void m_sys_aws_pub_sub_task(void *params)
{
  // Get AWS endpoint
  sys_nvs_get_aws_endpoint(m_aws_endpoint);

  // Connect to AWS
  m_sys_aws_connect();

  m_sys_aws_subscribe();

  while (1)
  {
    vTaskDelay(pdMS_TO_TICKS(1000));
    aws_iot_mqtt_yield(&m_aws_client, 1000);
  }
}

/**
 * @brief         AWS connect
 *
 * @param[in]     None
 *
 * @attention     None
 *
 * @return
 *  - true:   Connect success
 *  - false:  Connect failed
 */
static bool m_sys_aws_connect(void)
{
  IoT_Client_Init_Params    mqtt_init_params = iotClientInitParamsDefault;
  IoT_Client_Connect_Params connect_params   = iotClientConnectParamsDefault;

  mqtt_init_params.enableAutoReconnect       = false;
  mqtt_init_params.pHostURL                  = AWS_HOST_ADDRESS;
  mqtt_init_params.port                      = AWS_PORT;

  mqtt_init_params.pRootCALocation           = (const char *)AWS_OFFICIAL_ROOT_CA_PATH;
  mqtt_init_params.pDeviceCertLocation       = (const char *)AWS_OFFICIAL_CERTIFICATE_PATH;
  mqtt_init_params.pDevicePrivateKeyLocation = (const char *)AWS_OFFICIAL_PRIVATE_KEY_PATH;

  mqtt_init_params.mqttCommandTimeout_ms     = 20000;
  mqtt_init_params.tlsHandshakeTimeout_ms    = 5000;
  mqtt_init_params.isSSLHostnameVerify       = true;
  mqtt_init_params.disconnectHandler         = m_sys_aws_disconnect_callback_handler;
  mqtt_init_params.disconnectHandlerData     = NULL;

  connect_params.keepAliveIntervalInSec      = 60;
  connect_params.isCleanSession              = true;
  connect_params.MQTTVersion                 = MQTT_3_1_1;

  connect_params.pClientID                   = AWS_CLIENT_ID;
  connect_params.clientIDLen                 = (uint16_t)strlen(AWS_CLIENT_ID);
  connect_params.isWillMsgPresent            = false;

  // AWS mqtt init
  ESP_LOGI(TAG, "AWS init...");
  CHECK(SUCCESS == aws_iot_mqtt_init(&m_aws_client, &mqtt_init_params), false);

  // Connecting to AWS
  ESP_LOGI(TAG, "AWS connect...");
  CHECK(SUCCESS == aws_iot_mqtt_connect(&m_aws_client, &connect_params), false);

  // AWS auto reconnect set
  CHECK(SUCCESS == aws_iot_mqtt_autoreconnect_set_status(&m_aws_client, true), false);

  return true;
}

/**
 * @brief         AWS subscibe callback handler
 *
 * @param[in]     p_client        Pointer to client
 * @param[in]     topic_name      Pointer to topic name
 * @param[in]     topic_name_len  Topic name length
 * @param[in]     params          Pointer to params
 * @param[in]     p_data          Pointer to data
 *
 * @attention     None
 *
 * @return        None
 */
static void m_sys_aws_subscribe_callback_handler(AWS_IoT_Client *p_client,
                                                 char *topic_name,
                                                 uint16_t topic_name_len,
                                                 IoT_Publish_Message_Params *params,
                                                 void *p_data)
{
  ESP_LOGW(TAG, "Subscribe callback");
  ESP_LOGI(TAG, "%.*s\t%.*s", topic_name_len, topic_name, (int)params->payloadLen, (char *)params->payload);
}

/**
 * @brief         AWS disconnect callback handler
 *
 * @param[in]     p_client        Pointer to client
 * @param[in]     data            Pointer to data
 *
 * @attention     None
 *
 * @return        None
 */
static void m_sys_aws_disconnect_callback_handler(AWS_IoT_Client *p_client, void *data)
{
  ESP_LOGW(TAG, "MQTT Disconnect");
  IoT_Error_t status = FAILURE;

  if (NULL == p_client)
    return;

  if (aws_iot_is_autoreconnect_enabled(p_client))
  {
    ESP_LOGI(TAG, "Auto Reconnect is enabled, Reconnecting attempt will start now");
  }
  else
  {
    ESP_LOGW(TAG, "Auto Reconnect not enabled. Starting manual reconnect...");
    status = aws_iot_mqtt_attempt_reconnect(p_client);
    if (NETWORK_RECONNECTED == status)
    {
      ESP_LOGW(TAG, "Manual Reconnect Successful");
    }
    else
    {
      ESP_LOGW(TAG, "Manual Reconnect Failed - %d", status);
    }
  }
}

/* End of file -------------------------------------------------------------- */