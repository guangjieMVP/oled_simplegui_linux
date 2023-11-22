/*
 * @Author: xgj
 * @Date: 2023-07-10 16:27:09
 * @LastEditors: xgj
 * @LastEditTime: 2023-07-10 19:37:26
 * @FilePath: \simplegui\DemoProc\src\m2pc_para_page.c
 * @Description: 工位版参数显示列表界面
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

//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_PageList_Initialize(SGUI_SCR_DEV* pstDeviceIF);
static HMI_ENGINE_RESULT    HMI_PageList_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_PageList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_PageList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_PageList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
static const SGUI_RECT      s_arrstLayouts[] =          { {0, 0, 128, 64},
                                                            {0, 0, 192, 96},
                                                            {0, 0, 192, 128},
                                                            };
static SGUI_SIZE            s_iLayoutIndex = 1;

#if HMI_ENABLE_ZH == 0
static SGUI_ITEMS_ITEM      s_arrstListItems[] =        {   {"CTD_OPEN_SEEP", NULL},  // 0
                                                            {"CTD_CLOSE_SEEP", NULL},  // 1
                                                            {"INV_PRNT_SEEP", NULL},  // 2
                                                            {"INV_TO_SEEP", NULL},  // 3
                                                            {"INV_REC_SEEP", NULL},  // 4
                                                            {"INV_REREC_SEEP", NULL},  // 5
                                                            {"INV_PRNT_TMOT", NULL},  // 6
                                                            {"INV_TO_TMOT", NULL},  // 7
                                                            {"INV_AR_TIME", NULL},  // 8
                                                            {"INV_COIL_TIME", NULL},        // 9
                                                            {"INV_REC_TMOT", NULL},        // 11
                                                            {"INV_POST_REC_TIME 10000", NULL},        // 12
                                                            {"CARD_TA_TIME ", NULL},        // 13
                                                            {"CARD_IN_ANT_TIME", NULL},        // 14
                                                        };
#else
static SGUI_ITEMS_ITEM      s_arrstListItems[] =        {   {"投卡门打开速度", NULL},  // 0
                                                            {"投卡门关闭速度", NULL},  // 1
                                                            {"票据打印速度", NULL},  // 2
                                                            {"INV_TO_SEEP", NULL},  // 3
                                                            {"INV_REC_SEEP", NULL},  // 4
                                                            {"INV_REREC_SEEP", NULL},  // 5
                                                            {"INV_PRNT_TMOT", NULL},  // 6
                                                            {"INV_TO_TMOT", NULL},  // 7
                                                            {"INV_AR_TIME", NULL},  // 8
                                                            {"INV_COIL_TIME", NULL},        // 9
                                                            {"INV_REC_TMOT", NULL},        // 11
                                                            {"INV_POST_REC_TIME 10000", NULL},        // 12
                                                            {"CARD_TA_TIME ", NULL},        // 13
                                                            {"CARD_IN_ANT_TIME", NULL},        // 14
                                                        };
#endif 

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
static HMI_SCREEN_ACTION       s_stDemoListActions =    {   HMI_PageList_Initialize,
                                                            HMI_PageList_Prepare,
                                                            HMI_PageList_RefreshScreen,
                                                            HMI_PageList_ProcessEvent,
                                                            HMI_PageList_PostProcess
                                                        };

HMI_SCREEN_OBJECT       g_stHMIM2PC_Para_List =     {   HMI_SCREEN_ID_M2PC_PARA_PAGE,
                                                            &s_stDemoListActions
                                                        };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//
static HMI_ENGINE_RESULT HMI_PageList_Initialize(SGUI_SCR_DEV* pstDeviceIF)
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
#if HMI_ENABLE_ZH == 0
    SGUI_List_Initialize(&s_stDemoListObject, &stListLayout, &SGUI_DEFAULT_FONT_8, "参数", s_arrstListItems, sizeof(s_arrstListItems)/sizeof(SGUI_ITEMS_ITEM));
#else
    SGUI_List_Initialize(&s_stDemoListObject, &stListLayout, &GB2312_FZXS12, "参数", s_arrstListItems, sizeof(s_arrstListItems)/sizeof(SGUI_ITEMS_ITEM));
#endif
    return HMI_RET_NORMAL;
}

static HMI_ENGINE_RESULT HMI_PageList_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

static HMI_ENGINE_RESULT HMI_PageList_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    SGUI_List_Repaint(pstDeviceIF, &s_stDemoListObject);
    return HMI_RET_NORMAL;
}

static HMI_ENGINE_RESULT HMI_PageList_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
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

HMI_ENGINE_RESULT HMI_PageList_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    if(HMI_DEMO_PROC_CONFIRM == iActionID)
    {
        switch(SGUI_List_GetSelection(&s_stDemoListObject)->iIndex)
        {
            case 0:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "AAA");
                break;
            case 1:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "BBB");
                break;
            case 2:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "CCC");
                break;
            case 3:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "DDD");
                break;
            case 4:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "EEE");
                break;
            case 5:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "FFF");
                break;
            case 6:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "GGG");
                break;
            case 7:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "HHH");
                break;
            case 8:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "JJJ");
                break;
            case 9:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "KKK");
                break;
            case 10:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "LLL");
                break;
            case 11:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "MMM");
                break;
            case 12:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "NNN");
                break;
            case 13:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "OOO");
                break;
            case 14:
                HMI_SwitchScreen(HMI_SCREEN_ID_M2PC_PARA_SHOW_PAGE, "PPP");
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



