//
//  diskmgmt.c
//  ads_assignment3
//
//  Created by Khantil Choksi on 3/12/18.
//  Copyright © 2018 Khantil Choksi. All rights reserved.
//

//assn_3 mergesort-method index-file sorted-index-file
/*
 Your program must support three different mergesort methods.
 
 1.    --basic    Split the index file into sorted runs stored on disk, then merge the runs to produce a sorted index file.
 2.    --multistep    Split the index file into sorted runs. Merge subsets of runs to create super-runs, then merge the super-runs to produce a sorted index file.
 3.    --replacement    Split the index file into sorted runs created using replacement selection, then merge the runs to produce a sorted index file.
 */


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>

//Read Keys
void readKeyBlocks(FILE *fp, int* buffer, int count){
    //while(count>0){
        fread(buffer, sizeof(int), count, fp);
       // buffer++;
      //  count--;
    //}

}

//Write Runs to disk
void writeKeys(FILE *fp, int* buffer, int count){
    fwrite( buffer, sizeof( int ), count, fp );
}

//Read next block from disk and returns number of integers actaully read
int readBlock(FILE *fp, int* buffer, int start, int count){
    //for(;count>0;start++){
    //Assuming I have still whole block to read
    buffer = buffer+start;
    int readBlockReturnValue = fread(buffer, sizeof(int), count, fp);
    printf("\n READ BLOCK RETURN VALUE: %d",readBlockReturnValue);
    return readBlockReturnValue;
    //}
}

//Print Buffer
void printBuffer(int* buffer, int start, int end){
    
    for(; start<end;start++){
        printf(" %d ::  %d\n",start,buffer[start]);
    }
}

//Comparator function for integer to be used in quick sort
int compare_int( const void* a, const void* b )
{
    if( *(int*)a == *(int*)b ) return 0;
    return *(int*)a < *(int*)b ? -1 : 1;
}

//Creating Runs and return total number of runs created i.e. runs = runs or runs = runs + 1
int create_runs(FILE * inputFP, int* buffer, long input_keys){

    int i = 0, remaining_keys;
    int runs = input_keys/1000;
    
    //Run File name e.g. input.bin.012
    char runFileName[1000];
    //To store e.g. "012"
    char currentRunName[4];
    
    FILE *runFP = NULL;
    
    for(i = 0; i<runs; i++){
        readKeyBlocks(inputFP, buffer, 1000);
        qsort(buffer, 1000, sizeof(int), compare_int);
        //printBuffer(input_buffer, 0, 1000);
        strcpy(runFileName,"input.bin.");
        snprintf(currentRunName, 4*sizeof(char), "%03d", i);
        strcat(runFileName,currentRunName);
        
        runFP = fopen( runFileName, "wb" );
        writeKeys(runFP, buffer, 1000 );
        fclose(runFP);
        
        //printf("\n RUN: %s\n",runFileName);
    }
    
    remaining_keys = input_keys - (runs*1000);
    if(remaining_keys > 0){
        //Create one more last run file which will have keys < 1000
        runs++;     //Total runs will be runs+1 for further use
        
        readKeyBlocks(inputFP, buffer, remaining_keys);
        qsort(buffer, remaining_keys, sizeof(int), compare_int);
        
        strcpy(runFileName,"input.bin.");
        snprintf(currentRunName, 4*sizeof(char), "%03d", i);
        strcat(runFileName,currentRunName);
        runFP = fopen( runFileName, "wb" );
        writeKeys(runFP, buffer,remaining_keys );
        fclose(runFP);
    }
    
    return runs;
}

//Initial Load into input_buffer for all runs
void initial_load_runs(int runs, FILE **runsFP, int* buffer, int blockSize, int *currentBufferRunPointers, int* runBufferReadCount, int* isRunExhausted){
    //Run File name e.g. input.bin.012
    char runFileName[1000];
    //To store e.g. "012"
    char currentRunName[4];
    
    int i = 0;
    
    for(i = 0; i<runs;i++){
        strcpy(runFileName,"input.bin.");
        snprintf(currentRunName, 4*sizeof(char), "%03d", i);
        strcat(runFileName,currentRunName);
        
        runsFP[i] = fopen( runFileName, "rb" );

        
        readBlock(runsFP[i], buffer, i*blockSize, blockSize);
        runBufferReadCount[i] =blockSize;
        currentBufferRunPointers[i] = i*blockSize;
        isRunExhausted[i] = 0; //Default initially isRunExhausted will false bcz the run will have at-least one value
    }
    
    printf("\n Printing  runBufferReadCount -----------\n");
    for(i = 0; i<runs;i++){
        
        printf(" :: %d ",runBufferReadCount[i]);
        
        
    }
}

//Merge Runs
void merge_runs(int *input_buffer, int* output_buffer, FILE *sortedFP, FILE **runsFP, int runs, int blockSize, int *currentBufferRunPointers, int* runBufferReadCount, int* isRunExhausted){
    int minKey, minRunNumber, actualKeysRead;
    int outputBufferIndex = 0;
    
    //Testing Counter
    int testCounter = 0;
    
    int i = 0;
    
    while(1){       //Until all the runs are not exhausted
        testCounter++;
        //Find MIN
        minKey = INT_MAX;
        
        for(i = 0; i <runs; i++){
            if(isRunExhausted[i] == 0){//If run is not exhausted
                
                if(currentBufferRunPointers[i] >= ( i*blockSize + runBufferReadCount[i] ) ){
                    //ith run's buffer has been used, so load next block for this run
                    if(runBufferReadCount[i] < blockSize){
                        printf("\n RUN I: %d got exhausted",i);
                        isRunExhausted[i] = 1;
                        continue;
                    }else{
                        actualKeysRead = readBlock(runsFP[i], input_buffer, i*blockSize, blockSize); //Run next block size keys
                        
                        if(actualKeysRead < blockSize){ //This means that end of file occured and this run has been exhausted
                            
                            runBufferReadCount[i] = actualKeysRead;
                            fclose(runsFP[i]);  //Close this sorted file
                            
                            //This mean that, we don't have any more keys left for ready from that sorted file run i.e. situation like file having 250 keys divided by 4
                            if(actualKeysRead == 0){
                                //No more keys
                                printf("\n RUN I: %d got exhausted",i);
                                isRunExhausted[i] = 1;  //Run i exhausted set TRUE
                                continue;
                            }
                        }
                        
                        //Reset pointer
                        currentBufferRunPointers[i] = i*blockSize;
                    }
                    
                }
                
                if ( input_buffer[currentBufferRunPointers[i]] < minKey){
                    minKey = input_buffer[currentBufferRunPointers[i]];
                    //indexOfMinKey = currentBufferRunPointers[i];
                    minRunNumber = i;
                }
                
                
            }
        }
        
        
        //minKey will remain INT_MAX is all the runs are exhausted
        //This indicates we are done!
        if(minKey == INT_MAX){
            printf("\n COMPLETED EXECUTION");
            break;
        }else{
            printf("\n NOT COMPLETED EXECUTION");
        }
        
        
        currentBufferRunPointers[minRunNumber] += 1;    //minRunNumber run buffer got used i.e. advancing pointer
        
        //Output buffer is exhausted so write it to output file
        if(outputBufferIndex >= 1000){
            printf(" \n WRITING : %d KEYS TO OUTPUT FILE\n",1000);
            writeKeys(sortedFP, output_buffer, 1000);
            outputBufferIndex = 0;  //Indicates that output_buffer is empty
        }
        output_buffer[outputBufferIndex] = minKey;
        outputBufferIndex++;
        
        
        
        
    }
    
    //Write the remaining output buffer
    if(outputBufferIndex > 0){
        printf(" \n WRITING : %d KEYS TO OUTPUT FILE\n",outputBufferIndex);
        writeKeys(sortedFP, output_buffer, outputBufferIndex);
    }
    
    //Testing
    
    printf("\n TEST COUNTER: %d",testCounter);
}


int main(int argc, char** argv)
{

    //Input Buffer
    int input_buffer[1000];
    
    //Output Buffer
    int output_buffer[1000];
    
    //Input Keys
    long input_keys, file_size;
    //Number of RUNS
    int runs;
    
    int blockSize;  //The amount of each run we can buffer
    
    int i;
    
    FILE *inputFP = NULL, *sortedFP = NULL;
    FILE *runsFP[runs];
    
    
    
    if( ( inputFP = fopen( "input.bin", "rb" )) != NULL){
        //Read 1000 indexes
        //readKeyBlocks(inputFP, input_buffer, 1000);
        
        fseek(inputFP, 0L, SEEK_END);      //Setting file pointer at the end of the file
        file_size = ftell(inputFP);
        input_keys = file_size/sizeof(int);   //Total input keys present in the input.bin file
        printf(" Total Input Keys: %ld\n",input_keys);
        fseek(inputFP, 0L, SEEK_SET);    //Set input file pointer back to the beginning
        
        
        
        //Basic MergeSort
        //1. Create Runs
        runs = create_runs(inputFP, input_buffer, input_keys);

        //No need of input file now
        fclose( inputFP );
        
        //Calculating how many keys we can accomodate in input_buffer from each 
        blockSize = 1000/runs;
        printf("\n BLOCK SIZE: %d",blockSize);
        
        //Total Numbers of files created =  runs
        
        //Reference pointers to handle runs
        int currentBufferRunPointers[runs], isRunExhausted[runs] , runBufferReadCount[runs];
        
        
        //Initial load from all runs file into input buffer
        initial_load_runs(runs, runsFP, input_buffer, blockSize, currentBufferRunPointers, runBufferReadCount, runBufferReadCount);
        
        printf("\n ----- INITIAL LOADED BUFFER ---------- ");
        printBuffer(input_buffer, 0, 1000);
    
        
        //SORTED FILE
        sortedFP = fopen("sorted.bin", "wb");
        
        //Merge Runs
        merge_runs(input_buffer, output_buffer, sortedFP, runsFP, runs, blockSize, currentBufferRunPointers, runBufferReadCount, isRunExhausted);
        
        
        fclose(sortedFP);
        
        
    }
    
    


    //Print output file
    sortedFP = fopen("sorted.bin", "rb");
    fseek(sortedFP, 0L, SEEK_END);      //Setting file pointer at the end of the file
    file_size = ftell(sortedFP);
    input_keys = file_size/sizeof(int);
    printf(" Total OUTPUT Keys: %ld\n",input_keys);
    fseek(sortedFP, 0L, SEEK_SET);    //Set input file pointer back to the beginning
    
    runs = input_keys/1000;
    printf("\n\n FINAL SORTED FILE ---------------------- \n\n");
    //Basic MergeSort
    for(i = 0; i<runs; i++){
        readKeyBlocks(sortedFP, input_buffer, 1000);
        printf("\n RUN I: %d\n",i);
        printBuffer(input_buffer, 0, 1000);
    }
    
    
    
    
    //Sorted keys must be written to sort.bin as binary integers.
//    FILE *sortedFP; /* Output file stream */
//    int output_buf[ 1000 ]; /* Output buffer */
//    sortedFP = fopen( "sort.bin", "wb" );
//    fwrite( output_buf, sizeof( int ), 1000, sortedFP );
//    fclose( sortedFP );
    
    
    
    //printf( "Time: %ld.%06ld", exec_tm.tv_sec, exec_tm.tv_usec );
    
    return 0;
    
}


