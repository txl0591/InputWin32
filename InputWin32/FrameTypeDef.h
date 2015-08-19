#ifndef FrameTypeDef
#define FrameTypeDef

#include "Mutex.h"

/* FrameDef */
#define FRAME_START		    0
#define FRAME_VERSION	    1
#define FRAME_CTRL		    2
#define FRAME_DST		    3
#define FRAME_SRC		    4
#define FRAME_LEN		    5
#define FRAME_ID		    6
#define FRAME_CMD		    7
#define FRAME_DATA		    8

/* FrameAddr */
#define ADDR_NONE		    0xFF
#define ADDR_RFID		    0
#define ADDR_PC			    1
#define ADDR_ANDROID	    2

/* FrameType */
#define FRAME_SEND		    0x00
#define FRAME_ACK		    0x80

/* FrameAck */
#define ACK_NEED		    0
#define ACK_NONEED		    1

/* FrameCommond */
#define COMMAND_SENDPACK     0x01

/* FrameCommondEcho */
#define ECHO_OK 			 0x00
#define ECHO_ERR 			 0x01
#define ECHO_ERR_SIZE 		 0x02

/* Echo */
#define ECHO_COM_ERROR       0x00
#define ECHO_READ_ERROR	     0x01
#define ECHO_READ_SUCCESS	 0x02

#endif