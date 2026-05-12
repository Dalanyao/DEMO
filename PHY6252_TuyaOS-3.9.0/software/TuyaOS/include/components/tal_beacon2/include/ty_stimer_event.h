/*************************************************************************
	> File Name: ty_stimer_event.h
	> Author: 
	> Mail: 
	> Created Time: Thu 29 Sep 2022 16:57:44 CST
 ************************************************************************/

#ifndef _TY_STIMER_EVENT_H
#define _TY_STIMER_EVENT_H

#include "board.h"

//ret
// < 0 will delete the timer
// = 0 loop
// > 0 reset the interval_us
typedef int (*ty_timer_event_callback_t)(void);

typedef struct {
    ty_timer_event_callback_t  cb;
    u32                         t;
    u32                         interval;
}ty_timer_event_s;

void ty_stimer_event_start(u8 id, ty_timer_event_callback_t cb, u32 interval_us);
void ty_stimer_event_run(void);
void ty_stimer_event_stop(u8 id);
u8 ty_stimer_event_isstop(u8 id);

#define TY_STIMER_TIMER_ADV_SEND   				0
#define TY_STIMER_TIMER_PIAR_UNBIND   			1
#define TY_STIMER_TIMER_REMOTER_PAIR_WINDOWS	2
#define TY_STIMER_TIMER_USER  				    3
#if CHIP_KIND_FLASH
    #define MAX_TIMER_EVENT                         16
#else
    #define MAX_TIMER_EVENT                         7
#endif
#endif
