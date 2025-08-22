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
// for storing conversation message
typedef struct
{
    char message[200];
    char owner[50];
    // time stamp i need to add it 
}ConversationData;
typedef struct 
{
    ConversationData Conversation[100];
    int last_index;
    int count;
    char OwnerName[50];
}conversationsOwners;
conversationsOwners MessagesConversations[40];
typedef struct
{
    char sender[50];
    char recipient[50];
    int message_requested;
    int index;
    int type;
}RequestConversation;
// scroll offset we will filter the message that would be shown in the screen 
float Conversation_scrolloffset;
// request Conversation messages pass 
typedef struct
{
    char recipient[50];
    bool pass;
    bool no_more;
    bool messages_left;
}firstrequest;
firstrequest RecipientPass[40];
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
void insert_at_bottom(const char* Sender,const char* message,const char *owner);
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
            int result = recv(ConnectingTools.ClientSocketReceiving,(char *)&messagetest,sizeof(messagetest),0);
            if(result <= 0)
            {
                continue;
            }
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
            insert_at_bottom(messagetest.Sender,messagetest.buffer,messagetest.Sender);
            printf("Received message from %s: %s\n", messagetest.Sender, messagetest.buffer);

            /*HPEN Pen=CreatePen(BS_SOLID,3,RGB(0,0,0));
            HPEN OldPen=SelectObject(ConnectingTools.Mdc,Pen);
            SelectObject(ConnectingTools.Mdc,OldPen);
            DeleteObject(Pen);
            InvalidateRect(RcvStg->HandleWnd,NULL,FALSE);*/
        }
    }
    closesocket(ConnectingTools.ClientSocketReceiving);
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
        if(MemoryDcSndTool.UiInbox)
        {  
            i = 0;
            countclient = 0;
            strcpy(Choice,"FALSE");
            sendResult = send(ConnectingTools.ClientSocketSending, Choice, strlen(Choice), 0);
            if(sendResult == SOCKET_ERROR)
            {
                continue;
            } 
            else
            {
                GetClientRect(MemoryDcSndTool.ScrollBar,&MemoryDcSndTool.ScrollBarRect);
                HPEN Pen=CreatePen(BS_SOLID,2,RGB(0,0,0));
                HPEN OldPen=SelectObject(MemoryDcSndTool.Mdc_Child_1,Pen);
                recv(ConnectingTools.ClientSocketSending,(char *)&countclient,sizeof(countclient),0);
                // Ensure countclient doesn't exceed array bounds
                if(countclient > 40) countclient = 40;
                countclientStatus = countclient;
                for(int j=0;j<countclient;j++)
                {
                    recv(ConnectingTools.ClientSocketSending, (char *)&Message[j], sizeof(Clients), 0);
                    // incrementing the array of stocking recipient info
                    i++;
                    // naming the array each users get his own array 
                    strcpy(MessagesConversations[j].OwnerName,Message[j].Username);
                    strcpy(RecipientPass[j].recipient,Message[j].Username);
                }
                SelectObject(MemoryDcSndTool.Mdc_Child_1, OldPen);
                DeleteObject(Pen);
            }
        }
        else if(MemoryDcSndTool.UiGeneral)
        {
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
            GETBACK :
            if(!Send)
            {
                Sleep(100);
                goto GETBACK;
            }
            int lenRecipient = strlen(ConnectingTools.PrivateMessage.SelectedRecipient);
            while(lenRecipient > 0 && (ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\n' || ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\r' || 
            ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == '\t' || ConnectingTools.PrivateMessage.SelectedRecipient[lenRecipient-1] == ' '))
            {
                ConnectingTools.PrivateMessage.SelectedRecipient[--lenRecipient] = '\0';
            }
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
                // storing the message in the array of the conversation
                insert_at_bottom(ConnectingTools.PrivateMessage.SelectedRecipient,ConnectingTools.PrivateMessage.Buffer,SendingTools.username);
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
            // Ensure countclientStatus doesn't exceed array bounds
            if(countclientStatus < 100)
            {
                strcpy(Message[countclientStatus].Username,UserStatus.UserName);
                Message[countclientStatus].Clients = UserStatus.socket;
                Message[countclientStatus].IsActive = TRUE;
                Message[countclientStatus].StatusClient = UserStatus.Statussocket;
                countclientStatus++;
            }
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
// creating a conversation thread
unsigned __stdcall ConversationThread(void *param)
{
    typedef struct 
    {
        char Sender[50];
        char Recipient[50];
        int message_count;
        int last_index;
        bool no_more;
    } ResponseSetting;

    ResponseSetting Response;
    SOCKET ConversationSocket = *(SOCKET *)param;

    while (true)
    {
        int ConverResult = recv(ConversationSocket, (char *)&Response, sizeof(ResponseSetting), 0);
        if (ConverResult <= 0)
            break; // socket closed or error

        // Find the conversation index
        int convIndex = -1;
        for (int k = 0; k < countclient; k++)
        {
            if (strcmp(MessagesConversations[k].OwnerName, Response.Recipient) == 0)
            {
                convIndex = k;
                break;
            }
        }
        if (convIndex == -1)
            continue; // no matching conversation

        if(Response.message_count == 0)
        {
            MessagesConversations[convIndex].last_index = 0;
            for (int j = 0; j < countclient; j++)
            {
                if (strcmp(RecipientPass[j].recipient, Response.Recipient) == 0)
                {
                    RecipientPass[j].no_more = TRUE;
                    break;
                }
            }
        }
        else if (Response.message_count > 0)
        {
            typedef struct
            {
                char message[200];
                char owner[50];
                char sender[50];
                char recipient[50];
            } ResponseData;

            ResponseData SendingData;
            // Limit message count to prevent excessive processing
            int maxMessages = min(Response.message_count, 50);
            for (int j = 0; j < maxMessages; j++)
            {
                int resultr = recv(ConversationSocket, (char *)&SendingData, sizeof(ResponseData), 0);
                /*// Prevent duplicate oldest message
                if (MessagesConversations[convIndex].count > 0)
                {
                    ConversationData *lastMsg =
                        &MessagesConversations[convIndex].Conversation[MessagesConversations[convIndex].count - 1];
                    if (strcmp(lastMsg->message, SendingData.message) == 0 &&
                        strcmp(lastMsg->owner, SendingData.owner) == 0)
                    {
                        continue;
                    }
                }*/
                // Store new message with bounds checking
                if(MessagesConversations[convIndex].count < 100)
                {
                    ConversationData NewData;
                    strcpy(NewData.message, SendingData.message);
                    strcpy(NewData.owner, SendingData.owner);
                    MessagesConversations[convIndex].Conversation[MessagesConversations[convIndex].count] = NewData;
                    MessagesConversations[convIndex].count++;
                }
            }
            // Update scrolling state
            MessagesConversations[convIndex].last_index = Response.last_index;
            for (int j = 0; j < countclient; j++)
            {
                if (strcmp(RecipientPass[j].recipient, Response.Recipient) == 0)
                {
                    RecipientPass[j].no_more = Response.no_more;
                    break;
                }
            }
        }
    }
    closesocket(ConversationSocket);
    return 0;
}
// inserting new message at the bottom of the array
void insert_at_bottom(const char* Sender,const char* message,const char * owner)
{
    // Safety checks for null pointers
    if(!Sender || !message || !owner)
        return;
        
    for(int i=0;i<countclient && i<40;i++)
    {
        if(strcmp(MessagesConversations[i].OwnerName,Sender) == 0)
        {
            if(MessagesConversations[i].count >= 100)
            {
                printf("Message limit reached for %s array\n", Sender);
                return;
            }
            ConversationData NewData;
            strcpy(NewData.owner,owner);
            strcpy(NewData.message,message);
            for(int j=MessagesConversations[i].count;j>0;j--)
            {
                MessagesConversations[i].Conversation[j] = MessagesConversations[i].Conversation[j-1];
            }
            MessagesConversations[i].Conversation[0] = NewData;
            MessagesConversations[i].count++;
            break;
        }
    }
}