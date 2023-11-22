#ifndef __DISPLAY_H__
#define __DISPLAY_H__


enum display_type
{
    DISPLAY_SYS_VER = 0,
    DISPLAY_SYS_STATUS,
    DISPLAY_APP_VER,
    DISPLAY_APP_STATUS,
    DISPLAY_NO_REFRESH,
    DISPLAY_REFRESH,
};

struct display
{
    unsigned char type;
    unsigned char size;
    unsigned char data[0];
};

struct eth_ip {
    unsigned char eth0_ip[20];
    unsigned char eth1_ip[20];
};

extern struct eth_ip g_eth_ip;

extern bool gs_refresh;

#endif
