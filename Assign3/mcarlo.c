#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h> 
#include <string.h>

//global variables
int hit_count; 
double pi_calculated;

//threads
void *hit_scanner(void *param);
void *pi_calc(void *param);

//funtions
double random_double();


int main(int argc, char *argv[]) {

    srand(0);

    pthread_t tid[2]; /* the thread identifier */
    pthread_attr_t attr[2]; /* set of attributes for the thread */

    //checks for 2 arguemnts
    if (argc != 2) {
        fprintf(stderr,"usage: a.out <integer value>\n");
        /*exit(1);*/
        return -1;
    }

    //checks if the 2nd argument is only a number
    int i;
    for(i = 0; i < strlen(argv[1]); ++i){
        if(isalpha(argv[1][i]) == 1){
        fprintf(stderr,"Argument must be an integer\n");
        /*exit(1);*/
        return -1;
    }
    }

    //checks if the integer is negative
    if (atoi(argv[1]) < 0) {
        fprintf(stderr,"Argument %d must be non-negative\n",atoi(argv[1]));
        /*exit(1);*/
        return -1;
    }
    
    
    
    /* get the default attributes */
    pthread_attr_init(&attr[0]);
    pthread_attr_init(&attr[1]);

    /* create the hit counter thread */
    pthread_create(&tid[0],&attr[0],hit_scanner,argv[1]);

    /* now wait for the hit counter thread to exit */
    pthread_join(tid[0], NULL);

    /* create the pi calculation thread */
    pthread_create(&tid[1],&attr[1],pi_calc,argv[1]);

    /* now wait for the pi calculation thread to exit */
    pthread_join(tid[1], NULL);
    

    //returns pi
    printf("Pi = %f\n",pi_calculated);
    
}


//hit counter function
void *hit_scanner(void *param) {
    int i, npoints = atoi(param);
    double x, y;
    hit_count = 0;
    /* Check for points inside circle */
    for (i = 0; i < npoints; i++) {
        x = random_double() * 2.0 - 1.0;
        y = random_double() * 2.0 - 1.0;
        if (sqrt(x*x + y*y) < 1.0 )
        ++hit_count;

    }

        pthread_exit(0);
}

//produces a random double number
double random_double() {
    return random() / ((double)RAND_MAX + 1);
}

//calculates pi

void *pi_calc(void *param) {
    int npoints = atoi(param);
    pi_calculated = 4 * ((double)hit_count / (double)npoints);

    pthread_exit(0);
}