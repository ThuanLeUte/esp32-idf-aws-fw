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

/* Includes ----------------------------------------------------------------- */
#include "sys_mqtt.h"
#include "mqtt_client.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
static const char *TAG      = "sys/mqtt";

/* Private function prototypes ---------------------------------------------- */
esp_mqtt_client_handle_t m_mqtt_client;

/* Private variables -------------------------------------------------------- */
static void m_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data);
static esp_err_t m_mqtt_event_handler_cb(esp_mqtt_event_handle_t event);

/* Function definitions ----------------------------------------------------- */
void sys_mqtt_start(const char *uri)
{
  esp_mqtt_client_config_t mqtt_cfg = {
      .uri = uri,
  };

  m_mqtt_client = esp_mqtt_client_init(&mqtt_cfg);
  esp_mqtt_client_register_event(m_mqtt_client, ESP_EVENT_ANY_ID, m_mqtt_event_handler, m_mqtt_client);
  esp_mqtt_client_start(m_mqtt_client);
}

void sys_mqtt_publish(const char *topic, const char *data, int len)
{
  esp_mqtt_client_publish(m_mqtt_client, topic, data, len, 1, 0);
}

void sys_mqtt_subscribe(const char *topic)
{
  esp_mqtt_client_subscribe(m_mqtt_client, topic, 1);
}

/* Private function --------------------------------------------------------- */
/**
 * @brief         MQTT event handler
 * 
 * @param[in]     handler_args    Pointer to handler argument
 * @param[in]     base            Pointer event base
 * @param[in]     event_id        Pointer to event id
 * @param[in]     event_data      Pointer to event data
 * 
 * @attention     None
 * 
 * @return        None
 */
static void m_mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
  ESP_LOGI(TAG, "Event dispatched from event loop base=%s, event_id=%d", base, event_id);
  m_mqtt_event_handler_cb(event_data);
}

/**
 * @brief         MQTT event handler callback
 * 
 * @param[in]     event    Pointer to event
 * 
 * @attention     None
 * 
 * @return      esp_err_t enum
 */
static esp_err_t m_mqtt_event_handler_cb(esp_mqtt_event_handle_t event)
{
  esp_mqtt_client_handle_t client = event->client;
  int msg_id;
  switch (event->event_id)
  {
  case MQTT_EVENT_CONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_CONNECTED");
    msg_id = esp_mqtt_client_publish(client, "/topic/qos1", "data_3", 0, 1, 0);
    ESP_LOGI(TAG, "sent publish successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, "/topic/qos0", 0);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_subscribe(client, "/topic/qos1", 1);
    ESP_LOGI(TAG, "sent subscribe successful, msg_id=%d", msg_id);

    msg_id = esp_mqtt_client_unsubscribe(client, "/topic/qos1");
    ESP_LOGI(TAG, "sent unsubscribe successful, msg_id=%d", msg_id);
    break;
  case MQTT_EVENT_DISCONNECTED:
    ESP_LOGI(TAG, "MQTT_EVENT_DISCONNECTED");
    break;

  case MQTT_EVENT_SUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_UNSUBSCRIBED:
    ESP_LOGI(TAG, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_PUBLISHED:
    ESP_LOGI(TAG, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
    break;
  case MQTT_EVENT_DATA:
    ESP_LOGI(TAG, "MQTT_EVENT_DATA");
    printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
    printf("DATA=%.*s\r\n", event->data_len, event->data);
    break;
  case MQTT_EVENT_ERROR:
    ESP_LOGI(TAG, "MQTT_EVENT_ERROR");
    break;
  default:
    ESP_LOGI(TAG, "Other event id:%d", event->event_id);
    break;
  }

  return ESP_OK;
}

/* End of file -------------------------------------------------------------- */
