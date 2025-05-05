#include <windows.h>
#include <stdbool.h>
bool Account=FALSE;
// Message
RECT Choice_1;
RECT Choice_1_Button;

// online
RECT Choice_2;
RECT Choice_2_Button;

// Task
RECT Choice_3;
RECT Choice_3_Button;

// Project
RECT Choice_4;
RECT Choice_4_Button;

// Disconnect
RECT Choice_5;
RECT Choice_5_Button;

// Panel
RECT PanelRect;

#define MAX_BUTTON_WIDTH 160
#define MAX_BUTTON_HEIGHT 52
#define MIN_BUTTON_WIDTH 120
#define MIN_BUTTON_HEIGHT 35

// a function for measuring my window 
int MeasureWindowSize(int size, int min, int max) {
    if (size < min) return min;
    if (size > max) return max;
    return size;
}
void baseRectangle(RECT WindowSize,HWND HandleWnd)
{
    GetClientRect(HandleWnd,&WindowSize);
    
    int ButtonWidth=MeasureWindowSize((WindowSize.right-WindowSize.left)*0.12f,MIN_BUTTON_WIDTH,MAX_BUTTON_WIDTH);
    int ButtonHeight=MeasureWindowSize((WindowSize.bottom-WindowSize.top)*0.082f,MIN_BUTTON_HEIGHT,MAX_BUTTON_HEIGHT);

    Choice_1.left=(WindowSize.right-WindowSize.left)*0.02f;
    Choice_1.top=(WindowSize.bottom-WindowSize.top)*0.28f;
    Choice_1.right=Choice_1.left+ButtonWidth;
    Choice_1.bottom=Choice_1.top+ButtonHeight;
    
    Choice_2.left=(WindowSize.right-WindowSize.left)*0.02f;
    Choice_2.top=Choice_1.bottom+(WindowSize.bottom-WindowSize.top)*0.05f;
    Choice_2.right=Choice_2.left+ButtonWidth;
    Choice_2.bottom=Choice_2.top+ButtonHeight;

    Choice_3.left=(WindowSize.right-WindowSize.left)*0.02f;
    Choice_3.top=Choice_2.bottom+(WindowSize.bottom-WindowSize.top)*0.05f;
    Choice_3.right=Choice_3.left+ButtonWidth;
    Choice_3.bottom=Choice_3.top+ButtonHeight;

    Choice_4.left=(WindowSize.right-WindowSize.left)*0.02f;
    Choice_4.top=Choice_3.bottom+(WindowSize.bottom-WindowSize.top)*0.05f;
    Choice_4.right=Choice_4.left+ButtonWidth;
    Choice_4.bottom=Choice_4.top+ButtonHeight;

    Choice_5.left=(WindowSize.right-WindowSize.left)*0.02f;
    Choice_5.top=Choice_4.bottom+(WindowSize.bottom-WindowSize.top)*0.05f;
    Choice_5.right=Choice_5.left+ButtonWidth;
    Choice_5.bottom=Choice_5.top+ButtonHeight;

    Choice_1_Button = Choice_1;
    Choice_2_Button = Choice_2;
    Choice_3_Button = Choice_3;
    Choice_4_Button = Choice_4;
    Choice_5_Button = Choice_5;
}
// Creating message button
void CreateMessageAccount(HDC Mdc,float CurrentHMessage,float CurrentVMessage,RECT WindowSize)
{ 
    RECT MessageAnimation=Choice_1_Button;
    MessageAnimation.left-=(CurrentHMessage/2);
    MessageAnimation.top-=(CurrentVMessage/2);
    MessageAnimation.right+=(CurrentHMessage/2);
    MessageAnimation.bottom+=(CurrentVMessage/2);
    RECT MessageRect=MessageAnimation;
    MessageRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    MessageRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;
    
    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,MessageAnimation.left-(CurrentHMessage/2),MessageAnimation.top-15-(CurrentVMessage/2),MessageAnimation.right+(CurrentHMessage/2),MessageAnimation.bottom+2+(CurrentVMessage/2),32,28);
    DrawText(Mdc,"Message",-1,&MessageRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(OldButtonColor);
}
// creating online button
void CreateOnlineAccount(HDC Mdc,float CurrentHOnline,float CurrentVOnline,RECT WindowSize)
{
    RECT OnlineAnimation=Choice_2_Button;
    OnlineAnimation.left-=(CurrentHOnline/2);
    OnlineAnimation.top-=(CurrentVOnline/2);
    OnlineAnimation.right+=(CurrentHOnline/2);
    OnlineAnimation.bottom+=(CurrentVOnline/2);
    RECT OnlineRect=OnlineAnimation;
    OnlineRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    OnlineRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;

    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,OnlineAnimation.left-(CurrentHOnline/2),OnlineAnimation.top-15-(CurrentVOnline/2),OnlineAnimation.right+(CurrentHOnline/2),OnlineAnimation.bottom+2+(CurrentVOnline/2),32,28);
    DrawText(Mdc,"Online",-1,&OnlineRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating task button
void CreateTaskAccount(HDC Mdc,float CurrentHTask,float CurrentVTask,RECT WindowSize)
{
    RECT TaskAnimation=Choice_3_Button;
    TaskAnimation.left-=(CurrentHTask/2);
    TaskAnimation.top-=(CurrentVTask/2);
    TaskAnimation.right+=(CurrentHTask/2);
    TaskAnimation.bottom+=(CurrentVTask/2);
    RECT TaskRect=TaskAnimation;
    TaskRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    TaskRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;

    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,TaskAnimation.left-(CurrentHTask/2),TaskAnimation.top-15-(CurrentVTask/2),TaskAnimation.right+(CurrentHTask/2),TaskAnimation.bottom+2+(CurrentVTask/2),32,28);
    DrawText(Mdc,"Task",-1,&TaskRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating Project button
void CreateProjectAccount(HDC Mdc,float CurrentHProject,float CurrentVProject,RECT WindowSize)
{
    RECT ProjectAnimation=Choice_4_Button;
    ProjectAnimation.left-=(CurrentHProject/2);
    ProjectAnimation.top-=(CurrentVProject/2);
    ProjectAnimation.right+=(CurrentHProject/2);
    ProjectAnimation.bottom+=(CurrentVProject/2);
    RECT ProjectRect=ProjectAnimation;
    ProjectRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    ProjectRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;

    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,ProjectAnimation.left-(CurrentHProject/2),ProjectAnimation.top-15-(CurrentVProject/2),ProjectAnimation.right+(CurrentHProject/2),ProjectAnimation.bottom+2+(CurrentVProject/2),32,28);
    DrawText(Mdc,"Projects",-1,&ProjectRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating disconnect button
void CreateDisconnectAccount(HDC Mdc,float CurrentHDisconnect,float CurrentVDisconnect,RECT WindowSize)
{
    RECT DisconnectAnimation=Choice_5_Button;
    DisconnectAnimation.left-=(CurrentHDisconnect/2);
    DisconnectAnimation.top-=(CurrentVDisconnect/2);
    DisconnectAnimation.right+=(CurrentHDisconnect/2);
    DisconnectAnimation.bottom+=(CurrentVDisconnect/2);
    RECT DisconnectRect=DisconnectAnimation;
    DisconnectRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    DisconnectRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;

    HFONT Font=CreateFont(
    25,
    12,
    0,
    0,
    FW_NORMAL,
    FALSE,
    FALSE,
    FALSE,
    DEFAULT_CHARSET,
    OUT_OUTLINE_PRECIS,
    CLIP_DEFAULT_PRECIS,
    CLEARTYPE_QUALITY,
    DEFAULT_PITCH | FF_DONTCARE,
    "Segoe UI");
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,DisconnectAnimation.left-(CurrentHDisconnect/2),DisconnectAnimation.top-15-(CurrentVDisconnect/2),
    DisconnectAnimation.right+(CurrentHDisconnect/2),DisconnectAnimation.bottom+2+(CurrentVDisconnect/2),32,28);
    DrawText(Mdc,"Disconnect",-1,&DisconnectRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating the panel 
void CreatePanel(HDC Mdc,RECT WindowSize,HWND HandleWnd)
{
    GetClientRect(HandleWnd,&WindowSize);
    PanelRect.top=WindowSize.top;
    PanelRect.left=WindowSize.left;
    PanelRect.right=WindowSize.right;
    PanelRect.bottom=PanelRect.top+(WindowSize.bottom-WindowSize.top)*0.176;
    HBRUSH PanelColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldPanelColor=SelectObject(Mdc,PanelColor);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,PanelRect.left,PanelRect.top,
    PanelRect.right,PanelRect.bottom,42,42);
    RECT HAHA={PanelRect.left,PanelRect.top,PanelRect.left+35,PanelRect.top+35};
    RECT HOHO={PanelRect.right-35,PanelRect.top,PanelRect.right,PanelRect.top+35};
    FillRect(Mdc,&HAHA,PanelColor);
    FillRect(Mdc,&HOHO,PanelColor);
    SelectObject(Mdc,OldPanelColor);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    DeleteObject(PanelColor);
}
// the line who will diffrence 
void LineDifference(HDC Mdc,HWND HandleWnd,RECT WindowSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    int lineX = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025; 
    int lineTop = PanelRect.bottom +(WindowSize.bottom-WindowSize.top)*0.04;
    int lineBottom = WindowSize.bottom - (WindowSize.bottom-WindowSize.top)*0.04; 
    MoveToEx(Mdc, lineX, lineTop, NULL);
    LineTo(Mdc, lineX, lineBottom);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
}