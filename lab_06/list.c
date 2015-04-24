/*
 *  Written by roman hargrave for lab 6
 */

#include "list.h"

Node* List_allocate(void* initial) {
	Node* newNode = allocate(Node);
	newNode->data = initial;

	return newNode;
}

void Node_free(Node* node) {
	sfree(node);
}

void List_free(Node* node) {
	unless(node) return;
	Node* next = node->next;
	node->next = NULL;
	Node_free(node);
	List_free(next);
}

Node* List_tail(Node* node) {
	unless(node) return NULL;
	unless(node->next) return node;
	return List_tail(node->next);
}
Node* List_append(Node* node, void* ptr) {
	Node* new = List_allocate(ptr);
	node->next = new;
	return new;
}

ListSize List_countLinks(Node* node) {
	unless(node) return 0;
	return 1 + List_countLinks(node->next);
}
