#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

struct graphNode{
    char* nodeName;
    struct graphNode** edges;
    int edgeCount;

    int liveStart; // corresponds to which instruction variable is first referenced in
    int liveEnd; // corresponds to which instruction variable is last referenced in

    int allocatedRegister; // id for whatever register is currently allocated for
}graphNode;

struct graph{
    struct graphNode* nodes;
    int size;
}graph;

void allocateRegisters(struct stack* s);