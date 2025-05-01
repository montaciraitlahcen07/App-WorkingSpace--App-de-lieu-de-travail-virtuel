#include <windows.h>
#include <stdbool.h>
bool Account=FALSE;
// Message
RECT Choice_1;
RECT Choice_1_Button;
HBRUSH ButtonColor;
HBRUSH OldButtonColor;
HPEN Pen;
void CreateMessageAccount(HDC Mdc,float CurrentH,float CurrentV)
{
    Choice_1.left=40;
    Choice_1.top=148;
    Choice_1.right=Choice_1.left+120;
    Choice_1.bottom=Choice_1.top+40;
    Choice_1_Button=Choice_1;
    Choice_1.left=40-(CurrentH/2);
    Choice_1.top=148-(CurrentV/2)+5;
    Choice_1.right=Choice_1.left+120+(CurrentH/2);
    Choice_1.bottom=Choice_1.top+40+(CurrentV/2);
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
    ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    Pen=CreatePen(BS_SOLID,1,RGB(210, 210, 210));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,Choice_1_Button.left-(CurrentH/2),Choice_1_Button.top-15-(CurrentV/2),Choice_1_Button.right+(CurrentH/2),Choice_1_Button.bottom+2+(CurrentV/2),32,28);
    DrawText(Mdc,"Message",-1,&Choice_1,DT_SINGLELINE | DT_CENTER | HS_HORIZONTAL | HS_VERTICAL);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(OldButtonColor);

}