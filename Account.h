#include <windows.h>
#include <stdbool.h>
bool Account=FALSE;
// Message
RECT Choice_1;
RECT Choice_1_Button;

// online
RECT Choice_2;
RECT Choice_2_Button;
void baseRectangle()
{
    Choice_1.left=40;
    Choice_1.top=148;
    Choice_1.right=Choice_1.left+120;
    Choice_1.bottom=Choice_1.top+40;
    
    Choice_2.left=40;
    Choice_2.top=Choice_1.bottom+40;
    Choice_2.right=Choice_1.right;
    Choice_2.bottom=Choice_2.top+40;
    
    Choice_1_Button = Choice_1;
    Choice_2_Button = Choice_2;
}
// Message creating button
void CreateMessageAccount(HDC Mdc,float CurrentHMessage,float CurrentVMessage)
{ 
    RECT MessageRect=Choice_1;
    RECT MessageAnimation=Choice_1_Button;
    MessageRect.left=40-(CurrentHMessage/2);
    MessageRect.top=148-(CurrentVMessage/2)+5;
    MessageRect.right=MessageRect.left+120+(CurrentHMessage/2);
    MessageRect.bottom=MessageRect.top+40+(CurrentVMessage/2);
    HFONT Font=CreateFont(
    20,
    10,
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
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(210, 210, 210));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,MessageAnimation.left-(CurrentHMessage/2),MessageAnimation.top-15-(CurrentVMessage/2),MessageAnimation.right+(CurrentHMessage/2),MessageAnimation.bottom+2+(CurrentVMessage/2),32,28);
    DrawText(Mdc,"Message",-1,&MessageRect,DT_SINGLELINE | DT_CENTER | HS_HORIZONTAL | HS_VERTICAL);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(OldButtonColor);
}
// Online is creating button
void CreateOnlineAccount(HDC Mdc,float CurrentHOnline,float CurrentVOnline)
{
    RECT OnlineAnimation=Choice_2_Button;
    
    RECT OnlineRect= Choice_2;
    OnlineRect.left = OnlineRect.left-(CurrentHOnline/2);
    OnlineRect.top = OnlineRect.top-(CurrentVOnline/2)+5;
    OnlineRect.right = OnlineRect.right+(CurrentHOnline/2);
    OnlineRect.bottom = OnlineRect.bottom+(CurrentVOnline/2);

    HFONT Font=CreateFont(
    20,
    10,
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
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(210, 210, 210));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,OnlineAnimation.left-(CurrentHOnline/2),OnlineAnimation.top-15-(CurrentVOnline/2),OnlineAnimation.right+(CurrentHOnline/2),OnlineAnimation.bottom+2+(CurrentVOnline/2),32,28);
    DrawText(Mdc,"Online",-1,&OnlineRect,DT_SINGLELINE | DT_CENTER | HS_HORIZONTAL | HS_VERTICAL);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(OldButtonColor);
}