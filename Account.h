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
HICON MontaPictureProfile;
HWND HandleMontaPicProfile;
HICON MohammedPicture;
HWND HandleMohammedPic;
HICON MohammedPictureRecipient;
HWND HandleMohammedPicRecipient;
HICON MohammedPictureProfile;
HWND HandleMohammedPicProfile;
#define MAX_BUTTON_WIDTH 160
#define MAX_BUTTON_HEIGHT 52
#define MIN_BUTTON_WIDTH 101
#define MIN_BUTTON_HEIGHT 35
char WorkersPicture[][50] = {"montacir","mohammed"};
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

// UiSchedule 
void ScheduleUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,bool FontSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(210,210,210));
    HPEN OldPen=SelectObject(Mdc,Pen);
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
    RECT RoundedRec;
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.009,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.63,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.06,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    RoundedRec.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.3;
    RoundedRec.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.02;
    RoundedRec.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.546;
    RoundedRec.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.046;
    DrawText(Mdc,"This Week",-1,&RoundedRec,DT_SINGLELINE | DT_CENTER);
    Pen=CreatePen(BS_SOLID,2,RGB(180, 180, 190));
    OldPen=SelectObject(Mdc,Pen);
    MoveToEx(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.09,NULL);
    LineTo(Mdc,(Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.39);
    Pen=CreatePen(BS_SOLID,2,RGB(244,238,210));
    OldPen=SelectObject(Mdc,Pen);
    ButtonColor = CreateSolidBrush(RGB(244,238,210));
    OldButtonColor = SelectObject(Mdc,ButtonColor);
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.095,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.15,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.17,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.225,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.245,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.3,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.04,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.32,
    ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.375,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);


    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.132,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.187,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);

    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.262,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    
    RoundRect(Mdc,((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.02),PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282,
    WindowSize.right - (WindowSize.right-WindowSize.left)*0.02,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.337,
    (WindowSize.right-WindowSize.left)*0.025,(WindowSize.right-WindowSize.left)*0.025);
    SelectObject(Mdc,OldPen);
    DeleteObject(Pen);
    RECT ShiftDaysRect,ShiftTimeRect;
    ShiftDaysRect.left = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.05;
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.11;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.132;
    ShiftTimeRect = ShiftDaysRect;
    DrawText(Mdc,"Monday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.left = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.24);
    ShiftTimeRect.right = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 - (WindowSize.right-WindowSize.left)*0.02);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1105;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.185;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207;
    DrawText(Mdc,"Tuesday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.185;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1855;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.207;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);
    
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.17;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.26;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282;
    DrawText(Mdc,"Wednesday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2605;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.282;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.right = Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.15;
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.335;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.357;
    DrawText(Mdc,"Thursday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.3355;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.357;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);


    ShiftDaysRect.left = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.04);
    ShiftDaysRect.right = ((Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.24 + Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.615) / 2 + (WindowSize.right-WindowSize.left)*0.13);
    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.147;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.169;
    DrawText(Mdc,"Friday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.left = WindowSize.right - (WindowSize.right-WindowSize.left)*0.26;
    ShiftTimeRect.right = WindowSize.right - (WindowSize.right-WindowSize.left)*0.05;
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.1475;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.169;
    DrawText(Mdc,"9:00 AM - 5:00 PM",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.222;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.244;
    DrawText(Mdc,"Saturday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2225;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.244;
    DrawText(Mdc,"Day off",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);

    ShiftDaysRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.297;
    ShiftDaysRect.bottom = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.319;
    DrawText(Mdc,"Sunday :",-1,&ShiftDaysRect,DT_SINGLELINE | DT_CENTER);
    ShiftTimeRect.top = PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.2975;
    ShiftTimeRect.bottom =  PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.319;
    DrawText(Mdc,"Day off",-1,&ShiftTimeRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
}
// profile Ui
void ProfileUi(HDC Mdc,HWND HandleWnd,RECT WindowSize,bool FontSize)
{
    GetClientRect(HandleWnd,&WindowSize);
    HPEN Pen=CreatePen(BS_SOLID,2,RGB(244,238,210));
    HPEN OldPen=SelectObject(Mdc,Pen);
    HFONT Font,SecondFont,ThirdFont;
    if(FontSize)
    {
        Font=CreateFont(
        28,
        13,
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
        SecondFont=CreateFont(
        21,
        9,
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
        ThirdFont = CreateFontW(
            22,                      // Height - INCREASE THIS NUMBER (try 48, 64, 80, etc.)
            0,                       // Width (0 = default)
            0,                       // Escapement
            0,                       // Orientation
            FW_NORMAL,              // Weight
            FALSE,                   // Italic
            FALSE,                   // Underline
            FALSE,                   // StrikeOut
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            L"Segoe UI Emoji"       // Font name
        );
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
        SecondFont=CreateFont(
        19,
        9,
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
        ThirdFont = CreateFontW(
            24,                      // Height - INCREASE THIS NUMBER (try 48, 64, 80, etc.)
            0,                       // Width (0 = default)
            0,                       // Escapement
            0,                       // Orientation
            FW_NORMAL,              // Weight
            FALSE,                   // Italic
            FALSE,                   // Underline
            FALSE,                   // StrikeOut
            DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS,
            CLIP_DEFAULT_PRECIS,
            DEFAULT_QUALITY,
            DEFAULT_PITCH | FF_DONTCARE,
            L"Segoe UI Emoji"       // Font name
        );
    }
    HBRUSH ButtonColor=CreateSolidBrush(RGB(244,238,210));
    HBRUSH OldButtonColor=SelectObject(Mdc,ButtonColor);
    HFONT OldFont=SelectObject(Mdc,Font);
    RECT LeftRoundedRect;
    RoundRect(Mdc,Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.18,PanelRect.bottom + (WindowSize.right-WindowSize.left)*0.009,
    Choice_1_Button.right + (WindowSize.right-WindowSize.left)*0.7,PanelRect.bottom + (WindowSize.bottom - WindowSize.top)*0.81,
    (WindowSize.right-WindowSize.left)*0.02,(WindowSize.right-WindowSize.left)*0.02);
    if(strcmp(WorkersPicture[0],SendingTools.username) == 0)
    {
        SetWindowPos(HandleMontaPicProfile, NULL,
        (WindowSize.left + (WindowSize.right - WindowSize.left)*0.52),
        (WindowSize.top + (WindowSize.bottom - WindowSize.top) / 2 - (WindowSize.bottom - WindowSize.top) * 0.28),
        (WindowSize.right - WindowSize.left) * 0.115,
        (WindowSize.bottom - WindowSize.top) * 0.222,
        SWP_NOZORDER);  
        ShowWindow(HandleMontaPicProfile, SW_SHOW);
    }
    else if(strcmp(WorkersPicture[1],SendingTools.username) == 0)
    {
        SetWindowPos(HandleMohammedPicProfile, NULL,
        (WindowSize.left + (WindowSize.right - WindowSize.left)*0.52),
        (WindowSize.top + (WindowSize.bottom - WindowSize.top) / 2 - (WindowSize.bottom - WindowSize.top) * 0.28),
        (WindowSize.right - WindowSize.left) * 0.115,
        (WindowSize.bottom - WindowSize.top) * 0.222,
        SWP_NOZORDER);  
        ShowWindow(HandleMohammedPicProfile, SW_SHOW);
    }
    else 
    {
        ShowWindow(HandleMontaPicProfile, SW_HIDE);
        ShowWindow(HandleMohammedPicProfile, SW_HIDE);
    }
    LeftRoundedRect.left = (WindowSize.right - WindowSize.left) / 2 + (WindowSize.right - WindowSize.left)*0.02;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 - (WindowSize.bottom - WindowSize.top)*0.05;
    LeftRoundedRect.right = (WindowSize.right - WindowSize.left) / 2 + (WindowSize.right - WindowSize.left)*0.138;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.01;
    SetTextColor(Mdc,RGB(0,0,0));
    DrawText(Mdc,SendingTools.username,-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.335));
    LeftRoundedRect.right = LeftRoundedRect.left + (WindowSize.right - WindowSize.left)*0.05;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.063;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.11;
    OldFont=SelectObject(Mdc,ThirdFont);
    DrawTextW(Mdc,L"📧",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    OldFont=SelectObject(Mdc,SecondFont);
    SIZE textSize;
    GetTextExtentPoint32A(Mdc,"EMAIL ADRESS",strlen("EMAIL ADRESS"),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.372));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.0665;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.135;
    DrawTextW(Mdc,L"Email Address",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(120, 120, 120));
    GetTextExtentPoint32A(Mdc,Myinfo.email,strlen(Myinfo.email),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.349));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.11;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.16;
    DrawText(Mdc,Myinfo.email,-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(0,0,0));
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.335));
    LeftRoundedRect.right = LeftRoundedRect.left + (WindowSize.right - WindowSize.left)*0.05;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.169;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.224;
    OldFont=SelectObject(Mdc,ThirdFont);
    DrawTextW(Mdc,L"💼",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    OldFont=SelectObject(Mdc,SecondFont);
    GetTextExtentPoint32A(Mdc,"JOB TITLE",strlen("JOB TITLE"),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.372));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.1725;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.374;
    DrawText(Mdc,"Job Title",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(120, 120, 120));
    GetTextExtentPoint32A(Mdc,Myinfo.JobTitle,strlen(Myinfo.JobTitle),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.349));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.216;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.27;
    DrawText(Mdc,Myinfo.JobTitle,-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(0,0,0));
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.333));
    LeftRoundedRect.right = LeftRoundedRect.left + (WindowSize.right - WindowSize.left)*0.05;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.271;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.32;
    OldFont=SelectObject(Mdc,ThirdFont);
    DrawTextW(Mdc,L"💰",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    OldFont=SelectObject(Mdc,SecondFont);
    GetTextExtentPoint32A(Mdc,"SALARY",strlen("SALARY"),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.372));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.274;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.369;
    DrawText(Mdc,"Salary",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    char text[100];
    sprintf(text,"$%d / month",Myinfo.salarie);
    SetTextColor(Mdc,RGB(120, 120, 120));
    GetTextExtentPoint32A(Mdc,text,strlen(text),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.349));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.3175;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.3715;
    DrawText(Mdc,text,-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(0,0,0));
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.333));
    LeftRoundedRect.right = LeftRoundedRect.left + (WindowSize.right - WindowSize.left)*0.05;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.365;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.42;
    OldFont=SelectObject(Mdc,ThirdFont);
    DrawTextW(Mdc,L"📊",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    OldFont=SelectObject(Mdc,SecondFont);
    GetTextExtentPoint32A(Mdc,"EMPLOYEE STATUS",strlen("EMPLOYEE STATUS"),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.3695));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.368;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.469;
    DrawText(Mdc,"Employee Status",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);

    SetTextColor(Mdc,RGB(50, 205, 50));
    GetTextExtentPoint32A(Mdc,"ACTIVE",strlen("ACTIVE"),&textSize);
    LeftRoundedRect.left = ((WindowSize.left + (WindowSize.right - WindowSize.left)*0.349));
    LeftRoundedRect.right = LeftRoundedRect.left + textSize.cx;
    LeftRoundedRect.top = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.4115;
    LeftRoundedRect.bottom = (WindowSize.bottom - WindowSize.top) / 2 + (WindowSize.bottom - WindowSize.top)*0.4655;
    DrawText(Mdc,"Active",-1,&LeftRoundedRect,DT_SINGLELINE | DT_CENTER);
    SelectObject(Mdc, OldPen);
    DeleteObject(Pen);
    SelectObject(Mdc,OldFont);
    DeleteObject(Font);
    DeleteObject(SecondFont);
}
 