#include "oled.h"
#include "sgui_port.h"

void sgui_port_setpixel(int iX, int iY, unsigned int uiColor)
{
    oled_draw_pixel(iX, iY, uiColor);
}

void sgui_port_refreshdisplay(void)
{
    oled_refresh();
}

void sgui_port_cleardisplay(void)
{
    oled_clear_scr();
}

// static struct rt_timer systick_timer;
// static struct rt_timer rtc_timer;
static uint8_t tmr_timeout = 0;
struct simplegui_rtc simgui_rtc = {
    .hour = 16,
    .min = 29,
    .sec = 0,
};

static void _rt_tmr_cb(void *parg)
{
    tmr_timeout = 1;
    simgui_rtc.sec++;
    if (simgui_rtc.sec >= 60)
    {
        simgui_rtc.sec  = 0;
        simgui_rtc.min++;
        if (simgui_rtc.min >= 60)
        {
            simgui_rtc.min = 0;
            simgui_rtc.hour++;
            if (simgui_rtc.hour >= 24)
            {
                simgui_rtc.hour = 0;
            }
        }
    }
}

void sgui_timer_init(void)
{
    // rt_timer_init(&systick_timer, "systick_tmr", _rt_tmr_cb, NULL, 1000, RT_TIMER_FLAG_PERIODIC | RT_TIMER_FLAG_SOFT_TIMER);
}

void sgui_timer_enable(int en)
{
    if (en)
    {
        // rt_timer_start(&systick_timer);
    }
    else
    {
        // rt_timer_stop(&systick_timer);
        tmr_timeout = 0;
    }
}

int sgui_timer_istimeout(void)
{
    if (tmr_timeout)
    {
        tmr_timeout = 0;
        return 1;
    }
    return 0;
}

void sgui_rtc_get_time(struct simplegui_rtc *rtc)
{
    memmove(rtc, &simgui_rtc, sizeof(simgui_rtc));
}




