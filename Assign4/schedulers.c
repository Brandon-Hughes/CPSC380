
#include "task.h"
#include "cpu.h"
#include "LinkedList.h"
//needs a list of Tasks objects

//adds all items into a Task and then into a LinkedList
void add(char *name, int priority, int burst){
   Task newt;
   newt.name = name;
   newt.priority = priority;
   newt.burst = burst;
   insertatend(newt);

}

//takes in first job and then runs and deletes it
void schedule_fcfs(){
    
    while(isEmpty() == 0){
        run(&head->data, head->data.burst);
        deleteatbegin();
    }
}

//sorts based on priority and then does the jobs in order
void schedule_priority(){
    //based on priority, lowest number in priority goes first
    //after that search for new lowest and continue with that one
    //repeat until empty
    pSort(head);
    while(isEmpty() == 0){
        run(&head->data, head->data.burst);
        deleteatbegin();
    }
}

//does the job for 10 seconds or does the job for whatever remaining time it has
void schedule_rr(){
    while(isEmpty() == 0){
        if(head->data.burst > 10){
            run(&head->data, 10);
            head->data.burst = head->data.burst - 10;
            insertatend(head->data);
            deleteatbegin();
        }
        else{
            run(&head->data, head->data.burst);
            deleteatbegin();
        }
        
        
    }
}

//sorts based on burst and then does the jobs in order
void schedule_sfj(){
    bSort(head);
    while(isEmpty() == 0){
        run(&head->data, head->data.burst);
        deleteatbegin();
    }
}