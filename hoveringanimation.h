#include <windows.h>
#include <stdbool.h>
int Mx,My;
#define HOVER_H 8
#define HOVER_V 6

// message variable 
#define MessageTimer 1003
float CurrentHMessage=0.0f;
float CurrentVMessage=0.0f;
bool HoveringMessage=FALSE;
bool WasHoveringMessage=FALSE;
bool CheckMessage=FALSE;

// online variable
#define OnlineTimer 1004
float CurrentHOnline=0.0f;
float CurrentVOnline=0.0f;
bool HoveringOnline=FALSE;
bool WasHoveringOnline=FALSE;
bool CheckOnline=FALSE;

// Task variable
#define TaskTimer 1005
float CurrentHTask=0.0f;
float CurrentVTask=0.0f;
bool HoveringTask=FALSE;
bool WasHoveringTask=FALSE;
bool CheckTask=FALSE;

// Project variable
#define ProjectTimer 1006
float CurrentHProject=0.0f;
float CurrentVProject=0.0f;
bool HoveringProject=FALSE;
bool WasHoveringProject=FALSE;
bool CheckProject=FALSE;

// Disconnect variable
#define DisconnectTimer 1007
float CurrentHDisconnect=0.0f;
float CurrentVDisconnect=0.0f;
bool HoveringDisconnect=FALSE;
bool WasHoveringDisconnect=FALSE;
bool CheckDisconnect=FALSE;

// Inbox variable
#define InboxTimer 1008
float CurrentHInbox=0.0f;
float CurrentVInbox=0.0f;
bool HoveringInbox=FALSE;
bool WasHoveringInbox=FALSE;
bool CheckInbox=FALSE;


// General variable
#define GeneralTimer 1009
float CurrentHGeneral=0.0f;
float CurrentVGeneral=0.0f;
bool HoveringGeneral=FALSE;
bool WasHoveringGeneral=FALSE;
bool CheckGeneral=FALSE;


// Animation message
void UpdateMessageAnimation(bool HoveringMessage,HWND HandleWnd)
{
    float IncremmentHMessage=HOVER_H/8;
    float IncremmentVMessage=HOVER_V/8;
    if(HoveringMessage)
    {
        CurrentHMessage+=IncremmentHMessage;
        CurrentVMessage+=IncremmentVMessage;
        if(CurrentHMessage >=HOVER_H)
        {
            CurrentHMessage=HOVER_H;
            CurrentVMessage=HOVER_V;
            KillTimer(HandleWnd,MessageTimer);
        }
    }
    else
    {
        CurrentHMessage-=IncremmentHMessage;
        CurrentVMessage-=IncremmentVMessage;
        if(CurrentHMessage <=0)
        {
            CurrentHMessage=0;
            CurrentVMessage=0;
            KillTimer(HandleWnd,MessageTimer);
        }
    }        
}
// animation online
void UpdateOnlineAnimation(bool HoveringOnline,HWND HandleWnd)
{
    float IncremmentHOnline=HOVER_H/8;
    float IncremmentVOnline=HOVER_V/8;
    if(HoveringOnline)
    {
        CurrentHOnline+=IncremmentHOnline;
        CurrentVOnline+=IncremmentVOnline;
        if(CurrentHOnline >=HOVER_H)
        {
            CurrentHOnline=HOVER_H;
            CurrentVOnline=HOVER_V;
            KillTimer(HandleWnd,OnlineTimer);
        }
    }
    else
    {
        CurrentHOnline-=IncremmentHOnline;
        CurrentVOnline-=IncremmentVOnline;
        if(CurrentHOnline <=0)
        {
            CurrentHOnline=0;
            CurrentVOnline=0;
            KillTimer(HandleWnd,OnlineTimer);
        }
    }        
}
// animation Task
void UpdateTaskAnimation(bool HoveringTask,HWND HandleWnd)
{
    float IncremmentHTask=HOVER_H/8;
    float IncremmentVTask=HOVER_V/8;
    if(HoveringTask)
    {
        CurrentHTask+=IncremmentHTask;
        CurrentVTask+=IncremmentVTask;
        if(CurrentHTask >=HOVER_H)
        {
            CurrentHTask=HOVER_H;
            CurrentVTask=HOVER_V;
            KillTimer(HandleWnd,TaskTimer);
        }
    }
    else
    {
        CurrentHTask-=IncremmentHTask;
        CurrentVTask-=IncremmentVTask;
        if(CurrentHTask <=0)
        {
            CurrentHTask=0;
            CurrentVTask=0;
            KillTimer(HandleWnd,TaskTimer);
        }
    }        
}
// animation Project
void UpdateProjectAnimation(bool HoveringProject,HWND HandleWnd)
{
    float IncremmentHProject=HOVER_H/8;
    float IncremmentVProject=HOVER_V/8;
    if(HoveringProject)
    {
        CurrentHProject+=IncremmentHProject;
        CurrentVProject+=IncremmentVProject;
        if(CurrentHProject >=HOVER_H)
        {
            CurrentHProject=HOVER_H;
            CurrentVProject=HOVER_V;
            KillTimer(HandleWnd,ProjectTimer);
        }
    }
    else
    {
        CurrentHProject-=IncremmentHProject;
        CurrentVProject-=IncremmentVProject;
        if(CurrentHProject <=0)
        {
            CurrentHProject=0;
            CurrentVProject=0;
            KillTimer(HandleWnd,ProjectTimer);
        }
    }        
}
// animation Disconnect
void UpdateDisconnectAnimation(bool HoveringDisconnect,HWND HandleWnd)
{
    float IncremmentHDisconnect=HOVER_H/8;
    float IncremmentVDisconnect=HOVER_V/8;
    if(HoveringDisconnect)
    {
        CurrentHDisconnect+=IncremmentHDisconnect;
        CurrentVDisconnect+=IncremmentVDisconnect;
        if(CurrentHDisconnect >=HOVER_H)
        {
            CurrentHDisconnect=HOVER_H;
            CurrentVDisconnect=HOVER_V;
            KillTimer(HandleWnd,DisconnectTimer);
        }
    }
    else
    {
        CurrentHDisconnect-=IncremmentHDisconnect;
        CurrentVDisconnect-=IncremmentVDisconnect;
        if(CurrentHDisconnect <=0)
        {
            CurrentHDisconnect=0;
            CurrentVDisconnect=0;
            KillTimer(HandleWnd,DisconnectTimer);
        }
    }        
}
// animation Inbox
void UpdateInboxAnimation(bool HoveringInbox,HWND HandleWnd)
{
    float IncremmentHInbox=HOVER_H/8;
    float IncremmentVInbox=HOVER_V/8;
    if(HoveringInbox)
    {
        CurrentHInbox+=IncremmentHInbox;
        CurrentVInbox+=IncremmentVInbox;
        if(CurrentHInbox >=HOVER_H)
        {
            CurrentHInbox=HOVER_H;
            CurrentVInbox=HOVER_V;
            KillTimer(HandleWnd,InboxTimer);
        }
    }
    else
    {
        CurrentHInbox-=IncremmentHInbox;
        CurrentVInbox-=IncremmentVInbox;
        if(CurrentHInbox <=0)
        {
            CurrentHInbox=0;
            CurrentVInbox=0;
            KillTimer(HandleWnd,InboxTimer);
        }
    }        
}
// animation General
void UpdateGeneralAnimation(bool HoveringGeneral,HWND HandleWnd)
{
    float IncremmentHGeneral=HOVER_H/8;
    float IncremmentVGeneral=HOVER_V/8;
    if(HoveringGeneral)
    {
        CurrentHGeneral+=IncremmentHGeneral;
        CurrentVGeneral+=IncremmentVGeneral;
        if(CurrentHGeneral >=HOVER_H)
        {
            CurrentHGeneral=HOVER_H;
            CurrentVGeneral=HOVER_V;
            KillTimer(HandleWnd,GeneralTimer);
        }
    }
    else
    {
        CurrentHGeneral-=IncremmentHGeneral;
        CurrentVGeneral-=IncremmentVGeneral;
        if(CurrentHGeneral <=0)
        {
            CurrentHGeneral=0;
            CurrentVGeneral=0;
            KillTimer(HandleWnd,GeneralTimer);
        }
    }        
}