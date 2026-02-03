
#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define MASTER 0               /* taskid of first task */
#define FROM_MASTER 1          /* setting a message type */
#define FROM_WORKER 2          

//#define DEBUG

# define PI		3.14159265358979323846	/* pi */
# define E		2.7182818284590452354	/* e */
/*   ttype: type to use for representing time */
typedef double ttype;
ttype tdiff(struct timespec a, struct timespec b)
/* Find the time difference. */
{
  ttype dt = (( b.tv_sec - a.tv_sec ) + ( b.tv_nsec - a.tv_nsec ) / 1E9);
  return dt;
}

struct timespec now()
/* Return the current time. */
{
  struct timespec t;
  clock_gettime(CLOCK_REALTIME, &t);
  return t;
}

struct timespec begin, end;
double time_spent;

int	numtasks,            /* number of tasks in partition */
	taskid,                /* a task identifier */
	numworkers,            /* number of worker tasks */
	source,                /* task id of message source */
	dest,                  /* task id of message destination */
	mtype,                /* message type */
    aveN,
    extra; /* used to determine rows sent to each worker */

MPI_Status status;

double function_x(double x){
    return (double) (8.0 * sqrt(2.0 * PI)) / pow(E, 4.0 * x * x);
}


double estimate_g(double lower_bound, double upper_bound, long long int N){
   
    if (numtasks < 2 ) {
        printf("Need at least two MPI tasks. Quitting...\n");
        MPI_Abort(MPI_COMM_WORLD, 1);
        exit(1);
    }
    

    numworkers = numtasks-1;
    aveN  = N / numworkers;
    extra = N % numworkers;
    
    //Workers populate random data between [a, b]
    double range = upper_bound - lower_bound;
    double sum = 0;
    double randDouble; 
    int localSize; 


    if(taskid != MASTER){
        
        localSize = (taskid <= extra) ? aveN+1 : aveN; 
        #ifdef DEBUG
        printf("Worker %d Generating and summing %d random numbers\n", taskid, localSize);
        #endif
        for(int i = 0; i < localSize; i++){
            //random double between [0, 1]
            randDouble = lower_bound + ((double)rand() / (double)RAND_MAX) * range;
            sum += function_x(randDouble);
        }
        return sum * range / (double) N;
    }

    return 0;


    
}

void collect_results(double *result){
    double final_result = 0;
    MPI_Reduce(result, &final_result, 1, MPI_DOUBLE, MPI_SUM, MASTER, MPI_COMM_WORLD);
    if(taskid == MASTER){
        end = now();
        time_spent = tdiff(begin, end);
        printf("Final Result: %f\n", final_result);
        printf("Time spent: %f\n", time_spent);
    }
     
         

}

void init_rand_seed(){
    MPI_Comm_rank(MPI_COMM_WORLD,&taskid);
    MPI_Comm_size(MPI_COMM_WORLD,&numtasks);
    srand(time(NULL) + taskid);
}




int main(int argc, char **argv)
{
    double result = 0.0;
    MPI_Init(&argc, &argv);
    float lower_bound = atof(argv[1]);
    float upper_bound = atof(argv[2]);
    long long int N = atof(argv[3]);

    init_rand_seed(); // using srand()
    begin = now();
    result = estimate_g(lower_bound, upper_bound, N);
    collect_results(&result);
    MPI_Finalize();
    return 0;
}
