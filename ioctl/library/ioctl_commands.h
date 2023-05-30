/*****************************************
* Group:1
* Name:Prasenjit Karmakar,   Roll: 22CS91P03
* Name:Harsh Borse,          Roll: 21CS72P02
******************************************/
#include <linux/ioctl.h>

#ifndef IOCTL_COMMANDS_H_
#define IOCTL_COMMANDS_H_

struct obj_info {
      int32_t prio_que_size;
      int32_t capacity;
};

#define PB2_SET_CAPACITY   _IOW(0x10, 0x31, int32_t*)
#define PB2_INSERT_INT     _IOW(0x10, 0x32, int32_t*)
#define PB2_INSERT_PRIO    _IOW(0x10, 0x33, int32_t*)
#define PB2_GET_INFO       _IOR(0x10, 0x34, int32_t*)
#define PB2_GET_MIN        _IOR(0x10, 0x35, int32_t*)
#define PB2_GET_MAX        _IOR(0x10, 0x36, int32_t*)

#endif