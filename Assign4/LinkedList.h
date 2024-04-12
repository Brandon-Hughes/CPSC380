#ifndef LIST_H
#define LIST_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task.h"

struct node {
   Task data;
   struct node *next;
};
struct node *head;
struct node *current;
int num_items;

// display the list
void printList();
//insertion at the beginning
void insertatbegin(Task data); 
void insertatend(Task data);
void insertafternode(struct node *list, Task data);
void deleteatbegin();
void deleteatend();
void deletenode(Task key);
int searchlist(Task key);
int isEmpty();
void pSort(struct node* head);
void bSort(struct node* head);
void swap(struct node* a, struct node* b);

#endif