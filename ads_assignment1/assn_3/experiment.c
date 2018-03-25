//
//  experiment.c
//  ads_assignment1
//
//  Created by Khantil Choksi on 3/14/18.
//  Copyright © 2018 Khantil Choksi. All rights reserved.
//

#include <stdio.h>
void printBuffer(int* buffer, int start, int end){
    
    for(; start<end;start++){
        printf("start %d ::  %d\n",start,buffer[start]);
    }
}

int main(int argc, char** argv)
{
    int input_buffer[10];
    int i = 0, length = 10;
    for(; i<10; i++){
        input_buffer[i] = i;
    }
    
    printBuffer(input_buffer, 0 , 10);
    
    int *buffer = input_buffer;
    
    buffer = buffer+4;
    printf("\n OKAY: %d",*buffer);
    
    buffer = buffer+1;
    printf("\n OKAY: %d",*buffer);

    
    return 0;
}
