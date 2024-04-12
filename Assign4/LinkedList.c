#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "task.h"
#include "LinkedList.h"

int isEmpty(){
    if(head == NULL){
        return 1;
    } else {
        return 0;
    }
}
// display the list
void printList(){
   struct node *p = head;
   printf("\n[");

   //start from the beginning
   while(p != NULL) {
      printf(" %s ",p->data.name);
      p = p->next;
   }
   printf("]");
}

//insertion at the beginning
void insertatbegin(Task data){

   //create a link
   struct node *lk = (struct node*) malloc(sizeof(struct node));
   lk->data = data;

   // point it to old first node
   lk->next = head;

   //point first to new first node
   head = lk;
   ++num_items;
}
void insertatend(Task data){

   //create a link
   struct node *lk = (struct node*) malloc(sizeof(struct node));
   lk->data = data;
   struct node *linkedlist = head;
   if(head != NULL){
   // point it to old first node
      while(linkedlist->next != NULL)
         linkedlist = linkedlist->next;
      //point first to new first node
      linkedlist->next = lk;
      ++num_items;
   } else {
      head = lk;
   }
   
}
void insertafternode(struct node *list, Task data){
   struct node *lk = (struct node*) malloc(sizeof(struct node));
   lk->data = data;
   lk->next = list->next;
   list->next = lk;
   ++num_items;
}
void deleteatbegin(){
   head = head->next;
   --num_items;
}
void deleteatend(){
   struct node *linkedlist = head;
   while (linkedlist->next->next != NULL)
      linkedlist = linkedlist->next;
   linkedlist->next = NULL;
   --num_items;
}
void deletenode(Task key){
   struct node *temp = head, *prev;
   if (temp != NULL && strcmp(temp->data.name, key.name) == 0) {
      head = temp->next;
      return;
   }

   // Find the key to be deleted
   while (temp != NULL && strcmp(temp->data.name, key.name) != 0) {
      prev = temp;
      temp = temp->next;
   }

   // If the key is not present
   if (temp == NULL) return;

   // Remove the node
   prev->next = temp->next;
   --num_items;
}
int searchlist(Task key){
   struct node *temp = head;
   while(temp != NULL) {
      if (strcmp(temp->data.name, key.name) == 0) {
         return 1;
      }
      temp=temp->next;
   }
   return 0;
}
void swap(struct node* a, struct node* b) {
    Task temp_data = a->data;
    a->data = b->data;
    b->data = temp_data;
}

void pSort(struct node* head) {
    int swapped;
    struct node* temp;
    struct node* last = NULL;

    // Check for empty list or single element list
    if (head == NULL)
        return;

    do {
        swapped = 0;
        temp = head;

        while (temp->next != last) {
            if (temp->data.priority > temp->next->data.priority) {
                swap(temp, temp->next);
                swapped = 1;
            }
            temp = temp->next;
        }
        last = temp;
    } while (swapped);
}
void bSort(struct node* head) {
    int swapped;
    struct node* temp;
    struct node* last = NULL;

    // Check for empty list or single element list
    if (head == NULL)
        return;

    do {
        swapped = 0;
        temp = head;

        while (temp->next != last) {
            if (temp->data.burst > temp->next->data.burst) {
                swap(temp, temp->next);
                swapped = 1;
            }
            temp = temp->next;
        }
        last = temp;
    } while (swapped);
}