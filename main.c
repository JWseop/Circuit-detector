//
//  main.c
//  Data Structure HW4
//
//  Created by 정우섭 on 2021/05/27.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TRUE 0;
#define FALSE 1;

#define MAX_NODE_SIZE 50
#define MAX_STACK_SIZE 50

typedef struct NODE{
    struct NODE* Node_arr[MAX_NODE_SIZE];
}node;

typedef struct STACK{
    int top;
    int stack[MAX_NODE_SIZE];
}stack;

/*--------------------STACK--------------------------*/
void init_stack(stack* s);
int IsEmpty(stack* s);
int IsFull(stack* s);
void push(int value,stack* s);
int pop(stack* s,int now);
int check_stack(stack* s, int value, int N);
/*--------------------NODE--------------------------*/
void init_Node(node input_tree[], int Node_size);
void Link_Node(node tree[],int N, int** M);
/*--------------------DFS CIRCUIT DETECTOR--------------------------*/
void DFS_search(node* tree, stack* s,int now, int N, int* result);
/*--------------------RESULT PASSER--------------------------*/
void pass_result(int result_num,int *result,int result_arr[]);
void return_result(int result_arr[],int how_many);


void init_stack(stack* s){
    s->top=-1;
    for(int i=0;i<MAX_NODE_SIZE;i++)
    {
        s->stack[i]=-1;
    }
}
 
int IsEmpty(stack* s){
    if(s->top<0){
        return TRUE;
    }
    else{
        return FALSE;
    }
}
int IsFull(stack* s){
    if(s->top>=MAX_STACK_SIZE-1){
        return TRUE;
    }
    else{
        return FALSE;
    }
}

void push(int value,stack* s){
    if(IsFull(s)==0)
    {
        printf("error : stack is full");
    }
    else{
        s->stack[++(s->top)]=value;
    }
}
 
int pop(stack* s,int now){
    if(IsEmpty(s)==0)
    {
        printf("error : stack is empty");
        return -1;
    }
    else{
        s->stack[s->top]=-1;
        return s->stack[(s->top)--];
    }
}
int check_stack(stack* s, int value, int N){
    for(int i=0;i<N;i++)
    {
        if(s->stack[i]==value){
            return TRUE;
        }
    }
    return FALSE;
}

void init_Node(node input_tree[], int Node_size)        //node initiator
{
    for(int i=0;i<Node_size;i++)                        //allocate Node_arr dynamically and reset data to 0
    {
        for(int j=0;j<MAX_NODE_SIZE;j++)
        {
            input_tree[i].Node_arr[j]=NULL;
        }
    }
}

void Link_Node(node tree[],int N, int** M)
{
    for(int i=0;i<N;i++)
    {
        for(int j=0;j<N;j++)
        {
            if(M[i][j]==1)
            {
                tree[i].Node_arr[j]=&tree[j];
            }
        }
    }
}

void DFS_search(node* tree, stack* s,int now, int N, int* result)
{
    push(now, s);
    for(int i=0;i<N;i++)    //loop for N to figure out if the current node is linked with other N nodes.
    {
        if(tree[now].Node_arr[i]!=NULL&&check_stack(s, i, N)==0)    //linked to i node, already visited then return result++
        {
            (*result)++;
            break;
        }
        if(tree[now].Node_arr[i]!=NULL&&check_stack(s, i, N)==1)    //linked to i node, not visited then DFS_search to i node.
        {
            DFS_search(tree, s, i, N, result);
            if(*result==1){ //if result=1, after DFS_search() then finish the operation
                pop(s,i);
                return;
            }
        }
    }
    pop(s,now);
    return;
}


void pass_result(int result_num,int *result,int result_arr[])   //pass the result to result array
{
    result_arr[result_num]=(*result);
}

void return_result(int result_arr[],int how_many)               //pass result array to output file
{
    FILE *of = fopen("/Users/jungwooseop/모시공/수업자료/자료구조/HW4/output-4.txt","w");
    for(int i=0;i<how_many;i++)
    {
    fprintf(of, "%d\n",result_arr[i]);
    }
    fclose(of);
}

int main()
{
    int how_many_times;      //the number of input expression
    int N;                   //input size of Nodes, size of Matrix[NxN]
    int result;              //result of expression
    stack s;                 //stack for DFS


    FILE *fp = fopen("/Users/jungwooseop/모시공/수업자료/자료구조/HW4/input-4.txt","r");
    fscanf(fp, "%d", &how_many_times);
    int *result_arr=(int*)malloc(sizeof(int)*how_many_times);
    
        for(int i=0;i<how_many_times;i++)
        {
            result=0;
            fscanf(fp, "%d", &N);
            
            int **M = (int**)malloc(sizeof(int *) * N);    //allocate M to define 2 dimensional array
            for (int i = 0; i < N; i++)
            {
                M[i] = (int*)malloc(sizeof(int) * N);
            }
            
            for(int i=0;i<N;i++)                           //input matrix element data
            {
                for(int j=0;j<N;j++)
                {
                    fscanf(fp, "%d", &M[i][j]);
                }
            }
            
            node* tree = (node*)malloc(sizeof(node)*N);  //dynamic allocation of nodes
            init_Node(tree, N);                          //initialize nodes
            Link_Node(tree, N, M);                       //link nodes using matrix M
            init_stack(&s);                              //initialize stacks
            for(int i=0;i<N;i++){                        //search every node as a starting node
                DFS_search(tree, &s, i, N, &result);
                if(result==1) break;
            }
            
            pass_result(i, &result, result_arr);        //pass the result
            free(tree);
        }
    return_result(result_arr, how_many_times);          //return result in txt file
    free(result_arr);                                   //free result heep data
}
