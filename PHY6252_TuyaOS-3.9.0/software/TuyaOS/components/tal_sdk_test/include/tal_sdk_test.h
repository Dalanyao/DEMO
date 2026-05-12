/**
 * @file tal_sdk_test.h
 * @brief This is tuya app_tal_test file
 * @version 1.0
 * @date 2021-09-10
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#include "tuya_cloud_types.h"
#if TUYA_SDK_TEST

#ifndef __TUYA_APP_TAL_TEST_H__
#define __TUYA_APP_TAL_TEST_H__

/***********************************************************************
 ** INCLUDE                                                           **
 **********************************************************************/
#if (1 == TUYA_SDK_TEST_TYPE)
#include "tuya_ble_type.h"
#include "tal_bluetooth_def.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif


/***********************************************************************
 ** CONSTANT ( MACRO AND ENUM )                                       **
 **********************************************************************/
#define TEST_GID_SYSTEM                 0x01
#define TEST_GID_DEVICE_INFO            0x02
#define TEST_GID_ADV                    0x03
#define TEST_GID_SCAN                   0x04
#define TEST_GID_CONN                   0x05
#define TEST_GID_DATA                   0x06
#define TEST_GID_ELSE                   0x07
#define TEST_GID_GPIO                   0x10
#define TEST_GID_UART                   0x11
#define TEST_GID_PWM                    0x12
#define TEST_GID_ADC                    0x13
#define TEST_GID_SPI                    0x14
#define TEST_GID_IIC                    0x15
#define TEST_GID_RTC                    0x16
#define TEST_GID_FLASH                  0x17
#define TEST_GID_WATCHDOG               0x18
#define TEST_GID_POWERMANGER            0x19

// TEST_GID_SYSTEM
#define TEST_CID_SHAKE_HAND             0x00
#define TEST_CID_RESET                  0x01
#define TEST_CID_FACTORY_RESET          0x02
#define TEST_CID_GET_TIME               0x03
#define TEST_CID_REQ_TIME               0x04
#define TEST_CID_GET_RAND               0x05
#define TEST_CID_GET_QUEUE_SIZE         0x06
#define TEST_CID_POWER_ON               0x80

// TEST_GID_DEVICE_INFO
#define TEST_CID_SET_MAC                0x00
#define TEST_CID_GET_MAC                0x01
#define TEST_CID_SET_PID                0x02
#define TEST_CID_GET_PID                0x03
#define TEST_CID_SET_VERSION            0x04
#define TEST_CID_GET_VERSION            0x05

// TEST_GID_ADV
#define TEST_CID_ADV_ENABLE             0x00
#define TEST_CID_SET_ADV_INTERVAL       0x01
#define TEST_CID_SET_ADV_PARAM          0x02
#define TEST_CID_SET_ADV_DATA           0x03
#define TEST_CID_CONN_REQUEST           0x04
#define TEST_CID_MESH_FAST_PROV         0x05

// TEST_GID_SCAN

// TEST_GID_CONN
#define TEST_CID_CONN                   0x00
#define TEST_CID_SET_CONN_INTERVAL      0x01
#define TEST_CID_SET_CONN_PARAM         0x02
#define TEST_CID_DISCONN                0x03
#define TEST_CID_NET_STATE              0x04
#define TEST_CID_GET_MESH_ADDR          0x05
#define TEST_CID_UNBIND_MODE            0x80
#define TEST_CID_CONN_PARAM_UPDATE      0x81
#define TEST_CID_MTU_UPDATE             0x82

// TEST_GID_DATA
#define TEST_CID_MASTER_SEND            0x00
#define TEST_CID_SLAVE_SEND             0x01
#define TEST_CID_DP_REPORT              0x02
#define TEST_CID_LONG_DP_REPORT         0x03
#define TEST_CID_DP_REPORT_TIME         0x04
#define TEST_CID_LONG_DP_REPORT_TIME    0x05
#define TEST_CID_DP_PASSTHROUGH         0x06
#define TEST_CID_MESH_DP_REPORT         0x07
#define TEST_CID_DP_REPORT_RSP          0x80
#define TEST_CID_DP_REPORT_TIME_RSP     0x81
#define TEST_CID_DP_WRITE               0x82
#define TEST_CID_LONG_DP_WRITE          0x83
#define TEST_CID_MESH_DP_WRITE          0x84

// TEST_GID_ELSE
#define TEST_CID_GET_WEATHER            0x00
#define TEST_CID_SET_BULK_DATA          0x01
#define TEST_CID_GET_WEATHER_RSP        0x80
#define TEST_CID_GET_WEATHER_DATA       0x81
	
// TEST_GID_GPIO
#define TEST_CID_PIN_DEINIT             0x00
#define TEST_CID_OUTPUT_HIGH            0x01
#define TEST_CID_OUTPUT_LOW             0x02
#define TEST_CID_PIN_READ               0x03

// TEST_GID_UART
#define TEST_CID_SET_BAUDRATE           0x00
#define TEST_CID_TX_UART_DATA           0x01
#define TEST_CID_RX_UART_PORT           0x80
#define TEST_CID_RX_UART_DATA           0x81

// TEST_GID_PWM
#define TEST_CID_PWM_DEINIT             0x00
#define TEST_CID_SET_FREQ_DUTY          0x01

// TEST_GID_ADC
#define TEST_CID_ADC_DEINIT             0x00
#define TEST_CID_READ_ADC_DATA          0x01
#define TEST_CID_READ_ADC_DATA_RSP      0x80

// TEST_GID_SPI
#define TEST_CID_TX_SPI_DATA            0x00
#define TEST_CID_RX_SPI_DATA            0x80

// TEST_GID_IIC
#define TEST_CID_TX_IIC_DATA            0x00
#define TEST_CID_RX_IIC_DATA            0x80

// TEST_GID_RTC
#define TEST_CID_SET_RTC_TIME           0x00
#define TEST_CID_GET_RTC_TIME           0x01
#define TEST_CID_START_RTC              0x02
#define TEST_CID_STOP_RTC               0x03

// TEST_GID_FLASH
#define TEST_CID_READ_FLASH_DATA        0x00
#define TEST_CID_ERASE_FLASH_DATA       0x01
#define TEST_CID_WRITE_FLASH_DATA       0x02

// TEST_GID_WATCHDOG
#define TEST_CID_START_WDG              0x00
#define TEST_CID_FEED_WDG               0x01
#define TEST_CID_STOP_WDG               0x02

// TEST_GID_POWERMANGER
#define TEST_CID_ENTER_SLEEP            0x00


#define TEST_ID_GET(GID, CID)           ((GID << 8) | CID)

/***********************************************************************
 ** STRUCT                                                            **
 **********************************************************************/


/***********************************************************************
 ** VARIABLE                                                          **
 **********************************************************************/


/***********************************************************************
 ** FUNCTON                                                           **
 **********************************************************************/




/**
 * @brief 
 * 
 * @param[in] param1: 
 * @param[in] param2: 
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
#if (1 == TUYA_SDK_TEST_TYPE)
VOID_T tal_sdk_test_ble_evt_callback(TAL_BLE_EVT_PARAMS_T *p_event);
VOID_T tal_sdk_test_ble_protocol_callback(tuya_ble_cb_evt_param_t* event);
#endif

VOID_T tal_sdk_test_init(VOID_T);
OPERATE_RET test_group_system(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_device_info(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_adv(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_scan(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_conn(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_data(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_else(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_gpio(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_uart(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_pwm(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_adc(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_spi(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_iic(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_rtc(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_flash(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_watchdog(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_group_powermanger(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data);
OPERATE_RET test_cmd_send(UINT16_T cmdId, UINT8_T* buf, UINT16_T size);
VOID_T tal_ble_sdk_test_wake_up_handler(VOID_T);


#ifdef __cplusplus
}
#endif

#endif /* __TUYA_APP_TAL_TEST_H__ */

#endif /* TUYA_SDK_TEST */

