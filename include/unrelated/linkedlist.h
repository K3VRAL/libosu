#ifndef OU_LINKEDLIST_H
#define OU_LINKEDLIST_H

#include <stdlib.h>
#include <string.h>

typedef struct LinkedList {
	void *data;
	unsigned int len;
	struct LinkedList *next;
} LinkedList;

void ou_linkedlist_add(LinkedList **, void *, unsigned int);
void ou_linkedlist_remove(LinkedList **, void **, unsigned int *);
void ou_linkedlist_count(LinkedList *, unsigned int *);

#endif