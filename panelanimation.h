#include <windows.h>
#include <stdbool.h>
// creating tghe animation of the panel when when we click on button inside account 
float AddLenght = 0;
#define TimerPanel 45
void PanelAnimationUp(HWND HandleWnd,RECT WindowSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    #define PanelAnim (WindowSize.bottom - WindowSize.top)*0.032
    #define Add PanelAnim/4
    if(AddLenght <= PanelAnim)
    {
        AddLenght+=Add;
    }
    if(AddLenght == PanelAnim)
    {
        AddLenght = PanelAnim;
        KillTimer(HandleWnd,TimerPanel);
    }
    InvalidateRect(HandleWnd,&WindowSize,FALSE);
}