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
}Clients;
Clients ClientsStoring;
typedef struct
{
    char Username[100];
    char Buffer[100];
    char Recipient[100];
    char GeneralPrivate[20];
}Into;
Into Message;
int Counter=0;
FILE *ClientsData;
// for receiving and printing message from teh client
unsigned __stdcall ReceivingAndPrintingData(void *param);
int main()
{
    ClientsData = fopen("ClientsData.txt","r+");
    WSADATA wsaData;
    int result = WSAStartup(MAKEWORD(2, 2), &wsaData);
    if (result != 0) {
        printf("WSAStartup failed: %d\n", result);
        return 1;
    }
    struct sockaddr_in Server,ClientData;
    SOCKET ServerSocket=socket(AF_INET,SOCK_STREAM,0);
    if(ServerSocket==INVALID_SOCKET)
    {
        printf("broken socket\n");
        closesocket(ServerSocket);
        return 1;
    }
    Server.sin_family=AF_INET;
    Server.sin_port=htons(2000);
    Server.sin_addr.S_un.S_addr=INADDR_ANY;
    if(bind(ServerSocket, (struct sockaddr *)&Server, sizeof(Server)) == SOCKET_ERROR)
    {
    printf("Bind failed with error: %d\n", WSAGetLastError());
    closesocket(ServerSocket);
    WSACleanup();
    return 1;
    }
    if(listen(ServerSocket,5)==SOCKET_ERROR)
    {
        closesocket(ServerSocket);
        WSACleanup();
        return 1;
    }
    printf("the server is listening\n");
    int sizeData=sizeof(ClientData);
    SOCKET Client;
    while(TRUE)
    {
        Client=accept(ServerSocket,(struct sockaddr *)&ClientData,&sizeData);
        SOCKET *ClientAdress=(SOCKET *)malloc(sizeof(SOCKET));
        *ClientAdress = Client;
        HANDLE ClientThread = (HANDLE)_beginthreadex(NULL,0,ReceivingAndPrintingData,ClientAdress,0,NULL);
        if(ClientThread)
        {
            CloseHandle(ClientThread);
        }
    }
    closesocket(ServerSocket);
    WSACleanup();
    return 0;
}
unsigned __stdcall ReceivingAndPrintingData(void *param)
{
    bool Check=FALSE;
    memset(Message.Username,0,sizeof(Message.Username));
    int resultnumber = recv(*(SOCKET*)param,Message.Username,sizeof(Message.Username),0);
    int len = strlen(Message.Username);
    while (len > 0 && (Message.Username[len-1] == '\n' || Message.Username[len-1] == '\r' || 
    Message.Username[len-1] == '\t' || Message.Username[len-1] == ' '))
    {
        Message.Username[--len] = '\0';
    }
    if(resultnumber > 0)
    {
        rewind(ClientsData);
        while(fread(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData) == 1)
        {
            if(strcmp(ClientsStoring.Username,Message.Username) == 0)
            {
                ClientsStoring.Clients = *(SOCKET *)param;
                ClientsStoring.IsActive = TRUE;
                Check=TRUE;
                fseek(ClientsData,-sizeof(ClientsStoring),SEEK_CUR);
                fwrite(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData);
                printf("%s reconnect\n",ClientsStoring.Username);
                break;
            }
        }
        if(!Check)
        {
            ClientsStoring.Clients=*(SOCKET *)param;
            strcpy(ClientsStoring.Username,Message.Username);
            ClientsStoring.IsActive = TRUE;
            fwrite(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData);
            printf("%s connect\n",ClientsStoring.Username);
        }
    }
    int resultnumberBoleen = recv(*(SOCKET*)param,Message.GeneralPrivate,sizeof(Message.GeneralPrivate),0);
    if(resultnumber <= 0)
    {
        printf("there is a problem with the client is choice to send into all the users or to a specific user \n");
    }
    else
    {
        int len = strlen(Message.GeneralPrivate);
        while (len > 0 && (Message.GeneralPrivate[len-1] == '\n' || Message.GeneralPrivate[len-1] == '\r' || 
        Message.GeneralPrivate[len-1] == '\t' || Message.GeneralPrivate[len-1] == ' '))
        {
            Message.GeneralPrivate[--len] = '\0';
        }
    }
    while(TRUE)
    {
        if(resultnumber>0)
        {
            if(strcmp(Message.GeneralPrivate,"TRUE") == 0)
            {
                memset(Message.Buffer,0,sizeof(Message.Buffer));
                int resultnumber = recv(*(SOCKET*)param,Message.Buffer,sizeof(Message.Buffer),0);
                int len = strlen(Message.Buffer);
                while (len > 0 && (Message.Buffer[len-1] == '\n' || Message.Buffer[len-1] == '\r' || 
                Message.Buffer[len-1] == '\t' || Message.Buffer[len-1] == ' '))
                {
                    Message.Buffer[--len] = '\0';
                }
                rewind(ClientsData);
                while(fread(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData) == 1)
                {
                    if(ClientsStoring.Clients != *(SOCKET *)param && ClientsStoring.IsActive)
                    {
                        send(ClientsStoring.Clients,Message.Username,strlen(Message.Username),0);
                        send(ClientsStoring.Clients,Message.Buffer,strlen(Message.Buffer),0);
                    }
                }
            }
            else
            {
                memset(Message.Recipient,0,sizeof(Message.Recipient));
                int resultnumberb = recv(*(SOCKET*)param,Message.Recipient,sizeof(Message.Recipient),0);
                if(resultnumberb <= 0)
                {
                    break;
                }
                int lenr = strlen(Message.Recipient);
                while (lenr > 0 && (Message.Recipient[lenr-1] == '\n' || Message.Recipient[lenr-1] == '\r' || 
                Message.Recipient[lenr-1] == '\t' || Message.Recipient[lenr-1] == ' '))
                {
                    Message.Recipient[--lenr] = '\0';
                }
                memset(Message.Buffer,0,sizeof(Message.Buffer));
                int resultnumber = recv(*(SOCKET*)param,Message.Buffer,sizeof(Message.Buffer),0);
                int len = strlen(Message.Buffer);
                while (len > 0 && (Message.Buffer[len-1] == '\n' || Message.Buffer[len-1] == '\r' || 
                Message.Buffer[len-1] == '\t' || Message.Buffer[len-1] == ' '))
                {
                    Message.Buffer[--len] = '\0';
                }
                int  searching = 0;
                rewind(ClientsData);
                while(fread(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData) == 1)
                {
                    if(strcmp(Message.Recipient,ClientsStoring.Username) == 0 && ClientsStoring.IsActive)
                    {
                        if(send(ClientsStoring.Clients,Message.Username,strlen(Message.Username),0) == SOCKET_ERROR ||
                           send(ClientsStoring.Clients,Message.Buffer,strlen(Message.Buffer),0) == SOCKET_ERROR)
                        {
                            ClientsStoring.IsActive = FALSE;
                        }
                        break;
                    }
                    else
                    {
                        searching++;                        
                    }
                }
                if(searching == Counter)
                {
                    printf("the recipient are not found \n");
                }
            }
        }
        if(resultnumber <= 0)
        {
            break;
        }
    }
    rewind(ClientsData);
    while(fread(&ClientsStoring,sizeof(ClientsStoring),1,ClientsData) == 1)
    {
        if(ClientsStoring.Clients == *(SOCKET *)param)
        {
            ClientsStoring.IsActive = FALSE;
            printf("the User %s disconnected\n", ClientsStoring.Username);
            break;
        }
    }
    fclose(ClientsData);
    closesocket(*(SOCKET*)param);
    free(param);
    return 0;
}