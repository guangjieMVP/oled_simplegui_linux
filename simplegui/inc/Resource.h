
#ifndef _INCLUDE_RESOURCE_H_
#define _INCLUDE_RESOURCE_H_

//#define _SIMPLE_GUI_DEMO_INNER_CHS_

#include "SGUI_Typedef.h"
#ifndef _SIMPLE_GUI_DEMO_INNER_CHS_
#include "DemoResource_ASCII.h"
#else
#include "DemoResource_GB2312.h"
//#include "DemoResource_UTF8.h"
#endif

extern const SGUI_FONT_RES GB2312_FZXS12;
extern const SGUI_FONT_RES GB2312_SONGTI16X16;
extern const SGUI_FONT_RES GB2312_SONGTI18X18;
/* 自定义的汉字库 */
extern const SGUI_FONT_RES GB2312_Font12;  


#endif // _INCLUDE_RESOURCE_H_
