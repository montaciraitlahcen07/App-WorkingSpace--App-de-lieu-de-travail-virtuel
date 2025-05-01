#include <windows.h>
#include <stdbool.h>
bool checkMrect(RECT Choice_1,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_1.left && Mx<=Choice_1.right) && (My >=Choice_1.top && My<=Choice_1.bottom))
    {
        return TRUE;
    }
    return FALSE;
}