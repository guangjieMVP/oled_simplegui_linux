/*
 * @Author: xgj
 * @Date: 2023-07-10 16:35:21
 * @LastEditors: xgj
 * @LastEditTime: 2023-08-05 15:43:17
 * @FilePath: \simplegui\DemoProc\src\List.c
 * @Description: 缴费机LCD主界面
 *
 * Copyright (c) 2023 by Shenzhen Genvict Technologies Co., Ltd. All Rights Reserved.
 * @Change Log:
 */
//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_List.h"
#include "SGUI_FontResource.h"
#include "Resource.h"

//=======================================================================//
//= Macro definition.                                                   =//
//=======================================================================//
#define                 NOTICE_TEXT_BUFFER_SIZE             (64)

// #define SCREEN1_TITLE   ("主菜单")

// #define SCREEN1_TITLE   ("\x97\x62\x96\x50")
#define SCREEN1_TITLE   ("\x83\xDC\x81\x55")
#define LIST_ITEM_0   ("\x81\x61\x81\x47\x81\xDE\x81\x36")      //卡片回收
#define LIST_ITEM_1   ("\x8B\xBE\x97\x77\x97\x78\x97\x79")      // 设备状态
#define LIST_ITEM_2   ("\x8F\x6F\x81\xF6\x81\x48\x81\x2C")    // 软件版本
#define LIST_ITEM_3   ("\x81\x6C\x81\xF6\x81\x48\x81\x2C")      //硬件版本 
#define LIST_ITEM_4   ("\x81\xE5\x81\x4D\x97\x72\x97\x73")      //工位类型
// #define LIST_ITEM_4   ("\x81\xC2\x81\x71\x81\x3E\x81\x3A")      //参数显示
#define LIST_ITEM_5   ("\x81\xF8\x94\x9F")      //时钟
#define LIST_ITEM_6   ("\x81\x9E\x81\xF8\x81\xF2\x81\xBF")      //实时曲线
//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT    HMI_DemoList_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
static const SGUI_RECT      s_arrstLayouts[] =          { {0, 0, 128, 64},
                                                            {0, 0, 192, 96},
                                                            {0, 0, 192, 128},
                                                            };
static SGUI_SIZE            s_iLayoutIndex = 1;

static SGUI_ITEMS_ITEM      s_arrstListItems[] =        {
                                                            {LIST_ITEM_0, NULL},
                                                            {LIST_ITEM_1, NULL},  
                                                            {LIST_ITEM_2, NULL},        
                                                            {LIST_ITEM_3, NULL},        
                                                            {LIST_ITEM_4, NULL},
                                                            // {LIST_ITEM_5, NULL},
                                                            // {LIST_ITEM_6, NULL},
                                                        };

typedef struct
{
    SGUI_ITEMS_ITEM stItem;
    SGUI_BOOL       bUsed;
}SGUI_TEST_ITEM;

static SGUI_TEST_ITEM       s_arrstAppendListItems[] =  {   {{SCR1_LIST_APPEND1, NULL}, SGUI_FALSE},
                                                            {{SCR1_LIST_APPEND2, NULL}, SGUI_FALSE},
                                                            {{SCR1_LIST_APPEND3, NULL}, SGUI_FALSE},
                                                            {{SCR1_LIST_APPEND4, NULL}, SGUI_FALSE},
                                                            {{SCR1_LIST_APPEND5, NULL}, SGUI_FALSE},
                                                            {{SCR1_LIST_APPEND6, NULL}, SGUI_FALSE}
                                                        };

static SGUI_LIST        s_stDemoListObject =        {0x00};

//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_ACTION       s_stDemoListActions =           {   HMI_DemoList_Initialize,
                                                            HMI_DemoList_Prepare,
                                                            HMI_DemoList_RefreshScreen,
                                                            HMI_DemoList_ProcessEvent,
                                                            HMI_DemoList_PostProcess
                                                        };
HMI_SCREEN_OBJECT       g_stHMIDemo_List =              {   HMI_SCREEN_ID_DEMO_LIST,
                                                            &s_stDemoListActions
                                                        };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
HMI_ENGINE_RESULT HMI_DemoList_Initialize(SGUI_SCR_DEV* pstDeviceIF)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT                   stListLayout;
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Initialize list data.
    SGUI_SystemIF_MemorySet(&s_stDemoListObject, 0x00, sizeof(SGUI_LIST));
     //Initialize list object.
    stListLayout.iX = 0;
    stListLayout.iY = 0;
    stListLayout.iWidth = pstDeviceIF->stSize.iWidth;
    stListLayout.iHeight = pstDeviceIF->stSize.iHeight;
    SGUI_List_Initialize(&s_stDemoListObject, &stListLayout, &GB2312_Font12, SCREEN1_TITLE, s_arrstListItems, sizeof(s_arrstListItems)/sizeof(SGUI_ITEMS_ITEM));
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

HMI_ENGINE_RESULT HMI_DemoList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_UINT16                 uiKeyCode;
    SGUI_UINT16                 uiKeyValue;
    KEY_PRESS_EVENT*            pstKeyEvent;
    SGUI_INT                    iProcessAction;
    SGUI_SIZE                   sInsertDataIdx;
    SGUI_ITEMS_ITEM*            pstItemObj;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    pstKeyEvent =               (KEY_PRESS_EVENT*)pstEvent;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;

    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    // Check event is valid.
    if(SGUI_FALSE == HMI_EVENT_SIZE_CHK(*pstKeyEvent, KEY_PRESS_EVENT))
    {
        // Event data is invalid.
        eProcessResult = HMI_RET_INVALID_DATA;
    }
    else if(EVENT_ID_KEY_PRESS == pstEvent->iID)
    {
        uiKeyCode = pstKeyEvent->Data.uiKeyValue;
        uiKeyValue = KEY_CODE_VALUE(uiKeyCode);
        switch(uiKeyValue)
        {
            case KEY_VALUE_ENTER:
            {
                iProcessAction = HMI_DEMO_PROC_CONFIRM;
                break;
            }
            case KEY_VALUE_ESC:
            {
                iProcessAction = HMI_DEMO_PROC_CANCEL;
                break;
            }
            case KEY_VALUE_UP:
            {
                if(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex > 0)
                {
                    SGUI_List_Selecte(&s_stDemoListObject, SGUI_List_GetSelection(&s_stDemoListObject)->iIndex-1);
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_DOWN:
            {
                if(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex < (s_stDemoListObject.stItems.iCount-1))
                {
                    SGUI_List_Selecte(&s_stDemoListObject, SGUI_List_GetSelection(&s_stDemoListObject)->iIndex+1);
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_PLUS_PAD:
            {
                // Erase old paint.
                SGUI_Basic_DrawRectangle(pstDeviceIF, s_arrstLayouts[s_iLayoutIndex].iX, s_arrstLayouts[s_iLayoutIndex].iY, s_arrstLayouts[s_iLayoutIndex].iWidth, s_arrstLayouts[s_iLayoutIndex].iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
                // Select new layout.
                s_iLayoutIndex++;
                s_iLayoutIndex = s_iLayoutIndex%(sizeof(s_arrstLayouts)/sizeof(SGUI_RECT));
                // Resize and repaint.
                SGUI_List_Resize(&s_stDemoListObject, &(s_arrstLayouts[s_iLayoutIndex]));
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_SUB_PAD:
            {
                // Erase old paint.
                SGUI_Basic_DrawRectangle(pstDeviceIF, s_arrstLayouts[s_iLayoutIndex].iX, s_arrstLayouts[s_iLayoutIndex].iY, s_arrstLayouts[s_iLayoutIndex].iWidth, s_arrstLayouts[s_iLayoutIndex].iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
                // Select new layout.
                s_iLayoutIndex--;
                s_iLayoutIndex = s_iLayoutIndex%(sizeof(s_arrstLayouts)/sizeof(SGUI_RECT));
                // Resize and repaint.
                SGUI_List_Resize(&s_stDemoListObject, &(s_arrstLayouts[s_iLayoutIndex]));
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_DEL:
            {
                sInsertDataIdx = 0;
                pstItemObj = SGUI_List_RemoveItem(&s_stDemoListObject, SGUI_List_GetSelection(&s_stDemoListObject)->iIndex);
                while(sInsertDataIdx < sizeof(s_arrstAppendListItems)/sizeof(SGUI_TEST_ITEM))
                {
                    if(pstItemObj == &(s_arrstAppendListItems[sInsertDataIdx].stItem))
                    {
                        s_arrstAppendListItems[sInsertDataIdx].bUsed = SGUI_FALSE;
                        break;
                    }
                    else
                    {
                        sInsertDataIdx++;
                    }
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_INSERT:
            {
                sInsertDataIdx = 0;
                while(sInsertDataIdx < sizeof(s_arrstAppendListItems)/sizeof(SGUI_TEST_ITEM))
                {
                    if(SGUI_FALSE == s_arrstAppendListItems[sInsertDataIdx].bUsed)
                    {
                        s_arrstAppendListItems[sInsertDataIdx].bUsed = SGUI_List_InsertItem(&s_stDemoListObject, &(s_arrstAppendListItems[sInsertDataIdx].stItem),
                                                                                             (SGUI_List_Count(&s_stDemoListObject)>0)?(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex):0)?SGUI_TRUE:SGUI_FALSE;
                        break;
                    }
                    else
                    {
                        sInsertDataIdx++;
                    }
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_HOME:
            {
                sInsertDataIdx = 0;
                while(sInsertDataIdx < sizeof(s_arrstAppendListItems)/sizeof(SGUI_TEST_ITEM))
                {
                    if(SGUI_FALSE == s_arrstAppendListItems[sInsertDataIdx].bUsed)
                    {
                        s_arrstAppendListItems[sInsertDataIdx].bUsed = (SGUI_List_InsertItem(&s_stDemoListObject, &(s_arrstAppendListItems[sInsertDataIdx].stItem), 0)?SGUI_TRUE:SGUI_FALSE);
                        break;
                    }
                    else
                    {
                        sInsertDataIdx++;
                    }
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            case KEY_VALUE_END:
            {
                sInsertDataIdx = 0;
                while(sInsertDataIdx < sizeof(s_arrstAppendListItems)/sizeof(SGUI_TEST_ITEM))
                {
                    if(SGUI_FALSE == s_arrstAppendListItems[sInsertDataIdx].bUsed)
                    {
                        s_arrstAppendListItems[sInsertDataIdx].bUsed = (SGUI_List_InsertItem(&s_stDemoListObject, &(s_arrstAppendListItems[sInsertDataIdx].stItem), SGUI_List_Count(&s_stDemoListObject))?SGUI_TRUE:SGUI_FALSE);
                        break;
                    }
                    else
                    {
                        sInsertDataIdx++;
                    }
                }
                SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
                break;
            }
            default:
            {
                break;
            }
        }
    }
    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

HMI_ENGINE_RESULT HMI_DemoList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_DEMO_PROC_CONFIRM == iActionID)
    {
        switch(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex)
        {
            case 0:
                // HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_BASIC_PAINT, NULL);
                HMI_SwitchScreen(HMI_SCREEN_ID_CARD_REC_PAGE, NULL);  //打开设备状态界面
                break;
            case 1:
                HMI_SwitchScreen(HMI_SCREEN_ID_DEV_STATE_PAGE, NULL); 
                break;
            case 2:
                HMI_SwitchScreen(HMI_SCREEN_ID_SOFT_VER_PAGE, NULL); // 打开软件版本界面
                break;  
            case 3:
                HMI_SwitchScreen(HMI_SCREEN_ID_HAREWARE_PAGE, NULL);  // 打开硬件版本界面
                break;
            case 4:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_TYPE_PAGE, NULL); // 打开工位类型界面
                break;
            case 5:
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_POLOR_COORD, NULL);
                break;
            case 6:
                // HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_CURVE, NULL);
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_REAL_TIME_GRAPH, NULL);
                break;
            case 7:
                HMI_SwitchScreen(HMI_SCREEN_ID_DEMO_POLOR_COORD, NULL);
                break;
            case 8:
                HMI_SwitchScreen(HMI_SCREEN_ID_HAREWARE_PAGE, NULL);
                break;
            case 9:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_TYPE_PAGE, NULL);
                break;
            case 10:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_PAGE, NULL);
                break;
            default:
                /* do nothing. */
                break;
        }
    }
    else if(HMI_DEMO_PROC_CANCEL == iActionID)
    {
        HMI_GoBack(NULL);
    }

    return HMI_RET_NORMAL;
}



