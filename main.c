#include <windows.h>
#include <windowsx.h>
#include <stdbool.h>
#include <math.h>
#include <commctrl.h>
// all file of the project 
#include "WndClr.h"
#include "Login.h"
#include "authentification.h"
#include "Account.h"
#include "hoveringanimation.h"
#include "checkmessagerectangle.h"
#include "Client.h"
#include "message.h"
#include "panelanimation.h"
#ifndef EM_SETCUEBANNER
#define EM_SETCUEBANNER 0x1501
#endif
#ifndef EM_GETCUEBANNER  
#define EM_GETCUEBANNER 0x1502
#endif
#pragma comment(lib, "comctl32.lib")
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
// Authentification
bool Green=FALSE;
// for the account
extern bool Account;
//
extern HICON CompanyLogo;
extern HWND HandleLogo;
// for then click of the mouse
int x,y;
//
bool Start = TRUE;
bool StartR = TRUE;
char buffer[256];
//
WNDPROC OriginalEditProc = NULL;
WNDPROC OriginalUsernameBarProc = NULL;
WNDPROC OriginalPasswordBarProc = NULL;
WNDPROC OriginalMessageBarProc = NULL;
// 
extern RECT MessageBarRect;
LRESULT CALLBACK MessageBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_PAINT :
        {
            LRESULT result = CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                HDC hdc = GetDC(hwnd);
                RECT rect;
                GetClientRect(hwnd,&rect);
                SetTextColor(hdc, RGB(128, 128, 128));
                SetBkMode(hdc, TRANSPARENT);
                rect.left += (rect.right - rect.left)*0.021;
                DrawText(hdc, "Type a message...", -1, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                ReleaseDC(hwnd, hdc);
            }
            return result;
        }
        case WM_CHAR:
        {
            if(UiInbox && GetFocus() == MessageBarHandle && wParam == VK_RETURN)
            {
                GetWindowText(hwnd, buffer, sizeof(buffer));
                if(strlen(buffer) != 0)
                {
                    Send = TRUE;
                }
                InvalidateRect(HandleWnd,&WindowSize,FALSE);
                return 0;
            }
            InvalidateRect(MessageBarHandle,NULL,FALSE);
            return CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
        }
        /*case WM_LBUTTONDBLCLK:
        GetWindowText(hwnd, buffer, sizeof(buffer));
        if(strlen(buffer) == 0)
        {
            return 0;
        }
        InvalidateRect(hwnd, NULL, FALSE);
        return CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);*/
        case WM_LBUTTONDOWN:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, FALSE);
                InvalidateRect(HandleSearch, NULL, FALSE);
                return 0;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            InvalidateRect(HandleSearch, NULL, FALSE);
            return CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
        }
        case WM_LBUTTONDBLCLK:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, FALSE);
                return 0;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
        }
        case WM_PASTE:
        {
            LRESULT result = CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case EM_SETSEL:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                InvalidateRect(hwnd, NULL, FALSE);
                return 0;
            }
            LRESULT result = CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case WM_SETFOCUS:
        {
            LRESULT result = CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case WM_KILLFOCUS:
        {
            LRESULT result = CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        default:
        return CallWindowProc(OriginalMessageBarProc, hwnd, msg, wParam, lParam);
    }
}
// this WndProc is for Password bar
LRESULT CALLBACK PasswordBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CHAR:
        {
            if(Login && GetFocus() == PLogin && wParam == VK_RETURN)
            {
                Green = TRUE;
                InvalidateRect(HandleWnd,&WindowSize,FALSE);
                return 0;
            }
            return CallWindowProc(OriginalPasswordBarProc, hwnd, msg, wParam, lParam);
        }
        default:
        return CallWindowProc(OriginalPasswordBarProc, hwnd, msg, wParam, lParam);
    }
}
// this WndProc is for Username bar
LRESULT CALLBACK UsernameBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch(msg)
    {
        case WM_CHAR:
        {
            if(Login && GetFocus() == ULogin && wParam == VK_RETURN)
            {
                SetFocus(PLogin);
                return 0;
            }
            return CallWindowProc(OriginalUsernameBarProc, hwnd, msg, wParam, lParam);
        }
        default:
        return CallWindowProc(OriginalUsernameBarProc, hwnd, msg, wParam, lParam);
    }
}
// this WndProc for the bar search window
LRESULT CALLBACK SearchBarProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
        case WM_PAINT:
        {
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                HDC hdc = GetDC(hwnd);
                RECT rect;
                GetClientRect(hwnd, &rect);
                SetTextColor(hdc, RGB(128, 128, 128));
                SetBkMode(hdc, TRANSPARENT);
                rect.left += (rect.right - rect.left)*0.05;
                DrawText(hdc, "Search...", -1, &rect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
                ReleaseDC(hwnd, hdc);
            }
            return result;
        }   
    case WM_LBUTTONDOWN:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, FALSE);
                InvalidateRect(MessageBarHandle, NULL, FALSE);
                return 0;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            InvalidateRect(MessageBarHandle, NULL, FALSE);
            return CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
        }
        case WM_LBUTTONDBLCLK:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                SetFocus(hwnd);
                InvalidateRect(hwnd, NULL, FALSE);
                return 0;
            }
            InvalidateRect(hwnd, NULL, FALSE);
            return CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
        }
        case WM_PASTE:
        {
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case EM_SETSEL:
        {
            GetWindowText(hwnd, buffer, sizeof(buffer));
            if(strlen(buffer) == 0)
            {
                InvalidateRect(hwnd, NULL, FALSE);
                return 0;
            }
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case WM_CHAR:
        {
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case WM_SETFOCUS:
        {
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        case WM_KILLFOCUS:
        {
            LRESULT result = CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
            InvalidateRect(hwnd, NULL, FALSE);
            return result;
        }
        default:
        return CallWindowProc(OriginalEditProc, hwnd, msg, wParam, lParam);
    }
}
/*// this WndProc is for Scroll bar window
RECT RecipientClicked;
#define Recipient_1 1011
bool IndexRecipient_1 = FALSE;
#define Recipient_2 1012
bool IndexRecipient_2 = FALSE;
#define Recipient_3 1013
bool IndexRecipient_3 = FALSE;
#define Recipient_4 1014
bool IndexRecipient_4 = FALSE;
#define Recipient_5 1015
bool IndexRecipient_5 = FALSE;
#define Recipient_6 1016
bool IndexRecipient_6 = FALSE;
#define Recipient_7 1017
bool IndexRecipient_7 = FALSE;
#define Recipient_8 1018
bool IndexRecipient_8 = FALSE;*/
// condition for bubble logo
bool BubbleLogo = FALSE;
LRESULT CALLBACK ScrollBarWindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg)
    {
        case WM_CREATE:
            g_scrollbar.min_val = 0;
            g_scrollbar.current_val = 0;
            if(total_items <= items_per_page)
            {
                g_scrollbar.max_val = 2; 
                g_scrollbar.page_size = items_per_page;
            } 
            else
            {
                g_scrollbar.max_val = total_items - items_per_page + 1;
                g_scrollbar.page_size = items_per_page;
            }
            InvalidateRect(hwnd, &ScrollBarRect, FALSE);
            break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            DeviceContext_Child_1 = BeginPaint(hwnd, &ps);
            GetClientRect(hwnd, &ScrollBarRect);
            MemoryDcSndTool.ScrollBarRect = ScrollBarRect;
            SendingTrdStatus.ScrolBarRect = ScrollBarRect;
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
            SetBkMode(Mdc_Child_1, TRANSPARENT);
            /*if(IndexRecipient_1)
            {
                RecipientClicked.top = ScrollBarRect.top;
                RecipientClicked.left = ScrollBarRect.left;
                RecipientClicked.right = ScrollBarRect.right;
                RecipientClicked.bottom = ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)*0.2;
                HBRUSH Gray = CreateSolidBrush(RGB(220,220,220));
                FillRect(Mdc_Child_1,&RecipientClicked,Gray);
                SetTimer(ScrollBar,Recipient_1,1000,NULL);
                IndexRecipient_1 = FALSE;
                DeleteObject(Gray);
            }
            else if(IndexRecipient_2)
            {
                RecipientClicked.top = ScrollBarRect.top + (ScrollBarRect.bottom - ScrollBarRect.top)*0.2;
                RecipientClicked.left = ScrollBarRect.left;
                RecipientClicked.right = ScrollBarRect.right;
                RecipientClicked.bottom = ScrollBarRect.top + ((ScrollBarRect.bottom - ScrollBarRect.top)*0.2)*2;
                HBRUSH Gray = CreateSolidBrush(RGB(220,220,220));
                FillRect(Mdc_Child_1,&RecipientClicked,Gray);
                SetTimer(ScrollBar,Recipient_2,100,NULL);
                IndexRecipient_2 = FALSE;
                DeleteObject(Gray);
            }*/
            DrawScrollBar(Mdc_Child_1, hwnd,WindowSize);
            UpdateScrollbarRange(hwnd,ScrollBarRect,&g_scrollbar);
            DrawContentWithScroll(Mdc_Child_1,hwnd,ScrollBarRect,Message,&g_scrollbar);
            BuildVisibleItemsList(Mdc_Child_1, hwnd, ScrollBarRect,Message,&g_scrollbar);
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
        case WM_SIZE:
        {
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }
        case WM_MOUSEMOVE:
        {
            POINT pt = {GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam)};
            BOOL was_hovering = g_scrollbar.thumb_hover;
            CalculateThumbRect(hwnd, &g_scrollbar.thumb_rect,WindowSize);
            g_scrollbar.thumb_hover = PointInRect(pt, &g_scrollbar.thumb_rect);
            if(was_hovering != g_scrollbar.thumb_hover)
            {
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            if (g_scrollbar.is_dragging)
            {
                RECT ScrollBarRect;
                GetClientRect(hwnd, &ScrollBarRect);
                float track_height = ScrollBarRect.bottom - 80;
                float range = g_scrollbar.max_val - g_scrollbar.min_val;
                if (range > 0)
                {
                    float new_pos = ((pt.y - 4 - g_scrollbar.drag_offset) * range) / track_height;
                    UpdateScrollValue(hwnd, new_pos);
                }
            }
            InvalidateRect(hwnd, &ScrollBarRect, FALSE);
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
            CalculateThumbRect(hwnd, &g_scrollbar.thumb_rect,WindowSize);
            client_rect.right -= SCROLLBAR_WIDTH - 2;
            // this is when i click on the thumb
            if(PointInRect(pt, &g_scrollbar.thumb_rect))
            {
                g_scrollbar.is_dragging = TRUE;
                g_scrollbar.thumb_pressed = TRUE;
                g_scrollbar.drag_offset = pt.y - g_scrollbar.thumb_rect.top;
                SetCapture(hwnd);
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            // this is when i click below the thumb or above it not in it 
            else if((pt.x >=g_scrollbar.thumb_rect.left && pt.x <=g_scrollbar.thumb_rect.right))
            {
                float track_height = ScrollBarRect.bottom - 80;
                float range = g_scrollbar.max_val - g_scrollbar.min_val;
                float new_pos = ((pt.y - 4) * range) / track_height;
                UpdateScrollValue(hwnd, new_pos);
                InvalidateRect(hwnd, &ScrollBarRect, FALSE);
            }
            // this is when i click on a recipient to chat with him 
            else if(pt.y >= 0 && PointInRect(pt,&client_rect))
            {
                // this is for hiding the two logo of a message when i click on a recipient 
                if(!BubbleLogo)
                {
                    BubbleLogo = TRUE;
                    // creating message bar 
                    RECT MessageBarRect;
                    GetClientRect(HandleWnd, &MessageBarRect);
                    MessageBarHandle = CreateWindowEx(0,"EDIT",0, 
                    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 
                    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375,MessageBarRect.bottom - (MessageBarRect.bottom - MessageBarRect.top)*0.09,
                    (MessageBarRect.right - MessageBarRect.left)*0.41,(MessageBarRect.bottom - MessageBarRect.top)*0.07,
                    HandleWnd,0,IDhInstance, NULL);
                    if(MessageBarHandle)
                    {
                        OriginalMessageBarProc = (WNDPROC)SetWindowLongPtr(MessageBarHandle, GWLP_WNDPROC, (LONG_PTR)MessageBarProc);
                    }
                    // taking a copy for the sending thread
                    SendingTools.MessageBarHandle = MessageBarHandle;
                }
                char EmptyFull[50];
                int Index = GetClickedRecipient(pt);
                GetWindowText(HandleSearch,EmptyFull,sizeof(EmptyFull));
                if(Index >= 0 && strlen(EmptyFull) == 0)
                {
                    // taking a copy into receiving thread
                    strcpy(ConnectingTools.PrivateMessage.SelectedRecipient,Message[Index].Username);
                }
                else if(Index >= 0 && strlen(EmptyFull) != 0)
                {
                    // taking a copy into receiving thread and use it to draw the recipient conversation 
                    strcpy(ConnectingTools.PrivateMessage.SelectedRecipient,Message[Index].Username);
                }          
            }
            SetFocus(MessageBarHandle);
            InvalidateRect(MessageBarHandle,NULL,FALSE);
            InvalidateRect(HandleWnd,&WindowSize,FALSE);
            InvalidateRect(HandleSearch,NULL,FALSE);
            InvalidateRect(hwnd,NULL,FALSE);
            break;
        }
        case WM_LBUTTONUP:
        {
            if(g_scrollbar.is_dragging)
            {
                g_scrollbar.is_dragging = FALSE;
                g_scrollbar.thumb_pressed = FALSE;
                ReleaseCapture();
                InvalidateRect(hwnd, &g_scrollbar.thumb_rect, FALSE);
            }
            break;
        }
        case WM_MOUSEWHEEL:
        {
            float delta = GET_WHEEL_DELTA_WPARAM(wParam);
            float step = (delta > 0) ? -1.0f : 1.0f;
            UpdateScrollValue(hwnd, g_scrollbar.current_val + step);
            InvalidateRect(hwnd, &ScrollBarRect, FALSE);
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}
// this WndProc is for the main window 
LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
    switch (msg)
    {
            case WM_SIZE:
            GetClientRect(hwnd, &WindowSize);
            ConnectingTools.WindowSize = WindowSize;
            WindowLeft = WindowSize.left;
            WindowTop = WindowSize.top;
            WindowWidth = WindowSize.right - WindowSize.left;
            WindowHeight = WindowSize.bottom - WindowSize.top;            
            if(UiInbox)
            {
                RECT rect;
                GetClientRect(HandleSearch, &rect);
                MoveWindow(ScrollBar,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.03,
                ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.08,
                (WindowSize.right - WindowSize.left)/2 - ((WindowSize.right - WindowSize.left)*0.25),
                (WindowSize.bottom-WindowSize.top)*0.643,TRUE);
                InvalidateRect(HandleSearch,&rect, FALSE);
            }
            UpdateScrollbarRange(ScrollBar, ScrollBarRect, &g_scrollbar);
            InvalidateRect(hwnd, &WindowSize, FALSE);
            break;  
            case WM_CREATE:
            //InitializeCriticalSection(&socketLock);
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
            MemoryDcSndTool.ScrollBar = ScrollBar;
            SendingTrdStatus.ScrollBar = ScrollBar;
            UpdateScrollbarRange(ScrollBar,ScrollBarRect, &g_scrollbar);
            // this is for the client winsock2 things
            WSADATA wsaData;
            int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
            if (result != 0) {
                printf("WSAStartup failed: %d\n", result);
                return 1;
            }
            /*struct hostent *host = gethostbyname("imad.stil.fun");
            if (host != NULL) {
            memcpy(&ConnectingTools.Server.sin_addr.S_un.S_addr,host->h_addr, host->h_length);
            ConnectingTools.Server.sin_port = htons(8000);  // The port from Pinggy
            } else {
                printf("Failed to resolve hostname\n");
            }*/
           // Sending connection
            ConnectingTools.ServerSending.sin_family=AF_INET;
            ConnectingTools.ServerSending.sin_port=htons(8000);
            ConnectingTools.ServerSending.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
            ConnectingTools.ClientSocketSending=socket(AF_INET,SOCK_STREAM,0);
            if(ConnectingTools.ClientSocketSending==INVALID_SOCKET)
            {
                printf("broken socket\n");
                closesocket(ConnectingTools.ClientSocketSending);
                return 1;
            }
            int ServerResult=connect(ConnectingTools.ClientSocketSending,(const struct sockaddr *)&ConnectingTools.ServerSending,sizeof(ConnectingTools.ServerSending));
            Sleep(100);
            // Receiving connection
            ConnectingTools.ServerReceiving.sin_family=AF_INET;
            ConnectingTools.ServerReceiving.sin_port=htons(8001);
            ConnectingTools.ServerReceiving.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
            //struct sockaddr_in ClientData;
            ConnectingTools.ClientSocketReceiving=socket(AF_INET,SOCK_STREAM,0);
            if(ConnectingTools.ClientSocketReceiving==INVALID_SOCKET)
            {
                printf("broken socket\n");
                closesocket(ConnectingTools.ClientSocketReceiving);
                return 1;
            }
            int ServerResultR=connect(ConnectingTools.ClientSocketReceiving,(const struct sockaddr *)&ConnectingTools.ServerReceiving,sizeof(ConnectingTools.ServerReceiving));
            Sleep(100);
            /*struct hostent *hostfirst = gethostbyname("moncef.stil.fun");
            if (hostfirst != NULL)
            {
            memcpy(&ConnectingTools.ServerSendingStatus.sin_addr.S_un.S_addr,hostfirst->h_addr, hostfirst->h_length);
            ConnectingTools.ServerStatus.sin_port = htons(8000);  // The port from Pinggy
            }
            else
            {
                printf("Failed to resolve hostname\n");
            }*/
            // this socket is for status recv
            ConnectingTools.ServerStatus.sin_family=AF_INET;
            ConnectingTools.ServerStatus.sin_port=htons(8002);
            ConnectingTools.ServerStatus.sin_addr.S_un.S_addr=inet_addr("127.0.0.1");
            struct sockaddr_in ClientsStatus;
            ConnectingTools.StatusSocket=socket(AF_INET,SOCK_STREAM,0);
            if(ConnectingTools.StatusSocket==INVALID_SOCKET)
            {
                printf("broken socket\n");
                closesocket(ConnectingTools.StatusSocket);
                return 1;
            }
            int StatusResult=connect(ConnectingTools.StatusSocket,(const struct sockaddr *)&ConnectingTools.ServerStatus,sizeof(ConnectingTools.ServerStatus));
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
            // taking a copy into receiving thread
            ConnectingTools.Mdc = Mdc;
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
                Authentification(ULogin,PLogin,UserData_2,Creme,WindowSize,Mdc,hwnd,&SendingTools,ConnectingTools,&Green);
            }
            Green=FALSE;
            if(Start)
            {
                // this is for creating threads to communicate with the server (communicate with other friend in the server (companie)
                HANDLE ThreadSending = (HANDLE)_beginthreadex(NULL ,0,SendingThread,&SendingTools,0,NULL);
                HANDLE ThreadStatus = (HANDLE)_beginthreadex(NULL, 0,StatusThread,&SendingTrdStatus, 0, NULL);
                HANDLE ThreadReceive = (HANDLE)_beginthreadex(NULL, 0, receivingClient, &RcvStg, 0, NULL);
                /*if(ThreadSending)
                {
                    WaitForSingleObject(ThreadSending, INFINITE);
                    CloseHandle(ThreadSending);
                    }
                    if(ThreadReceive)
                {
                    CloseHandle(ThreadReceive);
                }*/
                Start = FALSE;
            } 
            if(Account)
            {
                float FontReturn = baseRectangle(WindowSize,hwnd);
                bool FontSize = (FontReturn >= 106?TRUE:FALSE);
                //rendering the message button
                CreateMessageAccount(Mdc,CurrentHMessage,CurrentVMessage,WindowSize,FontSize);
                //rendering the online button
                CreateOnlineAccount(Mdc,CurrentHOnline,CurrentVOnline,WindowSize,FontSize);
                //rendering  the task button
                CreateTaskAccount(Mdc,CurrentHTask,CurrentVTask,WindowSize,FontSize);
                // rendering  the project button
                CreateProjectAccount(Mdc,CurrentHProject,CurrentVProject,WindowSize,FontSize);
                // rendering  the disconnect button
                CreateDisconnectAccount(Mdc,CurrentHDisconnect,CurrentVDisconnect,WindowSize,FontSize);
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
                    float FontReturn = baseRectangle(WindowSize,hwnd);
                    bool FontSize = (FontReturn >= 106?TRUE:FALSE);
                    CreateMessageUi(Mdc,hwnd,WindowSize,CurrentHInbox,CurrentVInbox,CurrentHGeneral,CurrentVGeneral,FontSize);
                }
                // when the user click on the inbox or general button this created the ui
                if(UiInbox)
                {
                    CreateInboxUi(Mdc,hwnd,WindowSize,IDhInstance);
                    LineDifferenceMessage(Mdc,hwnd,WindowSize);
                    MoveWindow(HandleSearch,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.045,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.009,(WindowSize.right-WindowSize.left)*0.195,(WindowSize.bottom - WindowSize.top)*0.064,TRUE);
                    /*
                    else
                    {

                    }*/
                    if(!BubbleLogo)
                    {
                        DrawMessageBubbleLogoLeft(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.14,
                        (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1,(WindowSize.right-WindowSize.left)*0.125,
                        (WindowSize.bottom - WindowSize.top)*0.175, 3,WindowSize);
                        DrawMessageBubbleLogoRight(Mdc, Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.285 + (WindowSize.right-WindowSize.left)*0.1 + (WindowSize.right-WindowSize.left)*0.148,
                        (WindowSize.bottom - WindowSize.top)/2 -(WindowSize.bottom - WindowSize.top)*0.1 + (WindowSize.bottom - WindowSize.top)*0.14,(WindowSize.right-WindowSize.left)*0.1,
                        (WindowSize.bottom - WindowSize.top)*0.145,3,WindowSize);
                    }
                    else if(BubbleLogo)
                    {
                        // put here a function who draw the recipient 
                        ShowWindow(MessageBarHandle,SW_SHOW);
                        MoveWindow(MessageBarHandle,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375,WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.09,
                        (WindowSize.right - WindowSize.left)*0.41,(WindowSize.bottom - WindowSize.top)*0.07,TRUE);
                        UiInboxConversation(HandleWnd,Mdc,ConnectingTools,WindowSize,PanelRect,CurrentHEmoji,CurrentVEmoji,CurrentHAttach,CurrentVAttach,CurrentHSend,CurrentVSend);
                    }
                }  
                else if(!UiInbox)
                {
                    ShowWindow(MessageBarHandle,SW_HIDE);
                }
                if(UiInbox)
                {
                    InvalidateRect(ScrollBar, &ScrollBarRect, FALSE);
                    ShowWindow(HandleSearch,SW_SHOW);
                    ShowWindow(ScrollBar, SW_SHOW);
                    if(!BubbleLogo)
                    {
                        SetFocus(HandleSearch); 
                    }
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
            case EmojiTimer :
            UpdateEmojiAnimation(HoveringEmoji,hwnd,WindowSize);
            break;
            case AttachTimer :
            UpdateAttachAnimation(HoveringAttach,hwnd,WindowSize);
            break;
            case SendTimer :
            UpdateSendAnimation(HoveringSend,hwnd,WindowSize);
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
            // taking a copy for the receiving thread
            RcvStg.UiInbox = UiInbox;
            MessageButtonClicked = TRUE;   
        }
        else if(MessageButtonClicked)
        {
            // i need to make a condition in the end of the condition messagebuttonclicked about turning messagebuttonclicked into false
            //MessageButtonClicked = FALSE;
            // this is for checking the inbox button 
            if((x>=Choice_1_Inbox_Button.left && x<=Choice_1_Inbox_Button.right) && (y>=Choice_1_Inbox_Button.top && y<=Choice_1_Inbox_Button.bottom ))
            {
                UiInbox = TRUE;
                MemoryDcSndTool.UiInbox = UiInbox;
                // taking a copy for the receiving thread
                RcvStg.UiInbox = UiInbox;
                UiMessage = FALSE;
                UiGeneral = FALSE;
                MemoryDcSndTool.UiGeneral = UiGeneral;
                SetTimer(hwnd,TimerPanel,30,NULL);
                // for the search of the recipient
                if(HandleSearch == NULL)
                {
                    GetClientRect(hwnd, &WindowSize); 
                    ChatRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.018;
                    ChatRect.top = WindowSize.top+(WindowSize.bottom-WindowSize.top)*0.176+ (WindowSize.bottom - WindowSize.top)*0.043;
                    ChatRect.right = ChatRect.left + (WindowSize.right - WindowSize.left)*0.1;
                    ChatRect.bottom = ChatRect.top + (WindowSize.bottom - WindowSize.top)*0.04;
                    HandleSearch = CreateWindowEx(0,"EDIT",0, 
                    WS_CHILD | WS_VISIBLE | WS_BORDER | ES_LEFT | ES_AUTOHSCROLL, 
                    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.1,ChatRect.bottom + (WindowSize.bottom - WindowSize.top)*0.01,(WindowSize.right-WindowSize.left)*0.22,(WindowSize.bottom - WindowSize.top)*0.064,
                    hwnd,0,IDhInstance, NULL);
                    if(HandleSearch)
                    {
                        OriginalEditProc = (WNDPROC)SetWindowLongPtr(HandleSearch, GWLP_WNDPROC, (LONG_PTR)SearchBarProc);
                    }
                }
            }
            else if((x>=Choice_1_General_Button.left && x<=Choice_1_General_Button.right) && (y>=Choice_1_General_Button.top && y<=Choice_1_General_Button.bottom ))
            {
                UiGeneral = TRUE;
                MemoryDcSndTool.UiGeneral = UiGeneral;
                UiMessage = FALSE;
                UiInbox = FALSE;
                MemoryDcSndTool.UiInbox = UiInbox;
                // taking a copy for the receiving thread
                RcvStg.UiInbox = UiInbox;
                SetTimer(hwnd,TimerPanel,30,NULL);
            }
            else if(UiInbox)
            {
                float left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 - CurrentHSend;
                float right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 + (WindowSize.right - WindowSize.left)*0.02 + CurrentHSend;
                float top = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0875 - CurrentVSend;
                float bottom = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.075 + (WindowSize.bottom - WindowSize.top)*0.05 + CurrentVSend;
                if(x>=left && x<=right && y>= top && y<=bottom)
                {
                    GetWindowText(MessageBarHandle, buffer, sizeof(buffer));
                    if(strlen(buffer) != 0)
                    {
                        Send = TRUE;
                    }
                }
            }
        }
        InvalidateRect(hwnd,&WindowSize,FALSE);
        InvalidateRect(HandleSearch,NULL,FALSE);
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
        // Emoji hovering 
        Emoji_Button.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.296 - CurrentHEmoji;
        Emoji_Button.top = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - CurrentVEmoji;
        Emoji_Button.right =  Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.326 + CurrentHEmoji;
        Emoji_Button.bottom =  WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + CurrentVEmoji;
        WasHoveringEmoji=HoveringEmoji;
        CheckEmoji=CheckEmojiRect(Emoji_Button,hwnd,Mx,My);
        HoveringEmoji=CheckEmoji;
        if(HoveringEmoji && !WasHoveringEmoji)
        {
            // increase the button is size
            SetTimer(hwnd,EmojiTimer,17,NULL);
        }
        else if(!HoveringEmoji && WasHoveringEmoji)
        {
            // decrease the button is size
            SetTimer(hwnd,EmojiTimer,17,NULL); 
        }
        // Attach hovering 
        Attach_Button.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.335 - CurrentHAttach;
        Attach_Button.top = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0872 - CurrentVAttach;
        Attach_Button.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3655 + CurrentHAttach;
        Attach_Button.bottom = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0285 + CurrentVAttach;
        WasHoveringAttach=HoveringAttach;
        CheckAttach=CheckAttachRect(Attach_Button,hwnd,Mx,My);
        HoveringAttach=CheckAttach;
        if(HoveringAttach && !WasHoveringAttach)
        {
            // increase the button is size
            SetTimer(hwnd,AttachTimer,17,NULL);
        }
        else if(!HoveringAttach && WasHoveringAttach)
        {
            // decrease the button is size
            SetTimer(hwnd,AttachTimer,17,NULL); 
        }
        // Send hovering 
        Send_Button.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.425 - CurrentHSend;
        Send_Button.top = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.0875 - CurrentVSend;
        Send_Button.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.375 + (WindowSize.right - WindowSize.left)*0.44 +  
        (WindowSize.right - WindowSize.left)*0.02 + CurrentHSend;
        Send_Button.bottom = WindowSize.bottom - (WindowSize.bottom - WindowSize.top)*0.075 + (WindowSize.bottom - WindowSize.top)*0.05 + CurrentVSend;
        WasHoveringSend=HoveringSend;
        CheckSend=CheckSendRect(Send_Button,hwnd,Mx,My);
        HoveringSend=CheckSend;
        if(HoveringSend && !WasHoveringSend)
        {
            // increase the button is size
            SetTimer(hwnd,SendTimer,17,NULL);
        }
        else if(!HoveringSend && WasHoveringSend)
        {
            // decrease the button is size
            SetTimer(hwnd,SendTimer,17,NULL); 
        }
        break;
        case WM_DESTROY:
        if(HandleSearch != 0)
        {
        DestroyWindow(HandleSearch);
        HandleSearch = 0;
        }    
        // Clean up any other resources (icons, etc.)
        if(CompanyLogo)
        {
            DestroyIcon(CompanyLogo);
        }
        if(CompanyBigLogo)
        {
            DestroyIcon(CompanyBigLogo);
        }
        DeleteCriticalSection(&socketLock);
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
    FILE *UserData=0;
    UserData_2=UserData;
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
    child_wc.lpfnWndProc = ScrollBarWindowProc; 
    child_wc.hInstance = hInstance;
    child_wc.lpszClassName = CHILD_CLASS_NAME;
    child_wc.hCursor = LoadCursor(NULL, IDC_HAND);
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
    if(!HandleWnd)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error", MB_ICONEXCLAMATION | MB_OK);
        return 1;
    }
    RcvStg.HandleWnd = HandleWnd;
    ShowWindow(HandleWnd, nCmdShow);
    UpdateWindow(HandleWnd);
    MSG msg;
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }   
    return (int)msg.wParam;
}