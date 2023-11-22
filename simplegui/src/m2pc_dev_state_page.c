/*
 * @Author: xgj
 * @Date: 2023-07-10 15:06:51
 * @LastEditors: xgj
 * @LastEditTime: 2023-08-07 11:23:28
 * @FilePath:
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

// #include "M2UdpApp\lcd_app.h"

#define DEV_STATE_OK_STR   ("\x97\x7A\x97\x7B")    // Õý³£
#define DEV_STATE_BAD_STR  ("\x97\x7C\x97\x7B")    // Òì³£

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
HMI_SCREEN_OBJECT       g_stHMI_DevState_Page =  {   HMI_SCREEN_ID_DEV_STATE_PAGE,
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

    stDisplayArea.iWidth = 100;
    stDisplayArea.iHeight = 20;
    stDisplayArea.iX = stDisplayArea.iWidth/2;
    stDisplayArea.iY = 10;

    char buf[100] = {0};

    // if (g_lcd_app_ctx.is_normal)
    // {
    //     sprintf(buf, "%s", DEV_STATE_OK_STR);
    // }
    // else
    // {
    //     sprintf(buf, "%s", DEV_STATE_BAD_STR);
    // }
    
    SGUI_Text_DrawMultipleLinesText(pstDeviceIF, buf, &GB2312_Font12, &stDisplayArea, 0, SGUI_DRAW_NORMAL);

    stDisplayArea.iWidth = pstDeviceIF->stSize.iWidth;
    stDisplayArea.iHeight = 20;
    stDisplayArea.iX = 20;
    stDisplayArea.iY += 30;

    // if (g_lcd_app_ctx.is_normal)
    // {
    //     sprintf(buf, "0x%x  0x%x", 0xFFFFFFFE, 0x00);
    // }
    // else
    // {
    //     sprintf(buf, "0x%x  0x%x", g_lcd_app_ctx.err_code_head, g_lcd_app_ctx.err_code);
    // }
    
    SGUI_Text_DrawMultipleLinesText(pstDeviceIF, buf, &GB2312_FZXS12, &stDisplayArea, 0, SGUI_DRAW_NORMAL);
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

        switch (uiKeyValue)
        {
            case KEY_VALUE_F5:
                HMI_TextPage_RefreshScreen(pstDeviceIF, NULL);
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

