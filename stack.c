#include "stack.h"
#include "node.h"
#define DEBUG

struct node* push(struct stack* s, char* nodeName, int nodeVal){
    struct node* n = newNode(nodeName, nodeVal);

    // set as bottom node in stack
    if(s->height == 0){
        s->bottom = n; // set as root/bottom nopde
    }else{
        s->top->next = n; // append to top of stack
    }
    s->height++; // increment height to reflect addition of new node
    s->top = n;
    return s->top;
}

struct node* pop(struct stack* s){
    struct node* ptr1 = s->top;
    struct node* ptr2 = s->bottom;

    // Navigate to second to last node in stack
    while (ptr2->next != ptr1){
        ptr2 = ptr2->next;
    }

    // Re assign values
    s->top = ptr2;
    s->top->next = NULL;
    s->height--;
    return ptr1;
}

// helper functions
void printStack(struct stack* s){
    struct node* pointer = s->bottom;
    while(pointer->next != NULL){
        // need to cover conditional case
        if(strstr(pointer->equation, "if(") == NULL){
            printf("%s%s", pointer->nodeName, pointer->equation);
        }else{
            printf("%s", pointer->equation);
        }
        // progress in stack
        pointer = pointer->next;
    }
}

void deleteStack(struct stack* s){
    struct node* pointer = s->bottom;
    deleteNodes(pointer);
    free(s);
}