#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    int temp;
    fp=fopen("input4.bin","wb");
    for(int i=0;i<=456;i++){
        temp=rand();
        fwrite(&temp,sizeof(int),1,fp);
        printf("%d\n",temp);
    }
    fclose(fp);
}