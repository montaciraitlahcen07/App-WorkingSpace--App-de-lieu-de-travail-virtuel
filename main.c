#include <windows.h>
#include <windowsx.h>
#include <stdbool.h>
// all file of the project 
#include "WndClr.h"
#include "Login.h"
#include "authentification.h"
#include "usersdata.h"
#include "Account.h"
#include "message.h"
#include "hoveringanimation.h"
#include "checkmessagerectangle.h"

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
            InvalidateRect(HandleWnd, &WindowSize, FALSE);
            break;        
            case WM_CREATE:
            ButtonHandle = CreateWindowEx( 0,"BUTTON","Log In",WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
            0, 0, 100, 32,hwnd,(HMENU)ButtonID,IDhInstance,NULL);   
            break;
            break;
            case WM_PAINT:
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
                InterfaceLogin(WindowLeft, WindowTop, WindowWidth, WindowHeight, Mdc);
            }
            int buttonWidth = 185;
            int buttonHeight = 55;
            int buttonX = (WindowWidth - buttonWidth) / 2;  
            int buttonY = WindowTop + (WindowHeight / 2) + 40;            
            SetWindowPos(
                ButtonHandle,
                NULL,
                buttonX,
                buttonY,
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
            baseRectangle();
            if(Account)
            {
                CreateMessageAccount(Mdc,CurrentHMessage,CurrentVMessage);
                CreateOnlineAccount(Mdc,CurrentHOnline,CurrentVOnline);
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
            case OnlineTimer :
            UpdateOnlineAnimation(HoveringOnline,HandleWnd);
            break;
        }
        InvalidateRect(HandleWnd,&WindowSize,FALSE);
        break;
        /*case WM_LBUTTONDOWN :
        x =GET_X_LPARAM(lParam);
        y =GET_Y_LPARAM(lParam);
        if((x>=Choice_1.left && x<=Choice_1.right) && (y>=Choice_1.top && y<=Choice_1.bottom ))
        {
        }
        InvalidateRect(HandleWnd,&WindowSize,FALSE);
        break;*/
        case WM_MOUSEMOVE :
        Mx=LOWORD(lParam);
        My=HIWORD(lParam);
        // message hovering
        WasHoveringMessage=HoveringMessage;
        CheckMessage=CheckMessageRect(Choice_1_Button,HandleWnd,Mx,My);
        HoveringMessage=CheckMessage;
        // online hovering 
        WasHoveringOnline=HoveringOnline;
        CheckOnline=CheckOnlineRect(Choice_2_Button,HandleWnd,Mx,My);
        HoveringOnline=CheckOnline;
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
        else if(HoveringOnline && !WasHoveringOnline)
        {
            // increase the button is size
            SetTimer(HandleWnd,OnlineTimer,4,NULL);
        }
        else if(!HoveringOnline && WasHoveringOnline)
        {
            // decrease the button is size
            SetTimer(HandleWnd,OnlineTimer,4,NULL); 
        }
        break;
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;     
        case WM_DESTROY:
        PostQuitMessage(0);
        break;
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