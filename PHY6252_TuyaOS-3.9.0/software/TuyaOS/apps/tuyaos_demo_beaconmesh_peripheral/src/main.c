/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2022 17:42:57 CST
 ************************************************************************/

#include "board.h"
#include "ty_beacon2.h"

void app_dps_download(u8 dpid, u8 dpty, u8 dplen, u8 *dpvalue, beacon_frame_s *p_frame){
    PR_DEBUG("dpid=%d dpty=%d, dplen=%d, dpvalue=[",dpid,dpty,dplen);
    PR_DEBUG_HEX_ARRAY(dpvalue,dplen);
    PR_DEBUG_RAW("]\n");
	switch(dpid){
		case 0x01://onoff
		case 101:
			app_led_onoff(dpvalue[0]);
			break;
		case 0x02://mode:white,color,scene,music
			break;
		default:
			break;
	}
}

void ty_beacon2_event_cb(ty_beacon2_event_e e, void *params){
	PR_DEBUG("EVENT:%d\n",e);
    switch(e){
		case BEACON2_EVENT_PAIR_OK:app_led_onoff(1);break;
		case BEACON2_EVENT_RESET:break;
		case BEACON2_EVENT_RECOVERY:app_led_onoff(1);break;
		case BEACON2_EVENT_GROUP_ADD_SUCCESS:break;
		case BEACON2_EVENT_GROUP_ADD_ERR:break;
		case BEACON2_EVENT_GROUP_DEL_SUCCESS:break;
		default:break;
	}
}

static beacon_dev_s beacon_dev = {
    .mac = {uuide3c0486bfcdcd9f7},//需要填写从平台申请的授权码清单中一个设备的MAC
    .authkey = "rOQhR4zQMfcMSQhEKZcN6acGYo1lUNMB",//需要填写从平台申请的授权码清单中一个设备的Authkey前16字节
    .pid = "wzojep0f",//需要填写您自己产品的 PID
    .version = 0x10,//固件版本号，0x14 表示 1.4 版本
    .flag = 0x00, //默认写 0x00
    .kind = 0x1015,
    .state = STATE_NOT_PAIRED
};

VOID_T tal_framework_init_first(VOID_T){
	#ifdef PRINT_DEBUG
	TUYA_UART_BASE_CFG_T uart_cfg = {
        .baudrate = 1000000,
        .databits = TUYA_UART_DATA_LEN_8BIT,
        .stopbits = TUYA_UART_STOP_LEN_1BIT,
        .parity = TUYA_UART_PARITY_TYPE_NONE,
    };
	tkl_uart_init(1,&uart_cfg);
	#endif
}

VOID_T tal_framework_init_second(VOID_T){
	
}

VOID_T tal_framework_init_third(VOID_T){
#ifndef BOARD_FPGA
	memcpy(beacon_dev.mac,(u8*)0x3FE4,6);
	memcpy(beacon_dev.authkey,(u8*)0x3FC4,16);
#endif
}

VOID_T tal_framework_init_last(VOID_T){	
	app_led_init();
	
    ty_beacon2_node_init(&beacon_dev);
	ty_beacon_remoter_init(30);//遥控器在设备上电的前30S，开启对码时间窗（最大不建议超过1min）
	if(beacon_dev.state == STATE_NOT_PAIRED){
		ty_beacon2_node_pair_start(180000000);
		app_led_blink(720);
    }else{
        u8 payload[16];
        memset(payload,0,16);
        payload[0] = 0x01;
        frame_send(0x08, 0x8000, 0x02, payload, beacon_dev.beaconkey, 0);//上电上报心跳，不转发
    }
}

VOID_T tal_framework_loop(VOID_T){
	ty_beacon2_node_run();//beacon mesh 节点主逻辑维护进程
	app_led_run();//led 闪烁维护进程
	app_led_reset_run(&beacon_dev);//三次快速上电重置逻辑维护进程
}
