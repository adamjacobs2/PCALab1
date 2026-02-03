
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2  

long long int* Xi;
int	numtasks,            /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype;                /* message type */
    aveN, extra; /* used to determine rows sent to each worker */

MPI_Status status;

double estimate_g(double lower_bound, double upper_bound, long long int N){
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    if (numtasks < 2 ) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }
    

    numworkers = numtasks-1;
    
    aveN  = N / numworkers;
    extra = N % numworkers;
    
    mtype = FROM_MASTER;
    

    if(taskid != MASTER){
        for(int i = 1; i <= numworkers; i++){
            size_t localSize = (dest <= extra) ? aveN+1 : aveN; 
            Xi = malloc(localSize);
            for(int j = 0; j < localSize; j++){
                srand(taskid);
                Xi[j] = (long long int) rand() % N;
            }
        }
    }
}

void collect_results(double *result){

}

void init_rand_seed(){
    srand(time(NULL));
}




int main(int argc, char **argv)
{
    double result = 0.0;
    MPI_Init(&argc, &argv);
    float lower_bound = atof(argv[1]);
    float upper_bound = atof(argv[2]);
    long long int N = atof(argv[3]);

    init_rand_seed(N); // using srand()
    result = estimate_g(lower_bound, upper_bound, N);
    collect_results(&result);
    MPI_Finalize();
    return 0;
}
