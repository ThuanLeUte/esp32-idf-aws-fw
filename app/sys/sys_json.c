/**
 * @file       sys_json.h
 * @copyright  Copyright (C) 2020 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-07-30 
 * @author     Thuan Le
 * @brief      System module to parse and create json data
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------------- */
#include "sys_json.h"
#include "sys_damos_ram.h"
#include "sys_can.h"
#include "frozen.h"
#include "jsmn.h"
#include "aws_iot_config.h"
#include "aws_iot_json_utils.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
static const char *TAG = "sys_json";

/* Private function prototypes ---------------------------------------------- */
void m_sys_json_create_encode_data(char *p_data);

/* Private variables -------------------------------------------------------- */
/* Function definitions ----------------------------------------------------- */
void sys_json_create(char *p_data, sys_json_type_t type)
{
  switch (type)
  {
  case SYS_JSON_SEND_ENCODE_DATA:
    m_sys_json_create_encode_data(p_data);
    break;
  
  default:
    break;
  }
}

void sys_json_parse(char *p_data, uint32_t len)
{
  jsmntok_t *json_obs;
  jsmn_parser json_parser;
  jsmntok_t json_token_struct[MAX_JSON_TOKEN_EXPECTED];

  // Json parse data
  jsmn_init(&json_parser);
  jsmn_parse(&json_parser,
             (char *)p_data,
             (int)len,
             json_token_struct,
             sizeof(json_token_struct) / sizeof(json_token_struct[0]));

  // A response has been recieved from the service that contains set device id data.
  json_obs = findToken("set_device_id", p_data, json_token_struct);
  if (json_obs)
  {
    json_obs = findToken("device_id", p_data, json_token_struct);
    if (json_obs)
    {
      memcpy(&g_can_msg_set_device_id.device_id, p_data + json_obs->start,
             json_obs->end - json_obs->start);

      sys_can_send_msg(TX_MSG_SET_DEVICE_ID);
    }
  }

  // A response has been recieved from the service that contains command to motor data.
  json_obs = findToken("motor_cmd", p_data, json_token_struct);
  if (json_obs)
  {
    // Motor command start
    json_obs = findToken("start", p_data, json_token_struct);
    if (json_obs)
    {
      g_can_msg_command.command = MOTOR_FORWARD;
    }

    // Motor command stop
    json_obs = findToken("stop", p_data, json_token_struct);
    if (json_obs)
    {
      g_can_msg_command.command = MOTOR_STOP;
    }

    // Motor command set speed
    json_obs = findToken("speed", p_data, json_token_struct);
    if (json_obs)
    {
      g_can_msg_command.command = MOTOR_SPEED;

      json_obs = findToken("motor_speed", p_data, json_token_struct);
      if (json_obs)
      {
        memcpy(&g_can_msg_command.motor_speed, p_data + json_obs->start,
               json_obs->end - json_obs->start);
      }
    }

    sys_can_send_msg(TX_MSG_WRITE_COMMAND);
  }
}

/* Private function definitions --------------------------------------------- */
void m_sys_json_create_encode_data(char *p_data)
{
  char buff[200];
  struct json_out out = JSON_OUT_BUF(buff, sizeof(buff));

  json_printf(&out, "{device_id: %d, operation: %Q, total_rotation: %d, motor_speed: %d}",
                      g_can_msg_encoder_value.device_id,
                      "encoder_data",
                      g_can_msg_encoder_value.motor_total_rotation,
                      g_can_msg_encoder_value.motor_speed);

  sprintf((char *)p_data, "%s", buff);

  ESP_LOGW(TAG, "Encoder data json: %s", buff);
}

/* End of file -------------------------------------------------------------- */
