#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include<sys/time.h>

//Global Variables...//
int num_theads;
long long int num_toss_for_one_thread;
long long int total_in_circle = 0;
sem_t sem;

double serial_calculate_pi(long long int num_toss){
    long long int num_in_circle = 0;
    srand(time(NULL));

    long long int toss;
    for(toss=0; toss<num_toss; toss++){

        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        double distance = x*x + y*y;
        if(distance <= 1)
            num_in_circle++;
    }

    return 4*num_in_circle/(double)num_toss;
}


void* calculate_pi(void *arg){
    long long int num_in_circle = 0;
    srand(time(NULL));

    long long int toss;
    unsigned int seed = rand();
    for(toss=0; toss<num_toss_for_one_thread; toss++){
        //-1~1//
        double x = 2.0 * (double)rand_r(&seed) / (RAND_MAX + 1.0) + (-1.0);
        double y = 2.0 * (double)rand_r(&seed) / (RAND_MAX + 1.0) - (-1.0);
        num_in_circle = (x*x + y*y <= 1.0)? num_in_circle+1 : num_in_circle;
    }

    //Critiacal Section...//
    sem_wait(&sem);
    total_in_circle += num_in_circle;
    sem_post(&sem);
    pthread_exit(NULL);
}

int main (int argc, char *argv[]){

    if(argc != 3){
        printf("-> Error: Not Pass two arguments. <# of Cores> <# of tosses>\n");
        exit(-1);
    }

    int num_core = atoi(argv[1]);
    num_theads = num_core*4;
    long long int num_toss = atoll(argv[2]);
    printf("-> Number of Cores: %d\n", num_core);
    printf("-> Number of Toss: %lli\n", num_toss);
    num_toss_for_one_thread = num_toss / num_theads;

    //Time Start...//
    struct timeval start, end;
    gettimeofday(&start, 0);

    // Running... //
    //pi = serial_calculate_pi(num_toss);

    pthread_t tid[num_theads];
    sem_init(&sem, 0, 1);

    int t;
    for(t=0; t<num_theads; t++){
        int rc = pthread_create(&tid[t], NULL, calculate_pi, NULL);
        if (rc){
            printf("-> ERROR: pthread_create() is %d\n", rc);
            exit(-1);
        }
    }
    
    for(t=0; t<num_theads; t++){
        pthread_join(tid[t], NULL);
    }
    sem_destroy(&sem);

    //Time End...//
    gettimeofday(&end, 0);
    int sec = end.tv_sec - start.tv_sec;
    int usec = end.tv_usec - start.tv_usec;

    //Output Result...//
    printf("-> pi: %lf\n", 4*total_in_circle / (double)num_toss);
    printf("------------------------------------------------------\n");
    printf("-> Elapsed Time: %lf sec\n", (sec*1000+(usec/1000.0))/1000);
    return 0;
}
