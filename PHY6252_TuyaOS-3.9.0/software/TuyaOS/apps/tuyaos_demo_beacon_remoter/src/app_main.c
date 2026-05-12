#include "app_main.h"
#include "tkl_uart.h"
#include "tkl_i2c.h"
#include "tkl_pwm.h"
#include "tkl_bluetooth.h"
#include "tuya_cloud_types.h"
#include "tkl_rtc.h"
#include "tkl_adc.h"
#include "tkl_spi.h"
#include "ty_log.h"
#include "ty_clock.h"
#include "tal_beacon_remoter.h"
#include "tal_authorize.h"

VOID_T tal_framework_init_first(VOID_T)
{
    
}

VOID_T tal_framework_init_second(VOID_T)
{
#if  LOG_ENABLE
    tkl_log_init();
#endif
}
VOID_T tal_framework_init_third(VOID_T)
{
    tkl_ble_stack_init(0);
    TKL_BLE_GAP_ADDR_T const addr_cfg = {
        .type = TKL_BLE_GAP_ADDR_TYPE_PUBLIC,
        .addr = {0xDC,0x23,0x4E,0x36,0xd3,0xa2},
    };
    tkl_ble_gap_addr_set(&addr_cfg);
}

VOID_T tal_framework_init_last(VOID_T)
{
    TKL_BLE_GAP_ADDR_T mac_addr = {
        .type = 0,
        .addr = {0xDC,0x23,0x4E,0x36,0xd3,0xa2},
    };
    u8 authkey[16] = "fvGApHHMTke0twMV";
    u8 psrand[16] = {0x32, 0x52, 0xfb, 0x33, 0x69, 0x45, 0xe7, 0x37, 0xd, 0xaf, 0x2a, 0xd6, 0xfb, 0xbd, 0x9d, 0x60};
    u8 pid[9] = "        ";
    u8 version = 0x14;//1.4
    u16 kind = 0x1054;
    tkl_ble_gap_address_get(&mac_addr);
#ifdef AUTHKEY_1FC4
    memcpy(mac_addr.addr,(unsigned char *)0x1FE0,16);
    memcpy(authkey,(unsigned char *)0x1FC0,16);
    memcpy(psrand,(unsigned char *)0x1FEE,16);
#endif
    ty_beacon_init(mac_addr.addr,authkey,psrand,pid,version,kind);
}


VOID_T tal_framework_loop(VOID_T)
{
   
}




