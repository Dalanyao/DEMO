/*************************************************************************
	> File Name: main.c
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2022 17:42:57 CST
 ************************************************************************/

#include "board.h"
#include "ty_beacon_remoter2.h"
#include "app_remoter.h"
#include "app_config.h"


static dev_beacon_info_s beacon_dev = {
	.mac = {0xdc,0x23,0xFF,0xFF,0xFF,0xFF},//需要填写从平台申请的授权码清单中一个设备的MAC
    .authkey = "IAmFFFFFFFFFFFFF",//需要填写从平台申请的授权码清单中一个设备的Authkey前16字节
    .pid = "c4mc2yte",//需要填写您自己产品的 PID
    .version = FIRMWARE_VERSION_HEX,//固件版本号，0x14 表示 1.4 版本
    .kind = 0x1054,
};

VOID_T tal_framework_init_first(VOID_T){
}

VOID_T tal_framework_init_second(VOID_T){
}

VOID_T tal_framework_init_third(VOID_T){
#ifndef BOARD_FPGA
#ifdef CHIP_KIND_PHY6230
	memcpy(beacon_dev.mac,(u8*)0x1FFFBFD4,6);
	memcpy(beacon_dev.authkey,(u8*)0x1FFFBFC4,16);
#else
    memcpy(beacon_dev.mac,(u8*)0x3FE4,6);
	memcpy(beacon_dev.authkey,(u8*)0x3FC4,16);
#endif
#endif
}

VOID_T tal_framework_init_last(VOID_T){	
	app_remoter_demo_init();
    ty_beacon_init(&beacon_dev);
}

VOID_T tal_framework_loop(VOID_T){
	app_remoter_demo_run();
	ty_beacon_run();
}
