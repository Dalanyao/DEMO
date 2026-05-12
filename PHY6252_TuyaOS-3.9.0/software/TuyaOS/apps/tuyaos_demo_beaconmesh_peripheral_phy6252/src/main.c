/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2022 17:42:57 CST
 ************************************************************************/

#include "app_led.h"
#include "tkl_rtc.h"
#include "tal_authorize.h"
#include "tkl_beacon.h"

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
    .mac = {0xdc,0x23,0x4e,0xcc,0x3e,0xf3},
    .authkey = "AUTHKEY前16字节（如果DEVELOP_DEBUG=1时，可以直接在这里填写mac和authkey信息，如果为0，则通过授权方式获取）", 
    .pid = "wzojep0f", 
    .version = FIRMWARE_VERSION_HEX, 
    .flag = 0x00, 
    .kind = 0x1015,
    .state = STATE_NOT_PAIRED
};

VOID_T tal_framework_init_first(VOID_T){
	//#ifdef PRINT_DEBUG
	TUYA_UART_BASE_CFG_T uart_cfg = {
        .baudrate = 9600,
        .databits = TUYA_UART_DATA_LEN_8BIT,
        .stopbits = TUYA_UART_STOP_LEN_1BIT,
        .parity = TUYA_UART_PARITY_TYPE_NONE,
		.flowctrl = TUYA_UART_PARITY_TYPE_NONE,
    };
    tkl_rtc_init();
	tkl_uart_init(1,&uart_cfg);
	//#endif
}

VOID_T tal_framework_init_second(VOID_T){
	    tal_authorize_init();
}

VOID_T tal_framework_init_third(VOID_T){
}

VOID_T tal_framework_init_last(VOID_T){	
		
#if (DEVELOP_DEBUG == 0)
	get_pid(beacon_dev.pid);
    get_anthkey(beacon_dev.authkey);
    tkl_beacon_get_mac(beacon_dev.mac);
#endif
	
	app_led_init();
	
    ty_beacon2_node_init(&beacon_dev);
	ty_beacon_remoter_init(30);
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
