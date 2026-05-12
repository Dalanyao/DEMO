/*************************************************************************
	> File Name: ty_beacon2_fifo.h
	> Author: 
	> Mail: 
	> Created Time: Tue 25 Oct 2022 20:47:50 CST
 ************************************************************************/

#ifndef _TY_BEACON2_FIFO_H
#define _TY_BEACON2_FIFO_H
#include "board.h"

u8 ty_beacon2_fifo_insert(u8  *p_frame);
u8 ty_beacon2_fifo_get(u8 *p_frame);

#endif
