#ifndef __INCLUDE_DEMO_PROC_H__
#define __INCLUDE_DEMO_PROC_H__
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include <stdlib.h>
#include <stdio.h>
#include "SGUI_Typedef.h"
#include "SGUI_Common.h"
#include "HMI_Engine.h"
#include "DemoActions.h"
#include "Resource.h"
#ifdef _SIMPLE_GUI_IN_VIRTUAL_SDK_
#include "SDKInterface.h"
#endif

//=======================================================================//
//= Data type definition.                                               =//
//=======================================================================//
// HMI action type
typedef enum
{
    EVENT_ID_UNKNOW =               0,
    // User defined action type.
    EVENT_ID_KEY_PRESS,
    EVENT_ID_TIMER,
    EVENT_ID_RTC,
    // User defined action above.
    EVENT_ID_MAX
}EVENT_ID;

// HMI event type.
typedef enum
{
    EVENT_TYPE_ANY =                0,
    EVENT_TYPE_ACTION,
    EVENT_TYPE_DATA,
}EVENT_TYPE;

enum DEMO_PROC_ACTION_ID
{
    HMI_DEMO_PROC_NO_ACT =          0,
    HMI_DEMO_PROC_CONFIRM =         1,
    HMI_DEMO_PROC_CANCEL =          2,
};

typedef struct
{
    SGUI_UINT16     uiKeyValue;
}KEY_EVENT_DATA;

typedef struct
{
    SGUI_INT        iValue;
}DUMMY_DATA_EVENT_DATA;

HMI_EVENT_TYPE_DECLARE(KEY_PRESS_EVENT, KEY_EVENT_DATA)
HMI_EVENT_TYPE_DECLARE(DATA_EVENT, DUMMY_DATA_EVENT_DATA);

//=======================================================================//
//= Macro definition.                                                   =//
//=======================================================================//
#define     HMI_SCREEN_ID_DEMO_SCROLLING_TEXT           (1001)
#define     HMI_SCREEN_ID_DEMO_PAINT_TEXT               (1002)
#define     HMI_SCREEN_ID_DEMO_LIST                     (1003)
#define     HMI_SCREEN_ID_DEMO_TEXT_NOTICE              (1004)
#define     HMI_SCREEN_ID_DEMO_BASIC_PAINT              (1005)
#define     HMI_SCREEN_ID_DEMO_VARIABLE_BOX             (1006)
#define     HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH          (1007)
#define     HMI_SCREEN_ID_DEMO_MENU                     (1008)
#define     HMI_SCREEN_ID_DEMO_CURVE                    (1009)
#define     HMI_SCREEN_ID_DEMO_POLOR_COORD              (1010)
#define     HMI_SCREEN_ID_HAREWARE_PAGE                 (1011)
#define     HMI_SCREEN_ID_M2PC_TYPE_PAGE                (1012)
#define     HMI_SCREEN_ID_M2PC_PARA_PAGE                (1013)
#define     HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE           (1014)
#define     HMI_SCREEN_ID_SOFT_VER_PAGE                 (1015)
#define     HMI_SCREEN_ID_DEV_STATE_PAGE                (1016)
#define     HMI_SCREEN_ID_CARD_REC_PAGE                 (1017)
#define     HMI_SCREEN_ID_IP_SHOW_PAGE                  (1018)
#define     DEMO_HEART_BEAT_INTERVAL_MS                 (20)

#define HMI_ENABLE_ZH   (1)

//=======================================================================//
//= Public variable declaration.                                        =//
//=======================================================================//
/* HMI engine object. */
extern HMI_ENGINE_OBJECT        g_stDemoEngine;
/* Screen display objects. */
extern HMI_SCREEN_OBJECT        g_stHMIDemo_ScrollingText;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_List;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_Notice;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_RTCNotice;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_VariableBox;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_RealtimeGraph;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_Menu;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_BasicPaint;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_TextPaint;
extern HMI_SCREEN_OBJECT        g_stHMIDemo_Curve;
extern HMI_SCREEN_OBJECT		g_stHMIDemo_PolorCoord;
extern HMI_SCREEN_OBJECT        g_stHMI_Hardware_VerPage;
extern HMI_SCREEN_OBJECT        g_stHMI_M2_Type_Page;
extern HMI_SCREEN_OBJECT        g_stHMIM2PC_Para_List;
extern HMI_SCREEN_OBJECT        g_stHMIM2PC_ParaShow_Page;
extern HMI_SCREEN_OBJECT       g_stHMI_SoftVer_Page;
extern HMI_SCREEN_OBJECT       g_stHMI_DevState_Page;
extern HMI_SCREEN_OBJECT       g_stHMI_CardRec_Page;
extern HMI_SCREEN_OBJECT       g_stHMI_IpShow_Page;
//=======================================================================//
//= Function declare.                                                   =//
//=======================================================================//
#ifdef __cplusplus
extern "C"
{
#endif /* __cplusplus */
void                sgui_init(void);
void                sgui_run(void);
bool                SysTickTimerTriggered(void);
bool                RTCTimerTriggered(void);
bool                UserEventTriggered(void);

void                SysTickTimerEnable(bool bEnable);
void                RTCTimerEnable(bool bEnable);

#ifdef __cplusplus
}
#endif /* __cplusplus */
#endif // __INCLUDE_DEMO_PROC_H__