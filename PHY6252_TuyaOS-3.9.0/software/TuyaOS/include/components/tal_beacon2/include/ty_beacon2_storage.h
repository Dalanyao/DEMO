/*************************************************************************
	> File Name: ty_beacon2_storage.h
	> Author: 
	> Mail: 
	> Created Time: Tue 18 Oct 2022 10:12:50 CST
 ************************************************************************/

#ifndef _TY_BEACON2_STORAGE_H
#define _TY_BEACON2_STORAGE_H

OPERATE_RET ty_beacon_storage_when_init(beacon_dev_s *beacon_dev);
OPERATE_RET ty_beacon_storage_when_reset(beacon_dev_s *beacon_dev, u8 true_reset_flag);
OPERATE_RET ty_beacon_storage_when_pair_ok(beacon_dev_s *beacon_dev);
OPERATE_RET ty_beacon_storage_when_reset_timeout(beacon_dev_s *beacon_dev);
OPERATE_RET ty_beacon_storage_when_sn_change(beacon_dev_s *beacon_dev);
OPERATE_RET ty_beacon_storage_when_group_change(beacon_dev_s *beacon_dev);

#endif
