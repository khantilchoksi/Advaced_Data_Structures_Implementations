//
//  experiment.c
//  ads_assignment1
//
//  Created by Khantil Choksi on 3/14/18.
//  Copyright © 2018 Khantil Choksi. All rights reserved.
//

#include <stdio.h>
#include <string.h>
void printBuffer(int* buffer, int start, int end){
    
    for(; start<end;start++){
        printf("start %d ::  %d\n",start,buffer[start]);
    }
}

int main(int argc, char** argv)
{
    //Run File name e.g. input.bin.012
    char runFileName[1000];
    //To store e.g. "012"
    char currentRunName[4];
    char* fileName = argv[1];
    for(int i = 0 ; i<10; i++){
        strcpy(runFileName,fileName);
        snprintf(currentRunName, 4*sizeof(char), "%03d", (i));
        strcat(runFileName,currentRunName);
        printf("\n %s",runFileName);
    }


    //int input_buffer[10];
    //int i = 0, length = 10;
    //for(; i<10; i++){
    //    input_buffer[i] = i;
    //}
    
    //printBuffer(input_buffer, 0 , 10);
    
    //int *buffer = input_buffer;
    
    //buffer = buffer+4;
    //printf("\n OKAY: %d",*buffer);
    
    //buffer = buffer+1;
    //printf("\n OKAY: %d",*buffer);

    // int runs = 250;
    // int i = (runs/15) * 15;
    // printf("\n ANSWER: %d",i);

    // int blockSize = 1000/(runs - (runs/15)*15);
    // printf("\n NEW BLOCK SIZE: %d",blockSize);
    return 0;
}
