/** @file libscheduler.h
 */

#ifndef LIBSCHEDULER_H_
#define LIBSCHEDULER_H_
#include "../libpriqueue/libpriqueue.h"


/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job queue elements.
*/
typedef struct job_t
{
  int jobid;
  int priority;
  int arrivalTime;
  int runTime;
  int beenScheduled;
  int waitTime;
  int responseTime;
  int lastTimeScheduled;
  int firstTimeScheduled;
  int lastPutinQueue;


  //other control members will be added as necessary.
} job_t;


/**
  Constants which represent the different scheduling algorithms
*/
typedef enum {FCFS = 0, SJF, PSJF, PRI, PPRI, RR} scheme_t;

scheme_t currentScheme;
int numCores;
priqueue_t queue;

job_t** jobsArray;
int preemptFlag;


//functions to be used for inserting into queue
int   fcfsCompare         (const void* a, const void* b);
int   sjfCompare          (const void* a, const void* b);
int   psjfCompare         (const void* a, const void* b);
int   priorityCompare     (const void* a, const void* b);
int   roundrobinCompare   (const void* a, const void* b);
int   ppriCompare         (const void* a, const void* b);


void  scheduler_start_up               (int cores, scheme_t scheme);
int   scheduler_new_job                (int job_number, int time, int running_time, int priority);
int   scheduler_job_finished           (int core_id, int job_number, int time);
int   scheduler_quantum_expired        (int core_id, int time);
float scheduler_average_turnaround_time();
float scheduler_average_waiting_time   ();
float scheduler_average_response_time  ();
void  scheduler_clean_up               ();

void  scheduler_show_queue             ();

#endif /* LIBSCHEDULER_H_ */
