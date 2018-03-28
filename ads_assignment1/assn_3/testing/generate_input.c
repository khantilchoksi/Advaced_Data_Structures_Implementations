#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    int temp;
    fp=fopen("input.bin","wb");
    for(int i=0;i<249999;i++){
        temp=rand();
        fwrite(&temp,sizeof(int),1,fp);
        printf("%d\n",temp);
    }
    fclose(fp);
}