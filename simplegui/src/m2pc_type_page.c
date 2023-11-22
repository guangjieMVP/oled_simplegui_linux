/*
 * @Author: xgj
 * @Date: 2023-07-10 15:06:51
 * @LastEditors: xgj
 * @LastEditTime: 2023-08-07 15:13:30
 * @FilePath: 
 * @Description: 
 *
 * Copyright (c) 2023 by Shenzhen Genvict Technologies Co., Ltd. All Rights Reserved.
 * @Change Log:
 */


//=======================================================================//
//= Include files.                                                      =//
//=======================================================================//
#include "DemoProc.h"
#include "SGUI_Text.h"
#include "SGUI_FontResource.h"
#include "Resource.h"

#if 0
#define M2_TYPE_UP_STR     "上工位"
#define M2_TYPE_DOWN_STR   "下工位"
#else
#define M2_TYPE_UP_STR     ("\x97\x74\x81\xE5\x81\x4D")  // 上工位
#define M2_TYPE_DOWN_STR   ("\x97\x75\x81\xE5\x81\x4D")  // 下工位
#endif
//=======================================================================//
//= Static function declaration.                                        =//
//=======================================================================//
static HMI_ENGINE_RESULT    HMI_TextPage_Prepare(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_TextPage_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters);
static HMI_ENGINE_RESULT    HMI_TextPage_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID);
static HMI_ENGINE_RESULT    HMI_TextPage_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID);

//=======================================================================//
//= Static variable declaration.                                        =//
//=======================================================================//
static HMI_SCREEN_ACTION           s_stDemoTextActions =   {   NULL,
                                                            HMI_TextPage_Prepare,
                                                            HMI_TextPage_RefreshScreen,
                                                            HMI_TextPage_ProcessEvent,
                                                            HMI_TextPage_PostProcess
                                                        };

//=======================================================================//
//= Global variable declaration.                                        =//
//=======================================================================//
HMI_SCREEN_OBJECT       g_stHMI_M2_Type_Page =  {   HMI_SCREEN_ID_M2PC_TYPE_PAGE,
                                                        &s_stDemoTextActions
                                                    };

//=======================================================================//
//= Function define.                                                    =//
//=======================================================================//

static HMI_ENGINE_RESULT HMI_TextPage_Prepare (SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(NULL != pstDeviceIF->fnClear)
    {
        pstDeviceIF->fnClear();
    }
    else
    {
        SGUI_Basic_DrawRectangle(pstDeviceIF, 0, 0, pstDeviceIF->stSize.iWidth, pstDeviceIF->stSize.iHeight, SGUI_COLOR_BKGCLR, SGUI_COLOR_BKGCLR);
    }
    HMI_TextPage_RefreshScreen(pstDeviceIF, pstParameters);
    return HMI_RET_NORMAL;
}

static HMI_ENGINE_RESULT HMI_TextPage_RefreshScreen(SGUI_SCR_DEV* pstDeviceIF, const void* pstParameters)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    SGUI_RECT                   stDisplayArea;
    SGUI_POINT                  stInnerPos;

    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    stInnerPos.iX =         0;
    stInnerPos.iY =         0;

    stDisplayArea.iX = pstDeviceIF->stSize.iWidth/2-20;
    stDisplayArea.iY = pstDeviceIF->stSize.iHeight/2-10;
    stDisplayArea.iWidth = pstDeviceIF->stSize.iWidth;
    stDisplayArea.iHeight = pstDeviceIF->stSize.iHeight;

    // if (__panel_is_hw_fixed())
    // {
    //     SGUI_Text_DrawMultipleLinesText(pstDeviceIF, M2_TYPE_UP_STR, &GB2312_Font12, &stDisplayArea, 0, SGUI_DRAW_NORMAL);
    // }
    // else
    // {
    //     SGUI_Text_DrawMultipleLinesText(pstDeviceIF, M2_TYPE_DOWN_STR, &GB2312_Font12, &stDisplayArea, 0, SGUI_DRAW_NORMAL);
    // }
    
    return HMI_RET_NORMAL;
}

static HMI_ENGINE_RESULT HMI_TextPage_ProcessEvent(SGUI_SCR_DEV* pstDeviceIF, const HMI_EVENT_BASE* pstEvent, SGUI_INT* piActionID)
{
    /*----------------------------------*/
    /* Variable Declaration             */
    /*----------------------------------*/
    HMI_ENGINE_RESULT           eProcessResult;
    SGUI_INT                    iProcessAction;
    KEY_PRESS_EVENT*            pstKeyEvent;
    /*----------------------------------*/
    /* Initialize                       */
    /*----------------------------------*/
    eProcessResult =            HMI_RET_NORMAL;
    iProcessAction =            HMI_DEMO_PROC_NO_ACT;
    pstKeyEvent =               (KEY_PRESS_EVENT*)pstEvent;

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
        SGUI_UINT16  uiKeyValue;
        uiKeyValue = KEY_CODE_VALUE(pstKeyEvent->Data.uiKeyValue);
        switch(uiKeyValue)
        {
            case KEY_VALUE_F5:

                break;
            case KEY_VALUE_ESC:
                iProcessAction = HMI_DEMO_PROC_CANCEL;
                break;
        }
    }
    if(NULL != piActionID)
    {
        *piActionID = iProcessAction;
    }

    return eProcessResult;
}

static HMI_ENGINE_RESULT HMI_TextPage_PostProcess(SGUI_SCR_DEV* pstDeviceIF, HMI_ENGINE_RESULT eProcResult, SGUI_INT iActionID)
{
    /*----------------------------------*/
    /* Process                          */
    /*----------------------------------*/
    if(HMI_PROCESS_SUCCESSFUL(eProcResult))
    {
        if(HMI_DEMO_PROC_CANCEL == iActionID)
        {
            HMI_GoBack(NULL);
        }
    }

    return HMI_RET_NORMAL;
}

