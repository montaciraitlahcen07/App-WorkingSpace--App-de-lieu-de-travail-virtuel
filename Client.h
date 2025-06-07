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
#pragma comment(lib, "ws2_32.lib")
typedef struct
{
    char username[100];
    char buffer[100];
}Client;

Client param;
int ConnectClient=0;
char Recipient[100];
char Generale[20] = "TRUE";
char Private[20] = "TRUE";



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
unsigned __stdcall receivingClient(void *param);
unsigned __stdcall SendingThread(void *param);

unsigned __stdcall receivingClient(void *param)
{
    char username[100];
    char buffer[100];
    int Result;
    
    while(TRUE)
    {
        memset(username, 0, sizeof(username));
        memset(buffer, 0, sizeof(buffer));
        
        
        Result = recv(ConnectingTools.ClientSocket, username, sizeof(username)-1, 0);
        if(Result <= 0)
        {
            break;
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
            break;
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
        DrawText(ConnectingTools.Mdc,buffer,-1,&BufferRect,DT_SINGLELINE | DT_CENTER);
        fflush(stdout);
    }
    
    return 0;
}
SndTrd MemoryDcSndTool = {0};
unsigned __stdcall SendingThread(void *param)
{
    MemoryDcSndTool = *(SndTrd *)param;
    
    int sendResult;
    //Sleep(500);
    char Buffer[100];
    while(TRUE)
    {
        char Choice[20];
        if(MemoryDcSndTool.UiInbox)
        {
            memset(Message, 0, sizeof(Message));
            i = 0;
            strcpy(Choice,"FALSE");
            sendResult = send(ConnectingTools.ClientSocket, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                break;
            }
            else
            {
                GetClientRect(MemoryDcSndTool.ScrollBar,&MemoryDcSndTool.ScrollBarRect);
                HPEN Pen=CreatePen(BS_SOLID,2,RGB(0,0,0));
                HPEN OldPen=SelectObject(MemoryDcSndTool.Mdc_Child_1,Pen);
                while(recv(ConnectingTools.ClientSocket, (char *)&Message[i], sizeof(Clients), 0) > 0)
                {
                    // incrementing the array of stocking recipient info
                    i++;
                }
                SelectObject(MemoryDcSndTool.Mdc_Child_1, OldPen);
                DeleteObject(Pen);
            }
        }
        else if(MemoryDcSndTool.UiGeneral)
        {
            strcpy(Choice,"TRUE");
            sendResult = send(ConnectingTools.ClientSocket, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                break;
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
            
            if(strcmp(Buffer, "quit") == 0)
            {
                break;
            }
            sendResult = send(ConnectingTools.ClientSocket, Buffer, strlen(Buffer), 0);
            if(sendResult == SOCKET_ERROR)
            {
                break;
            }
            
            Sleep(100);   
        }
        else if(strcmp(Choice, "FALSE") == 0)
        {
            int lenRecipient = strlen(ConnectingTools.SelectedRecipient);
            while (lenRecipient > 0 && (ConnectingTools.SelectedRecipient[lenRecipient-1] == '\n' || ConnectingTools.SelectedRecipient[lenRecipient-1] == '\r' || 
            ConnectingTools.SelectedRecipient[lenRecipient-1] == '\t' || ConnectingTools.SelectedRecipient[lenRecipient-1] == ' '))
            {
                ConnectingTools.SelectedRecipient[--lenRecipient] = '\0';
            }
            sendResult = send(ConnectingTools.ClientSocket, ConnectingTools.SelectedRecipient, strlen(ConnectingTools.SelectedRecipient), 0);
            if(sendResult == SOCKET_ERROR)
            {
                break;
            }
            // do it when i make the ui of sending message and get that message with getwindowtext
            strcpy(Buffer,"fen");
            
            int lenb = strlen(Buffer);
            while (lenb > 0 && (Buffer[lenb-1] == '\n' || Buffer[lenb-1] == '\r' || 
            Buffer[lenb-1] == '\t' || Buffer[lenb-1] == ' '))
            {
                Buffer[--lenb] = '\0';
            }
            
            if(strcmp(Buffer, "quit") == 0)
            {
                break;
            }
            sendResult = send(ConnectingTools.ClientSocket, Buffer, strlen(Buffer), 0);
            if(sendResult == SOCKET_ERROR)
            {
                break;
            }
            
            Sleep(100);
        }
    }
    
    closesocket(ConnectingTools.ClientSocket);
    WSACleanup();
    return 0;
}