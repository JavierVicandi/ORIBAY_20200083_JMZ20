/******************************************************************************
*
* Freescale Semiconductor Inc.
* (c) Copyright 2013-2016 Freescale Semiconductor, Inc.
* Copyright 2016-2020 NXP
* ALL RIGHTS RESERVED.
*
****************************************************************************//*!
*
* @file      lin_cfg.h
*
* @author    FPT Software
*
* @version   1.0
*
* @date      Wed Jul 15 18:39:12 CEST 2020
*
* @brief     Hardware configuration file
*
******************************************************************************/
#ifndef    _LIN_CFG_H_
#define    _LIN_CFG_H_
#include "lin_hw_cfg.h"
/* Define operating mode */
#define _MASTER_MODE_     0
#define _SLAVE_MODE_      1
#define LIN_MODE   _SLAVE_MODE_
/* Define protocol version */
#define PROTOCOL_21       0
#define PROTOCOL_J2602    1
#define PROTOCOL_20       2
#define LIN_PROTOCOL    PROTOCOL_21

#define SCI_ADDR        SCI0_ADDR    /* For slave */

#define LIN_BAUD_RATE    19200    	 /*For slave*/
/**********************************************************************/
/***************          Diagnostic class selection  *****************/
/**********************************************************************/
#define _DIAG_CLASS_I_          0
#define _DIAG_CLASS_II_         1
#define _DIAG_CLASS_III_        2

#define _DIAG_CLASS_SUPPORT_    _DIAG_CLASS_I_

#define MAX_LENGTH_SERVICE 6

#define MAX_QUEUE_SIZE 1


#define _DIAG_NUMBER_OF_SERVICES_    5

#define DIAGSRV_READ_BY_IDENTIFIER_ORDER    0

#define DIAGSRV_ASSIGN_FRAME_ID_RANGE_ORDER    1

#define DIAGSRV_ASSIGN_NAD_ORDER    2

#define DIAGSRV_CONDITIONAL_CHANGE_NAD_ORDER    3

#define DIAGSRV_SAVE_CONFIGURATION_ORDER    4


/**************** FRAME SUPPORT DEFINITION ******************/
#define _TL_SINGLE_FRAME_       0
#define _TL_MULTI_FRAME_        1

#define _TL_FRAME_SUPPORT_      _TL_SINGLE_FRAME_

/* frame buffer size */
#define LIN_FRAME_BUF_SIZE			11
#define LIN_FLAG_BUF_SIZE			5

/**********************************************************************/
/***************               Interfaces           *******************/
/**********************************************************************/
typedef enum {
   LI0
}l_ifc_handle;

/**********************************************************************/
/***************               Signals              *******************/
/**********************************************************************/
/* Number of signals */
#define LIN_NUM_OF_SIGS  7
/* List of signals */
typedef enum {

   /* Interface_name = LI0 */

   LI0_SensorError

   , LI0_SensorErrorCode
  
   , LI0_SensorErrorValue
  
   , LI0_Humidity
  
   , LI0_LuminosityA
  
   , LI0_LuminosityB
  
   , LI0_Temperature
  
} l_signal_handle;
/**********************************************************************/
/*****************               Frame             ********************/
/**********************************************************************/
/* Number of frames */
#define LIN_NUM_OF_FRMS  7
/* List of frames */
typedef enum {
/* All frames for master node */

   /* Interface_name = LI0 */

   LI0_Humidity_f

   , LI0_LuminosityA_f
  
   , LI0_LuminosityB_f
  
   , LI0_Temperature_f
  
   , LI0_TestSensor_f
  
   , LI0_MasterReq
  
   , LI0_SlaveResp
  
} l_frame_handle;
/**********************************************************************/
/***************             Configuration          *******************/
/**********************************************************************/
/* Size of configuration in ROM and RAM used for interface: LI1 */
#define LIN_SIZE_OF_CFG  9
#define LIN_CFG_FRAME_NUM  5
/*********************************************************************
 * global macros
 *********************************************************************/
#define l_bool_rd(SIGNAL) l_bool_rd_##SIGNAL()
#define l_bool_wr(SIGNAL, A) l_bool_wr_##SIGNAL(A)
#define l_u8_rd(SIGNAL) l_u8_rd_##SIGNAL()
#define l_u8_wr(SIGNAL, A) l_u8_wr_##SIGNAL(A)
#define l_u16_rd(SIGNAL) l_u16_rd_##SIGNAL()
#define l_u16_wr(SIGNAL, A) l_u16_wr_##SIGNAL(A)
#define l_bytes_rd(SIGNAL, start, count, data)  l_bytes_rd_##SIGNAL(start, count, data)
#define l_bytes_wr(SIGNAL, start, count, data) l_bytes_wr_##SIGNAL(start, count, data)
#define l_flg_tst(FLAG) l_flg_tst_##FLAG()
#define l_flg_clr(FLAG) l_flg_clr_##FLAG()
#define LIN_TEST_BIT(A,B) ((l_bool)((((A) & (1U << (B))) != 0U) ? 1U : 0U))
#define LIN_SET_BIT(A,B)                      ((A) |= (l_u8) (1U << (B)))
#define LIN_CLEAR_BIT(A,B)               ((A) &= ((l_u8) (~(1U << (B)))))
#define LIN_BYTE_MASK  ((l_u16)(((l_u16)((l_u16)1 << CHAR_BIT)) - (l_u16)1))
#define LIN_FRAME_LEN_MAX                                             10U

/* Returns the low byte of the 32-bit value    */
#define BYTE_0(n)                              ((l_u8)((n) & (l_u8)0xFF))
/* Returns the second byte of the 32-bit value */
#define BYTE_1(n)                        ((l_u8)(BYTE_0((n) >> (l_u8)8)))
/* Returns the third byte of the 32-bit value  */
#define BYTE_2(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)16)))
/* Returns high byte of the 32-bit value       */
#define BYTE_3(n)                       ((l_u8)(BYTE_0((n) >> (l_u8)24)))

/*
 * defines for signal access
 */



#define LIN_BYTE_OFFSET_LI0_SensorError    8U
#define LIN_BIT_OFFSET_LI0_SensorError    0U
#define LIN_SIGNAL_SIZE_LI0_SensorError    1U
#define LIN_FLAG_BYTE_OFFSET_LI0_SensorError    4U
#define LIN_FLAG_BIT_OFFSET_LI0_SensorError    0U


#define LIN_BYTE_OFFSET_LI0_SensorErrorCode    9U
#define LIN_BIT_OFFSET_LI0_SensorErrorCode    0U
#define LIN_SIGNAL_SIZE_LI0_SensorErrorCode    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorCode    4U
#define LIN_FLAG_BIT_OFFSET_LI0_SensorErrorCode    1U


#define LIN_BYTE_OFFSET_LI0_SensorErrorValue    10U
#define LIN_BIT_OFFSET_LI0_SensorErrorValue    0U
#define LIN_SIGNAL_SIZE_LI0_SensorErrorValue    8U
#define LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorValue    4U
#define LIN_FLAG_BIT_OFFSET_LI0_SensorErrorValue    2U


#define LIN_BYTE_OFFSET_LI0_Humidity    0U
#define LIN_BIT_OFFSET_LI0_Humidity    0U
#define LIN_SIGNAL_SIZE_LI0_Humidity    16U
#define LIN_FLAG_BYTE_OFFSET_LI0_Humidity    0U
#define LIN_FLAG_BIT_OFFSET_LI0_Humidity    0U


#define LIN_BYTE_OFFSET_LI0_LuminosityA    2U
#define LIN_BIT_OFFSET_LI0_LuminosityA    0U
#define LIN_SIGNAL_SIZE_LI0_LuminosityA    16U
#define LIN_FLAG_BYTE_OFFSET_LI0_LuminosityA    1U
#define LIN_FLAG_BIT_OFFSET_LI0_LuminosityA    0U


#define LIN_BYTE_OFFSET_LI0_LuminosityB    4U
#define LIN_BIT_OFFSET_LI0_LuminosityB    0U
#define LIN_SIGNAL_SIZE_LI0_LuminosityB    16U
#define LIN_FLAG_BYTE_OFFSET_LI0_LuminosityB    2U
#define LIN_FLAG_BIT_OFFSET_LI0_LuminosityB    0U


#define LIN_BYTE_OFFSET_LI0_Temperature    6U
#define LIN_BIT_OFFSET_LI0_Temperature    0U
#define LIN_SIGNAL_SIZE_LI0_Temperature    16U
#define LIN_FLAG_BYTE_OFFSET_LI0_Temperature    3U
#define LIN_FLAG_BIT_OFFSET_LI0_Temperature    0U




#define LIN_FLAG_BYTE_OFFSET_LI0_Humidity_f             0
#define LIN_FLAG_BIT_OFFSET_LI0_Humidity_f              0

#define LIN_FLAG_BYTE_OFFSET_LI0_LuminosityA_f             1
#define LIN_FLAG_BIT_OFFSET_LI0_LuminosityA_f              0

#define LIN_FLAG_BYTE_OFFSET_LI0_LuminosityB_f             2
#define LIN_FLAG_BIT_OFFSET_LI0_LuminosityB_f              0

#define LIN_FLAG_BYTE_OFFSET_LI0_Temperature_f             3
#define LIN_FLAG_BIT_OFFSET_LI0_Temperature_f              0

#define LIN_FLAG_BYTE_OFFSET_LI0_TestSensor_f             4
#define LIN_FLAG_BIT_OFFSET_LI0_TestSensor_f              0


/**********************************************************************/
/***************        Static API Functions        *******************/
/**********************************************************************/
/*
 * the static signal access macros
 */


/* static access macros for signal LI0_SensorError */

 
#define l_bool_rd_LI0_SensorError() \
    (LIN_TEST_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorError], \
    LIN_BIT_OFFSET_LI0_SensorError))

#define l_bool_wr_LI0_SensorError(A) \
    {(A) ? \
    (LIN_SET_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorError], \
    LIN_BIT_OFFSET_LI0_SensorError)):\
    (LIN_CLEAR_BIT(lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorError], \
    LIN_BIT_OFFSET_LI0_SensorError));\
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorError],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorError);}
 
/* static access macros for signal LI0_SensorErrorCode */
 
#define l_u8_rd_LI0_SensorErrorCode() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorCode]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_SensorErrorCode(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorCode] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorCode] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorCode],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorCode); \
    }


 
/* static access macros for signal LI0_SensorErrorValue */
 
#define l_u8_rd_LI0_SensorErrorValue() \
    ((l_u8)  (((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorValue]) >> 0U) & 0xffU))


#define l_u8_wr_LI0_SensorErrorValue(A) \
    { \
    lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorValue] = \
    (l_u8)((lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_SensorErrorValue] & 0x00U) | \
    (((A) << 0U) & 0xffU)); \
    LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorValue],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorValue); \
    }


/* static access macros for signal LI0_Humidity */
 
#define l_bytes_rd_LI0_Humidity(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i)  (data)[i] = lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Humidity + i + (start)];}

#define l_bytes_wr_LI0_Humidity(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i) \
     { \
        buffer_backup_data[i + (start) + 0] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Humidity + i + (start)]; \
        lin_frame_updating_flag_tbl[LI0_Humidity_f] |= (1 << (i + (start) + 0)); \
        lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Humidity + i + (start)]  = (data)[i]; \
     } \
     lin_frame_updating_flag_tbl[LI0_Humidity_f] &= (~(((1 << count) -1) << (start + 0))); \
     LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Humidity],\
         LIN_FLAG_BIT_OFFSET_LI0_Humidity);}
/* static access macros for signal LI0_LuminosityA */
 
#define l_bytes_rd_LI0_LuminosityA(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i)  (data)[i] = lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityA + i + (start)];}

#define l_bytes_wr_LI0_LuminosityA(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i) \
     { \
        buffer_backup_data[i + (start) + 0] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityA + i + (start)]; \
        lin_frame_updating_flag_tbl[LI0_LuminosityA_f] |= (1 << (i + (start) + 0)); \
        lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityA + i + (start)]  = (data)[i]; \
     } \
     lin_frame_updating_flag_tbl[LI0_LuminosityA_f] &= (~(((1 << count) -1) << (start + 0))); \
     LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityA],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityA);}
/* static access macros for signal LI0_LuminosityB */
 
#define l_bytes_rd_LI0_LuminosityB(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i)  (data)[i] = lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityB + i + (start)];}

#define l_bytes_wr_LI0_LuminosityB(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i) \
     { \
        buffer_backup_data[i + (start) + 0] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityB + i + (start)]; \
        lin_frame_updating_flag_tbl[LI0_LuminosityB_f] |= (1 << (i + (start) + 0)); \
        lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_LuminosityB + i + (start)]  = (data)[i]; \
     } \
     lin_frame_updating_flag_tbl[LI0_LuminosityB_f] &= (~(((1 << count) -1) << (start + 0))); \
     LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityB],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityB);}
/* static access macros for signal LI0_Temperature */
 
#define l_bytes_rd_LI0_Temperature(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i)  (data)[i] = lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Temperature + i + (start)];}

#define l_bytes_wr_LI0_Temperature(start, count, data) \
    {l_u8       i; \
     for (i = 0; i < (count); ++i) \
     { \
        buffer_backup_data[i + (start) + 0] =  lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Temperature + i + (start)]; \
        lin_frame_updating_flag_tbl[LI0_Temperature_f] |= (1 << (i + (start) + 0)); \
        lin_pFrameBuf[LIN_BYTE_OFFSET_LI0_Temperature + i + (start)]  = (data)[i]; \
     } \
     lin_frame_updating_flag_tbl[LI0_Temperature_f] &= (~(((1 << count) -1) << (start + 0))); \
     LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Temperature],\
         LIN_FLAG_BIT_OFFSET_LI0_Temperature);}


/* Signal flag APIs */

#define l_flg_tst_LI0_SensorError_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorError],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorError)
#define l_flg_clr_LI0_SensorError_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorError],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorError)

#define l_flg_tst_LI0_SensorErrorCode_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorCode],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorCode)
#define l_flg_clr_LI0_SensorErrorCode_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorCode],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorCode)

#define l_flg_tst_LI0_SensorErrorValue_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorValue],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorValue)
#define l_flg_clr_LI0_SensorErrorValue_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_SensorErrorValue],\
         LIN_FLAG_BIT_OFFSET_LI0_SensorErrorValue)

#define l_flg_tst_LI0_Humidity_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Humidity],\
         LIN_FLAG_BIT_OFFSET_LI0_Humidity)
#define l_flg_clr_LI0_Humidity_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Humidity],\
         LIN_FLAG_BIT_OFFSET_LI0_Humidity)

#define l_flg_tst_LI0_LuminosityA_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityA],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityA)
#define l_flg_clr_LI0_LuminosityA_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityA],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityA)

#define l_flg_tst_LI0_LuminosityB_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityB],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityB)
#define l_flg_clr_LI0_LuminosityB_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_LuminosityB],\
         LIN_FLAG_BIT_OFFSET_LI0_LuminosityB)

#define l_flg_tst_LI0_Temperature_flag() \
         LIN_TEST_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Temperature],\
         LIN_FLAG_BIT_OFFSET_LI0_Temperature)
#define l_flg_clr_LI0_Temperature_flag() \
         LIN_CLEAR_BIT(lin_flag_handle_tbl[LIN_FLAG_BYTE_OFFSET_LI0_Temperature],\
         LIN_FLAG_BIT_OFFSET_LI0_Temperature)



/* Frame flag APIs */

   /* Interface_name = LI0 */

 #define l_flg_tst_LI0_Humidity_f_flag() \
          lin_frame_flag_tbl[LI0_Humidity_f]
 #define l_flg_clr_LI0_Humidity_f_flag() \
          lin_frame_flag_tbl[LI0_Humidity_f] = 0

 #define l_flg_tst_LI0_LuminosityA_f_flag() \
          lin_frame_flag_tbl[LI0_LuminosityA_f]
 #define l_flg_clr_LI0_LuminosityA_f_flag() \
          lin_frame_flag_tbl[LI0_LuminosityA_f] = 0

 #define l_flg_tst_LI0_LuminosityB_f_flag() \
          lin_frame_flag_tbl[LI0_LuminosityB_f]
 #define l_flg_clr_LI0_LuminosityB_f_flag() \
          lin_frame_flag_tbl[LI0_LuminosityB_f] = 0

 #define l_flg_tst_LI0_Temperature_f_flag() \
          lin_frame_flag_tbl[LI0_Temperature_f]
 #define l_flg_clr_LI0_Temperature_f_flag() \
          lin_frame_flag_tbl[LI0_Temperature_f] = 0

 #define l_flg_tst_LI0_TestSensor_f_flag() \
          lin_frame_flag_tbl[LI0_TestSensor_f]
 #define l_flg_clr_LI0_TestSensor_f_flag() \
          lin_frame_flag_tbl[LI0_TestSensor_f] = 0

 #define l_flg_tst_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq]
 #define l_flg_clr_LI0_MasterReq_flag() \
          lin_frame_flag_tbl[LI0_MasterReq] = 0

 #define l_flg_tst_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp]
 #define l_flg_clr_LI0_SlaveResp_flag() \
          lin_frame_flag_tbl[LI0_SlaveResp] = 0



/* INTERFACE MANAGEMENT */

#define l_ifc_init_LI0() l_ifc_init(LI0)



#define l_ifc_wake_up_LI0() l_ifc_wake_up(LI0)



#define l_ifc_rx_LI0() l_ifc_rx(LI0)



#define l_ifc_tx_LI0() l_ifc_tx(LI0)



#define l_ifc_aux_LI0() l_ifc_aux(LI0)



#define l_ifc_read_status_LI0() l_ifc_read_status(LI0)


#endif    /* _LIN_CFG_H_ */