#include "unrelated/linkedlist.h"

void ou_linkedlist_add(LinkedList **llist, void *data, unsigned int len) {
	LinkedList *node = calloc(1, sizeof(*node));
	node->data = data;
	node->len = len;
	node->next = NULL;

	if (*llist != NULL) {
		LinkedList *temp = *llist;
		while (temp != NULL && temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = node;
	} else {
		*llist = node;
	}
}

void ou_linkedlist_remove(LinkedList **llist, void **data, unsigned int *len) {
	*data = (*llist)->data;
	*len = (*llist)->len;
	LinkedList *temp = *llist;
	(*llist) = (*llist)->next;
	free(temp);
}

void ou_linkedlist_count(LinkedList *llist, unsigned int *count) {
	*count = 0;
	LinkedList *temp = llist;
	while (temp != NULL) {
		(*count)++;
		temp = temp->next;
	}
}