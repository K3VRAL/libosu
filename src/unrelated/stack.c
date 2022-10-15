#include "unrelated/stack.h"

void ou_stack_push(Stack **stack, void *data, unsigned int len) {
	Stack *node = calloc(1, sizeof(*node));
	node->data = data;
	node->len = len;
	node->next = *stack;
	*stack = node;
}

void ou_stack_pop(Stack **stack, void **data, unsigned int *len) {
	Stack *temp = *stack;
	*data = (*stack)->data;
	*len = (*stack)->len;
	(*stack) = (*stack)->next;
	free(temp);
}

void ou_stack_reverse(Stack **stack) {
	if (*stack != NULL) {
		void *data = NULL;
		unsigned int len = 0;
		ou_stack_pop(stack, &data, &len);
		ou_stack_reverse(stack);
		ou_stack_push(stack, data, len);
	}
}

void ou_stack_count(Stack *stack, unsigned int *count) {
	*count = 0;
	Stack *temp = stack;
	while (temp != NULL) {
		(*count)++;
		temp = temp->next;
	}
}