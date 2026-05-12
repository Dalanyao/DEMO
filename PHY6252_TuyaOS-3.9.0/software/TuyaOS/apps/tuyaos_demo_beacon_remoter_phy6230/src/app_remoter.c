/*************************************************************************
	> File Name: app_remoter.c
	> Author: 
	> Mail: 
	> Created Time: Tue 23 May 2023 17:46:48 CST
 ************************************************************************/

#include "app_remoter.h"
#include "ty_beacon_remoter2.h"
#include "board.h"

uint8_t cmd_onoff[7] = {0x00,0xff,0x04,0x00,0x00,0x00,0x00};
uint8_t cmd_dp_onoff[5] = {0x00,0x13,0x01,0x11,0x00}; // group_id + cmd + dp


#define STEP_PAIR 0
#define STEP_UNPAIR 1
#define STEP_OFF 2
#define STEP_ON 3

#define GROUP_ALL 0
#define GROUP_1 1
#define GROUP_2 2
#define GROUP_3 3
#define GROUP_4 4

// support tuya's two beacon remoter protocol
STATIC VOID_T app_remoter_demo_show(uint8_t step, uint8_t group){
	uint8_t cmd_type = DEVICE_CMD_TYPE_CTRL;
    uint8_t *params = NULL;
    uint8_t params_len = 7;
	uint8_t *dp_params = NULL;
    uint8_t dp_params_len = 10;
	uint8_t pairs[5][5] = {{0x00,0xff,0x02,0x00,0xff},
						   {0x01,0xff,0x02,0x01,0xff},
						   {0x02,0xff,0x02,0x02,0xff},
						   {0x03,0xff,0x02,0x03,0xff},
						   {0x04,0xff,0x02,0x04,0xff}
						   };
	
	uint8_t cmd_dp_pair[2] = {0x00,0x11};
	uint8_t cmd_dp_unpair[2] = {0x00,0x12};

	switch(step){
		case 0:{//PAIR GROUP(MAX 4)
			cmd_type = DEVICE_CMD_TYPE_PAIR;
			params = pairs[group];
			params[2] = 0x02;//cmd pair
			params_len = 5;

			dp_params = &cmd_dp_pair[0];
			*dp_params = group;
			dp_params_len = 2;				
			PR_DEBUG("------device %d group pair\n",group);
		}
		break;
		case 1:{//UNPAIR GROUP
			cmd_type = DEVICE_CMD_TYPE_UNPAIR;
			params = pairs[group];
			params[2] = 0x03;//cmd unpair
			params_len = 5;
			
			dp_params = &cmd_dp_unpair[0];
			*dp_params = group;
			dp_params_len = 2;
			PR_DEBUG("------device %d group unpair\n",group);
		}
		break;
		case 2:{//control off
			uint8_t r_onoff = 0;
			
			params = &cmd_onoff[0];
			*params = group;//group control
			*(params+3) = r_onoff;			
			
			dp_params = &cmd_dp_onoff[0];
			*dp_params = group;
			*(dp_params+4) = r_onoff;
			dp_params_len = 5;
			PR_DEBUG("---device %d group off\n",group);
		}
		break;
		case 3:{//control on
			uint8_t r_onoff = 1;
			
			params = &cmd_onoff[0];
			*params = group;//all control
			*(params+3) = r_onoff;			
			
			dp_params = &cmd_dp_onoff[0];
			*dp_params = group;
			*(dp_params+4) = r_onoff;
			dp_params_len = 5;
			PR_DEBUG("---device %d group on\n",group);
		}
		break;
		default:
		break;
	}
	
	ty_beacon_ctrl(cmd_type, params, params_len, dp_params, dp_params_len);
}

#if defined(CHIP_KIND_PHY6230_16PIN)
    #define BTN_NUM 11
    gpio_pin_e btn_pin[BTN_NUM] = {P0,P1,P2,P3,P6,P7,P8,P9,P10,P13,P15};//low_power.c
    UINT8_T btn_state[BTN_NUM];
    low_power_cfg_t low_power_cfg[BTN_NUM];
    gpioin_t pin_in[BTN_NUM];//must have, otherwise it cannot enter interrupt
#else
    #define BTN_NUM 15
    gpio_pin_e btn_pin[BTN_NUM] = {P2,P3,P6,P7,P8,P9,P10,P11,P12,P15,P16,P17,P18};//low_power.c
    UINT8_T btn_state[BTN_NUM];
    low_power_cfg_t low_power_cfg[BTN_NUM];
    gpioin_t pin_in[BTN_NUM];//must have, otherwise it cannot enter interrupt
#endif

VOID_T app_remoter_demo_init(VOID_T)
{
	for(int i=0;i<BTN_NUM;i++){
		gpio_pull_set(btn_pin[i],GPIO_PULL_DOWN);
		btn_state[i] = gpio_read(btn_pin[i]);
		low_power_cfg[i].pin = btn_pin[i];
		low_power_cfg[i].pull = GPIO_PULL_DOWN;
	}
	PR_DEBUG_HEX_ARRAY(btn_state,BTN_NUM);
	PR_DEBUG("<-- init btn state\n");
}

VOID_T app_remoter_demo_run(VOID_T)
{
	for(int i=0;i<BTN_NUM;i++){
		UINT8_T state = gpio_read(btn_pin[i]);
		if(state != btn_state[i]){
			PR_DEBUG("IO:%d change ->%d\n",i,state);
			btn_state[i] = state;
			if(state == 1){
				switch(i){
					case 4:app_remoter_demo_show(STEP_PAIR,GROUP_1);break;//P6-4
					case 12:app_remoter_demo_show(STEP_PAIR,GROUP_2);break;//P16-12
					case 5:app_remoter_demo_show(STEP_PAIR,GROUP_3);break;//P7-5
					case 11:app_remoter_demo_show(STEP_PAIR,GROUP_4);break;//P15-11
					case 6:app_remoter_demo_show(STEP_UNPAIR,GROUP_1);break;//P8-6
					case 7:app_remoter_demo_show(STEP_UNPAIR,GROUP_2);break;//P9-7
					case 8:app_remoter_demo_show(STEP_UNPAIR,GROUP_ALL);break;//P10-8
					case 2:app_remoter_demo_show(STEP_OFF,GROUP_1);break;//P2-2
					case 14:app_remoter_demo_show(STEP_ON,GROUP_1);break;//P18-14
					case 3:app_remoter_demo_show(STEP_OFF,GROUP_3);break;//P3-3
					case 13:app_remoter_demo_show(STEP_ON,GROUP_3);break;//P17-13
					case 0:app_remoter_demo_show(STEP_OFF,GROUP_ALL);break;//P0-0
					case 1:app_remoter_demo_show(STEP_ON,GROUP_ALL);break;//P1-1
					default:break;
				}
			}
		}
	}	
}

void ty_beacon_remoter_enter_sleep(void){
	LOG("enter power off\n");
	low_power_sleep(low_power_cfg, pin_in, BTN_NUM);
	//hal_pwrmgr_poweroff( pwr_wkp_cfg, sizeof(pwr_wkp_cfg)/sizeof(pwr_wkp_cfg[0]));
}
