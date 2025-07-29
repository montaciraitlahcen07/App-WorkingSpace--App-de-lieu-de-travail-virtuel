#include <windows.h>
#include <windowsx.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <string.h>
#include <stdbool.h>
#include <malloc.h>
#include <unistd.h>
#pragma comment(lib, "ws2_32.lib")
typedef struct
{
    char username[100];
    char buffer[100];
}Client;
Client param;
int ConnectClient=0;
char Recipient[100];
// this is for storing all the clients is info
typedef struct 
{
    char Username[30];
    char PassWord[30];
    int age;
    int salarie;
    SOCKET Clients;
    SOCKET ClientsSending;
    SOCKET StatusClient;
    bool IsActive;
}Clients;
Clients Message[100];
int i=0;
int CreatingThreads = 0;
//
CRITICAL_SECTION socketLock;
//
typedef struct
{
    int recipient_index;     // Index in the Message array
    RECT item_rect;         // Actual rendered rectangle
    RECT text_rect;         // Text area rectangle
    RECT status_rect;       // Status indicator rectangle
    bool is_visible;        // Currently visible flag
} VisibleItem;
VisibleItem visible_items[100];
int visible_item_count = 0;
typedef struct
{
    bool UiInbox;
    bool UiGeneral; 
    HWND HandleWnd;
}RcvSetting;
RcvSetting RcvStg;
unsigned __stdcall receivingClient(void *param);
unsigned __stdcall SendingThread(void *param);
int FillingSearchRecipientList(HWND HandleSearch,int countclient,Clients Message[100],int ListSearchedRecipient[100],int CompSearchedRecipient);
// add in this thread function to disperse between inbox message and general message
unsigned __stdcall receivingClient(void *param)
{
    typedef struct
    {
        bool UiInbox;
        bool UiGeneral; 
        HWND HandleWnd;
    }RcvSetting;
    RcvSetting *RcvStg = (RcvSetting*)param;
    char username[100];
    char buffer[100];
    int Result;
    while(TRUE)
    {
        if(RcvStg->UiInbox)
        { 
            typedef struct 
            {
                char Sender[50];
                char buffer[200];
            }messaget;
            messaget messagetest;
            recv(ConnectingTools.ClientSocketReceiving,(char *)&messagetest,sizeof(messagetest),0);
            Result = strlen(messagetest.Sender);
            messagetest.Sender[Result] = '\0';
            int lena = strlen(messagetest.Sender);
            while(lena > 0 && (messagetest.Sender[lena-1] == '\n' || messagetest.Sender[lena-1] == '\r' || 
            messagetest.Sender[lena-1] == '\t' || messagetest.Sender[lena-1] == ' '))
            {
                messagetest.Sender[--lena] = '\0';
            }
            Result = strlen(messagetest.buffer);
            messagetest.buffer[Result] = '\0';
            int len = strlen(messagetest.buffer);
            while (len > 0 && (messagetest.buffer[len-1] == '\n' || messagetest.buffer[len-1] == '\r' || 
            messagetest.buffer[len-1] == '\t' || messagetest.buffer[len-1] == ' '))
            {
                messagetest.buffer[--len] = '\0';
            }
            printf("Received message from %s: %s\n", messagetest.Sender, messagetest.buffer);
            HPEN Pen=CreatePen(BS_SOLID,3,RGB(0,0,0));
            HPEN OldPen=SelectObject(ConnectingTools.Mdc,Pen);
            RECT BufferRect;
            BufferRect.left = ConnectingTools.WindowSize.right - (ConnectingTools.WindowSize.right - ConnectingTools.WindowSize.left)*0.05;
            BufferRect.top = ConnectingTools.WindowSize.top + (ConnectingTools.WindowSize.bottom - ConnectingTools.WindowSize.top)/2;
            BufferRect.right = BufferRect.left + (ConnectingTools.WindowSize.right - ConnectingTools.WindowSize.left)*0.1;
            BufferRect.bottom = BufferRect.top + (ConnectingTools.WindowSize.bottom - ConnectingTools.WindowSize.top)*0.5;
            DrawText(ConnectingTools.Mdc,"TETS",-1,&BufferRect,DT_SINGLELINE | DT_CENTER);
            SelectObject(ConnectingTools.Mdc,OldPen);
            DeleteObject(Pen);
            InvalidateRect(RcvStg->HandleWnd,NULL,FALSE);
        }
    }
    return 0;
}
SndTrd MemoryDcSndTool = {0};
int countclient = 0;
int countclientStatus = 0;
bool Send = FALSE;
unsigned __stdcall SendingThread(void *param)
{
    MemoryDcSndTool = *(SndTrd *)param;
    countclient = 0;
    int sendResult;
    char Buffer[100];
    while(TRUE)
    {
        char Choice[20];
        memset(Choice, 0, sizeof(Choice));
        //EnterCriticalSection(&socketLock);
        if(MemoryDcSndTool.UiInbox)
        {  
            i = 0;
            countclient = 0;
            strcpy(Choice,"FALSE");
            //RcvStg.WaitingUserList = TRUE;
            sendResult = send(ConnectingTools.ClientSocketSending, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                //RcvStg.WaitingUserList = FALSE;
                continue;
            } 
            else
            {
                GetClientRect(MemoryDcSndTool.ScrollBar,&MemoryDcSndTool.ScrollBarRect);
                HPEN Pen=CreatePen(BS_SOLID,2,RGB(0,0,0));
                HPEN OldPen=SelectObject(MemoryDcSndTool.Mdc_Child_1,Pen);
                recv(ConnectingTools.ClientSocketSending,(char *)&countclient,sizeof(countclient),0);
                countclientStatus = countclient;
                for(int j=0;j<countclient;j++)
                {
                    recv(ConnectingTools.ClientSocketSending, (char *)&Message[j], sizeof(Clients), 0);
                    // incrementing the array of stocking recipient info
                    i++;
                }
                //RcvStg.WaitingUserList = FALSE;
                SelectObject(MemoryDcSndTool.Mdc_Child_1, OldPen);
                DeleteObject(Pen);
            }
            //LeaveCriticalSection(&socketLock);
        }
        else if(MemoryDcSndTool.UiGeneral)
        {
            //RcvStg.WaitingUserList = FALSE;
            strcpy(Choice,"TRUE");
            sendResult = send(ConnectingTools.ClientSocketSending, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
        }
        if(strcmp(Choice,"TRUE") == 0)
        {
            strcpy(Buffer,"hi");
            
            int lenb = strlen(Buffer);
            while (lenb > 0 && (Buffer[lenb-1] == '\n' || Buffer[lenb-1] == '\r' || 
            Buffer[lenb-1] == '\t' || Buffer[lenb-1] == ' '))
            {
                Buffer[--lenb] = '\0';
            }
            
            sendResult = send(ConnectingTools.ClientSocketSending, Buffer, strlen(Buffer), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
            
            Sleep(100);   
        }
        else if(strcmp(Choice,"FALSE") == 0)
        {
            //RcvStg.WaitingUserList = FALSE;
            GETBACK :
            if(!Send)
            {
                Sleep(100);
                goto GETBACK;
            }
            // i need here to send the recipient name into the server to send to me the file of our conversation (i will add this feature after i try simple way)
            int lenRecipient = strlen(ConnectingTools.PrivateMessage.SelectedRecipient);
            while(lenRecipient > 0 && (ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\n' || ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\r' || 
            ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\t' || ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == ' '))
            {
                ConnectingTools.PrivateMessage.SelectedRecipient[--lenRecipient] = '\0';
            }
            /*sendResult = send(ConnectingTools.ClientSocketSending, ConnectingTools.SelectedRecipient, strlen(ConnectingTools.SelectedRecipient), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }*/
           // taking the message from message bar
            GetWindowText(SendingTools.MessageBarHandle,ConnectingTools.PrivateMessage.Buffer,sizeof(ConnectingTools.PrivateMessage.Buffer));
            int lenb = strlen(ConnectingTools.PrivateMessage.Buffer);
            while (lenb > 0 && (ConnectingTools.PrivateMessage.Buffer[lenb-1] == '\n' || ConnectingTools.PrivateMessage.Buffer[lenb-1] == '\r' || 
            ConnectingTools.PrivateMessage.Buffer[lenb-1] == '\t' || ConnectingTools.PrivateMessage.Buffer[lenb-1] == ' '))
            {
                ConnectingTools.PrivateMessage.Buffer[--lenb] = '\0';
            }
            if(lenb !=0)
            {
                sendResult = send(ConnectingTools.ClientSocketSending,(char*)&ConnectingTools.PrivateMessage, sizeof(ConnectingTools.PrivateMessage), 0);
                if(sendResult == SOCKET_ERROR)
                {
                    continue;
                }
                SetWindowText(SendingTools.MessageBarHandle,"");
            }
            Send = FALSE;
            Sleep(100);
        }
    }
    
    closesocket(ConnectingTools.ClientSocketSending);
    WSACleanup();
    return 0;
}
// this thread is for keep checking the status of the user weather if they are on line or off line or new user or reconnected
typedef struct
{
    char UserName[50];
    int Type;
    SOCKET socket;
    SOCKET Statussocket;
}StatusType;
typedef struct
{
    HWND ScrollBar;
    RECT ScrolBarRect;
}StatusTools;
StatusType UserStatus;
StatusTools SendingTrdStatus;
unsigned __stdcall StatusThread(void *param)
{
    StatusTools ScrollBarAndRect = *(StatusTools *)param;
    while(TRUE)
    {
       int ServerResult = recv(ConnectingTools.StatusSocket, (char *)&UserStatus, sizeof(UserStatus),0);        
       if(ServerResult <= 0)
       {
            Sleep(50);
            continue;
       }
       if(UserStatus.Type == 1)
       {
            int j = 0;
            while(j < 8)
            {
                if(strcmp(Message[j].Username,UserStatus.UserName) == 0)
                {
                    Message[j].IsActive = TRUE;
                    Message[j].Clients = UserStatus.socket;
                    Message[j].StatusClient = UserStatus.Statussocket;
                    break;
                }
                j++;
            }
       }
       else if(UserStatus.Type == 2)
       {
            int j = 0;
            while(j < 8)
            {
                if(strcmp(Message[j].Username,UserStatus.UserName) == 0)
                {
                    Message[j].IsActive = FALSE;
                    Message[j].Clients = 0;
                    Message[j].StatusClient = 0;
                    break;
                }
                j++;
            }
       }
       else
       {
            strcpy(Message[countclientStatus].Username,UserStatus.UserName);
            Message[countclientStatus].Clients = UserStatus.socket;
            Message[countclientStatus].IsActive = TRUE;
            Message[countclientStatus].StatusClient = UserStatus.Statussocket;
            countclientStatus++;
       }
       InvalidateRect(ScrollBarAndRect.ScrollBar,&ScrollBarAndRect.ScrolBarRect,FALSE);
       
    }

    closesocket(ConnectingTools.StatusSocket);
    return 0;
}
// this is for using the bar search for a recipient 
int FillingSearchRecipientList(HWND HandleSearch,int countclientStatus,Clients Message[100],int ListSearchedRecipient[100],int CompSearchedRecipient)
{
    CompSearchedRecipient = 0;
    char SearchedRecipient[100];
    GetWindowText(HandleSearch,SearchedRecipient,sizeof(SearchedRecipient));
    for(int i=0;i<countclientStatus;i++)
    {
        if(strstr(Message[i].Username,SearchedRecipient) != 0)
        {
            ListSearchedRecipient[CompSearchedRecipient++] = i;           
        }
    }
    return CompSearchedRecipient;
}