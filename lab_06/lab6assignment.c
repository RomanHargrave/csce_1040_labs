/*
 *  re-written by roman hargrave for lab 6
 */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

typedef struct account {
	char* name;
	float balance;
} Account;

int main()
{

	Node* listOne	= NULL;
	Node* listTwo	= NULL;

	{
		Node* listOneTail = listOne;
		Node* listTwoTail = listTwo;
		int listId = 0;
		float balance = 0;
		char name[256];

		while(scanf("%d%s%f", &listId, name, &balance) != EOF) {

			Account* newAccount = allocate(Account);
			asprintf(&newAccount->name, name);
			newAccount->balance = balance;

			switch (listId) {
				case 1:
					listOneTail = List_appendOrNew(listOneTail, newAccount);
					unless(listOne) listOne = listOneTail;
					break;
				case 2:
					listTwoTail = List_appendOrNew(listTwoTail, newAccount);
					unless(listTwo) listTwo = listTwoTail;
					break;
				default:
					continue;
			}

		}

	}

	{
		Node* appendFrom = NULL;
		Node* appendTo   = NULL;
		ListSize sizeOfOne = List_countLinks(listOne);
		ListSize sizeOfTwo = List_countLinks(listTwo);

		if(sizeOfOne > sizeOfTwo) {
			appendFrom = listTwo;
			appendTo   = listOne;
		} else {
			appendFrom = listOne;
			appendTo   = listTwo;
		}

		List_tail(appendTo)->next = appendFrom;	
	}


	List_foreach(listOne, value, { 
			unless(value) continue;
			Account* acct = (Account*) value;
			printf("%20s %8.2f\n", acct->name, acct->balance);
			});

	printf("-------------------------------------------------------------------------\n");

	List_foreach(listTwo, value, { 
			unless(value) continue;
			Account* acct = (Account*) value;
			printf("%20s %8.2f\n", acct->name, acct->balance);
			});
	return 0;
}
