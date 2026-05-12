/*************************************************************************
	> File Name: app_led.c
	> Author: 
	> Mail: 
	> Created Time: 2022年10月18日 22:35:54
 ************************************************************************/
#include "app_led.h"
#include "tkl_gpio.h"


static u8 led_onoff = 1;
static u16 led_blink = 0;


#if defined(CHIP_KIND_PHY6230)
	static u32 led_pin = TUYA_GPIO_NUM_0;
#elif defined(CHIP_KIND_PHY6252)
	static u32 led_pin = TUYA_GPIO_NUM_34;
#else
	static u32 led_pin = TUYA_GPIO_NUM_4;
#endif

void app_led_init(void){
	TUYA_GPIO_BASE_CFG_T gpio_config = {
		.mode = TUYA_GPIO_PUSH_PULL,
		.direct = TUYA_GPIO_OUTPUT,
		.level = TUYA_GPIO_LEVEL_HIGH,
	};
	tkl_gpio_init(led_pin,&gpio_config);
	tkl_gpio_write(led_pin,TUYA_GPIO_LEVEL_HIGH);
}

void app_led_onoff(u8 onoff){
	led_blink = 0;
	led_onoff = onoff;
	tkl_gpio_write(led_pin,led_onoff == 1?TUYA_GPIO_LEVEL_HIGH:TUYA_GPIO_LEVEL_LOW);
}

void app_led_toggle(void){
	led_onoff = !led_onoff;
	tkl_gpio_write(led_pin,led_onoff == 1?TUYA_GPIO_LEVEL_HIGH:TUYA_GPIO_LEVEL_LOW);
}

void app_led_blink(u16 times){
	led_onoff = 1;
	tkl_gpio_write(led_pin,led_onoff == 1?TUYA_GPIO_LEVEL_HIGH:TUYA_GPIO_LEVEL_LOW);
	led_blink = times;
}

void app_led_run(void){
	static u32 tick = 0;
	if(hal_clock_time_exceed(tick,250000)){
		tick = hal_clock_get_system_tick();
		if(led_blink){
			if(led_blink != 0xFFFF)
				led_blink--;
			app_led_toggle();
		}
	}
}

#define RESET_MAX_CNT 3

void app_led_reset_run(beacon_dev_s *beacon_dev){
	static u8 reset_judge_time = 1; 
	static u8 state = STATE_NOT_PAIRED;
	static u32 count_1s_time = 0;
	static u32 count_1s = 0;
	
	u8 cnt;
					
	if(beacon_dev->state == STATE_PAIRED){
		if(reset_judge_time != 0){//300ms~6S
			if(hal_clock_time_exceed(0,reset_judge_time*300000)){//300MS
				if(reset_judge_time == 1){
					reset_judge_time = 20;
					if(0 == hal_storage_256_read_bytes(HS256_USER, &cnt,1)){//success
						if(cnt > RESET_MAX_CNT)cnt = 0;
					}else{
						cnt = 0;
					}
					
					cnt++;
					if(cnt >= RESET_MAX_CNT){
						cnt = 0;
						ty_beacon2_node_reset(0,180000000);
						//app_led_blink(0xFF);
						reset_judge_time = 0;
					}
					hal_storage_256_write_bytes(HS256_USER, &cnt,1);
				}else if(reset_judge_time == 20){
					reset_judge_time = 0;
					cnt = 0;
					hal_storage_256_write_bytes(HS256_USER, &cnt,1);
				}
			}
		}
	}
	
	if(state == STATE_NOT_PAIRED && beacon_dev->state == STATE_PAIRED){
		state = STATE_PAIRED;
		app_led_onoff(1);
	}else if(state == STATE_PAIRED && beacon_dev->state == STATE_NOT_PAIRED){
		state = STATE_NOT_PAIRED;
		ty_beacon2_node_pair_start(180000000);
		app_led_blink(720);
	}
	
	//if(hal_clock_time_exceed(count_1s_time,1000000)){//1S
	//	count_1s_time = hal_clock_get_system_tick();
	if(hal_clock_time_exceed_with_update(&count_1s_time,1000000)){//1S
		count_1s++;
		
		if(count_1s%10 == 0){
			PR_DEBUG("HEART BEAT: %ds, sn=%d\n", count_1s, beacon_dev->sn);
		}
	}
}
