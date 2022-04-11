#include <stdio.h>
#include <stdlib.h>

struct graph{
    struct graphNode* head[100];
};

struct graphNode{
    int dest;
    struct graphNode* next;
};

struct edge{
    int src;
    int dest;
};

struct graph* createGraph(struct edge e[], int n){
    
}

void printGraph(struct graph* g){

}

int main(){

}