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
    char Username[100];
    SOCKET Clients;
    bool IsActive;
} Clients;

typedef struct
{
    char Username[100];
    char Buffer[100];
    char Recipient[100];
    char GeneralPrivate[20];
} Into;

int Counter = 0;
FILE *ClientsData;
unsigned __stdcall ReceivingAndPrintingData(void *param);

bool FindAndUpdateUser(const char* username, SOCKET clientSocket, bool isNewConnection,FILE *ClientsData);
void MarkUserAsInactive(SOCKET clientSocket,FILE *ClientsData);
void BroadcastToAllUsers(const char* senderUsername, const char* message, SOCKET senderSocket,FILE *ClientsData);
bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message, SOCKET senderSocket,FILE *ClientsData);
void SendClient(char username[20],SOCKET clientSocketc,FILE *ClientsData);
int main()
{
    FILE *testFile = fopen("ClientsData.txt", "a+b");
    if (testFile == NULL) {
        printf("Failed to create/access ClientsData.txt\n");
        return 1;
    }
    fclose(testFile); 

    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }

    struct sockaddr_in Server, ClientData;
    SOCKET ServerSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ServerSocket == INVALID_SOCKET) {
        printf("Socket creation failed\n");
        WSACleanup();
        return 1;
    }

    Server.sin_family = AF_INET;
    Server.sin_port = htons(2000);
    Server.sin_addr.S_un.S_addr = INADDR_ANY;

    if (bind(ServerSocket, (struct sockaddr *)&Server, sizeof(Server)) == SOCKET_ERROR) {
        printf("Bind failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }

    if (listen(ServerSocket, 5) == SOCKET_ERROR) {
        printf("Listen failed with error: %d\n", WSAGetLastError());
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }

    printf("Server is listening on port 2000...\n");
    
    int sizeData = sizeof(ClientData);
    SOCKET Client;

    while (TRUE) {
        Client = accept(ServerSocket, (struct sockaddr *)&ClientData, &sizeData);
        if (Client == INVALID_SOCKET) {
            printf("Accept failed: %d\n", WSAGetLastError());
            continue;
        }

        printf("New client connected\n");
        
        SOCKET *ClientAddress = (SOCKET *)malloc(sizeof(SOCKET));
        *ClientAddress = Client;
        
        HANDLE ClientThread = (HANDLE)_beginthreadex(NULL, 0, ReceivingAndPrintingData, ClientAddress, 0, NULL);
        if (ClientThread) {
            CloseHandle(ClientThread);
        }
    }

    closesocket(ServerSocket);
    WSACleanup();
    return 0;
}

bool FindAndUpdateUser(const char* username, SOCKET clientSocket, bool isNewConnection,FILE *ClientsData)
{
    ClientsData = fopen("ClientsData.txt", "r+b");
    if (ClientsData == NULL) {
        ClientsData = fopen("ClientsData.txt", "w+b"); 
        if (ClientsData == NULL) {
            printf("Failed to open ClientsData.txt\n");
            return false;
        }
    }

    Clients ClientsStoring;
    bool userFound = false;

    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1) {
        if (strcmp(ClientsStoring.Username, username) == 0) {
            fseek(ClientsData, -sizeof(Clients), SEEK_CUR);
            ClientsStoring.Clients = clientSocket;
            ClientsStoring.IsActive = TRUE;  
            fwrite(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData);
            fflush(ClientsData); 
            printf("%s reconnected\n", ClientsStoring.Username);
            userFound = true;
            break;
        }
    }

    if (!userFound)
    {
        strcpy(ClientsStoring.Username, username);
        ClientsStoring.Clients = clientSocket;
        ClientsStoring.IsActive = TRUE;
        
        fseek(ClientsData, 0, SEEK_END);
        fwrite(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData);
        fflush(ClientsData); 
        printf("%s connected (new user)\n", ClientsStoring.Username);
        Counter++;
    }

    fclose(ClientsData);
    return userFound;
}

void MarkUserAsInactive(SOCKET clientSocket,FILE *ClientsData)
{
    ClientsData = fopen("ClientsData.txt", "r+b");
    if (ClientsData == NULL) {
        printf("Failed to open ClientsData.txt for marking inactive\n");
        return;
    }

    Clients ClientsStoring;
    
    while (fread(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData) == 1) {
        if (ClientsStoring.Clients == clientSocket) {
            fseek(ClientsData, -sizeof(Clients), SEEK_CUR);
            ClientsStoring.IsActive = FALSE;
            
            fwrite(&ClientsStoring, sizeof(ClientsStoring), 1, ClientsData);
            fflush(ClientsData); 
            printf("User %s disconnected\n", ClientsStoring.Username);
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
        if (tempClient.Clients != senderSocket && tempClient.IsActive) {
            printf("Sending to %s\n", tempClient.Username);
            if (send(tempClient.Clients, senderUsername, strlen(senderUsername), 0) == SOCKET_ERROR ||
                send(tempClient.Clients, message, strlen(message), 0) == SOCKET_ERROR) {
                printf("Failed to send to %s (client may have disconnected)\n", tempClient.Username);
            }
        }
    }

    fclose(ClientsData); 
}

bool SendPrivateMessage(const char* senderUsername, const char* recipient, const char* message, SOCKET senderSocket,FILE *ClientsData)
{
    ClientsData = fopen("ClientsData.txt", "rb");
    if (ClientsData == NULL) {
        printf("Failed to open ClientsData.txt for private message\n");
        return false;
    }

    Clients tempClient;
    bool found = false;
    
    while (fread(&tempClient, sizeof(tempClient), 1, ClientsData) == 1) {
        if (strcmp(recipient, tempClient.Username) == 0 && tempClient.IsActive) {
            if (send(tempClient.Clients, recipient, strlen(recipient), 0) == SOCKET_ERROR) {
                printf("Failed to send private message header to %s\n", recipient);
            } else {
                if (send(tempClient.Clients, message, strlen(message), 0) == SOCKET_ERROR) {
                    printf("Failed to send private message content to %s\n", recipient);
                } else {
                    printf("Private message sent to %s\n", recipient);
                    found = true;
                }
            }
            break;
        }
    }

    fclose(ClientsData);
    return found;
}

unsigned __stdcall ReceivingAndPrintingData(void *param)
{
    SOCKET clientSocket = *(SOCKET*)param;
    Into Message;
    
    memset(&Message, 0, sizeof(Message));

    int resultnumber = recv(clientSocket, Message.Username, sizeof(Message.Username) - 1, 0);
    if (resultnumber <= 0) {
        printf("Failed to receive username from client\n");
        goto cleanup;
    }
    
    Message.Username[resultnumber] = '\0';
    
    // Trim username (remove whitespace/newlines)
    int len = strlen(Message.Username);
    while (len > 0 && (Message.Username[len-1] == '\n' || Message.Username[len-1] == '\r' || 
                      Message.Username[len-1] == '\t' || Message.Username[len-1] == ' ')) {
        Message.Username[--len] = '\0';
    }

    printf("Client username received: %s\n", Message.Username);

    FindAndUpdateUser(Message.Username, clientSocket, true,ClientsData);

    while (TRUE) {
        memset(Message.GeneralPrivate, 0, sizeof(Message.GeneralPrivate));
        
        int resultnumberBool = recv(clientSocket, Message.GeneralPrivate, sizeof(Message.GeneralPrivate) - 1, 0);
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
            resultnumber = recv(clientSocket, Message.Buffer, sizeof(Message.Buffer) - 1, 0);
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

            BroadcastToAllUsers(Message.Username, Message.Buffer, clientSocket,ClientsData);
        }
        else if (strcmp(Message.GeneralPrivate, "FALSE") == 0)
        {
            SendClient(Message.Username,clientSocket,ClientsData);
            memset(Message.Recipient, 0, sizeof(Message.Recipient));
            
            int resultnumberRecipient = recv(clientSocket, Message.Recipient, sizeof(Message.Recipient) - 1, 0);
            if (resultnumberRecipient <= 0)
            {
                printf("Failed to receive recipient from %s\n", Message.Username);
                break;
            }
            
            Message.Recipient[resultnumberRecipient] = '\0';
            
            int lenr = strlen(Message.Recipient);
            while (lenr > 0 && (Message.Recipient[lenr-1] == '\n' || Message.Recipient[lenr-1] == '\r' || 
                               Message.Recipient[lenr-1] == '\t' || Message.Recipient[lenr-1] == ' ')) {
                Message.Recipient[--lenr] = '\0';
            }
        
            memset(Message.Buffer, 0, sizeof(Message.Buffer));
            resultnumber = recv(clientSocket, Message.Buffer, sizeof(Message.Buffer) - 1, 0);
            if (resultnumber <= 0) {
                printf("Failed to receive private message from %s\n", Message.Username);
                break;
            }
            
            Message.Buffer[resultnumber] = '\0';
            
            len = strlen(Message.Buffer);
            while (len > 0 && (Message.Buffer[len-1] == '\n' || Message.Buffer[len-1] == '\r' || 
                              Message.Buffer[len-1] == '\t' || Message.Buffer[len-1] == ' ')) {
                Message.Buffer[--len] = '\0';
            }
        
            printf("Private message from %s to %s: %s\n", Message.Username, Message.Recipient, Message.Buffer);
        
            bool messageSent = SendPrivateMessage(Message.Username, Message.Recipient, Message.Buffer, clientSocket,ClientsData);
            
            if (!messageSent)
            {
                printf("Recipient %s not found or not active\n", Message.Recipient);           
            }
        }
    }

    cleanup:
    MarkUserAsInactive(clientSocket,ClientsData);

    closesocket(clientSocket);
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