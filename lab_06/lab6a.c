#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct weapon {
  char          *name;
  int           price;
  struct weapon *next;
};

int main()
{
  int   price;
  char  item[20];
  struct weapon *head = NULL;
  struct weapon *tail = NULL;
  struct weapon *walk = NULL;
  struct weapon *temp = NULL;

  while(scanf("%s%d", item, &price) != EOF) {
    printf("Read in: %s %d\n", item, price);
    if(head == NULL) {

      // INSERT CODE HERE
    }
    else { // the list already has nodes

      // INSERT CODE HERE
    }

    // INSERT CODE HERE
  }
  // print the list
  printf("---------- The original list: -----\n");

  // INSERT CODE HERE

  // remove the items from the list

  // INSERT CODE HERE

  return;
}
