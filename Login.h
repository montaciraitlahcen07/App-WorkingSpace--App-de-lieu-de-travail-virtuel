#include <windows.h>
#include <stdbool.h>
#define ID_LogIn 1001
bool Login=FALSE;
RECT LoginTitle;
// user name 
HWND ULogin=0;
RECT UsernameRect;
// pass word
HWND PLogin=0;
RECT PasswordRect;
//Button log in
HWND LogInButton=0;
RECT LoginRect;
// showing and hiding log in controle
bool LogInCtl=FALSE;
void DrawLoginPage(HINSTANCE IDhInstance,int WindowLeft,int WindowTop,int WindowWidth,int WindowHeight,HWND HandleWnd,HDC Mdc)
{
    LoginTitle.left=WindowLeft+(WindowWidth/2)-70;
    LoginTitle.top=WindowTop+(WindowHeight/2)-180;
    LoginTitle.right=LoginTitle.left+140;
    LoginTitle.bottom=LoginTitle.top+80;
    SetBkMode(Mdc,TRANSPARENT);
    HFONT Font=CreateFont(20,8,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
    DEFAULT_PITCH|FF_SWISS,"Arial");
    HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
    DrawText(Mdc,"Login Page",-1,&LoginTitle,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    UsernameRect.left=LoginTitle.left;
    UsernameRect.top=LoginTitle.top+70;
    UsernameRect.right=LoginTitle.right;
    UsernameRect.bottom=LoginTitle.bottom+80;
    DrawText(Mdc,"Enter User name",-1,&UsernameRect,DT_SINGLELINE | DT_CENTER);
    PasswordRect.left=LoginTitle.left;
    PasswordRect.top=LoginTitle.top+155;
    PasswordRect.right=LoginTitle.right;
    PasswordRect.bottom=LoginTitle.bottom+110;
    DrawText(Mdc,"Enter Pass Word",-1,&PasswordRect,DT_SINGLELINE | DT_CENTER);
}
void CreateLoginControle(HINSTANCE IDhInstance,HWND HandleWnd,HDC Mdc,RECT WindowSize)
{
    if(ULogin == NULL && PLogin == 0)
    {
        GetClientRect(HandleWnd,&WindowSize);
        ULogin=CreateWindowEx(0, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_BORDER, 
        WindowSize.left+(WindowSize.right/2)-95,WindowSize.top+(WindowSize.bottom/2)-75,205,34,
        HandleWnd,0,IDhInstance, NULL);
        //add here the butto if the user want to see his pass word or not like put show or hide 
        PLogin=CreateWindowEx(0, "EDIT", "", 
        WS_CHILD | WS_VISIBLE | WS_BORDER | ES_PASSWORD, 
        WindowSize.left+(WindowSize.right/2)-99,WindowSize.top+(WindowSize.bottom/2)+3,205,34,
        HandleWnd,0,IDhInstance, NULL);
    }
}
void CreateButton(HINSTANCE IDhInstance,HWND HandleWnd,HDC Mdc,RECT WindowSize)
{
    if(LogInButton == 0)
    {
        LogInButton=CreateWindowEx(0,"BUTTON","Log In",WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_FLAT,
        WindowSize.left+(WindowSize.right/2)-82,WindowSize.top+(WindowSize.bottom/2)+55,170,45,
        HandleWnd,(HMENU)ID_LogIn,IDhInstance,NULL);
    }
}