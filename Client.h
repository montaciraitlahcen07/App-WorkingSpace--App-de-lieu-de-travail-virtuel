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
    char Username[100];
    SOCKET Clients;
    bool IsActive;
}Clients;
Clients Message[100];
int i=0;
int CreatingThreads = 0;
//
CRITICAL_SECTION socketLock;
//
unsigned __stdcall receivingClient(void *param);
unsigned __stdcall SendingThread(void *param);

unsigned __stdcall receivingClient(void *param)
{
    char username[100];
    char buffer[100];
    int Result;
    
    while(TRUE)
    {
        EnterCriticalSection(&socketLock);
        if(*(bool *)param)
        {
            LeaveCriticalSection(&socketLock);
            Sleep(50);
            continue;
        }
        memset(username, 0, sizeof(username));
        memset(buffer, 0, sizeof(buffer));

        Result = recv(ConnectingTools.ClientSocket, username, sizeof(username)-1, 0);
        if(Result <= 0)
        {
            continue;
        }
        
        username[Result] = '\0';
        
        int lena = strlen(username);
        while(lena > 0 && (username[lena-1] == '\n' || username[lena-1] == '\r' || 
        username[lena-1] == '\t' || username[lena-1] == ' '))
        {
            username[--lena] = '\0';
        }
        
        
        Result = recv(ConnectingTools.ClientSocket, buffer, sizeof(buffer)-1, 0);
        if(Result <= 0)
        {
            continue;
        }
        
        buffer[Result] = '\0';
        
        int len = strlen(buffer);
        while (len > 0 && (buffer[len-1] == '\n' || buffer[len-1] == '\r' || 
                          buffer[len-1] == '\t' || buffer[len-1] == ' '))
        {
            buffer[--len] = '\0';
        }
        RECT BufferRect;
        BufferRect.left = ConnectingTools.WindowSize.left + (ConnectingTools.WindowSize.right - ConnectingTools.WindowSize.left)/2 + (ConnectingTools.WindowSize.right - ConnectingTools.WindowSize.left)*0.07;
        BufferRect.top = ConnectingTools.WindowSize.top + (ConnectingTools.WindowSize.bottom - ConnectingTools.WindowSize.top)/2;
        BufferRect.right = BufferRect.left + (ConnectingTools.WindowSize.right - ConnectingTools.WindowSize.left)*0.1;
        BufferRect.bottom = BufferRect.top + (ConnectingTools.WindowSize.bottom - ConnectingTools.WindowSize.top)*0.5;
        //DrawText(ConnectingTools.Mdc,buffer,-1,&BufferRect,DT_SINGLELINE | DT_CENTER);
        //fflush(stdout);
        LeaveCriticalSection(&socketLock);
    }
    
    return 0;
}
SndTrd MemoryDcSndTool = {0};
bool WaitingUserList = FALSE;
int countclient = 0;
bool Send = FALSE;
unsigned __stdcall SendingThread(void *param)
{
    MemoryDcSndTool = *(SndTrd *)param;
    countclient = 0;
    int sendResult;
    char Buffer[100];
    while(TRUE)
    {
        EnterCriticalSection(&socketLock);
        char Choice[20];
        memset(Message, 0, sizeof(Message));
        memset(Choice, 0, sizeof(Choice));
        if(MemoryDcSndTool.UiInbox)
        {  
            i = 0;
            countclient = 0;
            strcpy(Choice,"FALSE");
            WaitingUserList = TRUE;
            sendResult = send(ConnectingTools.ClientSocket, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                WaitingUserList = FALSE;
                continue;
            } 
            else
            {
                GetClientRect(MemoryDcSndTool.ScrollBar,&MemoryDcSndTool.ScrollBarRect);
                HPEN Pen=CreatePen(BS_SOLID,2,RGB(0,0,0));
                HPEN OldPen=SelectObject(MemoryDcSndTool.Mdc_Child_1,Pen);
                recv(ConnectingTools.ClientSocket,(char *)&countclient,sizeof(countclient),0);
                for(int j=0;j<countclient;j++)
                {
                    recv(ConnectingTools.ClientSocket, (char *)&Message[j], sizeof(Clients), 0);
                    // incrementing the array of stocking recipient info
                    i++;
                }
                LeaveCriticalSection(&socketLock);
                WaitingUserList = FALSE;
                SelectObject(MemoryDcSndTool.Mdc_Child_1, OldPen);
                DeleteObject(Pen);
            }
        }
        else if(MemoryDcSndTool.UiGeneral)
        {
            WaitingUserList = FALSE;
            strcpy(Choice,"TRUE");
            sendResult = send(ConnectingTools.ClientSocket, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
        }
        
        if(strcmp(Choice, "TRUE") == 0)
        {
            strcpy(Buffer,"hi");
            
            int lenb = strlen(Buffer);
            while (lenb > 0 && (Buffer[lenb-1] == '\n' || Buffer[lenb-1] == '\r' || 
            Buffer[lenb-1] == '\t' || Buffer[lenb-1] == ' '))
            {
                Buffer[--lenb] = '\0';
            }
            
            sendResult = send(ConnectingTools.ClientSocket, Buffer, strlen(Buffer), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
            
            Sleep(100);   
        }
        else if(strcmp(Choice, "FALSE") == 0)
        {
            GETBACK :
            if(!Send)
            {
                Sleep(50);
                goto GETBACK;
            }
            int lenRecipient = strlen(ConnectingTools.SelectedRecipient);
            while (lenRecipient > 0 && (ConnectingTools.SelectedRecipient[lenRecipient-1] == '\n' || ConnectingTools.SelectedRecipient[lenRecipient-1] == '\r' || 
            ConnectingTools.SelectedRecipient[lenRecipient-1] == '\t' || ConnectingTools.SelectedRecipient[lenRecipient-1] == ' '))
            {
                ConnectingTools.SelectedRecipient[--lenRecipient] = '\0';
            }
            sendResult = send(ConnectingTools.ClientSocket, ConnectingTools.SelectedRecipient, strlen(ConnectingTools.SelectedRecipient), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
            // do it when i make the ui of sending message and get that message with getwindowtext
            strcpy(Buffer,"fen");
            
            int lenb = strlen(Buffer);
            while (lenb > 0 && (Buffer[lenb-1] == '\n' || Buffer[lenb-1] == '\r' || 
            Buffer[lenb-1] == '\t' || Buffer[lenb-1] == ' '))
            {
                Buffer[--lenb] = '\0';
            }
            sendResult = send(ConnectingTools.ClientSocket, Buffer, strlen(Buffer), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            }
            Send = FALSE;
            Sleep(100);
        }
    }
    
    closesocket(ConnectingTools.ClientSocket);
    WSACleanup();
    return 0;
}