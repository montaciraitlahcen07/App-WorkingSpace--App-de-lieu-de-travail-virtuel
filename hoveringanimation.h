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