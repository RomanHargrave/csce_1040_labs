/*
 * Written by roman hargrave for lab 6
 */


#include <stdlib.h>

#pragma once

#define sfree(ptr) ({if(ptr) free(ptr);})
#define allocate(type) ( (type *) malloc( sizeof( type ) ) )

#define unless(predicate) if(!(predicate))
#define until(predicate) while(!(predicate))

#define List_foreach(list, name, closure) ({\
		Node* current = list;\
		until (!current) {\
		void* name = current->data;\
		(closure);\
		current = current->next;\
		}\
		})

#define List_appendOrNew(lst, ptr) ({\
		Node* next;\
		if (lst) {\
		next = List_append(lst, ptr);\
		} else {\
		next = List_allocate(ptr);\
		}\
		next;\
		})

typedef struct List_Node {
	struct List_Node* next;
	void* data;
} Node;

typedef struct List_Iterator {
	Node* next;
	void* data;
} Iterator;

typedef unsigned long int ListSize;

Node* List_allocate(void*);

void Node_free(Node*);
void List_free(Node*);

Node* List_append(Node*, void*);

Node* List_tail(Node*);

ListSize List_countLinks(Node*);

