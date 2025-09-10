#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#pragma comment(lib, "ws2_32.lib")

typedef struct 
{
    char Username[30];
    char PassWord[30];
    int age;
    int salarie;
    SOCKET ClientsS;
    SOCKET ClientsR;
    SOCKET StatusClients;
    bool IsActive;
}Clients;
typedef struct
{
    char Username[30];
    char PassWord[30];
    char Buffer[100];
    char Recipient[30];
    char GeneralPrivate[20];
    char ChoiceChanging[50];
}Into;

int Counter = 0;
FILE *ClientsData;
// client is message
typedef struct 
{
    char Recipient[100];
    char Buffer[200];
    struct tm TimeStamp;
    char ChoiceChanging[50];
}PrivateGeneralMessage;
// client is socket
typedef struct 
{
    SOCKET SendingSocket;
    SOCKET ReceivingSocket;
    SOCKET StatusSocket;   
}ClientSockets;
// client is socket and message
typedef struct
{
    ClientSockets *Sockets;
    PrivateGeneralMessage MessageTools;
}ClientSocketsAndTools;
ClientSocketsAndTools SocketAndMessage;
// this is for Client Status
#define ONLINE 1
#define OFFLINE 2
#define NEWUSER 3
#define REMOVEUSER 4
// to disperse between Inbox and General message
#define Inbox 5
#define General 6 
typedef struct
{
    char UserName[50];
    int Type;
    SOCKET Socket;
    SOCKET StatusSocket;
}StatusType;
// message storing struct 
typedef struct
{
    char message[200];
    char owner[50];
    int index;
    struct tm TimeStamp;
}MessageStoring;
typedef struct
{
    char message[200];
    char owner[50];
    char sender[50];
    char recipient[50];
    struct tm TimeStamp;
}ResponseData;
unsigned __stdcall ReceivingAndPrintingData(void *param);
bool FindAndUpdateUser(const char* username,const char* PassWord, SOCKET clientSocketSending,SOCKET clientSocketReceiving,SOCKET StatusSocket, bool *isNewConnection,FILE *ClientsData,StatusType *UserStatus);
void MarkUserAsInactive(SOCKET clientSocket,SOCKET StatusSocket,FILE *ClientsData,StatusType *UserStatus,char username[30]);
void BroadcastToAllUsers(const char* senderUsername, const char* message, SOCKET senderSocket,FILE *ClientsData);
bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message,SOCKET senderSocketReceiving,FILE *ClientsData,struct tm TimeStamp);
void StoringConversation(const char* Sender, const char* Recipient,const char* message,struct tm TimeStamp);
void SendClient(char username[20],SOCKET clientSocketc,FILE *ClientsData);
unsigned __stdcall ConversationThread(void *param);
int main()
{
    // Storing the users is infos
    Clients FillingTrial[7] = {{"monta","sir",19,1000,0,0,0,FALSE},{"imad","mon",29,10000,0,0,0,FALSE},{"anwar","ba",25,10000,0,0,0,FALSE},
    {"bilal","zarkal",19,1000,0,0,0,FALSE},{"aymen","sale",19,1000,0,0,0,FALSE},{"mohammed","puakki",19,1000,0,0,0,FALSE},{"omar","ait",19,1000,0,0,0,FALSE}};
    FILE *testFile = fopen("ClientsData.txt", "w+b");
    if (testFile == NULL)
    {
        printf("Failed to create/access ClientsData.txt\n");
        return 1;
    }
    for(int i=0;i<7;i++)
    {
        fwrite(&FillingTrial[i], sizeof(Clients), 1, testFile);
    }

    fclose(testFile); 

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    struct sockaddr_in ServerSending, ClientDataSending,ServerReceiving, ClientDataReceiving,StatusData,StatusServer,ConversationData,ConversationServer;
    // Sending Connection
    SOCKET ServerSocketSending = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocketSending == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    ServerSending.sin_family = AF_INET;
    ServerSending.sin_port = htons(8000);
    ServerSending.sin_addr.S_un.S_addr = INADDR_ANY;
    //ServerSending.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(ServerSocketSending, (struct sockaddr *)&ServerSending, sizeof(ServerSending)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketSending);
        WSACleanup();
        return 1;
    }
    if(listen(ServerSocketSending, 5) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketSending);
        WSACleanup();
        return 1;
    }
    // receving Connection
    SOCKET ServerSocketReceiving = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocketReceiving == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    ServerReceiving.sin_family = AF_INET;
    ServerReceiving.sin_port = htons(8001);
    ServerReceiving.sin_addr.S_un.S_addr = INADDR_ANY;
    //ServerReceiving.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(ServerSocketReceiving, (struct sockaddr *)&ServerReceiving, sizeof(ServerReceiving)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketReceiving);
        WSACleanup();
        return 1;
    }
    if(listen(ServerSocketReceiving, 5) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketReceiving);
        WSACleanup();
        return 1;
    }
    // Status Connection
    SOCKET ServerSocketStatus = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocketStatus == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    StatusServer.sin_family = AF_INET;
    StatusServer.sin_port = htons(8002);
    StatusServer.sin_addr.S_un.S_addr = INADDR_ANY;
    //StatusServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(ServerSocketStatus, (struct sockaddr *)&StatusServer, sizeof(StatusServer)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketStatus);
        WSACleanup();
        return 1;
    }

    if(listen(ServerSocketStatus, 5) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketStatus);
        WSACleanup();
        return 1;
    }
    // Conversation Connection
    SOCKET ServerSocketConversation = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocketConversation == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }
    ConversationServer.sin_family = AF_INET;
    ConversationServer.sin_port = htons(8003);
    ConversationServer.sin_addr.S_un.S_addr = INADDR_ANY;
    //ConversationServer.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
    if (bind(ServerSocketConversation, (struct sockaddr *)&ConversationServer, sizeof(ConversationServer)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketConversation);
        WSACleanup();
        return 1;
    }

    if(listen(ServerSocketConversation, 5) == SOCKET_ERROR)
    {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocketConversation);
        WSACleanup();
        return 1;
    }
    printf("Server is listening on port 8000,8001,8002 and 8003...\n");
    
    int sizeDataClient = sizeof(ClientDataSending);
    int sizeDataClientReceiving = sizeof(ClientDataReceiving);
    int sizeDataStatus = sizeof(StatusData);
    int sizeDataConversation = sizeof(ConversationData);
    SOCKET Client;
    SOCKET ClientR;
    SOCKET StatusClient;
    SOCKET ConversationClient;
    ClientsData = fopen("ClientsData.txt", "r+b");
    if(ClientsData == NULL)
    {
        ClientsData = fopen("ClientsData.txt", "w+b"); 
        if (ClientsData == NULL)
        {
            printf("Failed to open ClientsData.txt\n");
            return false;
        }
    }
    Clients ClientsStorings;
    rewind(ClientsData);
    while(fread(&ClientsStorings, sizeof(ClientsStorings), 1, ClientsData) == 1) {
        if (ClientsStorings.IsActive)
        {
            fseek(ClientsData, -sizeof(Clients), SEEK_CUR);
            ClientsStorings.IsActive = FALSE; 
            fwrite(&ClientsStorings, sizeof(ClientsStorings), 1, ClientsData);
            fflush(ClientsData); 
        }
    }
    fclose(ClientsData);
    while (TRUE)
    {
        Client = accept(ServerSocketSending, (struct sockaddr *)&ClientDataSending, &sizeDataClient);
        if (Client == INVALID_SOCKET)
        {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }
        ClientR = accept(ServerSocketReceiving, (struct sockaddr *)&ClientDataReceiving, &sizeDataClientReceiving);
        if (ClientR == INVALID_SOCKET)
        {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }
        CONNECT :
        // this is connecting with the status socket 
        StatusClient = accept(ServerSocketStatus, (struct sockaddr *)&StatusData, &sizeDataStatus);
        if (StatusClient == INVALID_SOCKET)
        {
            printf("Accept (status) failed: %d\n", WSAGetLastError());
            Sleep(50);
            goto CONNECT;
        }
        // this is connecting with the Conversation socket 
        ConversationClient = accept(ServerSocketConversation, (struct sockaddr *)&ConversationData, &sizeDataConversation);
        if (ConversationClient == INVALID_SOCKET)
        {
            printf("Accept (status) failed: %d\n", WSAGetLastError());
            Sleep(50);
            goto CONNECT;
        }
        printf("New client connected\n");
        SocketAndMessage.Sockets = (ClientSockets*)malloc(sizeof(ClientSockets));
        // this is for client socket
        SocketAndMessage.Sockets->SendingSocket = Client;
        SocketAndMessage.Sockets->ReceivingSocket = ClientR;
        // this is for status socket  
        SocketAndMessage.Sockets->StatusSocket = StatusClient;
        SOCKET *ConversatonSocket = (SOCKET *)malloc(sizeof(SOCKET ));
        *ConversatonSocket = ConversationClient;
        HANDLE ClientThread = (HANDLE)_beginthreadex(NULL, 0, ReceivingAndPrintingData,&SocketAndMessage, 0, NULL);
        HANDLE ConversationHandle = (HANDLE)_beginthreadex(NULL, 0, ConversationThread,ConversatonSocket, 0, NULL);
        if(ClientThread)
        {
            CloseHandle(ClientThread);
        }
        if(ConversationHandle)
        {
            CloseHandle(ConversationHandle);
        }
    }
    closesocket(ServerSocketSending);
    WSACleanup();
    return 0;
}
bool FindAndUpdateUser(const char* username,const char* PassWord, SOCKET clientSocketSending,SOCKET clientSocketReceiving,SOCKET StatusSocket,bool *isNewConnection,FILE *ClientsData,StatusType *UserStatus)
{
    ClientsData = fopen("ClientsData.txt", "r+b");
    if (ClientsData == NULL)
    {
        ClientsData = fopen("ClientsData.txt", "w+b"); 
        if (ClientsData == NULL)
        {
            printf("Failed to open ClientsData.txt\n");
            return false;
        }
    }
    Clients ClientsStoring;
    bool userFound = false;
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
    {
        if(strcmp(ClientsStoring.Username, username) == 0 && strcmp(ClientsStoring.PassWord, PassWord) == 0)
        {
            fseek(ClientsData, -sizeof(Clients), SEEK_CUR);
            ClientsStoring.ClientsS = clientSocketSending;
            ClientsStoring.IsActive = TRUE; 
            ClientsStoring.StatusClients = StatusSocket; 
            ClientsStoring.ClientsR = clientSocketReceiving;
            fwrite(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData);
            fflush(ClientsData); 
            printf("%s connected\n", ClientsStoring.Username);
            userFound = true;
            strcpy(UserStatus->UserName,username);
            UserStatus->Type = ONLINE;
            UserStatus->Socket = clientSocketSending;
            UserStatus->StatusSocket = StatusSocket;
            send(clientSocketSending,"Correct",strlen("Correct"),0);
            *isNewConnection = TRUE;
            rewind(ClientsData);
            while(fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
            {
                if(strcmp(ClientsStoring.Username, username) != 0 && ClientsStoring.IsActive)
                {
                    send(ClientsStoring.StatusClients,(char *)UserStatus, sizeof(StatusType),0);
                }
            }
            break;
        }
    }
    if(!userFound)
    {
        send(clientSocketSending,"Incorrect",strlen("Incorrect"),0);
    }
    fclose(ClientsData);
    return userFound;
}
void MarkUserAsInactive(SOCKET clientSocket,SOCKET StatusSocket,FILE *ClientsData,StatusType *UserStatus,char username[30])
{
    ClientsData = fopen("ClientsData.txt", "r+b");
    if (ClientsData == NULL) {
        printf("Failed to open ClientsData.txt for marking inactive\n");
        return;
    }

    Clients ClientsStoring;
    
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
    {
        if (ClientsStoring.ClientsS == clientSocket)
        {
            fseek(ClientsData, -sizeof(Clients), SEEK_CUR);
            ClientsStoring.IsActive = FALSE;
            fwrite(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData);
            fflush(ClientsData); 
            printf("User %s disconnected\n", ClientsStoring.Username);
            strcpy(UserStatus->UserName,username);
            UserStatus->Type = OFFLINE;
            UserStatus->Socket = clientSocket;
            UserStatus->StatusSocket = StatusSocket;
            rewind(ClientsData);
            while(fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
            {
                if(ClientsStoring.ClientsS != clientSocket && ClientsStoring.IsActive)
                {
                    send(ClientsStoring.StatusClients,(char *)UserStatus, sizeof(StatusType),0);
                }
            }
            break;
        }
    }

    fclose(ClientsData);
}
void BroadcastToAllUsers(const char* senderUsername, const char* message, SOCKET senderSocket,FILE *ClientsData)
{
    ClientsData = fopen("ClientsData.txt", "rb"); 
    if (ClientsData == NULL)
    {
        printf("Failed to open ClientsData.txt for broadcasting\n");
        return;
    }
    Clients tempClient;
    while (fread(&tempClient, sizeof(tempClient), 1, ClientsData) == 1)
    {
        if(tempClient.ClientsS != senderSocket && tempClient.IsActive && strcmp(tempClient.Username,senderUsername) != 0)
        {
            printf("Sending to %s\n", tempClient.Username);
            // do it in one send here and add the time stamp of the message and add a color of the user name 
            if(send(tempClient.ClientsS, senderUsername, strlen(senderUsername), 0) == SOCKET_ERROR || send(tempClient.ClientsS, message, strlen(message), 0) == SOCKET_ERROR)
            {
                printf("Failed to send to %s (client may have disconnected)\n", tempClient.Username);
            }
        }
    }

    fclose(ClientsData); 
}

bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message,SOCKET senderSocketReceiving,FILE *ClientsData,struct tm TimeStamp)
{
    ClientsData = fopen("ClientsData.txt", "rb");
    if(ClientsData == NULL)
    {
        printf("Failed to open ClientsData.txt for private message\n");
        return false;
    }
    Clients tempClient;
    typedef struct 
    {
        char Sender[50];
        char buffer[200];
        struct tm TimeStamp;
    }messaget;
    messaget messagetest;
    bool found = false;
    while(fread(&tempClient, sizeof(tempClient), 1, ClientsData) == 1)
    {
        if(strcmp(recipient,tempClient.Username) == 0 && tempClient.IsActive)
        {
            found = TRUE;
            strcpy(messagetest.Sender,senderUsername);
            strcpy(messagetest.buffer,message);
            messagetest.TimeStamp = TimeStamp;
            if(send(tempClient.ClientsR,(char *)&messagetest,sizeof(messagetest), 0) == SOCKET_ERROR)
            {
                printf("Failed to send private message header to %s\n", recipient);
            }
            else
            {
                printf("Private message has been sent to %s\n", recipient);
            }
            break;
        }
    }
    fclose(ClientsData);
    return found;
}
unsigned __stdcall ReceivingAndPrintingData(void *param)
{
    ClientSocketsAndTools SocketAndMessage = *(ClientSocketsAndTools*)param;
    Into Message;
    StatusType UserStatus;
    Authentification :
    memset(&Message, 0, sizeof(Message));
    int resultnumber = recv(SocketAndMessage.Sockets->SendingSocket, Message.Username, sizeof(Message.Username) - 1, 0);
    if (resultnumber <= 0)
    {
        printf("Failed to receive username from client\n");
        goto cleanup;
    }
    int resultnumberP = recv(SocketAndMessage.Sockets->SendingSocket, Message.PassWord, sizeof(Message.PassWord) - 1, 0);
    if (resultnumberP <= 0)
    {
        printf("Failed to receive username from client\n");
        goto cleanup;
    }
    Message.Username[resultnumber] = '\0';
    
    int len = strlen(Message.Username);
    while (len > 0 && (Message.Username[len-1] == '\n' || Message.Username[len-1] == '\r' || 
                      Message.Username[len-1] == '\t' || Message.Username[len-1] == ' ')) {
        Message.Username[--len] = '\0';
    }
    Message.PassWord[resultnumberP] = '\0';
    
    int lenP = strlen(Message.PassWord);
    while (lenP > 0 && (Message.PassWord[lenP-1] == '\n' || Message.PassWord[lenP-1] == '\r' || 
                      Message.PassWord[lenP-1] == '\t' || Message.PassWord[lenP-1] == ' ')) {
        Message.PassWord[--lenP] = '\0';
    }
    bool IsNewConnection = FALSE;
    FindAndUpdateUser(Message.Username,Message.PassWord, SocketAndMessage.Sockets->SendingSocket,SocketAndMessage.Sockets->ReceivingSocket,SocketAndMessage.Sockets->StatusSocket,&IsNewConnection,ClientsData,&UserStatus);
    if(IsNewConnection)
    {
        printf("Client username received: %s\n", Message.Username);
    }
    else
    {
        goto Authentification;
    }
    IsNewConnection = FALSE;
    while(TRUE)
    {
        ChoiceChanging :
        memset(Message.GeneralPrivate, 0, sizeof(Message.GeneralPrivate));
        int resultnumberBool = recv(SocketAndMessage.Sockets->SendingSocket, Message.GeneralPrivate, sizeof(Message.GeneralPrivate) - 1, 0);
        if (resultnumberBool <= 0)
        {
            printf("Client %s disconnected (choice receive failed)\n", Message.Username);
            break;
        }
        Message.GeneralPrivate[resultnumberBool] = '\0';
        len = strlen(Message.GeneralPrivate);
        while (len > 0 && (Message.GeneralPrivate[len-1] == '\n' || Message.GeneralPrivate[len-1] == '\r' || Message.GeneralPrivate[len-1] == '\t' || Message.GeneralPrivate[len-1] == ' '))
        {
            Message.GeneralPrivate[--len] = '\0';
        }
        printf("Message type from %s: %s\n", Message.Username, Message.GeneralPrivate);
        if (strcmp(Message.GeneralPrivate, "TRUE") == 0)
        {
            memset(Message.Buffer, 0, sizeof(Message.Buffer));
            resultnumber = recv(SocketAndMessage.Sockets->SendingSocket, Message.Buffer, sizeof(Message.Buffer) - 1, 0);
            if (resultnumber <= 0)
            {
                printf("Failed to receive message from %s\n", Message.Username);
                break;
            }
            Message.Buffer[resultnumber] = '\0';
            len = strlen(Message.Buffer);
            while (len > 0 && (Message.Buffer[len-1] == '\n' || Message.Buffer[len-1] == '\r' || Message.Buffer[len-1] == '\t' || Message.Buffer[len-1] == ' '))
            {
                Message.Buffer[--len] = '\0';
            }
            printf("Broadcasting message from %s: %s\n", Message.Username, Message.Buffer);
            BroadcastToAllUsers(Message.Username, Message.Buffer, SocketAndMessage.Sockets->SendingSocket,ClientsData);
        }
        else if (strcmp(Message.GeneralPrivate, "FALSE") == 0)
        {
            // for sending the user the whole list of users who are they online rn
            SendClient(Message.Username,SocketAndMessage.Sockets->SendingSocket,ClientsData);
            // checking the ChoiceChanging String
            len = strlen(SocketAndMessage.MessageTools.ChoiceChanging);
            while (len > 0 && (SocketAndMessage.MessageTools.ChoiceChanging[len-1] == '\n' || SocketAndMessage.MessageTools.ChoiceChanging[len-1] == '\r' || 
                              SocketAndMessage.MessageTools.ChoiceChanging[len-1] == '\t' || SocketAndMessage.MessageTools.ChoiceChanging[len-1] == ' ')) {
                SocketAndMessage.MessageTools.ChoiceChanging[--len] = '\0';
            }
            if(strcmp(SocketAndMessage.MessageTools.ChoiceChanging,"RESET") == 0)
            {
                goto ChoiceChanging;
            }
            // receiving the recipient is name and the message and the time stamp
            resultnumber = recv(SocketAndMessage.Sockets->SendingSocket,(char *)&SocketAndMessage.MessageTools,sizeof(SocketAndMessage.MessageTools), 0);
            if (resultnumber <= 0)
            {
                printf("Failed to receive private message from %s\n", Message.Username);
                break;
            }
            int lenr = strlen(SocketAndMessage.MessageTools.Recipient);
            while (lenr > 0 && (SocketAndMessage.MessageTools.Recipient[lenr-1] == '\n' || SocketAndMessage.MessageTools.Recipient[lenr-1] == '\r' || 
                               SocketAndMessage.MessageTools.Recipient[lenr-1] == '\t' || SocketAndMessage.MessageTools.Recipient[lenr-1] == ' ')) {
                SocketAndMessage.MessageTools.Recipient[--lenr] = '\0';
            }
            //SocketAndMessage.MessageTools.Buffer[resultnumber] = '\0';
            len = strlen(SocketAndMessage.MessageTools.Buffer);
            while (len > 0 && (SocketAndMessage.MessageTools.Buffer[len-1] == '\n' || SocketAndMessage.MessageTools.Buffer[len-1] == '\r' || 
                              SocketAndMessage.MessageTools.Buffer[len-1] == '\t' || SocketAndMessage.MessageTools.Buffer[len-1] == ' ')) {
                SocketAndMessage.MessageTools.Buffer[--len] = '\0';
            }
            // storing message data inside a file  
            StoringConversation(Message.Username,SocketAndMessage.MessageTools.Recipient,SocketAndMessage.MessageTools.Buffer,SocketAndMessage.MessageTools.TimeStamp);
            printf("Private message from %s to %s: %s\n", Message.Username, SocketAndMessage.MessageTools.Recipient,SocketAndMessage.MessageTools.Buffer);
            // sending the message into the recipient by the name of the sender
            bool messageSent = SendPrivateMessage(Message.Username,SocketAndMessage.MessageTools.Recipient,SocketAndMessage.MessageTools.Buffer,SocketAndMessage.Sockets->ReceivingSocket,ClientsData,SocketAndMessage.MessageTools.TimeStamp);
            if(!messageSent)
            {
                printf("Recipient %s not found or not active\n", SocketAndMessage.MessageTools.Recipient);           
            }
            Sleep(100);
        }
    }
    cleanup:
    MarkUserAsInactive(SocketAndMessage.Sockets->SendingSocket,SocketAndMessage.Sockets->StatusSocket,ClientsData,&UserStatus,Message.Username);
    closesocket(SocketAndMessage.Sockets->SendingSocket);
    closesocket(SocketAndMessage.Sockets->ReceivingSocket);
    closesocket(SocketAndMessage.Sockets->StatusSocket);
    free(param);
    return 0;
}
void SendClient(char username[20],SOCKET clientSocket,FILE *ClientsData)
{
    int count = 0;
    ClientsData = fopen("ClientsData.txt", "r+b");
    if (ClientsData == NULL)
    {
        printf("Failed to open ClientsData.txt for marking inactive\n");
        return;
    }
    rewind(ClientsData);
    Clients ClientsStoring;
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
    {
        if(strcmp(ClientsStoring.Username, username) != 0) {
            count++;
        }
    }
    send(clientSocket, (char *)&count, sizeof(int), 0);
    rewind(ClientsData);
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1)
    {
        if (strcmp(ClientsStoring.Username, username) != 0)
        {
            send(clientSocket, (char *)&ClientsStoring, sizeof(Clients), 0);    
        }
    }
    fclose(ClientsData);
}
// creating a conversation thread that will send messages of a conversation into the client 
// Fixed ConversationThread function for server.c
unsigned __stdcall ConversationThread(void *param)
{
    typedef struct
    {
        char sender[50];
        char recipient[50];
        int message_requested; 
        int index;             
        int type;
    }RequestConversation;

    typedef struct
    {
        char Sender[50];
        char Recipient[50];
        int message_count; 
        int last_index;    
        bool no_more;      
    }ResponseSetting;

    RequestConversation RequestCnv;
    ResponseSetting Response;
    SOCKET ConversationSocket = *(SOCKET *)param;
    
    while(true)
    {
        int ResultRequest = recv(ConversationSocket, (char *)&RequestCnv, sizeof(RequestConversation), 0);
        if(ResultRequest <= 0)
        {
            continue;
        }
        // Create filename for conversation file
        char result[100];
        if(strcmp(RequestCnv.sender, RequestCnv.recipient) <= 0)
        {
            strcpy(result, RequestCnv.sender);
            strcat(result, RequestCnv.recipient);
        }
        else
        {
            strcpy(result, RequestCnv.recipient);
            strcat(result, RequestCnv.sender);
        }
        
        char filename[100];
        sprintf(filename, "%s.txt", result);
        FILE *Conversation = fopen(filename, "rb");
        
        if (!Conversation)
        {
            // No conversation file exists
            strcpy(Response.Sender, RequestCnv.sender);
            strcpy(Response.Recipient, RequestCnv.recipient);
            Response.message_count = 0;
            Response.no_more = TRUE;
            Response.last_index = 0;
            send(ConversationSocket, (char *)&Response, sizeof(ResponseSetting), 0);
            continue;
        }

        // Get total number of messages in file
        fseek(Conversation, 0, SEEK_END);
        long file_size = ftell(Conversation);
        int total_messages = file_size / sizeof(MessageStoring);

        if (total_messages == 0)
        {
            // Empty conversation file
            strcpy(Response.Sender, RequestCnv.sender);
            strcpy(Response.Recipient, RequestCnv.recipient);
            Response.message_count = 0;
            Response.no_more = TRUE;
            Response.last_index = 0;
            send(ConversationSocket, (char *)&Response, sizeof(ResponseSetting), 0);
            fclose(Conversation);
            continue;
        }

        int start_index;
        int batch_size = RequestCnv.message_requested;
        
        if (RequestCnv.type == 1)
        {
            // First time opening conversation - start from newest messages
            start_index = total_messages;
        }
        else if (RequestCnv.type == 2)
        {
            // Scrolling up - start from the index client specified
            start_index = RequestCnv.index;
            if (start_index < 1) start_index = 1;
        }
        else
        {
            // Invalid type
            fclose(Conversation);
            continue;
        }
        // Limit batch size to available messages
        if (batch_size > start_index)
        {
            batch_size = start_index;
        }
        // Prepare response
        strcpy(Response.Sender, RequestCnv.sender);
        strcpy(Response.Recipient, RequestCnv.recipient);
        Response.message_count = batch_size;
        Response.last_index = start_index - batch_size;
        
        if (Response.last_index <= 0)
        {
            Response.last_index = 0;
            Response.no_more = TRUE;
        }
        else
        {
            Response.no_more = FALSE;
        }
        // Send response header
        send(ConversationSocket, (char *)&Response, sizeof(ResponseSetting), 0);
        Sleep(50); // Small delay to ensure header is received first
        // Send messages from newest to oldest in the batch
        ResponseData SendingData;
        MessageStoring ReadingData; 
        for(int j = 0; j < batch_size; j++)
        {
            int msg_index = start_index - j; // Calculate absolute message position
            if (msg_index < 1) break; // Safety check
            
            // Seek to the specific message (1-based indexing)
            fseek(Conversation, (msg_index - 1) * sizeof(MessageStoring), SEEK_SET);
            fread(&ReadingData, sizeof(MessageStoring), 1, Conversation);
            // Prepare message data
            strcpy(SendingData.message, ReadingData.message);
            strcpy(SendingData.owner, ReadingData.owner);
            strcpy(SendingData.recipient, RequestCnv.recipient);
            strcpy(SendingData.sender, RequestCnv.sender);
            SendingData.TimeStamp = ReadingData.TimeStamp;
            // Send message data
            send(ConversationSocket, (char *)&SendingData, sizeof(ResponseData), 0);
            Sleep(15); // Small delay between messages
        }
        fclose(Conversation);
    }
    closesocket(ConversationSocket);
    free(param);
    return 0;
}
// storing the message into conversation files
void StoringConversation(const char* Sender, const char* Recipient,const char* message,struct tm TimeStamp)
{
    char result[100];
    MessageStoring MessageData,ReadingData;
    if(strcmp(Sender, Recipient) <= 0)
    {
        strcpy(result, Sender);
        strcat(result, Recipient);
    }
    else
    {
        strcpy(result, Recipient);
        strcat(result, Sender);
    }
    char filename[100];
    sprintf(filename, "%s.txt",result);
    FILE *Conversation = fopen(filename,"a+");
    fseek(Conversation,0,SEEK_END);
    long file_size = ftell(Conversation);
    int long_file = file_size/sizeof(MessageStoring);
    // if the file having some data already
    if(long_file > 0)
    {
        MessageData.index = long_file + 1;
        strcpy(MessageData.owner,Sender);
        strcpy(MessageData.message,message);
        MessageData.TimeStamp = TimeStamp;
        fwrite(&MessageData,sizeof(MessageStoring),1,Conversation);
    }
    // if the file does not have any data
    else 
    {
        MessageData.index = 1;
        strcpy(MessageData.owner,Sender);
        strcpy(MessageData.message,message);
        MessageData.TimeStamp = TimeStamp;
        fwrite(&MessageData,sizeof(MessageStoring),1,Conversation);
    }
    fclose(Conversation);
}
/*// Creating a thread for ResetChoice and make it inside this thread to prevent passing it and waiting  for the cycle of sending or receving messages
unsigned __stdcall ResetChoiceThread(void *param)
{
    while(TRUE)
    {

    }
    return 0;
}*/