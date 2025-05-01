#include <windows.h>
#include <stdbool.h>
// Message button is hovering check
bool CheckMessageRect(RECT Choice_1_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_1_Button.left && Mx<=Choice_1_Button.right) && (My >=Choice_1_Button.top && My<=Choice_1_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// Online button is hovering check
bool CheckOnlineRect(RECT Choice_2_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_2_Button.left && Mx<=Choice_2_Button.right) && (My >=Choice_2_Button.top && My<=Choice_2_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}