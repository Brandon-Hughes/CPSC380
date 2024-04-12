Name: Brandon Hughes
Student ID: 2405732
Chapman email: bhughes@chapman.edu 
Course number and section: CPSC 380-01
Programming Assignment 4: CPU Scheduling

Source Files: 
    - cpu.c
    - cpu.h 
    - LinkedList.c 
    - LinkedList.h 
    - schedulers.c 
    - schedulers.h 
    - task.h 
    - main.c

Test taskList Files:
    - TaskList.txt
    - TaskList2.txt 

References:
    - In Class
    - CPSC 298 (c++ course, old files)
    - Linked List: https://www.tutorialspoint.com/data_structures_algorithms/linked_list_program_in_c.htm
    - bubble sort: https://www.geeksforgeeks.org/bubble-sort/
    - Zybooks

How to Run:
    1) gcc -o ssched *.c
    2) ./ssched <sched> <taskList>
        ex) ./ssched sjf TaskList.txt
