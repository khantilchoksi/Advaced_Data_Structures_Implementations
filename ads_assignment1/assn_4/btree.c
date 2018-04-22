//
//  btree.c
//  ads_assignment1
//
//  Created by Khantil Choksi on 4/18/18.
//  Copyright © 2018 Khantil Choksi. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <limits.h>

int compare(const void *a, const void *b){
    int* x = (int*) a;
    int* y = (int*) b;
    
    return (*x - *y);
}



//-------- Queue Implementation


//Queue Node Struct
typedef struct queue_node{
    long key;
    int level;
    struct queue_node *next;
}queue_node;

//Queue Struct
typedef struct {
    queue_node *front;   //Points to first node in the queue
    queue_node *rear;     //Points to last node in the queue
}queue;

//Create new node in the queue
queue_node* createNewQueueNode(long key, int level){
    queue_node* newNode = (queue_node*) calloc(1, sizeof(queue_node));
    newNode->key = key;
    newNode->level = level;
    newNode->next = NULL;
    return newNode;
}

//Create New Queue Initially
queue* createNewQueue(){
    queue* newQueue = (queue*) calloc(1, sizeof(queue));
    newQueue->front = NULL;
    newQueue->rear = NULL;
    return newQueue;
}

void enqueue(queue* q, long key, int level){
    queue_node* newNode = createNewQueueNode(key,level);
    
    //Check if queue is empty
    if(q->front == NULL && q->rear == NULL){
        q->front = newNode;
        q->rear = newNode;
    }else{
        //Else add at the end of queue
        q->rear->next = newNode;
        q->rear = newNode;
    }
}

queue_node* dequeue(queue* q){
    //Check first if queue is empty or not
    if(q->front == NULL){
        return NULL;
    }
    
    queue_node* frontNode = q->front;
    q->front = q->front->next;
    
    if(q->front == NULL){
        //If queue is empty then set rear null too
        q->rear = NULL;
    }
    
    return frontNode;
}

int isEmptyQueue(queue *q){
    if(q->front == NULL){
        return 1;
    }
    return 0;
}

//------- End Queue Implementation

//B-Tree Node Struct
typedef struct {
    int n;          //Number of keys in node
    int * key;      //Node's Keys
    long *child;    //Node's child subtree offsets
} btree_node;


void writeBTreeNodeToFile(btree_node *node, int order, FILE *fp, long offset){
    //Set file pointer to offset
    fseek(fp, offset, SEEK_SET);
    
    fwrite(&node->n, sizeof (int), 1, fp);
    fwrite(node->key, sizeof(int), order-1, fp);
    fwrite(node->child, sizeof(long), order, fp);
}

void readBTreeNodeFromFile(btree_node *node, int order, FILE *fp, long offset){
    //Set File Pointer According to "offset"
    fseek(fp, offset, SEEK_SET);
    //Write Node to File
    fread(&node->n, sizeof (int), 1, fp);
    fread(node->key, sizeof(int), order-1, fp);
    fread(node->child, sizeof(long),order, fp);
}

void writeRootNodeOffsetToFile(long *rootNodeOffset, FILE *fp){
    //Set File Pointer to beginning of file
    fseek(fp, 0L, SEEK_SET);
    fwrite( rootNodeOffset, sizeof( long ), 1, fp );
}

void readRootNodeOffsetFromFile(long *rootNodeOffset, FILE *fp){
    //Set File Pointer to beginning of file
    fseek(fp, 0L, SEEK_SET);
    fread( rootNodeOffset, sizeof(long), 1, fp);
}

void printNode(btree_node node){
    int i; //Loop counter
    for( i = 0; i < node.n - 1; i++ ) {
        printf( "%d,", node.key[ i ] );
    }
    printf( "%d", node.key[ node.n - 1 ] );
}

void printBTree(btree_node *node, long rootNodeOffset, int order, FILE *fp){
    int level = 1;
    int s;
    queue_node* queue_node;
    queue* queue = createNewQueue();
    //Read root node
    readBTreeNodeFromFile(node, order, fp, rootNodeOffset);
    
    enqueue(queue,rootNodeOffset,level);
    printf("\n%d: ",level);
    while(isEmptyQueue(queue) != 1){
        queue_node = dequeue(queue);
        //Check if next level's node is this or not
        if(level != queue_node->level){
            level++;
            printf("\n%d: ",level);
        }
        
        s = 0;
        readBTreeNodeFromFile(node, order, fp, queue_node->key);
        //printf(" ");
        while(s < node->n-1){
            printf("%d,",node->key[s]);
            //If child node exists, add it to queue
            if(node->child[s]!=0){
                enqueue(queue, node->child[s], (level+1));
            }
            s++;
        }
        printf("%d ",node->key[s]);
        if(node->child[s]!=0){
            enqueue(queue, node->child[s], (level+1));
            s++;
        }
        if(node->child[s]!=0){
            enqueue(queue, node->child[s], (level+1));
        }

    }
    
}

long findLeafNodeForAddKey(int key, btree_node *node, int order, FILE *fp, long rootNodeOffset, int *level, int traveseTillLevel){
    *level = 1;
    //Find Leaf Node
    int s = 0; //Subtree to traverse
    
    readBTreeNodeFromFile(node, order, fp, rootNodeOffset);
    
    long currentOffsetOfNode = rootNodeOffset;
    while (node->child[0]!=0 && (*level)<traveseTillLevel) { //Traversing till leaf node
        s = 0;
        while( s < node->n){
            if(key == node->key[s]){
                printf("Entry with key=%d already exists\n",key);
                return -1L;
            }else if (key < node->key[s]){
                break;
            }else{
                s++;
            }
        }
        
        if(node->child[0]!=0){
            (*level)++;
        }
        
        currentOffsetOfNode = node->child[s];
        readBTreeNodeFromFile(node, order, fp, node->child[s]);
        //Redundant sometimes check at last ------ ??????
        
    }
    printf("\n Exit time LEVEL: %d\n",*level);
    return currentOffsetOfNode;
}

//Testing print array for debug
void printArray(int *array, int size){
    printf("\n");
    for(int i = 0;i<size; i++){
        printf(" %d ",array[i]);
    }
}

int checkLeafNodeForKeyAlreadyExists(btree_node *node, int key){
    int s = 0;
    while(s < node->n){
        if(node->key[s] == key){
            printf("Entry with key=%d already exists\n",key);
            return -1;
        }
        s++;
    }
    return 1;
}

void addKeyToParentNode(int key, int traveseTillLevel , long rightNodeOffset, FILE *fp, int order, btree_node *node, long *rootNodeOffset){
    //key = km
    printf("\n ADD KEY TO PARENT NODE FUNCTION CALLED");
    int currentLevel = 0;
    long currentOffsetOfNode = findLeafNodeForAddKey(key, node, order, fp, *rootNodeOffset, &currentLevel,  traveseTillLevel);
    
    if(  currentOffsetOfNode != -1){
        //Now node is the leaf node where we can add new node

        if(node->n < order-1){
            //If L's key list is not full, replace it with K, update L's child offsets, write L back to disk, and stop
            
            //Node has space to accomodate key
            node->key[node->n] = key;
            node->n++;
            
            qsort(node->key, node->n, sizeof(int), compare);
            
            //Update offsets
            printf("\n PARENT NODE STATUS: ");
            printArray(node->key, node->n);
            printf("\n");
            
            //First find key's position in this parent node
            int s = 0;
            while(s < node->n){
                if(node->key[s] == key){
                    break;
                }
                s++;
            }
            
            //s is at the index where this new key is added in parent node
            if(node->child[s+1] != 0){
                node->child[s+2] = node->child[s+1];
            }
            node->child[s+1] = rightNodeOffset;
            
            writeBTreeNodeToFile(node, order, fp, currentOffsetOfNode);
            
        }
    }
        
}

void addKeyToBtree(int key, FILE *fp, long *rootNodeOffset, btree_node *node, int order){
    
    int level = 0;
    if(*rootNodeOffset != -1){
        //First Read Root Node and load into btree_node node
        
        long currentOffsetOfNode = findLeafNodeForAddKey(key, node, order, fp, *rootNodeOffset,&level, INT_MAX);
        if(  currentOffsetOfNode != -1){
            //Now node is the leaf node where we can add new node
            if(checkLeafNodeForKeyAlreadyExists(node, key) == -1){      //????? Replace this with FIND FUNCTION
                return;
            }
            
            if(node->n < order-1){
                //If L's key list is not full, replace it with K, update L's child offsets, write L back to disk, and stop
                
                //Node has space to accomodate key
                node->key[node->n] = key;
                node->n++;
                
                qsort(node->key, node->n, sizeof(int), compare);
                
                writeBTreeNodeToFile(node, order, fp, currentOffsetOfNode);
                
            }else{
                
                //4.Otherwise, split K about its median key value km into left and right key lists KL = (k0, ... , km-1) and KR = (km+1, ... , kn-1).
                printf("\n\n Split node further!");
                
                //This leaf node will have (order-1) keys already
                int *tempKeys = (int*)calloc(order, sizeof(int));
                //int median = ceil(order-1/2);
                int median = order/2;
                int i = 0, j = 0;
                
                for(i=0;i<order-1;i++){
                    tempKeys[i] = node->key[i];
                }
                tempKeys[i] = key;
                qsort(tempKeys, order, sizeof(int), compare);
                printf("\n TEMPKEYS: ");
                printArray(tempKeys, order);
               
                //5. Save KL and its associated child offsets in L, then write L back to disk.
                node->n = median;
                //Copy keys from 0 to median index
                //Below not required cz I have node->n

                
                for(i=0;i<median;i++){
                    node->key[i] = tempKeys[i];
                }
                

                
                writeBTreeNodeToFile(node, order, fp, currentOffsetOfNode);
                
                
                
                //6. Save KR and its associated child offsets in a new node R, then append R to the end of the index file.
                fseek(fp, 0L, SEEK_END);      //going to end of file
                long endOfFileOffset = ftell(fp); //offset value for end of file
                
                //Initialize Root Node with key value
                printf("\n ORDER: %d, MEDIAN INDEX: %d\n",order, median);
                printf("\n Creating new right node with keys count: %d",(order-median-1));
                node->n = (order-median-1);
                node->key = (int *) calloc (order -1, sizeof(int) );
                node->child = (long *) calloc(order, sizeof(long) );
                
                i = median+1;
                j = 0;
                for(;i<order;i++,j++){
                    node->key[j] = tempKeys[i];
                }
                writeBTreeNodeToFile(node, order, fp, endOfFileOffset);
                
                //7. Promote km , L's offset, and R's offset and insert them in L's parent node. If the parent's key list is full, recursively split its list and promote the median to its parent
                printf("\n LEVEL : %d", level);
                if(level != 1){
                        addKeyToParentNode(tempKeys[median], (level-1) , endOfFileOffset, fp, order, node, rootNodeOffset);
                }else{
                    //8. If a promotion is made to a root node with a full key list, split and create a new root node holding km and offsets to L and R.
                    //If root is full and level is 1 requested to add key so do following steps
                    printf("\n Creating new Root node.");
                    node->n = 1;
                    node->key = (int *) calloc (order -1, sizeof(int) );
                    node->child = (long *) calloc(order, sizeof(long) );
                    node->key[0] = tempKeys[median];
                    node->child[0] = *rootNodeOffset;
                    node->child[1] = endOfFileOffset;
                    
                    fseek(fp, 0L, SEEK_END);      //going to end of file
                    endOfFileOffset = ftell(fp); //offset value for end of file
                    writeBTreeNodeToFile(node, order, fp, endOfFileOffset);
                    writeRootNodeOffsetToFile(&endOfFileOffset,fp); //Updating Root Node offset
                    *rootNodeOffset = endOfFileOffset;
                }
                
                
                
               
                
            }
        }

        
    }else{
        //Create Root Node
        *rootNodeOffset = (long) sizeof(long);
        //Write offset to file
        writeRootNodeOffsetToFile(rootNodeOffset,fp);
        
        //Initialize Root Node with key value
        node->n = 1;
        node->key = (int *) calloc (order -1, sizeof(int) );
        node->child = (long *) calloc(order, sizeof(long) );
        node->key[0] = key;
        //Write Root Node to file
        writeBTreeNodeToFile(node, order, fp, *rootNodeOffset);
    }
    
}





int main(int argc, char** argv)
{
    int order;      //User Input
    btree_node node;    //Single B-tree node
    
    long rootNodeOffset;    //Root Node Offset in File
    
    FILE *inputFP;  //Input File Stream
    
    //Options
    char *addCommand = "add";
    char *findCommand = "find";
    char *printCommand = "print\n";
    char *endCommand = "end\n";
    
    int key;
    
    order = atoi(argv[1]);
    printf("\n Given Input Order is: %d",order);
    
    inputFP = fopen("index.bin","r+b");
    /* If file doesn't exist, set root offset to unknown and create file,
     otherwise read the root offset at the front of the file */
    if ( inputFP == NULL ){
        printf("\n Created new index.bin\n");
        rootNodeOffset = -1;
        inputFP = fopen( "index.bin", "w+b" );
        writeRootNodeOffsetToFile(&rootNodeOffset,inputFP);
    } else {
        printf("\n Reading already creatd index.bin\n");
        fread( &rootNodeOffset, sizeof( long ), 1, inputFP);
    }
    
    
    char *commandLine = NULL, *command, *delimiter = " ", *record;
    size_t length;
    ssize_t numberOfBytesRead;
    //printf("Insert Input:");
    while ((numberOfBytesRead = getline(&commandLine, &length, stdin)) != -1) {
        //printf("Retrieved line of length %zu:\n Line: %s\n", numberOfBytesRead, commandLine);
        command = strtok(commandLine, delimiter);
        //printf("\n COmmand: %s",command);
        if(strcmp(command,addCommand) == 0){
            //printf("ADD Command\n");
            //Key
            command = strtok(NULL, delimiter);
            key = atoi(command);
            printf("\n Add key to B-Tree: %d",key);
            
            addKeyToBtree(key,inputFP,&rootNodeOffset,&node, order);
            
        } else if(strcmp(command,findCommand) == 0){
            command = strtok(NULL, delimiter);
            key = atoi(command);
            printf("\n Find key in B-Tree: %d",key);
        } else if(strcmp(command,printCommand) == 0){
            
            printf("\n Print B-Tree:");
            printBTree(&node, rootNodeOffset, order, inputFP);
        }else if(strcmp(command,endCommand) == 0){
            
            printf("\n End program!");
            break;
        }else{
            printf("\n Not a valid command!");
        }
    }
    
    
    
    //Test Order 4 Node
//    node.n = 3;
//    node.key[0] = 5;
//    node.key[1] = 6;
//    node.key[2] = 7;
//
//    printf("\n Before Writing TO FILE: ");
//    printNode(node);
//
//    writeBTreeNodeToFile(&node,order,inputFP);
//    long tempRootAddress = 8;
//    writeRootNodeOffsetToFile(&tempRootAddress,inputFP);

    fclose(inputFP);
    
    inputFP = fopen("index.bin","r+b");
    
    fread( &rootNodeOffset, sizeof( long ), 1, inputFP);
    readBTreeNodeFromFile(&node, order, inputFP, 8);
    
    printf("\n After Reading From FILE: ");
    printNode(node);
    
    printf("\n");
    return 0;
}

