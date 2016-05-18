#include<stdio.h>
#include"structGenie.c"

//extern struct struct_Genie genieStruct;

int main()
{
    printf("before : %d\n",getState("lamp"));
    printf("result : %d\n",setDevice("lamp"));
    printf("after : %d\n",getState("lamp"));
    return 0;
}
