#include<stdio.h>
#include<stdlib.h>
#include<string.h>
int main(){
    FILE *fp, *fp1;
    int temp=0,count,temp1,count2=0,count3=0;
    char filname1[14]="input.bin.";
    char filname2[4];
    while(count2<168){
    printf("RUN NUMBER %d HAS STARTED\n",count2);
    sprintf(filname2, "%03d",count2);
    strcat(filname1, filname2);            
    fp=fopen(filname1, "rb");
    strcpy(filname1, "input.bin.");
    // fp=fopen("input.bin.010","rb");
    // fp1=fopen("input.bin 2.010","rb");
    while(fread(&temp,sizeof(int),1,fp)){
 //       printf("%d   %d\n",count3,temp);
        count3++;
        count++;
        //count3++;
        //count++;
        //count3++;
    }
    printf("This Run has count %d\n\n",count3);
    count3=0;
    count2++;
}
    //printf("The Final Count is: %d\n",count);
}
