/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"




//these functions will be used to store jobs according to the current scheme
int   fcfsCompare(const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;
  //return b1->arrivalTime - a1->arrivalTime;
  return a1->arrivalTime - b1->arrivalTime;
}

int   sjfCompare (const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;

  return a1->runTime - b1->runTime;
}

int   psjfCompare(const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;

  if(a1->runTime == b1->runTime)
  {
    return a1->arrivalTime - b1->arrivalTime;
  }
  return a1->runTime - b1->runTime;

}

int   priorityCompare(const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;
  return a1->priority - b1->priority;
}

int   roundrobinCompare(const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;

  return -1;
}

//preemptive priority
int   ppriCompare(const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;
  if (a1->priority == b1->priority)
  {
   return a1->arrivalTime - b1->arrivalTime;
  }

  return a1->priority - b1->priority;
}

//GLOBAL VARIABLES
double totalWaitTime = 0;
double totalResponseTime = 0;
double numJobsFinished = 0;
double totalTurnAroundTime = 0;

/**
  Initalizes the scheduler.
  possible schemes: FCFS = 0, SJF, PSJF, PRI, PPRI, RR

  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/

void scheduler_start_up(int cores, scheme_t scheme)
{
  numCores = cores;
  currentScheme = scheme;
  jobsArray = malloc( cores * (sizeof(job_t)));

  for (int i =0; i<cores; i++)
  {
    jobsArray[i] = NULL;
  }

  switch(scheme)
  {
    case FCFS:
      priqueue_init(&queue, fcfsCompare);
      preemptFlag=0;
      break;
    case SJF:
      priqueue_init(&queue, sjfCompare);
      preemptFlag=0;
      break;
    case PSJF:
      priqueue_init(&queue, psjfCompare);
      preemptFlag=1;
      break;
    case PRI:
      priqueue_init(&queue, priorityCompare);
      preemptFlag=0;
      break;
    case PPRI:
      priqueue_init(&queue, ppriCompare);
      preemptFlag=1;
      break;
    case RR:
      priqueue_init(&queue, roundrobinCompare);
      break;

  }
}


/**
  Called when a new job arrives.

  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made.

 */


/*keep an array of size[numCores]. this array will hold a pointer to the current job
  being run on core i; Every time a job finishes, pop the front of the queue to
  the array. the new job being run is going to be what was popped into the array of size 1;
  this will make things alot easier. The queue is just there to store the list of jobs.
if current job gets preempted, reinsert the job from the array into the queue, this will
preserve the order, and re inserting into the queue will always keep it in order
*/

//TODO: IMPLEMENT JAMIE'S ARRAY SOLUTION
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
      job_t* newJob = malloc(sizeof(newJob));
      newJob->jobid=job_number;
      newJob->arrivalTime=time;
      newJob->lastTimeScheduled = time;
      newJob->runTime=running_time;
      newJob->priority=priority;
      newJob->waitTime = 0;
      if(jobsArray[0] != NULL)
      {
        jobsArray[0] -> runTime = time - jobsArray[0]->lastTimeScheduled;
      }
    //  newJob->beenScheduled=0;

      job_t* currentJob = jobsArray[0];

      if(!preemptFlag)
      {
        if(priqueue_empty(&queue) && jobsArray[0] == NULL)
        {
          jobsArray[0] = newJob;
        //  newJob->beenScheduled = 1;
          return 0;
        }
        else if (priqueue_empty(&queue) && jobsArray[0] != NULL)
        {
          priqueue_offer(&queue, newJob);
        //  newJob->beenScheduled = 0;
          return -1;
        }
        else if (priqueue_not_empty(&queue) && jobsArray != NULL)
        {
          priqueue_offer(&queue, newJob);
        //  newJob->beenScheduled = 0;
          return -1;
        }
        return -1;

      }
      else // preemptive algorithm was selected, different conditions
      //TODO: PROBLEM ADDING BACK INTO QUEUE
      //HAS TO DO WITH COMPARISON
      {
        if (currentScheme == PPRI)// || currentScheme == PSJF)
        {//empty queue and no assigned jobs
          if (priqueue_empty(&queue) && jobsArray[0] == NULL)//easy case
          {
            jobsArray[0] = newJob;
            //  newJob->beenScheduled = 1;
            return 0;
          }//empty queue and job is running
          else if(priqueue_empty(&queue) && jobsArray[0] != NULL)
          {
            job_t* curJob = jobsArray[0];

            //we need to check if the priorities are the same and if so, sort
            //by arrival time
            if(curJob->priority == newJob->priority)
            {
              //cant priqueue insert front because we need to check if the front
              //is the same priority and insert behind it
              //priqueue_insert_front(&queue, newJob);
              priqueue_offer(&queue,newJob);
            //    newJob->beenScheduled = 0;
              return -1;
            }
            else if(curJob->priority > newJob->priority)
            {
              priqueue_insert_front(&queue, curJob);
              curJob = newJob;
              //  newJob->beenScheduled = 1;
              return 0;
            }
            else
            {
              priqueue_offer(&queue, newJob);
              //  newJob->beenScheduled = 0;
              return -1;
            }

          }//nonEmpty queue and job is running
          else if(priqueue_not_empty(&queue) && jobsArray[0] !=NULL)
          {
            //here we need to preempt
            if (jobsArray[0] -> priority == newJob->priority)
            {
              if(jobsArray[0] ->arrivalTime <= newJob->arrivalTime)
              {
                priqueue_insert_front(&queue,newJob);
                return -1;
              }
              else//new job belongs in job array
              {
                job_t* temp = jobsArray[0];
                jobsArray[0]=newJob;
                priqueue_insert_front(&queue, temp);//&temp instead?
                return 0;
              }
              //priqueue_insert_front(&queue, newJob);
              //return -1;
            }
            else if (jobsArray[0]->priority > newJob -> priority)
            {

              priqueue_insert_front(&queue, jobsArray[0]);
              jobsArray[0] = newJob;
              return 0;
            }
            else
            {
              priqueue_offer(&queue, newJob);
              return -1;
            }


          }

        }
        else if(currentScheme == PSJF)
        {
          if(jobsArray[0] == NULL)
          {
            jobsArray[0] = newJob;
            newJob -> lastTimeScheduled = time;
            return 0;
          }
          else if(jobsArray[0] != NULL)
          {
            if(newJob->runTime < jobsArray[0]->runTime || newJob->runTime == jobsArray[0]->runTime)
            {
              priqueue_offer(&queue, jobsArray[0]);
              newJob->lastTimeScheduled = time;
              jobsArray[0] = newJob;
              return 0;
            }
            else
            {
              priqueue_offer(&queue, newJob);
              return -1;
            }
          }
          else if(priqueue_not_empty(&queue))
          {
            priqueue_offer(&queue, newJob);
            return -1;
          }

        //  else if(priqueue_not_empty(&queue)) //assumes jobsArray[0] != NULL
        //  {

        //  }
        }
        // if (priqueue_empty(&queue) && jobsArray[0] == NULL)//easy case
        // {
        //   jobsArray[0] = newJob;
        //   //  newJob->beenScheduled = 1;
        //   return 0;
        // }//empty queue and job is running
        // else if(priqueue_empty(&queue) && jobsArray[0] != NULL)
        // {
        //   job_t* curJob = jobsArray[0];
        //
        //   //we need to check if the priorities are the same and if so, sort
        //   //by arrival time
        //   if(jobsArray[0]->runTime < newJob->runTime)
        //   {
        //     priqueue_offer(&queue, jobsArray[0]);
        //     jobsArray[0]=newJob;
        //     return 0;
        //   }
        //
        //   if(curJob->runTime == newJob->runTime)
        //   {
        //     //cant priqueue insert front because we need to check if the front
        //     //is the same priority and insert behind it
        //     //priqueue_insert_front(&queue, newJob);
        //     priqueue_offer(&queue,newJob);
        //   //    newJob->beenScheduled = 0;
        //     return -1;
        //   }
        //   else if(curJob->runTime > newJob->runTime)
        //   {
        //     priqueue_offer(&queue, curJob);
        //   //  curJob -> runTime = time - curJob->arrivalTime;
        //     curJob = newJob;
        //     //  newJob->beenScheduled = 1;
        //     return 0;
        //   }
        //   else
        //   {
        //     priqueue_offer(&queue, newJob);
        //     //  newJob->beenScheduled = 0;
        //     return -1;
        //   }
        //
        // }//nonEmpty queue and job is running
        // else if(priqueue_not_empty(&queue) && jobsArray[0] !=NULL)
        // {
        //   //here we need to preempt
        //   if (jobsArray[0] -> runTime == newJob->runTime)
        //   {
        //     if(jobsArray[0] ->arrivalTime <= newJob->arrivalTime)
        //     {
        //       //priqueue_insert_front(&queue,newJob);
        //       priqueue_offer(&queue, newJob);
        //       return -1;
        //     }
        //     else//new job belongs in job array
        //     {
        //       job_t* temp = jobsArray[0];
        //     //  jobsArray[0] -> runTime = time - jobsArray[0] ->arrivalTime;
        //       jobsArray[0]=newJob;
        //     //  priqueue_insert_front(&queue, temp);//&temp instead?
        //       priqueue_offer(&queue, temp);
        //       return 0;
        //     }
        //     //priqueue_insert_front(&queue, newJob);
        //     //return -1;
        //   }
        //   else if (jobsArray[0]->runTime > newJob -> runTime)
        //   {
        //
        // //    priqueue_insert_front(&queue, jobsArray[0]);
        //     priqueue_offer(&queue, jobsArray[0]);
        //   //  jobsArray[0]-> runTime = time - jobsArray[0] -> arrivalTime;
        //     jobsArray[0] = newJob;
        //     return 0;
        //   }
        //   else
        //   {
        //     priqueue_offer(&queue, newJob);
        //     return -1;
        //   }
        //}
      }

  return -1;
}


/**
  Called when a job has completed execution.

  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.

  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
        numJobsFinished++;

        if( !preemptFlag)
        {
          if(priqueue_empty(&queue))
          {
            job_t* lastJob = jobsArray[0];
            totalTurnAroundTime += time - lastJob->arrivalTime;
            free(lastJob);
            jobsArray[0] = NULL;
            return -1;
          }
          else
          {
            job_t* lastJob = jobsArray[0];
            totalTurnAroundTime += time - lastJob->arrivalTime;
            free(lastJob);

            job_t* nextJob = (job_t*)priqueue_poll(&queue);
            nextJob->lastTimeScheduled = time;
            jobsArray[0] = nextJob;

            totalWaitTime+= time - nextJob->arrivalTime;
            totalResponseTime+= time - nextJob->arrivalTime;

            return nextJob->jobid;
          }

        }
        else
        {
          if(priqueue_empty(&queue))
          {
            job_t* lastJob = jobsArray[0];
            totalWaitTime += lastJob->waitTime;
            totalTurnAroundTime += time - lastJob->arrivalTime;

            free(lastJob);
            jobsArray[0] = NULL;
            return -1;
          }
          else
          {
            job_t* lastJob = jobsArray[0];
            totalWaitTime+=lastJob->waitTime;
            totalTurnAroundTime += time - lastJob->arrivalTime;
            free(lastJob);

            job_t* nextJob = (job_t*)priqueue_poll(&queue);

            jobsArray[0] = nextJob;

            nextJob->lastTimeScheduled = time;

            return nextJob->jobid;
          }
        }
}


/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.

  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time)
{
	return -1;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
	return totalWaitTime/numJobsFinished;

}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
	return totalTurnAroundTime/numJobsFinished;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
	return totalResponseTime/numJobsFinished;
}


/**
  Free any memory associated with your scheduler.

  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{
  priqueue_destroy(&queue);
  free(jobsArray);
}


/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)

  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
  if(jobsArray[0] != NULL)
  {
    printf("CORE 0: %d(%d)\n", jobsArray[0] -> jobid, jobsArray[0]->runTime);
  }
  else
  {
    printf("NO JOB CURRENTLY RUNNING");
  }
  if(priqueue_not_empty(&queue))
  {
    for(int i = 0; i< priqueue_size(&queue); i++)
    {
      int curid = ((job_t*)priqueue_at(&queue, i))->jobid;
      int curPrior = ((job_t*)priqueue_at(&queue, i))->priority;
      int arriv = ((job_t*)priqueue_at(&queue, i))->arrivalTime;
      int currunTime = ((job_t*)priqueue_at(&queue, i))->runTime;
      //printf(" ID: %d STAT: %d PRI: %d Ariv: %d, ",
       //curid, curStatus, curPrior, arriv);

       printf(" %d(%d), " , curid,currunTime);
    }
  }

}
