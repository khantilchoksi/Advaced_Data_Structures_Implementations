#include<stdio.h>
#include<stdlib.h>
int main(){
    FILE *fp;
    int temp;
    fp=fopen("input2.bin","wb");
    for(int i=998;i>=0;i--){
        temp=i;
        fwrite(&temp,sizeof(int),1,fp);
        printf("%d\n",temp);
    }
    fclose(fp);
}