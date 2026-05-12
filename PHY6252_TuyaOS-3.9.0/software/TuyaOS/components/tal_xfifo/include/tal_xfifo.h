/**
 * @file tal_xfifo.h
 * @brief This is tbs_fifo file
 * @version 1.0
 * @date 2022-06-14
 *
 * @copyright Copyright 2021-2031 Tuya Inc. All Rights Reserved.
 *
 */

#ifndef __TAL_XFIFO_H__
#define __TAL_XFIFO_H__

#include "tuya_cloud_types.h"
#include "tuya_error_code.h"

#ifdef __cplusplus
extern "C" {
#endif

/***********************************************************************
 ********************* constant ( macro and enum ) *********************
 **********************************************************************/


/***********************************************************************
 ********************* struct ******************************************
 **********************************************************************/
/*
 应用层在使用该组件时，需要自己定义一个实际BUFF，然后将该BUFF赋值给唯一的一个TBS_FIFO_T:

 #define MAXSIZE 1024
 UINT8_T buf[MAXSIZE];
 TBS_FIFO_T fifo;
 tbs_fifo_init(&fifo,buff,MAXSIZE);

 这样设计可以支持多个FIFO的情况，而且可以根据应用层自己的需求设置FIFO BUFF大小
 */
typedef struct {
    UINT16_T rear;    // 头
    UINT16_T size;    // 当前容量
    UINT16_T maxsize; // 最大容量
    UINT8_T *mdata;   // buff 指针
} TAL_XFIFO_T;

/***********************************************************************
 ********************* variable ****************************************
 **********************************************************************/


/***********************************************************************
 ********************* function ****************************************
 **********************************************************************/

/**
 * @brief tal_xfifo_init
 *
 * @param[in] *p: *p
 * @param[in] *buff: *buff
 * @param[in] maxsize: maxsize
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
VOID_T tal_xfifo_init(TAL_XFIFO_T *p, UINT8_T *buff, UINT16_T maxsize);

/**
 * @brief tal_xfifo_isfull
 *
 * @param[in] *p: *p
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_isfull(TAL_XFIFO_T *p);

/**
 * @brief tal_xfifo_isempty
 *
 * @param[in] *p: *p
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_isempty(TAL_XFIFO_T *p);

/**
 * @brief tal_xfifo_clean
 *
 * @param[in] *p: *p
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_clean(TAL_XFIFO_T *p);

/**
 * @brief tal_xfifo_get_size
 *
 * @param[in] *p: *p
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_get_size(TAL_XFIFO_T *p);

/**
 * @brief tal_xfifo_get_left
 *
 * @param[in] *p: *p
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_get_left(TAL_XFIFO_T *p);

/**
 * @brief tal_xfifo_add_one
 *
 * @param[in] *p: *p
 * @param[in] mdata: mdata
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT8_T tal_xfifo_add_one(TAL_XFIFO_T *p, UINT8_T mdata);

/**
 * @brief tal_xfifo_add
 *
 * @param[in] *p: *p
 * @param[in] *pdata: *pdata
 * @param[in] len: len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_add(TAL_XFIFO_T *p, UINT8_T *pdata, UINT16_T len);

/**
 * @brief tal_xfifo_read
 *
 * @param[in] *p: *p
 * @param[in] *pdata: *pdata
 * @param[in] len: len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_read(TAL_XFIFO_T *p, UINT8_T *pdata, UINT16_T len);

/**
 * @brief tal_xfifo_pop
 *
 * @param[in] *p: *p
 * @param[in] len: len
 *
 * @return OPRT_OK on success. Others on error, please refer to tuya_error_code.h
 */
UINT16_T tal_xfifo_pop(TAL_XFIFO_T *p, UINT16_T len);


#ifdef __cplusplus
}
#endif

#endif /* __TBS_FIFO_H__ */

