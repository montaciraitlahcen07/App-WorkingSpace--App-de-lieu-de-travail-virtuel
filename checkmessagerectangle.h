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
// Task button is hovering check
bool CheckTaskRect(RECT Choice_3_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_3_Button.left && Mx<=Choice_3_Button.right) && (My >=Choice_3_Button.top && My<=Choice_3_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// Project button is hovering check
bool CheckProjectRect(RECT Choice_4_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_4_Button.left && Mx<=Choice_4_Button.right) && (My >=Choice_4_Button.top && My<=Choice_4_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// Disconnect button is hovering check
bool CheckDisconnectRect(RECT Choice_5_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_5_Button.left && Mx<=Choice_5_Button.right) && (My >=Choice_5_Button.top && My<=Choice_5_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// Inbox button is hovering check
bool CheckInboxRect(RECT Choice_1_Inbox_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_1_Inbox_Button.left && Mx<=Choice_1_Inbox_Button.right) && (My >=Choice_1_Inbox_Button.top && My<=Choice_1_Inbox_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// general button is hovering check
bool CheckGeneralRect(RECT Choice_1_General_Button,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=Choice_1_General_Button.left && Mx<=Choice_1_General_Button.right) && (My >=Choice_1_General_Button.top && My<=Choice_1_General_Button.bottom))
    {
        return TRUE;
    }
    return FALSE;
}
// Search button is hovering check
bool CheckSearchRect(RECT SearchAnimation,HWND HandleWnd,int Mx,int My)
{
    if((Mx >=SearchAnimation.left && Mx<=SearchAnimation.right) && (My >=SearchAnimation.top && My<=SearchAnimation.bottom))
    {
        return TRUE;
    }
    return FALSE;
}