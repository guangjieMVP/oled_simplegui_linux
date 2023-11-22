#ifndef __OLED_H__
#define __OLED_H__
#include <stdint.h>

void oled_init(void);
void oled_deinit(void);
void oled_reset(void);
void oled_display_off(void);
void oled_display_on(void);
void oled_bklight_ctl(int onoff);
void oled_refresh(void);
void oled_clear_scr(void);
void oled_full_scr(void);
int oled_draw_pixel(uint8_t x, uint8_t y, uint8_t bit);


#if 1

int show_init(char *dev);

void show_clean();

void show_clean_row(unsigned char x, unsigned char y, unsigned char num, unsigned char sizey);

void show_string(unsigned char x, unsigned char y, unsigned char *str, unsigned sizey);

void show_char(unsigned char x, unsigned char y, unsigned char chr, unsigned sizey);

void show_on();

void show_off();

void show_unlock();

void show_lock();

int send_cmd(char *cmd, int size);

#endif

#endif /* __OLED_H__ */