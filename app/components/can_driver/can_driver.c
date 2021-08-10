/**
 * @file       can_driver.c
 * @copyright  Copyright (C) 2021 ThuanLe. All rights reserved.
 * @license    This project is released under the ThuanLe License.
 * @version    1.0.0
 * @date       2020-03-13
 * @author     Thuan Le
 * @brief      CAN bus driver
 * @note       None
 * @example    None
 */

/* Includes ----------------------------------------------------------------- */
#include "can_driver.h"

/* Private enum/structs ----------------------------------------------------- */
/* Private defines ---------------------------------------------------------- */
#define TX_GPIO_NUM             (5)
#define RX_GPIO_NUM             (4)

/* Private function prototypes ---------------------------------------------- */
/* Private variables -------------------------------------------------------- */
static const char *TAG      = "can_driver";

static const can_timing_config_t t_config  = CAN_TIMING_CONFIG_125KBITS();
static const can_filter_config_t f_config  = CAN_FILTER_CONFIG_ACCEPT_ALL();
static const can_general_config_t g_config = CAN_GENERAL_CONFIG_DEFAULT(TX_GPIO_NUM, RX_GPIO_NUM, CAN_MODE_NORMAL);

/* Function definitions ----------------------------------------------------- */
void can_driver_init(void)
{
  // Install CAN driver
  ESP_ERROR_CHECK(can_driver_install(&g_config, &t_config, &f_config));
  ESP_LOGI(TAG, "Can driver installed");

  // Start CAN driver
  ESP_ERROR_CHECK(can_start());
  ESP_LOGI(TAG, "Can driver started");
}

/* Private function --------------------------------------------------------- */
/* End of file -------------------------------------------------------------- */
