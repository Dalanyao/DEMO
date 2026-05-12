/*************************************************************************
	> File Name: ty_beacon2.h
	> Author: 
	> Mail: 
	> Created Time: Tue 20 Sep 2022 18:02:41 CST
 ************************************************************************/

#ifndef _TY_BEACON2_H
#define _TY_BEACON2_H
#include "board.h"

//#define BEACON2_GW
#define BEACON2_NODE

#define SMALL_END 

#define STATE_NOT_PAIRED 0
#define STATE_PAIRED   1
#define STATE_PAIRING  2

#define CMD_PAIR_INFO_UPLOAD        0x01
#define CMD_PAIR_INFO_DOWNLOAD      0x02
#define CMD_PAIR_PAIR_OK_UPLOAD     0x03
#define CMD_PAIR_IDENTIFY_DOWNLOAD  0x04

#define CMD_CTRL_SINGLE_SEND        0x01
#define CMD_CTRL_GROUP_SEND         0x02


#define GROUP_MAX_SIZE 8
typedef struct{
    u8 srand1[4];
    u8 mac[6];
	u8 pid[8];
	u8 version;
    u8 flag;
    u8 encode_srand1[4];
	u8 authkey[16];
    u8 state;//storage
    u16 sn;//storage
    u16 nodeid;
	u8 srand_tick;
    u16 kind;
    u8 key1[16];//storage
    u8 beaconkey[16];//storage
    u8 group[GROUP_MAX_SIZE];//storage
}beacon_dev_s;

#pragma pack(1)
typedef struct{
#ifdef BIG_END
    u8 is_pairing:1;
    u8 cmd:4;
    u8 rev:1;
    u8 ttl:2;
#endif
#ifdef SMALL_END
    u8 ttl:2;
    u8 rev:1;
    u8 cmd:4;
    u8 is_pairing:1;
#endif
    u8 payload_x[8];
    u8 payload_y[16];
    u8 crc;
}beacon_frame_s;
#pragma pack()

#define MAX_SN_TABLE 48
typedef struct{
    u8 index_write;
    u16 nodeid[MAX_SN_TABLE];
    u16 sn[MAX_SN_TABLE];
}sn_table_s;

typedef enum {
    BEACON2_EVENT_PAIR_OK = 0x00,
    BEACON2_EVENT_RESET,
    BEACON2_EVENT_RECOVERY,
    BEACON2_EVENT_GROUP_ADD_SUCCESS,
    BEACON2_EVENT_GROUP_ADD_ERR,
    BEACON2_EVENT_GROUP_DEL_SUCCESS,
	BEACON2_EVENT_REMOTER_ADD_SUCCESS,
	BEACON2_EVENT_REMOTER_DEL_SUCCESS,
}ty_beacon2_event_e;


u8 ty_beacon2_node_init(beacon_dev_s *p_beacon_dev);
void ty_beacon_remoter_init(u8 pair_window_s);
u8 ty_beacon2_node_pair_start(u32 timeout_us);
u8 ty_beacon2_node_reset(u8 is_not_recovery, u32 pair_timeout_us);
u8 ty_beacon2_node_run(void);
extern u8 frame_send(u8 head, u16 dst, u8 subcmd, u8* payload, u8 *key, u8 ttl);



#endif
