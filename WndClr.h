#include <windows.h>
//login interface variable
HBRUSH Creme;
RECT LoginInterface;
HFONT Italique,OldFont;
RECT TitleRect;
bool ShowTitle=TRUE;
//Filling the window with creme color
void InterfaceLogin(int WindowLeft,int WindowTop,int WindowWidth,int WindowHeight,HDC Mdc)
{
    int middleY = WindowTop + (WindowHeight / 2);
    int textY = middleY - 130;
    TitleRect.left = WindowLeft;
    TitleRect.top = textY - 80;
    TitleRect.right = WindowLeft + WindowWidth;
    TitleRect.bottom = textY + 40;
    Italique=CreateFont(40,15,0,0,FW_NORMAL,TRUE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,
    CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,DEFAULT_PITCH | FF_DONTCARE,"Arial");
    
    OldFont = (HFONT)SelectObject(Mdc, Italique);    
    int oldBkMode = SetBkMode(Mdc, TRANSPARENT);
    DrawText(Mdc, "Welcome To", -1, &TitleRect, DT_SINGLELINE | DT_CENTER);    
    SetBkMode(Mdc, oldBkMode);
    SelectObject(Mdc, OldFont);
    DeleteObject(Italique);
    DeleteObject(Creme);
}
