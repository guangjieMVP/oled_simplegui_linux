/*
 * @Author: xgj
 * @Date: 2023-08-07 10:49:06
 * @LastEditors: xgj
 * @LastEditTime: 2023-08-17 18:44:49
 * @FilePath: 
 * 
 * Copyright (c) 2023 by Shenzhen Genvict Technologies Co., Ltd. All Rights Reserved. 
 * @Change Log:
 */
#ifndef __SIMPLEGUI_PORT__
#define __SIMPLEGUI_PORT__

struct simplegui_rtc {
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
};

void sgui_port_setpixel(int iX, int iY, unsigned int uiColor);
void sgui_port_refreshdisplay(void);
void sgui_port_cleardisplay(void);

void sgui_timer_init(void);
void sgui_timer_enable(int en);
int sgui_timer_istimeout(void);

void sgui_rtc_get_time(struct simplegui_rtc *rtc);

#endif /* __SIMPLEGUI_PORT__ */