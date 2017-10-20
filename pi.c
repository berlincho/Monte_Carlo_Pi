#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#include <semaphore.h>
#include<sys/time.h>

double calculate_pi(long long int num_toss){
    long long int num_in_circle = 0;
    int toss;
    srand(time(NULL));

    for(toss=0; toss<num_toss; toss++){
        double x = rand() / (double)RAND_MAX;
        double y = rand() / (double)RAND_MAX;
        double distance = x*x + y*y;
        if(distance <= 1)
            num_in_circle++;
    }
    return 4*num_in_circle/(double)num_toss;
}

int main (int argc, char *argv[]){
    int num_core = atoi(argv[1]);
    long long int num_toss = atoll(argv[2]);
    double pi;

    printf("-> Number of Cores: %d\n", num_core);
    printf("-> Number of Toss: %lli\n", num_toss);

    //Time Start...//
    struct timeval start, end;
    gettimeofday(&start, 0);

    // Running... //
    pi = calculate_pi(num_toss);

    //Time End...//
    gettimeofday(&end, 0);
    int sec = end.tv_sec-start.tv_sec;
    int usec = end.tv_usec-start.tv_usec;

    printf("-> pi: %.10lf\n", pi);
    printf("------------------------------------------------------\n");
    printf("-> Elapsed Time: %lf sec\n", (sec*1000+(usec/1000.0))/1000);
    return 0;
}
