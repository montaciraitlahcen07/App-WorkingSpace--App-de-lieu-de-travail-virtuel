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
// Emoji
RECT Emoji_Button;
RECT UiGeneralEmoji_Button;
// Attach
RECT Attach_Button;
RECT UiGeneralAttach_Button;
// Send
RECT Send_Button;
RECT UiGeneralSend_Button;
//Comapany Big logo
HICON CompanyBigLogo;
HWND HandleBigLogo;
HICON MontaPicture;
HWND HandleMontaPic;
HICON MontaPictureRecipient;
HWND HandleMontaPicRecipient;
HICON MohammedPicture;
HWND HandleMohammedPic;
HICON MohammedPictureRecipient;
HWND HandleMohammedPicRecipient;
#define MAX_BUTTON_WIDTH 160
#define MAX_BUTTON_HEIGHT 52
#define MIN_BUTTON_WIDTH 101
#define MIN_BUTTON_HEIGHT 35
char WorkersPicture[][50] = {"monta","mohammed"};
// a function for measuring my window 
int MeasureWindowSize(int size, int min, int max)
{
    if (size < min) return min;
    if (size > max) return max;
    return size;
}
float baseRectangle(RECT WindowSize,HWND HandleWnd)
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
    return ButtonWidth;
}
// Creating message button
void CreateMessageAccount(HDC Mdc,float CurrentHMessage,float CurrentVMessage,RECT WindowSize,bool FontSize)
{ 
    RECT MessageAnimation=Choice_1_Button;
    MessageAnimation.left-=(CurrentHMessage/2);
    MessageAnimation.top-=(CurrentVMessage/2);
    MessageAnimation.right+=(CurrentHMessage/2);
    MessageAnimation.bottom+=(CurrentVMessage/2);
    RECT MessageRect=MessageAnimation;
    MessageRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    MessageRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;
    HFONT Font;
    if(FontSize)
    {
        Font=CreateFont(
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
    }
    else
    {
        Font=CreateFont(
        22,
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
    }
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,MessageAnimation.left,MessageAnimation.top-15,MessageAnimation.right,MessageAnimation.bottom+2,32,32);
    DrawText(Mdc,"Message",-1,&MessageRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    DeleteObject(OldButtonColor);
}
// creating online button
void CreateOnlineAccount(HDC Mdc,float CurrentHOnline,float CurrentVOnline,RECT WindowSize,bool FontSize)
{
    RECT OnlineAnimation=Choice_2_Button;
    OnlineAnimation.left-=(CurrentHOnline/2);
    OnlineAnimation.top-=(CurrentVOnline/2);
    OnlineAnimation.right+=(CurrentHOnline/2);
    OnlineAnimation.bottom+=(CurrentVOnline/2);
    RECT OnlineRect=OnlineAnimation;
    OnlineRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    OnlineRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;
    HFONT Font;
    if(FontSize)
    {
        Font=CreateFont(
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
    }
    else
    {
        Font=CreateFont(
        22,
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
    }
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,OnlineAnimation.left,OnlineAnimation.top-15,OnlineAnimation.right,OnlineAnimation.bottom+2,32,32);
    DrawText(Mdc,"Team",-1,&OnlineRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating task button
void CreateTaskAccount(HDC Mdc,float CurrentHTask,float CurrentVTask,RECT WindowSize,bool FontSize)
{
    RECT TaskAnimation=Choice_3_Button;
    TaskAnimation.left-=(CurrentHTask/2);
    TaskAnimation.top-=(CurrentVTask/2);
    TaskAnimation.right+=(CurrentHTask/2);
    TaskAnimation.bottom+=(CurrentVTask/2);
    RECT TaskRect=TaskAnimation;
    TaskRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    TaskRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;

    HFONT Font;
    if(FontSize)
    {
        Font=CreateFont(
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
    }
    else
    {
        Font=CreateFont(
        22,
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
    }
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,TaskAnimation.left,TaskAnimation.top-15,TaskAnimation.right,TaskAnimation.bottom+2,32,32);
    DrawText(Mdc,"Schedule",-1,&TaskRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    SelectObject(Mdc,OldPen);
    DeleteObject(Font);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating Project button
void CreateProjectAccount(HDC Mdc,float CurrentHProject,float CurrentVProject,RECT WindowSize,bool FontSize)
{
    RECT ProjectAnimation=Choice_4_Button;
    ProjectAnimation.left-=(CurrentHProject/2);
    ProjectAnimation.top-=(CurrentVProject/2);
    ProjectAnimation.right+=(CurrentHProject/2);
    ProjectAnimation.bottom+=(CurrentVProject/2);
    RECT ProjectRect=ProjectAnimation;
    ProjectRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    ProjectRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;
    HFONT Font;
    if(FontSize)
    {
        Font=CreateFont(
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
    }
    else
    {
        Font=CreateFont(
        22,
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
    }
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    HFONT OldFont=SelectObject(Mdc,Font);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,ProjectAnimation.left,ProjectAnimation.top-15,ProjectAnimation.right,ProjectAnimation.bottom+2,32,32);
    DrawText(Mdc,"Profile",-1,&ProjectRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldPen);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    DeleteObject(Pen);
    SelectObject(Mdc,OldButtonColor);
    DeleteObject(ButtonColor);
}
// creating disconnect button
void CreateDisconnectAccount(HDC Mdc,float CurrentHDisconnect,float CurrentVDisconnect,RECT WindowSize,bool FontSize)
{
    RECT DisconnectAnimation=Choice_5_Button;
    DisconnectAnimation.left-=(CurrentHDisconnect/2);
    DisconnectAnimation.top-=(CurrentVDisconnect/2);
    DisconnectAnimation.right+=(CurrentHDisconnect/2);
    DisconnectAnimation.bottom+=(CurrentVDisconnect/2);
    RECT DisconnectRect=DisconnectAnimation;
    DisconnectRect.top+=(WindowSize.bottom-WindowSize.top)*0.012;
    DisconnectRect.bottom+=(WindowSize.bottom-WindowSize.top)*0.015;
    HFONT Font;
    if(FontSize)
    {
        Font=CreateFont(
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
    }
    else
    {
        Font=CreateFont(
        22,
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
    }
    HBRUSH ButtonColor=CreateSolidBrush(RGB(210, 210, 210));
    HFONT OldFont=SelectObject(Mdc,Font);
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    SetBkMode(Mdc,TRANSPARENT);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,DisconnectAnimation.left,DisconnectAnimation.top-15,DisconnectAnimation.right,DisconnectAnimation.bottom+2,32,32);
    DrawText(Mdc,"Disconnect",-1,&DisconnectRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc,OldButtonColor);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    DeleteObject(ButtonColor);
}
// creating the panel 
void CreatePanel(HDC Mdc,RECT WindowSize,HWND HandleWnd,float AddLenght)
{
    GetClientRect(HandleWnd,&WindowSize);
    PanelRect.top=WindowSize.top;
    PanelRect.left=WindowSize.left;
    PanelRect.right=WindowSize.right;
    PanelRect.bottom=PanelRect.top+(WindowSize.bottom-WindowSize.top)*0.176 - (float)AddLenght;
    HBRUSH PanelColor=CreateSolidBrush(RGB(210, 210, 210));
    HBRUSH OldPanelColor=SelectObject(Mdc,PanelColor);
    HPEN Pen=CreatePen(BS_SOLID,1,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    RoundRect(Mdc,PanelRect.left,PanelRect.top,
    PanelRect.right,PanelRect.bottom,42,42);
    RECT LeftCorner={PanelRect.left,PanelRect.top,PanelRect.left+35,PanelRect.top+35};
    RECT RightCorner={PanelRect.right-35,PanelRect.top,PanelRect.right,PanelRect.top+35};
    FillRect(Mdc,&LeftCorner,PanelColor);
    FillRect(Mdc,&RightCorner,PanelColor);
    SelectObject(Mdc,OldPanelColor);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    DeleteObject(PanelColor);
}
// the line who will difference 
void LineDifference(HDC Mdc,HWND HandleWnd,RECT WindowSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
    HPEN OldPen=SelectObject(Mdc,Pen);
    int lineX = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.025; 
    int lineTop = PanelRect.bottom +(WindowSize.bottom-WindowSize.top)*0.05;
    int lineBottom = WindowSize.bottom - (WindowSize.bottom-WindowSize.top)*0.05; 
    MoveToEx(Mdc, lineX, lineTop, NULL);
    LineTo(Mdc, lineX, lineBottom);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
}
void Points(HDC Mdc,HWND HandleWnd,RECT WindowSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,3,RGB(0,0,0));
    HPEN OldPen=SelectObject(Mdc,Pen);
    HBRUSH DotsColor=CreateSolidBrush(RGB(0,0,0));
    HBRUSH OldDotsColor=SelectObject(Mdc,DotsColor);
    for(int i=0;i<3;i++)
    {

        Ellipse(Mdc,(WindowSize.right-WindowSize.left)*0.97,(WindowSize.bottom-WindowSize.top)*0.06+i*8,
        (WindowSize.right-WindowSize.left)*0.97+3,(WindowSize.bottom-WindowSize.top)*0.06+2+i*8);
    }
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldDotsColor);
    DeleteObject(DotsColor);
}
void Panelinfo(HDC Mdc,HWND HandleWnd,RECT WindowSize,bool FontSize,float AddLenght)
{
    HFONT Font;
    HFONT FontJobTitle;
    HFONT OldFontTimeStamp;
    float TimeStamp_Height,TimeStamp_Width;
    int WidthFirst,HeightFirst;
    HPEN Pen=CreatePen(PS_DOT,3,RGB(0,0,0));
    HPEN OldPen=SelectObject(Mdc,Pen);
    if(FontSize)
    {
        HeightFirst = 24;
        WidthFirst = 12;
        Font=CreateFont(23,11,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontJobTitle = CreateFont(18,7,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    else 
    {
        HeightFirst = 22;
        WidthFirst = 10;
        Font=CreateFont(22,10,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
        FontJobTitle = CreateFont(16,6,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
        DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    }
    HFONT OldFont=SelectObject(Mdc,Font);
    float PanelHeight = PanelRect.top + (WindowSize.bottom-WindowSize.top)*0.176;
    if(strcmp(WorkersPicture[0],SendingTools.username) == 0)
    {
        SetWindowPos(HandleMontaPic, NULL,
        WindowSize.left + (WindowSize.right-WindowSize.left)*0.012,
        PanelHeight*0.09,
        WindowSize.left + (WindowSize.right-WindowSize.left)*0.083,PanelHeight*0.85,
        SWP_NOZORDER);  
        ShowWindow(HandleMontaPic, SW_SHOW);
    }
    else if(strcmp(WorkersPicture[1],SendingTools.username) == 0)
    {
        SetWindowPos(HandleMohammedPic, NULL,
        WindowSize.left + (WindowSize.right-WindowSize.left)*0.012,
        PanelHeight*0.09,
        WindowSize.left + (WindowSize.right-WindowSize.left)*0.083,PanelHeight*0.85,
        SWP_NOZORDER);  
        ShowWindow(HandleMohammedPic, SW_SHOW);
    }
    RECT UsernamePanel,dutyRect;
    UsernamePanel.left = WindowSize.left + (WindowSize.right-WindowSize.left)*0.04;
    UsernamePanel.top = PanelHeight*0.25;
    UsernamePanel.right = WindowSize.left + (WindowSize.right-WindowSize.left)*0.27;
    UsernamePanel.bottom = PanelHeight*0.5;
    DrawText(Mdc,SendingTools.username,-1,&UsernamePanel,DT_SINGLELINE | DT_CENTER | DT_LEFT);
    dutyRect = UsernamePanel;
    dutyRect.top = PanelHeight*0.41;
    dutyRect.bottom = PanelHeight*0.68;
    dutyRect.left = ((WindowSize.right - WindowSize.left) / 2) - (WindowSize.right - WindowSize.left)*0.045;
    dutyRect.right = ((WindowSize.right - WindowSize.left) / 2) + (WindowSize.right - WindowSize.left)*0.07;
    DrawText(Mdc,SendingTools.username,-1,&UsernamePanel,DT_SINGLELINE | DT_CENTER | DT_LEFT);
    SetTextColor(Mdc,RGB(50, 205, 50));
    Pen = CreatePen(PS_DOT,5,RGB(0,0,0));
    OldPen = SelectObject(Mdc,Pen);
    Font = CreateFont(25,13,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
    DEFAULT_PITCH|FF_SWISS,"Segoe UI");
    OldFont=SelectObject(Mdc,Font);
    DrawText(Mdc,"On duty",-1,&dutyRect,DT_SINGLELINE | DT_CENTER);
    SetTextColor(Mdc,RGB(0,0,0));
    Pen = CreatePen(PS_DOT,1,RGB(0,0,0));
    OldPen = SelectObject(Mdc,Pen);
    OldFont = SelectObject(Mdc,FontJobTitle);
    UsernamePanel.left = WindowSize.left + (WindowSize.right-WindowSize.left)*0.045;
    UsernamePanel.top = PanelHeight*0.47;
    UsernamePanel.bottom = PanelHeight*0.68;
    DrawText(Mdc,"Software engineer",-1,&UsernamePanel,DT_SINGLELINE | DT_CENTER | DT_LEFT);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
}
 