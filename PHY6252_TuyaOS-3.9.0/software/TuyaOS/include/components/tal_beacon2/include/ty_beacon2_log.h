/*************************************************************************
	> File Name: ty_beacon2_log.h
	> Author: 
	> Mail: 
	> Created Time: Thu 22 Sep 2022 14:38:54 CST
 ************************************************************************/

#ifndef _TY_BEACON2_LOG_H
#define _TY_BEACON2_LOG_H
#include "board.h"

#define BEACON_DEBUG 0

#if BEACON_DEBUG
#define LOG_PAIR_START_END() do{ \
    PR_DEBUG("START PAIR:["); \
    PR_DEBUG_HEX_ARRAY(package,31); \
    PR_DEBUG_RAW("\n"); \
                        \
    PR_DEBUG("SRAND:[");  \
    PR_DEBUG_HEX_ARRAY(beacon_dev->srand1,4); \
    PR_DEBUG_RAW("\n"); \
                            \
    PR_DEBUG("SRAND-EN:["); \
    PR_DEBUG_HEX_ARRAY(beacon_dev->encode_srand1,4); \
    PR_DEBUG_RAW("\n"); \
                            \
    PR_DEBUG("KEY1:["); \
    PR_DEBUG_HEX_ARRAY(beacon_dev->key1,16); \
    PR_DEBUG_RAW("\n"); \
                         \
    PR_DEBUG("MAC:["); \
    PR_DEBUG_HEX_ARRAY(beacon_dev->mac,6); \
    PR_DEBUG_RAW("\n"); \
                          \
    PR_DEBUG("VER:[%02x]\n",beacon_dev->version); \
    PR_DEBUG("FLAG:[%02x]\n",beacon_dev->flag); \
}while(0)

#define PRINT_BEACON_FRAME(p_frame) do{ \
    PR_DEBUG("IS_PAIRING:%d, CMD:%d, ttl:%d, PAYLOAD:[",p_frame->is_pairing,p_frame->cmd,p_frame->ttl); \
    PR_DEBUG_HEX_ARRAY(p_frame->payload_x,8); \
    PR_DEBUG_RAW(" | "); \
    PR_DEBUG_HEX_ARRAY(p_frame->payload_y,16); \
    PR_DEBUG_RAW("]-[%x]\n",p_frame->crc); \
}while(0)

#define PRINT_BEACON_CMD(p_frame) do{ \
    PR_DEBUG("IS_PAIRING:%d, CMD:%d, ttl:%d, src:%02x%02x dst:%02x%02x sn:%02x%02x subcmd:%d PAYLOADY:[",p_frame->is_pairing,p_frame->cmd,p_frame->ttl,p_frame->payload_x[0],p_frame->payload_x[1],p_frame->payload_x[2],p_frame->payload_x[3],p_frame->payload_x[4],p_frame->payload_x[5],p_frame->payload_x[6]); \
    PR_DEBUG_HEX_ARRAY(p_frame->payload_y,16); \
    PR_DEBUG_RAW("]-[%x]\n",p_frame->crc); \
}while(0)

#define PRINT_BEACON_DP(p_frame) do{ \
    u8 led_str[16]; \
    u8 xcmd = p_frame->payload_x[6]; \
    u8 *xp  = p_frame->payload_y; \
    sprintf(led_str,"[%02x] %02x%02x%02x%02x%02x",xcmd,xp[0],xp[1],xp[2],xp[3],xp[4]); \
	tal_oled_show_string(0,2,led_str,8); \
}while(0)

#define PRINT_BEACON_TX(start,xp) do{ \
    u8 led_str[17] = "T:NULL          "; \
    if(start == 1){ \
        sprintf(led_str,"T:%02x%02x%02x%02x%02x%02x%02x",(xp)[0],(xp)[1],(xp)[2],(xp)[3],(xp)[4],(xp)[5],(xp)[6]); \
    }else{ \
        sprintf(led_str,"X:"); \
    } \
    tal_oled_show_string(0,1,led_str,8); \
}while(0)

#define PRINT_BEACON_RX(rx_buf,len_pdu) do{ \
    PR_DEBUG("RX:["); \
    PR_DEBUG_HEX_ARRAY(rx_buf,len_pdu); \
    PR_DEBUG_RAW("]\n"); \
}while(0)

#define PRINT_CLOUD_API_PAIRING_INFO() do{ \
    PR_DEBUG("CLOUD API PAIRING INFO -> SRAND:[");\
    PR_DEBUG_HEX_ARRAY(srand,4);\
    PR_DEBUG_RAW("], MAC:[");\
    PR_DEBUG_HEX_ARRAY(mac,6);\
    u8 pidx[9]; \
    memcpy(pidx,pid,8);\
    pidx[8] = 0;\
    PR_DEBUG_RAW("], pid:%s, ver:%x, flag:%x, srand_en:[",pidx,ver,flag);\
    PR_DEBUG_HEX_ARRAY(srand_en,4);\
    PR_DEBUG_RAW("]\n");\
}while(0)

#define PRINT_BEACON_PAIR_CMD2() do{ \
    PR_DEBUG("PAIR INFO DOWNLOAD CHECK OK\n"); \
    PR_DEBUG("nodeid = %x\n",beacon_dev->nodeid); \
    PR_DEBUG("beaconkey = ["); \
    PR_DEBUG_HEX_ARRAY(beacon_dev->beaconkey,16); \
    PR_DEBUG_RAW("]\n");\
}while(0)

#else
#define LOG_PAIR_START_END() 
#define PRINT_BEACON_FRAME(p_frame)
#define PRINT_BEACON_CMD(p_frame) 
#define PRINT_BEACON_DP(p_frame) 
#define PRINT_BEACON_TX(start,xp)
#define PRINT_BEACON_RX(rx_buf,len_pdu) 
#define PRINT_CLOUD_API_PAIRING_INFO() 
#define PRINT_BEACON_PAIR_CMD2()
#endif

#endif
