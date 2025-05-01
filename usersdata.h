#include <windows.h>
#include <stdio.h>
void FillingData(FILE **UserData)
{
    typedef struct
    {
        char UserName[25];
        char PassWord[25];
        char FirstName[20];
        char LastName[20];
        int age;
        int salarie;
    }Workers;
    Workers WorkersData[3]={{"","","montacir","aitlahcen",19,10000},
    {"mohammed.ouakki","puakki","mohammed","ouakki",20,10000},
    {"bilal.zarkal","bilal","bilal","zarkal",20,10000}};
    *UserData=fopen("usersdata.txt","wb+");
    if(*UserData)
    {
        for(int i=0;i<3;i++)
        {
            fwrite(&WorkersData[i],sizeof(Workers),1,*UserData);
        }
        fclose(*UserData);
    }
    
}