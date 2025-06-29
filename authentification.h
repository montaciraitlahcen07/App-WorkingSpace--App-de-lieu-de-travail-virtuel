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
    char username[30];
    char password[30];
}SndTrd;
SndTrd SendingTools;
typedef struct 
{
    struct sockaddr_in Server;
    SOCKET ClientSocket;
    struct sockaddr_in ServerStatus;
    SOCKET StatusSocket;
    RECT WindowSize;
    HDC Mdc;
    char SelectedRecipient[100];
}CntTrd;
CntTrd ConnectingTools;
//
void Authentification(HWND ULogin,HWND PLogin,FILE *UserData_2,HBRUSH creme,RECT WindowSize,HDC Mdc,HWND HandleWnd,SndTrd *SendingTools,CntTrd ConnectingTools)
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
    strcpy(SendingTools->password,password);
    Workers WorkerData;
    char Authentification[30];
    UserData_2=fopen("usersdata.txt","rb+");
    int sendResult = send(ConnectingTools.ClientSocket,SendingTools->username, strlen(SendingTools->username), 0);
    if(sendResult == SOCKET_ERROR)
    {
        return ;
    }
    Sleep(10);
    int sendResultP = send(ConnectingTools.ClientSocket,SendingTools->password, strlen(SendingTools->password), 0);
    if(sendResultP == SOCKET_ERROR)
    {
        return ;
    }

    int Result = recv(ConnectingTools.ClientSocket, Authentification, sizeof(Authentification)-1, 0);
    if(Result <= 0)
    {
        printf("the authentification did not been sent from the server\n");
    }
    Authentification[Result] = '\0';
    int lena = strlen(Authentification);
    while(lena > 0 && (Authentification[lena-1] == '\n' || Authentification[lena-1] == '\r' || 
    Authentification[lena-1] == '\t' || Authentification[lena-1] == ' '))
    {
        Authentification[--lena] = '\0';
    }
    if(strcmp("Correct",Authentification) == 0)
    {
        Find = TRUE;
    }
    else
    {
        Find = FALSE;
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
}