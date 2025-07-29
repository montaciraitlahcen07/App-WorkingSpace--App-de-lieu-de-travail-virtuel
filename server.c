#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <process.h>
#include <string.h>
#include <stdbool.h>
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
}Into;

int Counter = 0;
FILE *ClientsData;
// client is message
typedef struct 
{
    char Recipient[100];
    char Buffer[200];
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
unsigned __stdcall ReceivingAndPrintingData(void *param);
bool FindAndUpdateUser(const char* username,const char* PassWord, SOCKET clientSocketSending,SOCKET clientSocketReceiving,SOCKET StatusSocket, bool *isNewConnection,FILE *ClientsData,StatusType *UserStatus);
void MarkUserAsInactive(SOCKET clientSocket,SOCKET StatusSocket,FILE *ClientsData,StatusType *UserStatus,char username[30]);
void BroadcastToAllUsers(const char* senderUsername, const char* message, SOCKET senderSocket,FILE *ClientsData);
bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message,SOCKET senderSocketReceiving,FILE *ClientsData);
void SendClient(char username[20],SOCKET clientSocketc,FILE *ClientsData);
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

    struct sockaddr_in ServerSending, ClientDataSending,ServerReceiving, ClientDataReceiving,StatusData,StatusServer;
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

    printf("Server is listening on port 8000 and 8001...\n");
    
    int sizeDataClient = sizeof(ClientDataSending);
    int sizeDataClientReceiving = sizeof(ClientDataReceiving);
    int sizeDataStatus = sizeof(StatusData);
    SOCKET Client;
    SOCKET ClientR;
    SOCKET StatusClient;
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
        // this is connecting with the status socket 
        CONNECT :
        StatusClient = accept(ServerSocketStatus, (struct sockaddr *)&StatusData, &sizeDataStatus);
        if (StatusClient == INVALID_SOCKET)
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
        HANDLE ClientThread = (HANDLE)_beginthreadex(NULL, 0, ReceivingAndPrintingData,&SocketAndMessage, 0, NULL);
        if (ClientThread)
        {
            CloseHandle(ClientThread);
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
    
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1) {
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
    if (ClientsData == NULL) {
        printf("Failed to open ClientsData.txt for broadcasting\n");
        return;
    }
    Clients tempClient;
    while (fread(&tempClient, sizeof(tempClient), 1, ClientsData) == 1) {
        if (tempClient.ClientsS != senderSocket && tempClient.IsActive) {
            printf("Sending to %s\n", tempClient.Username);
            if (send(tempClient.ClientsS, senderUsername, strlen(senderUsername), 0) == SOCKET_ERROR ||
                send(tempClient.ClientsS, message, strlen(message), 0) == SOCKET_ERROR) {
                printf("Failed to send to %s (client may have disconnected)\n", tempClient.Username);
            }
        }
    }

    fclose(ClientsData); 
}

bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message,SOCKET senderSocketReceiving,FILE *ClientsData)
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
    if (resultnumber <= 0) {
        printf("Failed to receive username from client\n");
        goto cleanup;
    }
    int resultnumberP = recv(SocketAndMessage.Sockets->SendingSocket, Message.PassWord, sizeof(Message.PassWord) - 1, 0);
    if (resultnumberP <= 0) {
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
    while (TRUE)
    {
        memset(Message.GeneralPrivate, 0, sizeof(Message.GeneralPrivate));
        int resultnumberBool = recv(SocketAndMessage.Sockets->SendingSocket, Message.GeneralPrivate, sizeof(Message.GeneralPrivate) - 1, 0);
        if (resultnumberBool <= 0){
            printf("Client %s disconnected (choice receive failed)\n", Message.Username);
            break;
        }
        Message.GeneralPrivate[resultnumberBool] = '\0';
        len = strlen(Message.GeneralPrivate);
        while (len > 0 && (Message.GeneralPrivate[len-1] == '\n' || Message.GeneralPrivate[len-1] == '\r' || 
                          Message.GeneralPrivate[len-1] == '\t' || Message.GeneralPrivate[len-1] == ' ')) {
            Message.GeneralPrivate[--len] = '\0';
        }
        printf("Message type from %s: %s\n", Message.Username, Message.GeneralPrivate);
        if (strcmp(Message.GeneralPrivate, "TRUE") == 0) {
            memset(Message.Buffer, 0, sizeof(Message.Buffer));
            resultnumber = recv(SocketAndMessage.Sockets->SendingSocket, Message.Buffer, sizeof(Message.Buffer) - 1, 0);
            if (resultnumber <= 0) {
                printf("Failed to receive message from %s\n", Message.Username);
                break;
            }
            Message.Buffer[resultnumber] = '\0';
            len = strlen(Message.Buffer);
            while (len > 0 && (Message.Buffer[len-1] == '\n' || Message.Buffer[len-1] == '\r' || 
                              Message.Buffer[len-1] == '\t' || Message.Buffer[len-1] == ' ')) {
                Message.Buffer[--len] = '\0';
            }
            printf("Broadcasting message from %s: %s\n", Message.Username, Message.Buffer);
            BroadcastToAllUsers(Message.Username, Message.Buffer, SocketAndMessage.Sockets->SendingSocket,ClientsData);
        }
        else if (strcmp(Message.GeneralPrivate, "FALSE") == 0)
        {
            // for sending the user the whole list of users who are they online rn
            SendClient(Message.Username,SocketAndMessage.Sockets->SendingSocket,ClientsData);
            // receiving the recipient is name and the message 
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
            printf("Private message from %s to %s: %s\n", Message.Username, SocketAndMessage.MessageTools.Recipient,SocketAndMessage.MessageTools.Buffer);
            // sending the message into the recipient by the name of the sender
            bool messageSent = SendPrivateMessage(Message.Username,SocketAndMessage.MessageTools.Recipient,SocketAndMessage.MessageTools.Buffer,SocketAndMessage.Sockets->ReceivingSocket,ClientsData);
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