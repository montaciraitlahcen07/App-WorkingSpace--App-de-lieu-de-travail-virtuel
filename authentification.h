#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>
char username[50];
char password[30];
RECT Autorisa;
bool Find=FALSE;
// id of the timer
#define TimerLogIn 1002
// this is for client file 
typedef struct 
{
    HDC Mdc_Child_1;
    bool UiInbox;
    bool UiGeneral;
    RECT ScrollBarRect;
    RECT WindowSize;
    HWND ScrollBar;
    char username[100];
}SndTrd;
SndTrd SendingTools;
typedef struct 
{
    struct sockaddr_in Server;
    SOCKET ClientSocket;
    RECT WindowSize;
    HDC Mdc;
    char SelectedRecipient[100];
}CntTrd;
CntTrd ConnectingTools;
//
void Authentifaction(HWND ULogin,HWND PLogin,FILE *UserData_2,HBRUSH creme,RECT WindowSize,HDC Mdc,HWND HandleWnd,SndTrd *SendingTools,CntTrd ConnectingTools)
{
    typedef struct
    {
        char UserName[25];
        char PassWord[25];
        char FirstName[20];
        char LastName[20];
        int age;
        int salarie;
    }Workers;
    GetWindowText(ULogin,username,sizeof(username));
    GetWindowText(PLogin,password,sizeof(password));
    // take a copie for the client file
    strcpy(SendingTools->username,username);
    Workers WorkerData;
    UserData_2=fopen("usersdata.txt","rb+");
    rewind(UserData_2);
    if(UserData_2)
    {
        while(fread(&WorkerData,sizeof(WorkerData),1,UserData_2) == 1)
        {
            if(strcmp(username,WorkerData.UserName) == 0 && strcmp(password,WorkerData.PassWord) == 0)
            {
                // this is for the client is file sending user name in to the server center server
                int sendResult = send(ConnectingTools.ClientSocket,SendingTools->username, strlen(SendingTools->username), 0);
                if(sendResult == SOCKET_ERROR)
                {
                    return ;
                }
                Find=TRUE;
                break;
            }
        }
        if(Find)
        {
            HFONT Font=CreateFont(22,9,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
            DEFAULT_PITCH|FF_SWISS,"Arial");
            HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
            Autorisa.left=WindowSize.left+(WindowSize.right/2)-70;
            Autorisa.top=WindowSize.top+(WindowSize.bottom/2)+150;
            Autorisa.right=Autorisa.left+140;
            Autorisa.bottom=Autorisa.top+100;
            HPEN Pen=CreatePen(PS_DOT,1,RGB(244, 239, 206));
            HBRUSH Brush=CreateSolidBrush(RGB(244, 239, 206));
            HPEN OldPen=SelectObject(Mdc,Pen);
            HBRUSH OldBrush=SelectObject(Mdc,Brush);
            RoundRect(Mdc,Autorisa.left-30,Autorisa.top,Autorisa.right+30,Autorisa.bottom-30,40,30);
            RECT CheckText=Autorisa;
            CheckText.top=CheckText.top+20;
            SetTimer(HandleWnd,TimerLogIn,2000,NULL);
            DrawText(Mdc,"Correct",-1,&CheckText,DT_SINGLELINE | DT_CENTER | HS_HORIZONTAL | HS_VERTICAL);
            SelectObject(Mdc,OldFont);
            SelectObject(Mdc,OldPen);
            SelectObject(Mdc,OldBrush);
            DeleteObject(Font);
            DeleteObject(Brush);
            DeleteObject(Pen);
            Find=FALSE;
        }
        else
        {
            HFONT Font=CreateFont(22,9,0,0,FW_NORMAL,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_DEFAULT_PRECIS,CLIP_DEFAULT_PRECIS,DEFAULT_QUALITY,
            DEFAULT_PITCH|FF_SWISS,"Arial");
            HFONT OldFont=(HFONT)SelectObject(Mdc,Font);
            Autorisa.left=WindowSize.left+(WindowSize.right/2)-70;
            Autorisa.top=WindowSize.top+(WindowSize.bottom/2)+150;
            Autorisa.right=Autorisa.left+140;
            Autorisa.bottom=Autorisa.top+100;
            HPEN Pen=CreatePen(PS_DOT,1,RGB(244, 239, 206));
            HBRUSH Brush=CreateSolidBrush(RGB(244, 239, 206));
            HPEN OldPen=SelectObject(Mdc,Pen);
            HBRUSH OldBrush=SelectObject(Mdc,Brush);
            RoundRect(Mdc,Autorisa.left-30,Autorisa.top,Autorisa.right+30,Autorisa.bottom-30,40,30);
            RECT CheckText=Autorisa;
            CheckText.top=CheckText.top+20;
            DrawText(Mdc,"Incorrect",-1,&CheckText,DT_SINGLELINE | DT_CENTER | HS_HORIZONTAL | HS_VERTICAL);
            SelectObject(Mdc,OldFont);
            SelectObject(Mdc,OldPen);
            SelectObject(Mdc,OldBrush);
            DeleteObject(Font);
            DeleteObject(Brush);
            DeleteObject(Pen);
        }
        fclose(UserData_2);
    }
    
}