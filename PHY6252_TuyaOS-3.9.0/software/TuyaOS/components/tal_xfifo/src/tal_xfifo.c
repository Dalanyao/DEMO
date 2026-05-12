/**
 * @file tal_xfifo.c
 * @brief This is tbs_fifo file
 * @version 1.0
 * @date 2022-06-14
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#include "tal_xfifo.h"

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/


/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/


/***********************************************************************
 ********************* variable ****************************************
 **********************************************************************/


/***********************************************************************
 ********************* function ****************************************
 **********************************************************************/
VOID_T tal_xfifo_init(TAL_XFIFO_T *p, UINT8_T *buff, UINT16_T maxsize)
{
    p->rear = 0;
    p->size = 0;
    p->mdata = buff;
    p->maxsize = maxsize;
}

UINT16_T tal_xfifo_isfull(TAL_XFIFO_T *p)
{
    return (p->size == p->maxsize);
}

UINT16_T tal_xfifo_isempty(TAL_XFIFO_T *p)
{
    return (p->size == 0);
}

UINT16_T tal_xfifo_clean(TAL_XFIFO_T *p)
{
    UINT16_T size = p->size;
    p->rear = 0;
    p->size = 0;
    return size;
}

UINT16_T tal_xfifo_get_size(TAL_XFIFO_T *p)
{
    return p->size;
}

UINT16_T tal_xfifo_get_left(TAL_XFIFO_T *p)
{
    return p->maxsize-p->size;
}

UINT8_T tal_xfifo_add_one(TAL_XFIFO_T *p, UINT8_T mdata)
{
    p->mdata[p->rear] = mdata;
    p->rear++;
    p->rear %= p->maxsize;
    p->size++;
    if (p->size > p->maxsize)
        p->size = p->maxsize;
    return 1;
}

UINT16_T tal_xfifo_add(TAL_XFIFO_T *p, UINT8_T *pdata, UINT16_T len)
{
    UINT16_T i;
    for (i=0; i<len; i++) {
        p->mdata[p->rear] = pdata[i];
        p->rear++;
        p->rear %= p->maxsize;
        p->size++;
        if (p->size > p->maxsize)
            p->size = p->maxsize;
    }

    return len;
}

UINT16_T tal_xfifo_read(TAL_XFIFO_T *p, UINT8_T *pdata, UINT16_T len)
{
    UINT16_T i;
    UINT16_T num = (p->size > len ? len:p->size);
    UINT16_T j = p->rear >= p->size ? p->rear - p->size : p->rear + p->maxsize - p->size;
    for (i=0; i<num; i++) {
        pdata[i] = p->mdata[j];
        j++;
        j %= p->maxsize;
    }
    return num;
}

UINT16_T tal_xfifo_pop(TAL_XFIFO_T *p, UINT16_T len)
{
    UINT16_T delete = len > p->size ? p->size : len;
    p->size -= delete;

    return delete;
}

