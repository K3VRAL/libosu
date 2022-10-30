#ifndef OU_STACK_H
#define OU_STACK_H

#include <stdlib.h>

typedef struct Stack {
	void *data;
	unsigned int len;
	struct Stack *next;
} Stack;

void ou_stack_push(Stack **, void *, unsigned int);
void ou_stack_pop(Stack **, void **, unsigned int *);
void ou_stack_reverse(Stack **);
void ou_stack_count(Stack *, unsigned int *);

#endif