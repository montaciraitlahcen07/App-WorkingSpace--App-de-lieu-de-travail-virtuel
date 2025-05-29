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
            GetClientRect(HandleWnd, &WindowSize);
            WindowLeft = WindowSize.left;
            WindowTop = WindowSize.top;
            WindowWidth = WindowSize.right - WindowSize.left;
            WindowHeight = WindowSize.bottom - WindowSize.top;            
            SetScrollInfo(ScrollBar, SB_CTL, &SCRL, TRUE);
            InvalidateRect(HandleWnd, &WindowSize, FALSE);
            break;  
            case WM_CREATE:
            GetClientRect(HandleWnd, &WindowSize);
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
            
            break;
            case WM_PAINT:
            GetClientRect(HandleWnd, &WindowSize);
            if(Mdc)
            {
                ReleaseDC(HandleWnd,Mdc);
            }
            GetClientRect(HandleWnd, &WindowSize);
            WindowLeft = WindowSize.left;
            WindowTop = WindowSize.top;
            WindowWidth = WindowSize.right - WindowSize.left;
            WindowHeight = WindowSize.bottom - WindowSize.top;
            DeviceContext = BeginPaint(HandleWnd, &PaintWnd);
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
                InterfaceLogin(WindowLeft, WindowTop, WindowWidth, WindowHeight, Mdc,IDhInstance,HandleWnd,WindowSize);
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
                DrawLoginPage(IDhInstance,WindowLeft, WindowTop, WindowWidth, WindowHeight,HandleWnd,Mdc);
                //user name and Pass word movement
                MoveWindow(ULogin,WindowSize.left+(WindowSize.right/2)-100,WindowSize.top+(WindowSize.bottom/2)-75,205,34,TRUE);
                MoveWindow(PLogin,WindowSize.left+(WindowSize.right/2)-99,WindowSize.top+(WindowSize.bottom/2)+3,205,34,TRUE);
                CreateButton(IDhInstance,HandleWnd,Mdc,WindowSize);
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
                Authentifaction(ULogin,PLogin,UserData_2,Creme,WindowSize,Mdc,HandleWnd);
            }
            Green=FALSE;
            baseRectangle(WindowSize,HandleWnd);
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
                CreatePanel(Mdc,WindowSize,HandleWnd,AddLenght);
                // the line 
                LineDifference(Mdc,HandleWnd,WindowSize);
                // three point of menu on the panel 
                Points(Mdc,HandleWnd,WindowSize);

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
                    CreateMessageUi(Mdc,HandleWnd,WindowSize,CurrentHInbox,CurrentVInbox,CurrentHGeneral,CurrentVGeneral);
                }
                // when the user click on the inbox or general button this created the ui
                if(UiInbox)
                {
                    CreateInboxUi(Mdc,HandleWnd,WindowSize,IDhInstance);
                    LineDifferenceMessage(Mdc,HandleWnd,WindowSize);
                    MoveWindow(HandleSearch,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.01,(WindowSize.right-WindowSize.left)*0.18,(WindowSize.bottom - WindowSize.top)*0.064,TRUE);
                    
                    DrawMessageBubbleLogoLeft(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.14,
                    (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1,(WindowSize.right-WindowSize.left)*0.125,
                    (WindowSize.bottom - WindowSize.top)*0.175, 3,WindowSize);
                    DrawMessageBubbleLogoRight(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.1 + (WindowSize.right-WindowSize.left)*0.148,
                    (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1 + (WindowSize.bottom - WindowSize.top)*0.14,(WindowSize.right-WindowSize.left)*0.1,
                    (WindowSize.bottom - WindowSize.top)*0.145,3,WindowSize);
                    //for button start searching for recipient
                    CreateSearchButton(Mdc,CurrentHSearch,CurrentVSearch,WindowSize,ChatRect,2);
                    //for the scroll bar 
                    CreateScrollBar(HandleWnd,IDhInstance,WindowSize);
                    // for one creation of the scrollbar
                    Scroll = FALSE;
                }  
                /*else if(UiGeneral)
                {
                }*/
                if(UiInbox)
                {
                    ShowWindow(HandleSearch,SW_SHOW);
                }
                else
                {
                    ShowWindow(HandleSearch,SW_HIDE);
                }
            }
            BitBlt(DeviceContext, WindowLeft, WindowTop, WindowWidth, WindowHeight, Mdc, 0, 0, SRCCOPY);
            SelectObject(Mdc, OldBitMap);
            DeleteObject(BitMap);
            DeleteDC(Mdc);
            
            EndPaint(HandleWnd, &PaintWnd);
        break;
        case WM_COMMAND:
            if (LOWORD(wParam) == ButtonID)
            {
                CreateLoginControle(IDhInstance,HandleWnd,Mdc,WindowSize);
                ShowButton = FALSE;
                ShowTitle=FALSE;
                Login=TRUE;
                LogInCtl=TRUE;
            }
            if(LOWORD(wParam) == ID_LogIn)
            {
                Green=TRUE;
            }
            InvalidateRect(HandleWnd, &WindowSize, FALSE);
        break;
        case WM_TIMER :
        GetClientRect(HandleWnd, &WindowSize);
        AreaRedraw.left = WindowSize.left;
        AreaRedraw.top = WindowSize.top;
        AreaRedraw.right = WindowSize.left + (WindowSize.right - WindowSize.left)*0.3;
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
            UpdateMessageAnimation(HoveringMessage,HandleWnd);
            break;
            // this is updating the the button online every time
            case OnlineTimer :
            UpdateOnlineAnimation(HoveringOnline,HandleWnd);
            break;
            // this is updating the the button task every time
            case TaskTimer :
            UpdateTaskAnimation(HoveringTask,HandleWnd);
            break;
            // this is updating the the button project every time
            case ProjectTimer :
            UpdateProjectAnimation(HoveringProject,HandleWnd);
            break;
            // this is updating the the button disconnect every time
            case DisconnectTimer :
            UpdateDisconnectAnimation(HoveringDisconnect,HandleWnd);
            break;
            // this is updating the the button Inbox every time
            case InboxTimer :
            UpdateInboxAnimation(HoveringInbox,HandleWnd,WindowSize);
            break;
            // this is updating the the button general every time
            case GeneralTimer :
            UpdateGeneralAnimation(HoveringGeneral,HandleWnd,WindowSize);
            break;
            case TimerPanel :
            PanelAnimationUp(HandleWnd,WindowSize);
            break;
            case SearchTimer :
            UpdateSearchAnimation(HoveringSearch,HandleWnd,WindowSize);
            break;
        }
        InvalidateRect(HandleWnd,&AreaRedraw,FALSE);
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
                SetTimer(HandleWnd,TimerPanel,6,NULL);
                // for the search of the recipient
                if(HandleSearch == NULL)
                {
                     GetClientRect(HandleWnd, &WindowSize); 
                    ChatRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.018;
                    ChatRect.top = WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043;
                    ChatRect.right = ChatRect.left + (WindowSize.right - WindowSize.left)*0.1;
                    ChatRect.bottom = ChatRect.top + (WindowSize.bottom - WindowSize.top)*0.04;
                    HandleSearch = CreateWindowEx(0,"EDIT","", 
                    WS_CHILD | WS_VISIBLE | WS_BORDER, 
                    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.01,(WindowSize.right-WindowSize.left)*0.18,(WindowSize.bottom - WindowSize.top)*0.064,
                    HandleWnd,0,IDhInstance, NULL);
                    ShowWindow(HandleSearch,SW_HIDE);
                }
            }
            else if((x>=Choice_1_General_Button.left && x<=Choice_1_General_Button.right) && (y>=Choice_1_General_Button.top && y<=Choice_1_General_Button.bottom ))
            {
                UiGeneral = TRUE;
                UiMessage = FALSE;
                UiInbox = FALSE;
                MessageButtonClicked = FALSE;
                SetTimer(HandleWnd,TimerPanel,30,NULL);
            }
        }
        InvalidateRect(HandleWnd,&WindowSize,FALSE);
        break;
        case WM_MOUSEMOVE :
        Mx=LOWORD(lParam);
        My=HIWORD(lParam);

        // message hovering
        WasHoveringMessage=HoveringMessage;
        CheckMessage=CheckMessageRect(Choice_1_Button,HandleWnd,Mx,My);
        HoveringMessage=CheckMessage;
        if(HoveringMessage && !WasHoveringMessage)
        {
            // increase the button is size
            SetTimer(HandleWnd,MessageTimer,4,NULL);
        }
        else if(!HoveringMessage && WasHoveringMessage)
        {
            // decrease the button is size
            SetTimer(HandleWnd,MessageTimer,4,NULL); 
        }


        // online hovering 
        WasHoveringOnline=HoveringOnline;
        CheckOnline=CheckOnlineRect(Choice_2_Button,HandleWnd,Mx,My);
        HoveringOnline=CheckOnline;
        if(HoveringOnline && !WasHoveringOnline)
        {
            // increase the button is size
            SetTimer(HandleWnd,OnlineTimer,4,NULL);
        }
        else if(!HoveringOnline && WasHoveringOnline)
        {
            // decrease the button is size
            SetTimer(HandleWnd,OnlineTimer,4,NULL); 
        }


        // Task hovering 
        WasHoveringTask=HoveringTask;
        CheckTask=CheckTaskRect(Choice_3_Button,HandleWnd,Mx,My);
        HoveringTask=CheckTask;
        if(HoveringTask && !WasHoveringTask)
        {
            // increase the button is size
            SetTimer(HandleWnd,TaskTimer,4,NULL);
        }
        else if(!HoveringTask && WasHoveringTask)
        {
            // decrease the button is size
            SetTimer(HandleWnd,TaskTimer,4,NULL); 
        }


        // Project hovering 
        WasHoveringProject=HoveringProject;
        CheckProject=CheckProjectRect(Choice_4_Button,HandleWnd,Mx,My);
        HoveringProject=CheckProject;
        if(HoveringProject && !WasHoveringProject)
        {
            // increase the button is size
            SetTimer(HandleWnd,ProjectTimer,4,NULL);
        }
        else if(!HoveringProject && WasHoveringProject)
        {
            // decrease the button is size
            SetTimer(HandleWnd,ProjectTimer,4,NULL); 
        }


        // Disconnect hovering 
        WasHoveringDisconnect=HoveringDisconnect;
        CheckDisconnect=CheckDisconnectRect(Choice_5_Button,HandleWnd,Mx,My);
        HoveringDisconnect=CheckDisconnect;
        if(HoveringDisconnect && !WasHoveringDisconnect)
        {
            // increase the button is size
            SetTimer(HandleWnd,DisconnectTimer,4,NULL);
        }
        else if(!HoveringDisconnect && WasHoveringDisconnect)
        {
            // decrease the button is size
            SetTimer(HandleWnd,DisconnectTimer,4,NULL); 
        }


        // Inbox hovering 
        WasHoveringInbox=HoveringInbox;
        CheckInbox=CheckInboxRect(Choice_1_Inbox_Button,HandleWnd,Mx,My);
        HoveringInbox=CheckInbox;
        if(HoveringInbox && !WasHoveringInbox)
        {
            // increase the button is size
            SetTimer(HandleWnd,InboxTimer,4,NULL);
        }
        else if(!HoveringInbox && WasHoveringInbox)
        {
            // decrease the button is size
            SetTimer(HandleWnd,InboxTimer,4,NULL); 
        }


        // general hovering 
        WasHoveringGeneral=HoveringGeneral;
        CheckGeneral=CheckGeneralRect(Choice_1_General_Button,HandleWnd,Mx,My);
        HoveringGeneral=CheckGeneral;
        if(HoveringGeneral && !WasHoveringGeneral)
        {
            // increase the button is size
            SetTimer(HandleWnd,GeneralTimer,4,NULL);
        }
        else if(!HoveringGeneral && WasHoveringGeneral)
        {
            // decrease the button is size
            SetTimer(HandleWnd,GeneralTimer,4,NULL); 
        }


        // search hovering 
        WasHoveringSearch=HoveringSearch;
        CheckSearch=CheckSearchRect(SearchAnimation,HandleWnd,Mx,My);
        HoveringSearch=CheckSearch;
        if(HoveringSearch && !WasHoveringSearch)
        {
            // increase the button is size
            SetTimer(HandleWnd,SearchTimer,4,NULL);
        }
        else if(!HoveringSearch && WasHoveringSearch)
        {
            // decrease the button is size
            SetTimer(HandleWnd,SearchTimer,4,NULL); 
        }
        break;
        case WM_VSCROLL :
        SCRL.fMask = SIF_ALL;
        GetScrollInfo(ScrollBar,SB_VERT,&SCRL);
        int oldpos = SCRL.nPos;
        switch(LOWORD(wParam))
        {
            case SB_LINEUP:   
            SCRL.nPos -= 10; 
            break;
            case SB_LINEDOWN: 
            SCRL.nPos += 10; 
            break;
            case SB_PAGEUP:   
            SCRL.nPos -= SCRL.nPage; 
            break;
            case SB_PAGEDOWN: 
            SCRL.nPos += SCRL.nPage; 
            break;
            case SB_THUMBTRACK: 
            SCRL.nPos = HIWORD(wParam); 
            break;
        }
        if(oldpos != SCRL.nPos)
        {
            SetScrollInfo(ScrollBar, SB_VERT, &SCRL, TRUE);
        }
        InvalidateRect(HandleWnd,&WindowSize,FALSE);
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
    
    HandleWnd = CreateWindowEx(
        0,
        wndClass.lpszClassName,
        "WorkSpace",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT,
        CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, NULL, hInstance, NULL
    );
    
    if (!HandleWnd) {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    
    ShowWindow(HandleWnd, nCmdShow);
    UpdateWindow(HandleWnd);
    
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    
    return (int)msg.wParam;
}