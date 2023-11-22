
#include <stdio.h>
#include <string.h>
#include <strings.h>
#include <stdlib.h>
#include <stdint.h>
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
#include <poll.h>

#define OLED_SDA_PATH    "/sys/class/gpio/gpio89/value"
#define OLED_SCL_PATH    "/sys/class/gpio/gpio91/value"
#define OLED_SPI_DEV     "/dev/spidev0.0"
#define OLED_HZ          6200000
#define OLED_DC_PATH     "/sys/class/gpio/gpio141/value"
#define OLED_BKL_PATH  "/sys/class/gpio/gpio140/value"
#define OLED_CS_PATH     "/sys/class/gpio/gpio90/value"
#define OLED_RST_PATH    "/sys/class/gpio/gpio88/value"

#define OLED_RST_H()   oled_rst_write(1)
#define OLED_RST_L()   oled_rst_write(0)

#define OLED_CS_H()   oled_cs_write(1)
#define OLED_CS_L()   oled_cs_write(0)

// #define OLED_SCL_H()   oled_scl_write(1)
// #define OLED_SCL_L()   oled_scl_write(0)

// #define OLED_SDA_H()   oled_sda_write(1)
// #define OLED_SDA_L()   oled_sda_write(0)

#define OLED_DC_CMD()   oled_dc_write(0)
#define OLED_DC_DATA()   oled_dc_write(1)

#define OLED_BKL_ON()   oled_bk_write(1)
#define OLED_BKL_OFF()   oled_bk_write(0)

typedef enum {
    OLED_Cmd = 0,
    OLED_Data = 1,
} oled_data_type_t;


static int g_oled_fd = -1;

static int get_oled_fd()
{
    return g_oled_fd;
}

static void sleep_ms(unsigned int ms)
{
    // struct timeval delay;
	// delay.tv_sec = 0;
	// delay.tv_usec = ms * 1000; 
	// select(0, NULL, NULL, NULL, &delay);
    usleep(ms*1000);
}


static int oled_dc_write(int val)
{
    char mode[8];
    static int s_fd = -1;

    mode[0] = '0' + val;

    if(s_fd > 0)
    {
        return write(s_fd, mode, 1);
    }
   
    s_fd = open(OLED_DC_PATH, O_RDWR | O_SYNC);
    if (s_fd <= 0)
    {
        printf("open failed %d\n", s_fd);
        perror("open");
        return -1;
    }

    write(s_fd, mode, 1);

    return 0;
}

static int oled_sda_write(int val)
{
    char mode[8];
    static int s_fd = -1;

    mode[0] = '0' + val;

    if(s_fd > 0)
    {
        return write(s_fd, mode, 1);
    }
   
    s_fd = open(OLED_SDA_PATH, O_RDWR | O_SYNC);
    if (s_fd <= 0)
    {
        printf("open failed %d\n", s_fd);
        perror("open");
        return -1;
    }

    return 0;
}

static int oled_scl_write(int val)
{
    // char mode[8];
    // static int s_fd = -1;

    // mode[0] = '0' + val;

    // if(s_fd > 0)
    // {
    //     return write(s_fd, mode, 1);
    // }
   
    // s_fd = open(OLED_SCL_PATH, O_RDWR | O_SYNC);
    // if (s_fd <= 0)
    // {
    //     printf("open failed %d\n", s_fd);
    //     perror("open");
    //     return -1;
    // }

    // write(s_fd, mode, 1);

    return 0;
}

static int oled_bk_write(int val)
{
    char mode[8];
    static int s_fd = -1;

    mode[0] = '0' + val;

    if(s_fd > 0)
    {
        return write(s_fd, mode, 1);
    }
   
    s_fd = open(OLED_BKL_PATH, O_RDWR | O_SYNC);
    if (s_fd <= 0)
    {
        printf("open failed %d\n", s_fd);
        perror("open");
        return -1;
    }

    write(s_fd, mode, 1);

    return 0;
}

static int oled_cs_write(int val)
{
    char mode[8];
    static int s_fd = -1;

    mode[0] = '0' + val;

    if(s_fd > 0)
    {
        return write(s_fd, mode, 1);
    }
   
    s_fd = open(OLED_CS_PATH, O_RDWR | O_SYNC);
    if (s_fd <= 0)
    {
        printf("open failed %d\n", s_fd);
        perror("open");
        return -1;
    }

    write(s_fd, mode, 1);

    return 0;
}

static int oled_rst_write(int val)
{
    char mode[8];
    static int s_fd = -1;

    mode[0] = '0' + val;

    if(s_fd > 0)
    {
        return write(s_fd, mode, 1);
    }
   
    s_fd = open(OLED_RST_PATH, O_RDWR | O_SYNC);
    if (s_fd <= 0)
    {
        printf("open failed %d\n", s_fd);
        perror("open");
        return -1;
    }

    write(s_fd, mode, 1);

    return 0;
}

void oled_reset(void)
{
    OLED_RST_H();
    sleep_ms(20);
    OLED_RST_L();
    sleep_ms(20);
    OLED_RST_H();
    sleep_ms(20);
}

void oled_bklight_ctl(int onoff)
{
    if (onoff)
    {
        OLED_BKL_ON();
    }
    else
    {
        OLED_BKL_OFF();
    }
}

static void oled_write( oled_data_type_t t, uint8_t data)
{
    // 判断写入的是命令或数据
    if (OLED_Cmd == t)
    {
        OLED_DC_CMD();
    }
    else
    {
        OLED_DC_DATA();
    }

    write(g_oled_fd, &data, 1);
}

/* LCD RAM Buffer */
static uint8_t lcd_gram[128][8] = {0};

/**
 * @description: 刷新重绘屏幕
 * @return {*}
 */
void oled_refresh(void)
{
    int i, j;

    for (i = 0; i < 8; i++)
    // for (i = 7; i >= 0; i--)
    {
        oled_write(OLED_Cmd, 0xb0+i);
        oled_write(OLED_Cmd, 0x10);
        oled_write(OLED_Cmd, 0x00);
        for (j = 0; j < 128; j++)
        // for (j = 127; j >= 0; j--)
        {
            oled_write(OLED_Data, lcd_gram[j][i]);
        }
    }
}

/**
 * @description: 清屏，也就是清除LCD RAM Buffer
 * @return {*}
 */
void oled_clear_scr(void)
{
    memset(&lcd_gram, 0, sizeof(lcd_gram));
}

void oled_full_scr(void)
{
    printf("oled frambuffer size %d\r\n", sizeof(lcd_gram));
    memset(&lcd_gram, 0xFF, sizeof(lcd_gram));
}

/**
 * @description: 写一个像素点
 * @param {uint8_t} x 坐标
 * @param {uint8_t} y 坐标
 * @param {uint8_t} bit 颜色值，单色屏只有0和1
 * @return {*}
 */
int oled_draw_pixel(uint8_t x, uint8_t y, uint8_t bit)
{
    int i, j;

    if (x > 127 || y > 63) return -1;

    uint8_t pos = y / 8;
    uint8_t bit_x = y % 8;

    uint8_t bit_data = (1 << bit_x);

    if (bit)
    {
        lcd_gram[x][pos] |= (1 << bit_x);
    }
    else
    {
        lcd_gram[x][pos] &= ~(1 << bit_x);
    }

    return 0;
}

void oled_display_off(void)
{
    oled_write(OLED_Cmd, 0xae);   // Lcd Disply OFF
    oled_bklight_ctl(0);
}

void oled_display_on(void)
{
    oled_write(OLED_Cmd, 0xaf); // Lcd Disply ON
    oled_bklight_ctl(1);
}

void oled_init(void)
{
    unsigned char mode = SPI_MODE_0;
	unsigned char bits = 8;
    int speed = 1000000;
    unsigned char cnt = 0;
    char buf[128];

    int fd = open(OLED_SPI_DEV, O_WRONLY | O_NOCTTY);

    if(0 > fd)
    {
        perror("open");
        return -1;
    }

    g_oled_fd = fd;

    int flags = fcntl(fd, F_GETFL, 0);
    flags |= O_NONBLOCK;
    fcntl(fd, F_SETFL, flags);

	if(ioctl(fd, SPI_IOC_WR_MODE, &mode) != 0)
	{
		printf("can't set spi mode\n");
		close(fd);
		return -1;
	}

	if(ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits) != 0)
	{
		printf("can't set bits per word\n");
		close(fd);
		return -1;
	}

	if(ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed) != 0)
	{
		printf("can't set max speed hz\n");
		close(fd);
		return -1;
	}   

    oled_reset();

    oled_bklight_ctl(1);

    oled_write(OLED_Cmd, 0xa2); // 1/9 Bias
    oled_write(OLED_Cmd, 0xa6); // Set Inverse display
    /* 设定列扫描方向? ?0xa0为常规方向 由左至右? 0xa1为反转方向 由右向左 */
    oled_write(OLED_Cmd, 0xa0); // ADC set (SEG)
    /* 行方向 0xc0为普通扫描 0xc8为反转扫描 从上到下 */
    oled_write(OLED_Cmd, 0xc8); // COM reves

    oled_write(OLED_Cmd, 0x24); // 有效值范围：0X20-0X27粗调 每升高一阶电压增加零点几伏
    oled_write(OLED_Cmd, 0x81); // Electronic Volume Mode Set
    oled_write(OLED_Cmd, 0x10); // 有效值范围：0X00-0X3F微调 每升高一阶电压增加零点零几伏

    oled_write(OLED_Cmd, 0x2c);
    sleep_ms(200); // 延时200mS

    oled_write(OLED_Cmd, 0x2e);
    sleep_ms(200);         // 延时200mS
    oled_write(OLED_Cmd, 0x2f); // The Power Control Set

    sleep_ms(200);         // 延时200mS
    oled_write(OLED_Cmd, 0xaf); // Lcd Disply ON
}

void oled_deinit(void)
{
    if (g_oled_fd > 0)
    {
        oled_bklight_ctl(0);
        close(g_oled_fd);
    }
}

void oled_test(void)
{
    for (int i = 0; i < 10; i++)
    {
        oled_clear_scr();
        oled_refresh();

        sleep_ms(1000);

        oled_full_scr();
        oled_refresh();

        sleep_ms(1000);
    }
}