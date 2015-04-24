#include <stdio.h>
#include "set.h"

typedef struct sNode{
   Set data;
   struct sNode *next;
}stackNode;

typedef struct stackHeader{
    stackNode *head;
} stack;


void initStack(stack *);

void push(stack *, Set );

stackNode* pop(stack *);
