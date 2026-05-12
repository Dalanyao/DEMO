/*************************************************************************
	> File Name: ty_beacon2_base.h
	> Author: 
	> Mail: 
	> Created Time: Fri 30 Sep 2022 15:09:03 CST
 ************************************************************************/

#ifndef _TY_BEACON2_BASE_H
#define _TY_BEACON2_BASE_H

#include "ty_beacon2.h"
#include "ty_beacon2_log.h"

typedef int (*block_send_finish_callback_t)(void);

extern void ty_beacon2_event_cb(ty_beacon2_event_e e, void *params);

u8 phy_ble_adv_send(u8 *pdu, u32 timeout_us);      // 物理层发蓝牙广播，该广播可设置一个超时

u8 frame_cal_crc8(u8 *vptr, u8 len);                             // crc 计算        
u8* frame_check(u8 *rx_buf, u8 len_pdu);             // 帧合法性校验（长度+广播类型+crc）
u8 frame_decrypt(beacon_frame_s* p_frame);                       // 包解密并且判断 check 是否合法 
u8 frame_sn_check(beacon_frame_s* p_frame);                      // 包 sn 校验，如果不符合直接过滤，符合直接更新 sn
u8 frame_froward(beacon_frame_s * p_frame);                      // 包转发
u8 frame_send(u8 head, u16 dst, u8 subcmd, u8* payload, u8 *key, u8 ttl);  // 高级封装包发送
u8 frame_send_set_block_mode(u8 mode, block_send_finish_callback_t cb); // 为了达到效果最好，某些广播包需要不被打断，并在广播结束有回调


#endif
