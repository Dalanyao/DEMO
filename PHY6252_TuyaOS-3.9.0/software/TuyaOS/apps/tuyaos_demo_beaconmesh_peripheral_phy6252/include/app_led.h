/*************************************************************************
	> File Name: app_led.h
	> Author: 
	> Mail: 
	> Created Time: Mon Aug 28 18:32:34 2023
 ************************************************************************/

#ifndef _APP_LED_H
#define _APP_LED_H
#include "board.h"
#include "ty_beacon2.h"

void app_led_init(void);
void app_led_onoff(u8 onoff);
void app_led_toggle(void);
void app_led_blink(u16 times);
void app_led_run(void);
void app_led_reset_run(beacon_dev_s *beacon_dev);

#endif
