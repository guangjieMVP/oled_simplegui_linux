#include <stdint.h>
#include <stdio.h>
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/select.h>
#include <fcntl.h>
#include <pthread.h>
#include <linux/spi/spidev.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <stdbool.h>
#include <linux/sched.h>
#include <signal.h>

#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/if.h>

#include "oled.h"
#include "DemoProc.h"
#include "display.h"
#include "log.h"

void oled_test(void);
#if 0
int main(int argc, char **argv)
{
    sgui_init();
    // oled_test();
    sleep(5);
    oled_off();
    // sgui_run();

    return 0;
}
#else
#define OLED_BUTTON_DEV "/dev/bt2"

bool gs_refresh = true;

struct eth_ip g_eth_ip = {0};

int get_ip(char *eth, char *ip)
{
    int sock = -1, ret = -1;
    struct ifreq ifr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(0 <= sock && ip)
    {
        strcpy(ifr.ifr_name, eth);
        ioctl(sock, SIOCGIFADDR, &ifr);
        strcpy(ip, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
        ret = 0;
    }

    if(0 <= sock)
    {
        close(sock);
    }

    return ret;
}

int get_udp_server(unsigned short port)
{
    int sock;
    struct sockaddr_in saddr;

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(0 <= sock)
    {
        bzero(&saddr, sizeof(saddr));
        saddr.sin_family = AF_INET;
        saddr.sin_addr.s_addr = htonl(INADDR_ANY);
        saddr.sin_port = htons(port);

        bind(sock, (struct sockaddr *)&saddr, sizeof(saddr));
    }
    return sock;
}

static int handle_msg(char *data)
{
    struct display *dp = (struct display *)data;

    if(dp)
    {
        if(13 < dp->size)
        {
            strcpy(dp->data, "?");
            dp->size = 1;
        }
        
        switch(dp->type)
        {
            case DISPLAY_SYS_VER:
             
                break;
            case DISPLAY_APP_VER:

                break;
            case DISPLAY_SYS_STATUS:

                break;
            case DISPLAY_APP_STATUS:

                break;
            case DISPLAY_NO_REFRESH:
                gs_refresh = false;
                break;
            case DISPLAY_REFRESH:
                gs_refresh = true;
                break;
            default:
                break;
        }
    }
}

void log_init(void)
{
#define LOG_FILE_PATH  "/var/log/oled_display.log"
    static FILE *log_fp = NULL;

    log_fp = fopen(LOG_FILE_PATH, "a+");
    if (log_fp != NULL)
    {
        log_add_fp(log_fp, LOG_TRACE);
    }
}

static void oled_show_sleep(int sig) {
    oled_display_off();
}

int main(int argc, char* argv[])
{
    int ret;
    int fd, bt_fd, max_fd;
    fd_set rfds;
    struct timeval tv;
    struct sched_param param;
    unsigned char ip[16];
    unsigned char msg[128];

    signal(SIGALRM, oled_show_sleep);

    pthread_getschedparam(pthread_self(), &ret, &param);
    param.sched_priority = 0;
    sched_setscheduler(0, SCHED_IDLE, &param);

    log_init();

    if(0 == get_ip("eth0", g_eth_ip.eth0_ip))
    {
        // show_string(COL[0], ROW[0], ip, 16);
        log_debug("eth0 ip %s\r\n", g_eth_ip.eth0_ip);
    }

    if(0 == get_ip("eth1", g_eth_ip.eth1_ip))
    {
        // show_string(COL[0], ROW[1], ip, 16);
        log_debug("eth1 ip %s\r\n", g_eth_ip.eth0_ip);
    }

    // show_string(COL[0], ROW[2], "SV:", 16);
    // show_string(COL[0], ROW[3], "AV:", 16);
    // show_string(COL[0], ROW[4], "SS:", 16);
    // show_string(COL[0], ROW[5], "AS:", 16);

    fd = get_udp_server(10050);
    if(0 > fd)
    {
        log_debug("get_udp_server err\n");
        return -1;
    }

    bt_fd = open(OLED_BUTTON_DEV, O_RDONLY);

    alarm(10);

    sgui_init();

    for(;;)
    {
        FD_ZERO(&rfds);
        FD_SET(fd, &rfds);
        
        if(0 <= bt_fd) {
            FD_SET(bt_fd, &rfds);
        }

        max_fd = fd > bt_fd ? fd : bt_fd;

        tv.tv_sec = 1;
        tv.tv_usec = 0;

        ret = select(max_fd + 1, &rfds, NULL, NULL, &tv);
        if(0 < ret)
        {
            if(FD_ISSET(fd, &rfds))
            {
                bzero(msg, sizeof(msg));
                if(0 < recv(fd, msg, sizeof(msg), 0))
                {
                    handle_msg(msg);
                }
            }
            if(0 <= bt_fd) {
                if(FD_ISSET(bt_fd, &rfds)) {
                   alarm(10);
                   oled_display_on();
                }
            }
        }
        else if(0 == ret)
        {
            if(gs_refresh)
            {
                if(0 == get_ip("eth0", g_eth_ip.eth0_ip))
                {
                    // log_debug("eth0 ip %s", g_eth_ip.eth0_ip);
                }

                if(0 == get_ip("eth1", g_eth_ip.eth1_ip))
                {
                    // log_debug("eth1 ip %s", g_eth_ip.eth1_ip);
                }

                // show_string(COL[0], ROW[2], "SV:", 16);
                // show_string(COL[0], ROW[3], "AV:", 16);
                // show_string(COL[0], ROW[4], "SS:", 16);
                // show_string(COL[0], ROW[5], "AS:", 16);
            }
       }

       sgui_run();
    }
    
    return 0;
}

#endif