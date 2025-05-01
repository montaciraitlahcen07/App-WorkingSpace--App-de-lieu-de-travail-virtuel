#include <windows.h>
#include <stdbool.h>
int Mx,My;
#define MessageTimer 1003
float CurrentH=0.0f;
float CurrentV=0.0f;
#define HOVER_H 8
#define HOVER_V 6
bool HoveringMessage=FALSE;
bool WasHoveringMessage=FALSE;
bool check=FALSE;
bool MessageIn=FALSE;
void UpdateMessageAnimation(bool Hovering,HWND HandleWnd)
{
    float IncremmentH=HOVER_H/8;
    float IncremmentV=HOVER_V/8;
    if(Hovering)
    {
        CurrentH+=IncremmentH;
        CurrentV+=IncremmentV;
        if(CurrentH >=HOVER_H)
        {
            CurrentH=HOVER_H;
            CurrentV=HOVER_V;
            KillTimer(HandleWnd,MessageTimer);
        }
    }
    else
    {
        CurrentH-=IncremmentH;
        CurrentV-=IncremmentV;
        if(CurrentH <=HOVER_H && CurrentV<=HOVER_V)
        {
            CurrentH=0;
            CurrentV=0;
            KillTimer(HandleWnd,MessageTimer);
        }
    }        
}