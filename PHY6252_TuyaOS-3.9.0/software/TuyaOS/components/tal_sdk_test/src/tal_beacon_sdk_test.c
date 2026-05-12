/**
 * @file tal_sdk_test.c
 * @brief This is tuya sdk_test file
 * @version 1.0
 * @date 2021-09-10
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */


/***********************************************************************
 ** INCLUDE                                                           **
 **********************************************************************/
#include "board.h"

#if TUYA_SDK_TEST
  
#if (3 == TUYA_SDK_TEST_TYPE)

 #define TUYA_BLE_SEND_MAX_DP_DATA_LEN  6
 /***********************************************************************
  ** STRUCT                                                            **
  **********************************************************************/
#pragma pack(1)
 typedef struct {
     UINT16_T sof;
     UINT8_T  version;
     UINT8_T  id;
     UINT16_T len;
     UINT8_T  type;
     UINT8_T  group_id;
     UINT8_T  cmd_id;
     UINT8_T  value[];
 } test_cmd_t;

#pragma pack()

 /***********************************************************************
  ** VARIABLE                                                          **
  **********************************************************************/
  
 STATIC UINT8_T tx_buffer[128] = {0x66, 0xAA, 0x00, 0xF2};

 //STATIC TIMER_ID sg_test_enter_sleep_timer_id = NULL;
 
 //STATIC TAL_ADC_CFG_T tal_adc_cfg = {0};
 //STATIC TAL_PWM_CFG_T tal_pwm_cfg = {0};


 /***********************************************************************
  ** FUNCTON                                                           **
  **********************************************************************/

 UINT8_T tal_util_check_sum8(UINT8_T* buf, UINT32_T size)
 {
     UINT8_T sum = 0;
     for(UINT32_T idx=0; idx<size; idx++) {
         sum += buf[idx];
     }
     return sum;
 }

 TUYA_WEAK_ATTRIBUTE OPERATE_RET test_cmd_send(UINT16_T cmdId, UINT8_T* buf, UINT16_T size)
 {
     UINT32_T len = 4;
     
     tx_buffer[len++] = (size+3)>>8;
     tx_buffer[len++] = (size+3)&0xFF;
     
     tx_buffer[len++] = 0x03;
     
     tx_buffer[len++] = cmdId>>8;
     tx_buffer[len++] = cmdId&0xFF;
     
     if(size > 0) {
         memcpy(&tx_buffer[len], buf, size);
         len += size;
     }
     
     tx_buffer[len] = tal_util_check_sum8(tx_buffer, len);
     len += 1;
     
 //    tuya_ble_production_test_asynchronous_response(0, tx_buffer, len);
#ifdef VENDOR_BOARD_UART
	tkl_uart_write(VENDOR_BOARD_UART,tx_buffer, len);
#else
     tkl_uart_write(1,tx_buffer, len);
#endif
     
     if(cmdId != 0x0105) {
         print("test_rsp\r\n");
     }
     
     return OPRT_OK;
 }

 TUYA_WEAK_ATTRIBUTE VOID_T tal_sdk_test_init(VOID_T)
{
    //tkl_i2c_init(I2C_CHANNEL, NULL);
    
    //tal_sw_timer_create(tal_sdk_test_enter_sleep_handler, NULL, &sg_test_enter_sleep_timer_id);
    
    test_cmd_send(TEST_ID_GET(TEST_GID_SYSTEM, TEST_CID_POWER_ON), NULL, 0);
}

VOID_T tuya_beacon_app_sdk_test_process(UINT8_T *p_in_data, UINT16_T in_len)
{
    test_cmd_t* cmd = (VOID_T*)p_in_data;
    //tal_util_reverse_byte((VOID_T*)&cmd->len, SIZEOF(UINT16_T));
    
    if((cmd->type != 3) || (cmd->len < 3)){
        print("cmd error!\r\n");
        return;
    }
    cmd->len = ( ( cmd->len & 0x00ff) << 8 | ( cmd->len & 0xff00) >> 8);
    UINT16_T cmd_data_len = cmd->len - 3;
    
    BOOL_T   rsp_flag = TRUE;
    UINT8_T  rsp_data[256] = {0};
    UINT32_T rsp_len = 0;

    switch (cmd->group_id)
    {
        case TEST_GID_GPIO: 
        {
            rsp_len = test_group_gpio(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        }
        
        case TEST_GID_IIC:
        {
            rsp_len = test_group_iic(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 
        
         case TEST_GID_UART:
        {
            rsp_len = test_group_uart(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 
        
        case TEST_GID_PWM:
        {
            rsp_len = test_group_pwm(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 

        case TEST_GID_ADC:
        {
            rsp_len = test_group_adc(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 
        
        case TEST_GID_FLASH:
        {
            rsp_len = test_group_flash(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 
        case TEST_GID_WATCHDOG:
        {
            rsp_len = test_group_watchdog(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        } 
        case TEST_GID_SPI:
        {
            rsp_len = test_group_spi(cmd->cmd_id, cmd->value, cmd_data_len, rsp_data);
            break;
        }
        default:
             break;
    }
    
    if (rsp_flag) {
        UINT16_T id = (cmd->group_id<<8) + cmd->cmd_id;
        test_cmd_send(id, rsp_data, rsp_len);
    }
}


TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_gpio(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    switch (cmd)
    {
        case TEST_CID_PIN_DEINIT: {
            ret = tkl_gpio_deinit(cmd_data[0]);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;
        
        case TEST_CID_OUTPUT_HIGH: {
            TUYA_GPIO_BASE_CFG_T gpio_cfg = {
                .mode = TUYA_GPIO_PUSH_PULL,
                .direct = TUYA_GPIO_OUTPUT,
                .level = TUYA_GPIO_LEVEL_LOW,
            };
            tkl_gpio_init(cmd_data[0], &gpio_cfg);
            
            ret = tkl_gpio_write(cmd_data[0], TUYA_GPIO_LEVEL_HIGH);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_OUTPUT_LOW: {
            TUYA_GPIO_BASE_CFG_T gpio_cfg = {
                .mode = TUYA_GPIO_PUSH_PULL,
                .direct = TUYA_GPIO_OUTPUT,
                .level = TUYA_GPIO_LEVEL_LOW,
            };
            tkl_gpio_init(cmd_data[0], &gpio_cfg);
            
            ret = tkl_gpio_write(cmd_data[0], TUYA_GPIO_LEVEL_LOW);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;
        case TEST_CID_PIN_READ: {
            TUYA_GPIO_BASE_CFG_T gpio_cfg = {
                .mode = TUYA_GPIO_PULLUP,
                .direct = TUYA_GPIO_INPUT,
                .level = TUYA_GPIO_LEVEL_LOW,
            };
			tkl_gpio_init(cmd_data[0], &gpio_cfg);
            UINT32_T gpio_level = TUYA_GPIO_LEVEL_LOW;
            ret = tkl_gpio_read(cmd_data[0], (TUYA_GPIO_LEVEL_E*)&gpio_level);
            rsp[idx++] = (gpio_level >> 24) & 0xFF;
            rsp[idx++] = (gpio_level >> 16) & 0xFF;
            rsp[idx++] = (gpio_level >> 8) & 0xFF;
            rsp[idx++] = (gpio_level) & 0xFF;
        } break;
        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_iic(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    switch (cmd)
    {
        case TEST_CID_TX_IIC_DATA: {
            UINT32_T channel        = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT8_T  *iic_data      = cmd_data + 4;
            UINT32_T iic_data_len   = cmd_data_len - 4;
            
            if(iic_data_len < 14) {
                UINT8_T buf[14] = {0};
                memcpy(buf, iic_data, iic_data_len);
                buf[iic_data_len] = '\0';
                
                //tal_oled_clear();
                //tal_oled_show_string(12, 1, (void*)buf, 16);
                ret = OPRT_OK;
            } else {
                ret = OPRT_INVALID_PARM;
            }
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_RX_IIC_DATA: {
        } break;

        default: {
        } break;
    }
    
    return idx;
}


TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_uart(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    TUYA_UART_BASE_CFG_T cfg_uart;
    switch (cmd)
    {
        case TEST_CID_SET_BAUDRATE: {
            UINT32_T port_num = (cmd_data[0]<<24) + (cmd_data[1]<<16) + (cmd_data[2]<<8) + cmd_data[3];
            cfg_uart.baudrate = (cmd_data[4]<<24) + (cmd_data[5]<<16) + (cmd_data[6]<<8) + cmd_data[7];
            ret = tkl_uart_init(port_num, &cfg_uart);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_TX_UART_DATA: {
            UINT32_T port_num = (cmd_data[0]<<24) + (cmd_data[1]<<16) + (cmd_data[2]<<8) + cmd_data[3];
            
            ret = tkl_uart_write(port_num, cmd_data + 4, cmd_data_len - 4);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_pwm(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    
    switch (cmd)
    {
        case TEST_CID_PWM_DEINIT: {
            UINT32_T channel = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            ret = tkl_pwm_deinit(channel);
            tkl_pwm_start(channel);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;
        
        case TEST_CID_SET_FREQ_DUTY: {
            UINT32_T channel = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT32_T frequency = (cmd_data[4] << 24) | (cmd_data[5] << 16) | (cmd_data[6] << 8) | cmd_data[7];
            UINT32_T duty = (cmd_data[8] << 24) | (cmd_data[9] << 16) | (cmd_data[10] << 8) | cmd_data[11];
            TUYA_PWM_BASE_CFG_T pwm_cfg = {0};
            pwm_cfg.polarity = TUYA_PWM_POSITIVE;
            pwm_cfg.duty = duty*10000;
            pwm_cfg.frequency = frequency;
            
            ret = tkl_pwm_info_set(channel, &pwm_cfg);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
			tkl_pwm_start(channel);
        } break;

        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_adc(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    
    switch (cmd)
    {
        case TEST_CID_ADC_DEINIT: {
            ret = tkl_adc_deinit(TUYA_ADC_NUM_0);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_READ_ADC_DATA: {
            UINT8_T  channel = cmd_data[3];
            UINT8_T  width = cmd_data[4];
            INT32_T adc_value = 0;
            
            tkl_adc_deinit(TUYA_ADC_NUM_0);
            
            TUYA_ADC_BASE_CFG_T adc_cfg = {
                .ch_list.data = (1<<channel),
                .type = TUYA_ADC_EXTERNAL_SAMPLE_VOL,
                .width = width,
            };
            ret = tkl_adc_init(TUYA_ADC_NUM_0, &adc_cfg);
            
            if(ret == OPRT_OK) {
            {
                //print("channel is %d\r\n",channel);
                ret = tkl_adc_read_voltage(TUYA_ADC_NUM_0, &adc_value, 1);
                //print("adc_value is %d\r\n",adc_value);
            }
                
                if(ret == OPRT_OK) {
                    test_cmd_send(TEST_ID_GET(TEST_GID_ADC, TEST_CID_READ_ADC_DATA_RSP), (VOID_T*)&adc_value, SIZEOF(UINT32_T));
                }
            }
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_flash(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    
    switch (cmd)
    {
        case TEST_CID_READ_FLASH_DATA: {
            UINT32_T addr = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT32_T len = (cmd_data[4] << 24) | (cmd_data[5] << 16) | (cmd_data[6] << 8) | cmd_data[7];
            
            tkl_flash_read(addr, &rsp[idx], len);
            idx += len;
        } break;

        case TEST_CID_ERASE_FLASH_DATA: {
            UINT32_T addr = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT32_T len = (cmd_data[4] << 24) | (cmd_data[5] << 16) | (cmd_data[6] << 8) | cmd_data[7];
            
            ret = tkl_flash_erase(addr, len);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_WRITE_FLASH_DATA: {
            UINT32_T addr = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT8_T  *flash_data = cmd_data + 4;
            UINT32_T flash_data_len = cmd_data_len - 4;
            ret = tkl_flash_write(addr, flash_data, flash_data_len);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;

        } break;

        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_watchdog(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    
    switch (cmd)
    {
        case TEST_CID_START_WDG: {
            UINT32_T interval_ms = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            
            TUYA_WDOG_BASE_CFG_T wdog_cfg = {
                .interval_ms = interval_ms,
            };
            ret = tkl_watchdog_init(&wdog_cfg);
            //ret = tal_watchdog_start(&wdog_cfg);
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_FEED_WDG: {
            ret = tkl_watchdog_refresh();
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        case TEST_CID_STOP_WDG: {
            ret = tkl_watchdog_deinit();
            rsp[idx++] = (ret >> 24) & 0xFF;
            rsp[idx++] = (ret >> 16) & 0xFF;
            rsp[idx++] = (ret >> 8) & 0xFF;
            rsp[idx++] = (ret) & 0xFF;
        } break;

        default: {
        } break;
    }
    
    return idx;
}

TUYA_WEAK_ATTRIBUTE OPERATE_RET test_group_spi(UINT8_T cmd, UINT8_T *cmd_data, UINT32_T cmd_data_len, UINT8_T *p_rsp_data)
{
    OPERATE_RET ret  = 0; 
    UINT32_T    idx  = 0; 
    UINT8_T     *rsp = p_rsp_data;
    
    switch (cmd)
    {
        case TEST_CID_TX_SPI_DATA: {
            UINT32_T channel        = (cmd_data[0] << 24) | (cmd_data[1] << 16) | (cmd_data[2] << 8) | cmd_data[3];
            UINT32_T frequency      = (cmd_data[4] << 24) | (cmd_data[5] << 16) | (cmd_data[6] << 8) | cmd_data[7];
            UINT8_T  *spi_data      = cmd_data + 8;
            UINT32_T spi_data_len   = cmd_data_len - 8;
            tkl_spi_deinit(channel);
            UINT8_T buf[10];
            TUYA_SPI_BASE_CFG_T spi_cfg = {
                .mode = TUYA_SPI_MODE0,
                .type = TUYA_SPI_AUTO_TYPE,
                .databits = TUYA_SPI_DATA_BIT8,
                .freq_hz = 100000,
            };
            tkl_spi_init(channel, &spi_cfg);
            ret = tkl_spi_transfer(channel, spi_data, buf, spi_data_len);
            if(memcmp(spi_data, buf, spi_data_len) == 0) {
                test_cmd_send(TEST_ID_GET(TEST_GID_SPI, TEST_CID_RX_SPI_DATA), buf, spi_data_len);
            } else {
                ret = OPRT_RECV_DA_NOT_ENOUGH;
            }
            
        } 
        rsp[idx++] = (ret >> 24) & 0xFF;
        rsp[idx++] = (ret >> 16) & 0xFF;
        rsp[idx++] = (ret >> 8) & 0xFF;
        rsp[idx++] = (ret) & 0xFF;
        break;

        case TEST_CID_RX_SPI_DATA: {
        } break;

        default: {
        } break;
    }
    
    return idx;
}

#endif
#endif
