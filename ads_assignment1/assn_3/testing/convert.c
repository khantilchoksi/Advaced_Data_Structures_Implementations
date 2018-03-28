#include<stdio.h>
#include<stdlib.h>

int main(int argc, char** argv){
    FILE *binaryFile = fopen(argv[1],"rb");
    //FILE *decimalFile = fopen("sorted.bin.txt","w");

    fseek(binaryFile, 0L, SEEK_END);      //Setting file pointer at the end of the file
    long file_size = ftell(binaryFile);
    long input_keys = file_size/sizeof(int);   //Total input keys present in the input.bin file
    
    fseek(binaryFile, 0L, SEEK_SET);    //Set input file pointer back to the beginning
    int temp = 0;
    for(int i = 0; i<input_keys;i++){
        fread(&temp, sizeof(int),1,binaryFile);
        printf("%d \n ",temp);
        //fwrite(&temp, sizeof(int), 1, decimalFile);
    }
    fclose(binaryFile);
    printf(" Total Input Keys: %ld\n",input_keys);
    //fclose(decimalFile);
    return 0;
}