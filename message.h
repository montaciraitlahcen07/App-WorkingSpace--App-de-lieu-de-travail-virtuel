#include <windows.h>
#include <stdbool.h>

// UI for message 
bool UiMessage=FALSE;
RECT Choice_1_Inbox;
RECT Choice_1_Inbox_Button;

#define MAX_BUTTON_WIDTHP 180
#define MAX_BUTTON_HEIGHTP 72
#define MIN_BUTTON_WIDTHP 135
#define MIN_BUTTON_HEIGHTP 42

// creatign the interface when the user click on the button message 
void CreateMessageUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,float CurrentHInbox,float CurrentVInbox)
{
    GetClientRect(HandleWnd,&WindowSize); 
    int ButtonWidth=MeasureWindowSize((WindowSize.right-WindowSize.left)*0.13f,MIN_BUTTON_WIDTHP,MAX_BUTTON_WIDTHP);
    int ButtonHeight=MeasureWindowSize((WindowSize.bottom-WindowSize.top)*0.12f,MIN_BUTTON_HEIGHTP,MAX_BUTTON_HEIGHTP);
    Choice_1_Inbox.left=Choice_1_Button.right*0.97+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2)-300-(CurrentHInbox/2);
    Choice_1_Inbox.top=(WindowSize.bottom-WindowSize.top)*0.27f-(CurrentVInbox/2);
    Choice_1_Inbox.right=Choice_1_Inbox.left+ButtonWidth+(CurrentHInbox/2);
    Choice_1_Inbox.bottom=Choice_1_Inbox.top+ButtonHeight+(CurrentVInbox/2);
    Choice_1_Inbox_Button=Choice_1_Inbox;
    Choice_1_Inbox.top+=(WindowSize.bottom-WindowSize.top)*0.04;
    Choice_1_Inbox.bottom+=(WindowSize.bottom-WindowSize.top)*0.043;
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    int lineX =Choice_1_Button.right*0.93+(((WindowSize.right)-(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025))/2); 
    int lineTop = PanelRect.bottom +(WindowSize.bottom-WindowSize.top)*0.047;
    int lineBottom = Choice_1_Button.bottom + (WindowSize.bottom-WindowSize.top)*0.055; 
    MoveToEx(Mdc, lineX, lineTop, NULL);
    LineTo(Mdc, lineX, lineBottom);
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
    HFONT OldFont=SelectObject(Mdc,Font);
    RoundRect(Mdc,Choice_1_Inbox_Button.left,Choice_1_Inbox_Button.top,Choice_1_Inbox_Button.right,Choice_1_Inbox_Button.bottom,32,32);
    DrawText(Mdc,"Inbox",-1,&Choice_1_Inbox,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(ButtonColor);
    DeleteObject(Font);
}