#include <windows.h>
#include <windowsx.h>
#include <stdbool.h>
// all file of the project 
#include "WndClr.h"
#include "Login.h"
#include "authentification.h"
#include "usersdata.h"
#include "Account.h"
#include "hoveringanimation.h"
#include "checkmessagerectangle.h"
#include "message.h"
#include "panelanimation.h"
#include "Client.h"

// boundaries of the window
#define MIN_WIDTH 840
#define MIN_HEIGHT 520

// the window is variable
HWND HandleWnd;
RECT WindowSize;
int WindowLeft, WindowTop, WindowWidth, WindowHeight;
HDC DeviceContext;
HDC Mdc;
PAINTSTRUCT PaintWnd;
HBITMAP BitMap, OldBitMap;
// for the child window 1
PAINTSTRUCT PaintChild_1;
int WindowLeft_Child_1, WindowTop_Child_1, WindowWidth_Child_1, WindowHeight_Child_1;
HDC DeviceContext_Child_1;
HDC Mdc_Child_1;
HBITMAP BitMap_Child_1, OldBitMap_Child_1;
//button log in
HWND ButtonHandle;
#define ButtonID 1000
HINSTANCE IDhInstance;
bool ShowButton = TRUE;

// logo montech company
HBITMAP LogoHandle;
//char *Image="C:\\Users\\Documents\\logo.bmp";

// file is copy
FILE *UserData_2=0;
// Authentifaction
bool Green=FALSE;
 // for the account
 extern bool Account;
 //
 extern HICON CompanyLogo;
 extern HWND HandleLogo;
// for then click of the mouse
int x,y;
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
            case WM_SIZE:
            GetClientRect(hwnd, &WindowSize);
            WindowLeft = WindowSize.left;
            WindowTop = WindowSize.top;
            WindowWidth = WindowSize.right - WindowSize.left;
            WindowHeight = WindowSize.bottom - WindowSize.top;            
            InvalidateRect(hwnd, &WindowSize, FALSE);
            if(UiInbox)
            {
                MoveWindow(ScrollBar,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,
                ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.08,
                (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
                (WindowSize.bottom-WindowSize.top)*0.643,TRUE);
            }
            break;  
            case WM_CREATE:
            GetClientRect(hwnd, &WindowSize);
            ButtonHandle = CreateWindowEx( 0,"BUTTON","Log in",WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
            0, 0, 100, 32,hwnd,(HMENU)ButtonID,IDhInstance,NULL); 

            // thoose are for rendering the logo of the company in the first page
            int logoSize = min(WindowWidth, WindowHeight) * 0.2;
            CompanyLogo=(HICON)LoadImage(0,"CompanyLogo.ico",IMAGE_ICON,logoSize,logoSize,LR_LOADFROMFILE);   
            HandleLogo=CreateWindowEx(0,"STATIC",0,WS_CHILD | WS_VISIBLE | SS_ICON,
            (WindowSize.left+(WindowSize.right-WindowSize.left)/2-(WindowSize.right-WindowSize.left)*0.149),
            (WindowSize.top+(WindowSize.bottom-WindowSize.top)/2-(WindowSize.bottom-WindowSize.top)*0.2),
            (WindowSize.right-WindowSize.left)*0.3,(WindowSize.bottom-WindowSize.top)*0.45,
            hwnd,0,0,NULL);
            SendMessage(HandleLogo,STM_SETICON,(WPARAM)CompanyLogo,0); 

            // company big logo in the account page
            int BiglogoSize = min(WindowWidth, WindowHeight) * 0.5;
            CompanyBigLogo=(HICON)LoadImage(0,"CompanyLogo.ico",IMAGE_ICON,
            (WindowSize.right-WindowSize.left)*0.6,(WindowSize.bottom-WindowSize.top)*0.82,LR_LOADFROMFILE);   
            HandleBigLogo=CreateWindowEx(0,"STATIC",0,WS_CHILD | WS_VISIBLE | SS_ICON,
            (WindowSize.left+(WindowSize.right-WindowSize.left)/2-(WindowSize.right-WindowSize.left)*0.28),
            (WindowSize.top+(WindowSize.bottom-WindowSize.top)/2-(WindowSize.bottom-WindowSize.top)*0.32),
            (WindowSize.right-WindowSize.left)*0.6,(WindowSize.bottom-WindowSize.top)*0.82,
            hwnd,0,0,NULL);
            SendMessage(HandleBigLogo,STM_SETICON,(WPARAM)CompanyBigLogo,0); 
            ShowWindow(HandleBigLogo,SW_HIDE);
            // create the child window of scrollbar
            
            ScrollBar = CreateWindowEx(
            0,
            "CustomScrollChildWindow",
            "",
            WS_CHILD,
            ((WindowSize.right-WindowSize.left)*0.02f + MeasureWindowSize((WindowSize.right-WindowSize.left)*0.12f,MIN_BUTTON_WIDTH,MAX_BUTTON_WIDTH)) + (WindowSize.right-WindowSize.left)*0.03,
            (WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043 + (WindowSize.bottom - WindowSize.top)*0.04) + (WindowSize.bottom - WindowSize.top)*0.08,
            (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
            (WindowSize.bottom-WindowSize.top)*0.643,
            hwnd, NULL, IDhInstance, NULL
            );
            break;
            case WM_PAINT:
            GetClientRect(hwnd,&WindowSize);
            if(Mdc)
            {
                ReleaseDC(hwnd,Mdc);
            }
            WindowLeft = WindowSize.left;
            WindowTop = WindowSize.top;
            WindowWidth = WindowSize.right - WindowSize.left;
            WindowHeight = WindowSize.bottom - WindowSize.top;
            DeviceContext = BeginPaint(hwnd, &PaintWnd);
            Mdc = CreateCompatibleDC(DeviceContext);
            BitMap = CreateCompatibleBitmap(DeviceContext, WindowWidth, WindowHeight);
            OldBitMap = (HBITMAP)SelectObject(Mdc, BitMap);
            Creme=CreateSolidBrush(RGB(250,245,230));
            LoginInterface.left=WindowLeft;
            LoginInterface.top=WindowTop;
            LoginInterface.right=WindowLeft+WindowWidth;
            LoginInterface.bottom=WindowTop+WindowHeight;
            FillRect(Mdc,&LoginInterface,Creme);
            if(ShowTitle)
            {
                InterfaceLogin(WindowLeft, WindowTop, WindowWidth, WindowHeight, Mdc,IDhInstance,hwnd,WindowSize);
            }
            // make the position of the logo
            SetWindowPos(HandleLogo, NULL,
            (WindowSize.left+(WindowSize.right-WindowSize.left)/2-(WindowSize.right-WindowSize.left)*0.149),
            (WindowSize.top+(WindowSize.bottom-WindowSize.top)/2-(WindowSize.bottom-WindowSize.top)*0.2),
            (WindowSize.right-WindowSize.left)*0.3,(WindowSize.bottom-WindowSize.top)*0.45,
            SWP_NOZORDER);  
            if(ShowTitle)
            {
                ShowWindow(HandleLogo, SW_SHOW);
            }
            else
            {
                ShowWindow(HandleLogo, SW_HIDE);
            }
            int buttonWidth = 185;
            int buttonHeight = 55;
            int buttonX = (WindowWidth - buttonWidth) / 2;  
            int buttonY = WindowTop + (WindowHeight / 2) + 40;            
            if(CompanyLogo)
            {
                SetWindowPos(
                    ButtonHandle,
                    NULL,
                    buttonX,
                    buttonY+(WindowSize.bottom-WindowSize.top)*0.2,
                    buttonWidth,
                    buttonHeight,
                    SWP_NOZORDER
                );
                if(ShowButton)
                {
                    ShowWindow(ButtonHandle, SW_SHOW);
                }
                else
                {
                    ShowWindow(ButtonHandle, SW_HIDE);
                }
            }
            if(Login)
            {
                DrawLoginPage(IDhInstance,WindowLeft, WindowTop, WindowWidth, WindowHeight,hwnd,Mdc);
                //user name and Pass word movement
                MoveWindow(ULogin,WindowSize.left+(WindowSize.right/2)-100,WindowSize.top+(WindowSize.bottom/2)-75,205,34,TRUE);
                MoveWindow(PLogin,WindowSize.left+(WindowSize.right/2)-99,WindowSize.top+(WindowSize.bottom/2)+3,205,34,TRUE);
                CreateButton(IDhInstance,hwnd,Mdc,WindowSize);
                MoveWindow(LogInButton,WindowSize.left+(WindowSize.right/2)-80,WindowSize.top+(WindowSize.bottom/2)+55,170,45,TRUE);
            }
            if(LogInCtl)
            {
                ShowWindow(ULogin,SW_SHOW);
                ShowWindow(PLogin,SW_SHOW);
                ShowWindow(LogInButton,SW_SHOW);

            }
            else
            {
                ShowWindow(ULogin,SW_HIDE);
                ShowWindow(PLogin,SW_HIDE);
                ShowWindow(LogInButton,SW_HIDE);

            }
            if(Green)
            {
                Authentifaction(ULogin,PLogin,UserData_2,Creme,WindowSize,Mdc,hwnd);
            }
            Green=FALSE;
            baseRectangle(WindowSize,hwnd);
            if(Account)
            {
                //rendering the message button
                CreateMessageAccount(Mdc,CurrentHMessage,CurrentVMessage,WindowSize);
                //rendering the online button
                CreateOnlineAccount(Mdc,CurrentHOnline,CurrentVOnline,WindowSize);
                //rendering  the task button
                CreateTaskAccount(Mdc,CurrentHTask,CurrentVTask,WindowSize);
                // rendering  the project button
                CreateProjectAccount(Mdc,CurrentHProject,CurrentVProject,WindowSize);
                // rendering  the disconnect button
                CreateDisconnectAccount(Mdc,CurrentHDisconnect,CurrentVDisconnect,WindowSize);
                // rendering the panel 
                CreatePanel(Mdc,WindowSize,hwnd,AddLenght);
                // the line 
                LineDifference(Mdc,hwnd,WindowSize);
                // three point of menu on the panel 
                Points(Mdc,hwnd,WindowSize);

                // rendering the big logo of the company
                if(CompanyBigLogo)
                { 
                    SetWindowPos(HandleBigLogo, NULL,
                    (WindowSize.left+(WindowSize.right-WindowSize.left)/2-(WindowSize.right-WindowSize.left)*0.28),
                    (WindowSize.top+(WindowSize.bottom-WindowSize.top)/2-(WindowSize.bottom-WindowSize.top)*0.32),
                    (WindowSize.right-WindowSize.left)*0.6,(WindowSize.bottom-WindowSize.top)*0.82,
                    SWP_NOZORDER);  
                    if(Account && !UiMessage && !UiInbox && !UiGeneral)
                    {
                        ShowWindow(HandleBigLogo, SW_SHOW);
                    }
                    else
                    {
                        ShowWindow(HandleBigLogo, SW_HIDE);
                    }
    
                }
                // for message is click 
                if(UiMessage)
                {
                    CreateMessageUi(Mdc,hwnd,WindowSize,CurrentHInbox,CurrentVInbox,CurrentHGeneral,CurrentVGeneral);
                }
                // when the user click on the inbox or general button this created the ui
                if(UiInbox)
                {
                    CreateInboxUi(Mdc,hwnd,WindowSize,IDhInstance);
                    LineDifferenceMessage(Mdc,hwnd,WindowSize);
                    MoveWindow(HandleSearch,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.01,(WindowSize.right-WindowSize.left)*0.18,(WindowSize.bottom - WindowSize.top)*0.064,TRUE);
                    
                    DrawMessageBubbleLogoLeft(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.14,
                    (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1,(WindowSize.right-WindowSize.left)*0.125,
                    (WindowSize.bottom - WindowSize.top)*0.175, 3,WindowSize);
                    DrawMessageBubbleLogoRight(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.1 + (WindowSize.right-WindowSize.left)*0.148,
                    (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1 + (WindowSize.bottom - WindowSize.top)*0.14,(WindowSize.right-WindowSize.left)*0.1,
                    (WindowSize.bottom - WindowSize.top)*0.145,3,WindowSize);
                    //for button start searching for recipient
                    CreateSearchButton(Mdc,CurrentHSearch,CurrentVSearch,WindowSize,ChatRect,2);
                }  
                /*else if(UiGeneral)
                {
                }*/
                if(UiInbox)
                {
                    ShowWindow(HandleSearch,SW_SHOW);
                    ShowWindow(ScrollBar, SW_SHOW);
                }
                else
                {
                    ShowWindow(HandleSearch,SW_HIDE);
                    ShowWindow(ScrollBar, SW_HIDE); 
                }
            }     
            BitBlt(DeviceContext, WindowLeft, WindowTop, WindowWidth, WindowHeight, Mdc, 0, 0, SRCCOPY);
            SelectObject(Mdc, OldBitMap);
            DeleteObject(BitMap);
            DeleteDC(Mdc);
            
            EndPaint(hwnd, &PaintWnd);
        break;
        case WM_COMMAND:
        if (LOWORD(wParam) == ButtonID)
        {
            CreateLoginControle(IDhInstance,hwnd,Mdc,WindowSize);
            ShowButton = FALSE;
            ShowTitle=FALSE;
            Login=TRUE;
            LogInCtl=TRUE;
        }
        if(LOWORD(wParam) == ID_LogIn)
        {
            Green=TRUE;
        }
        InvalidateRect(hwnd, &WindowSize, FALSE);
        break;
        case WM_TIMER :
        GetClientRect(hwnd, &WindowSize);
        AreaRedraw.left = WindowSize.left;
        AreaRedraw.top = WindowSize.top;
        AreaRedraw.right = WindowSize.left + (WindowSize.right - WindowSize.left)*0.15;
        AreaRedraw.bottom = WindowSize.bottom;
        switch(wParam)
        {
            case TimerLogIn :
            Login=FALSE;
            Green=FALSE;
            Account=TRUE;
            LogInCtl=FALSE;
            break;
            case MessageTimer :
            // this is updating the the button message every time
            UpdateMessageAnimation(HoveringMessage,hwnd);
            break;
            // this is updating the the button online every time
            case OnlineTimer :
            UpdateOnlineAnimation(HoveringOnline,hwnd);
            break;
            // this is updating the the button task every time
            case TaskTimer :
            UpdateTaskAnimation(HoveringTask,hwnd);
            break;
            // this is updating the the button project every time
            case ProjectTimer :
            UpdateProjectAnimation(HoveringProject,hwnd);
            break;
            // this is updating the the button disconnect every time
            case DisconnectTimer :
            UpdateDisconnectAnimation(HoveringDisconnect,hwnd);
            break;
            // this is updating the the button Inbox every time
            case InboxTimer :
            UpdateInboxAnimation(HoveringInbox,hwnd,WindowSize);
            break;
            // this is updating the the button general every time
            case GeneralTimer :
            UpdateGeneralAnimation(HoveringGeneral,hwnd,WindowSize);
            break;
            case TimerPanel :
            PanelAnimationUp(hwnd,WindowSize);
            break;
            case SearchTimer :
            UpdateSearchAnimation(HoveringSearch,hwnd,WindowSize);
            break;
        }
        InvalidateRect(hwnd,&AreaRedraw,FALSE);
        break;
        case WM_LBUTTONDOWN :
        x =GET_X_LPARAM(lParam);
        y =GET_Y_LPARAM(lParam);
        if((x>=Choice_1_Button.left && x<=Choice_1_Button.right) && (y>=Choice_1_Button.top && y<=Choice_1_Button.bottom ))
        {
            UiMessage=TRUE;   
            UiGeneral = TRUE;
            UiInbox = FALSE;
            MessageButtonClicked = TRUE;   
        }
        else if(MessageButtonClicked)
        {
            // this is for checking the inbox button 
            if((x>=Choice_1_Inbox_Button.left && x<=Choice_1_Inbox_Button.right) && (y>=Choice_1_Inbox_Button.top && y<=Choice_1_Inbox_Button.bottom ))
            {
                UiInbox = TRUE;
                UiMessage = FALSE;
                MessageButtonClicked = FALSE;
                UiGeneral = FALSE;
                SetTimer(hwnd,TimerPanel,30,NULL);
                // for the search of the recipient
                if(HandleSearch == NULL)
                {
                    GetClientRect(hwnd, &WindowSize); 
                    ChatRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.018;
                    ChatRect.top = WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043;
                    ChatRect.right = ChatRect.left + (WindowSize.right - WindowSize.left)*0.1;
                    ChatRect.bottom = ChatRect.top + (WindowSize.bottom - WindowSize.top)*0.04;
                    HandleSearch = CreateWindowEx(0,"EDIT","", 
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.01,(WindowSize.right-WindowSize.left)*0.18,(WindowSize.bottom - WindowSize.top)*0.064,
                    hwnd,0,IDhInstance, NULL);
                    ShowWindow(HandleSearch,SW_HIDE);
                }
            }
            else if((x>=Choice_1_General_Button.left && x<=Choice_1_General_Button.right) && (y>=Choice_1_General_Button.top && y<=Choice_1_General_Button.bottom ))
            {
                UiGeneral = TRUE;
                UiMessage = FALSE;
                UiInbox = FALSE;
                MessageButtonClicked = FALSE;
                SetTimer(hwnd,TimerPanel,30,NULL);
            }
        }
        InvalidateRect(hwnd,&WindowSize,FALSE);
        break;
        case WM_MOUSEMOVE :
        GetClientRect(hwnd,&WindowSize);
        Mx=LOWORD(lParam);
        My=HIWORD(lParam);

        // message hovering
        WasHoveringMessage=HoveringMessage;
        CheckMessage=CheckMessageRect(Choice_1_Button,hwnd,Mx,My);
        HoveringMessage=CheckMessage;
        if(HoveringMessage && !WasHoveringMessage)
        {
            // increase the button is size
            SetTimer(hwnd,MessageTimer,4,NULL);
        }
        else if(!HoveringMessage && WasHoveringMessage)
        {
            // decrease the button is size
            SetTimer(hwnd,MessageTimer,4,NULL); 
        }


        // online hovering 
        WasHoveringOnline=HoveringOnline;
        CheckOnline=CheckOnlineRect(Choice_2_Button,hwnd,Mx,My);
        HoveringOnline=CheckOnline;
        if(HoveringOnline && !WasHoveringOnline)
        {
            // increase the button is size
            SetTimer(hwnd,OnlineTimer,4,NULL);
        }
        else if(!HoveringOnline && WasHoveringOnline)
        {
            // decrease the button is size
            SetTimer(hwnd,OnlineTimer,4,NULL); 
        }


        // Task hovering 
        WasHoveringTask=HoveringTask;
        CheckTask=CheckTaskRect(Choice_3_Button,hwnd,Mx,My);
        HoveringTask=CheckTask;
        if(HoveringTask && !WasHoveringTask)
        {
            // increase the button is size
            SetTimer(hwnd,TaskTimer,4,NULL);
        }
        else if(!HoveringTask && WasHoveringTask)
        {
            // decrease the button is size
            SetTimer(hwnd,TaskTimer,4,NULL); 
        }


        // Project hovering 
        WasHoveringProject=HoveringProject;
        CheckProject=CheckProjectRect(Choice_4_Button,hwnd,Mx,My);
        HoveringProject=CheckProject;
        if(HoveringProject && !WasHoveringProject)
        {
            // increase the button is size
            SetTimer(hwnd,ProjectTimer,4,NULL);
        }
        else if(!HoveringProject && WasHoveringProject)
        {
            // decrease the button is size
            SetTimer(hwnd,ProjectTimer,4,NULL); 
        }


        // Disconnect hovering 
        WasHoveringDisconnect=HoveringDisconnect;
        CheckDisconnect=CheckDisconnectRect(Choice_5_Button,hwnd,Mx,My);
        HoveringDisconnect=CheckDisconnect;
        if(HoveringDisconnect && !WasHoveringDisconnect)
        {
            // increase the button is size
            SetTimer(hwnd,DisconnectTimer,4,NULL);
        }
        else if(!HoveringDisconnect && WasHoveringDisconnect)
        {
            // decrease the button is size
            SetTimer(hwnd,DisconnectTimer,4,NULL); 
        }


        // Inbox hovering 
        WasHoveringInbox=HoveringInbox;
        CheckInbox=CheckInboxRect(Choice_1_Inbox_Button,hwnd,Mx,My);
        HoveringInbox=CheckInbox;
        if(HoveringInbox && !WasHoveringInbox)
        {
            // increase the button is size
            SetTimer(hwnd,InboxTimer,4,NULL);
        }
        else if(!HoveringInbox && WasHoveringInbox)
        {
            // decrease the button is size
            SetTimer(hwnd,InboxTimer,4,NULL); 
        }


        // general hovering 
        WasHoveringGeneral=HoveringGeneral;
        CheckGeneral=CheckGeneralRect(Choice_1_General_Button,hwnd,Mx,My);
        HoveringGeneral=CheckGeneral;
        if(HoveringGeneral && !WasHoveringGeneral)
        {
            // increase the button is size
            SetTimer(hwnd,GeneralTimer,4,NULL);
        }
        else if(!HoveringGeneral && WasHoveringGeneral)
        {
            // decrease the button is size
            SetTimer(hwnd,GeneralTimer,4,NULL); 
        }


        // search hovering 
        WasHoveringSearch=HoveringSearch;
        CheckSearch=CheckSearchRect(SearchAnimation,hwnd,Mx,My);
        HoveringSearch=CheckSearch;
        if(HoveringSearch && !WasHoveringSearch)
        {
            // increase the button is size
            SetTimer(hwnd,SearchTimer,4,NULL);
        }
        else if(!HoveringSearch && WasHoveringSearch)
        {
            // decrease the button is size
            SetTimer(hwnd,SearchTimer,4,NULL); 
        }
        break;
        case WM_DESTROY:
        if(HandleSearch != 0)
        {
        DestroyWindow(HandleSearch);
        HandleSearch = 0;
        }    
        // Clean up any other resources (icons, etc.)
        if(CompanyLogo) {
            DestroyIcon(CompanyLogo);
        }
        if(CompanyBigLogo) {
            DestroyIcon(CompanyBigLogo);
        }
        PostQuitMessage(0);
        break;
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* pMinMax = (MINMAXINFO*)lParam;
            pMinMax->ptMinTrackSize.x = MIN_WIDTH;
            pMinMax->ptMinTrackSize.y = MIN_HEIGHT;
            return 0;
        }
        default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
LRESULT CALLBACK ChildWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
        case WM_CREATE:
            g_scrollbar.min_val = 0;
            g_scrollbar.max_val = 100;
            g_scrollbar.current_val = 0;
            g_scrollbar.page_size = 10;
            break;
            
        case WM_PAINT: {
            PAINTSTRUCT ps;
            DeviceContext_Child_1 = BeginPaint(hwnd, &ps);
            
            GetClientRect(hwnd, &ScrollBarRect);
            
            // Create compatible DC for double buffering
            Mdc_Child_1 = CreateCompatibleDC(DeviceContext_Child_1);
            HBITMAP memBitmap = CreateCompatibleBitmap(DeviceContext_Child_1, 
            ScrollBarRect.right - ScrollBarRect.left, ScrollBarRect.bottom - ScrollBarRect.top);
            HBITMAP oldBitmap = SelectObject(Mdc_Child_1, memBitmap);
            
            HBRUSH Creme = CreateSolidBrush(RGB(250,245,230));
            FillRect(Mdc_Child_1, &ScrollBarRect, Creme);
            DeleteObject(Creme);

            HPEN borderPen = CreatePen(PS_SOLID, 1, RGB(0, 0, 0));
            HPEN oldPen = SelectObject(Mdc_Child_1, borderPen);
            HBRUSH oldBrush = SelectObject(Mdc_Child_1, GetStockObject(NULL_BRUSH));
            //Rectangle(Mdc_Child_1, 0, 0, ScrollBarRect.right, ScrollBarRect.bottom);
            DrawScrollBar(Mdc_Child_1, hwnd);
            SetBkMode(Mdc_Child_1, TRANSPARENT);
            
            BitBlt(DeviceContext_Child_1, 0, 0, ScrollBarRect.right - ScrollBarRect.left, 
                   ScrollBarRect.bottom - ScrollBarRect.top, Mdc_Child_1, 0, 0, SRCCOPY);
            
            SelectObject(Mdc_Child_1, oldBrush);
            SelectObject(Mdc_Child_1, oldPen);
            SelectObject(Mdc_Child_1, oldBitmap);
            DeleteObject(borderPen);
            DeleteObject(memBitmap);
            DeleteDC(Mdc_Child_1);
            
            EndPaint(hwnd, &ps);
            break;
        }
        
        case WM_SIZE: {
            // Handle child window resizing
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        
        case WM_MOUSEMOVE: {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            BOOL was_hovering = g_scrollbar.thumb_hover;
            
            CalculateThumbRect(hwnd, &g_scrollbar.thumb_rect);
            g_scrollbar.thumb_hover = PointInRect(pt, &g_scrollbar.thumb_rect);
            
            if (was_hovering != g_scrollbar.thumb_hover) {
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            
            if (g_scrollbar.is_dragging) {
                RECT ScrollBarRect;
                GetClientRect(hwnd, &ScrollBarRect);
                int track_height = ScrollBarRect.bottom - 40;
                int range = g_scrollbar.max_val - g_scrollbar.min_val;
                
                if (range > 0) {
                    int new_pos = ((pt.y - 10 - g_scrollbar.drag_offset) * range) / track_height;
                    UpdateScrollValue(hwnd, new_pos);
                }
            }
            break;
        }
        
        case WM_LBUTTONDOWN:
        {
            RECT client_rect;
            GetClientRect(hwnd, &client_rect);
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            RECT track_rect = {
            client_rect.right - SCROLLBAR_WIDTH - 2,
            0,
            client_rect.right - 2,
            client_rect.bottom};
            
            CalculateThumbRect(hwnd, &g_scrollbar.thumb_rect);
            if(PointInRect(pt, &g_scrollbar.thumb_rect)) {
                g_scrollbar.is_dragging = TRUE;
                g_scrollbar.thumb_pressed = TRUE;
                g_scrollbar.drag_offset = pt.y - g_scrollbar.thumb_rect.top;
                SetCapture(hwnd);
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            else if((pt.x >=client_rect.left && pt.x <=client_rect.right))
            {
                int track_height = ScrollBarRect.bottom - 40;
                int range = g_scrollbar.max_val - g_scrollbar.min_val;
                int new_pos = ((pt.y - 10 - g_scrollbar.drag_offset) * range) / track_height;
                UpdateScrollValue(hwnd, new_pos);
                InvalidateRect(hwnd, &ScrollBarRect, FALSE);
            }
            break;
        }
        
        case WM_LBUTTONUP: {
            if (g_scrollbar.is_dragging) {
                g_scrollbar.is_dragging = FALSE;
                g_scrollbar.thumb_pressed = FALSE;
                ReleaseCapture();
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            break;
        }
        
        case WM_MOUSEWHEEL: {
            int delta = GET_WHEEL_DELTA_WPARAM(wParam);
            int step = (delta > 0) ? -5 : 5;
            UpdateScrollValue(hwnd, g_scrollbar.current_val + step);
            break;
        }
        
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    // data of the whole workers in the company 
    FILE *UserData=0;
    UserData_2=UserData;
    FillingData(&UserData);
    IDhInstance = hInstance;
    WNDCLASS wndClass = {0};
    wndClass.style = CS_OWNDC | CS_HREDRAW | CS_VREDRAW;
    wndClass.lpfnWndProc = WindowProc;
    wndClass.hInstance = hInstance;
    wndClass.lpszClassName = "MyWnd";
    wndClass.hCursor=LoadCursor(NULL, IDC_ARROW);
    wndClass.hIcon = CompanyLogo;
    if (!RegisterClass(&wndClass)) {
        MessageBox(NULL, "Window Registration Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    const char* CHILD_CLASS_NAME = "CustomScrollChildWindow";
    WNDCLASS child_wc = {0};
    child_wc.lpfnWndProc = ChildWindowProc; 
    child_wc.hInstance = hInstance;
    child_wc.lpszClassName = CHILD_CLASS_NAME;
    child_wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    child_wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&child_wc);
    
    HandleWnd = CreateWindowEx(
        0,
        wndClass.lpszClassName,
        "WorkSpace",
        WS_OVERLAPPEDWINDOW | WS_CLIPCHILDREN,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );
    
    if (!HandleWnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    // this is for child window
    
    ShowWindow(HandleWnd, nCmdShow);
    UpdateWindow(HandleWnd);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }   
    return (int)msg.wParam;
}