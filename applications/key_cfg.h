/*
 * Copyright (c) 2006-2021, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2024-01-20     Administrator       the first version
 */
#ifndef APPLICATIONS_KEY_CFG_H_
#define APPLICATIONS_KEY_CFG_H_

#include <rtthread.h>
#include <rtdevice.h>
#include "drv_common.h"

/* 引脚编号，通过查看设备驱动文件drv_gpio.c确定 */
#ifndef BEEP_PIN_NUM
    #define BEEP_PIN_NUM            GET_PIN(B, 8)
#endif
#ifndef KEY0_PIN_NUM
    #define KEY0_PIN_NUM            GET_PIN(E, 2)
#endif
#ifndef KEY1_PIN_NUM
    #define KEY1_PIN_NUM            GET_PIN(E, 3)
#endif

void io_init(void);
void add_face(void *args);
void del_face(void *args);

#endif /* APPLICATIONS_KEY_CFG_H_ */
