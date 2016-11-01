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

  return a1->arrivalTime - b1->arrivalTime;
}

int   sjfCompare (const void* a, const void* b)
{
  job_t* a1 = (job_t*)a;
  job_t* b1 = (job_t*)b;

  if(a1->runTime == b1->runTime)
  {
    return a1->arrivalTime - b1->arrivalTime;
  }
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
  if(a1->priority == b1->priority)
  {
    return a1->arrivalTime - b1->arrivalTime;
  }
  return a1->priority - b1->priority;
}

int   roundrobinCompare(const void* a, const void* b)
{
  (void*)a;
  (void*)b;
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


int coresAvailable(job_t** array, int cores)
{
  for (int i =0; i < cores; i++)
  {
    if (array[i] == NULL)
    {
      return 1;
    }
  }
  return 0;
}

int nextAvailableCore(job_t** array, int cores)
{
  for(int i =0; i< cores; i++)
  {
    if(array[i] == NULL)
    {
      return i;
    }
  }
  return -1;
}

void schedule(job_t** array, job_t* newJob, int coreID)
{
  array[coreID] = newJob;
}

int preemptNeeded(job_t** array, job_t* newJob, scheme_t scheme)
{

  if(scheme == PPRI)
  {
    int newPri = newJob->priority;
    for(int i=0; i<numCores; i++)
    {
      if(newPri < array[i]->priority)
      {
        return 1;
      }
    }
    return 0;
  }
  else
  {
    int newRunTime =newJob->runTime;
    for(int i=0; i<numCores; i++)
    {
      if(newRunTime < array[i]->runTime)
      {
        return 1;
      }
    }
    return 0;
  }
}

int duplicates(job_t** array, job_t* newJob, scheme_t scheme)
{
  for(int i=0; i<numCores; i++)
  {
    for(int j=0; j<numCores; j++)
    {
      if(array[i]->priority == array[j]->priority)
      {
        return 1;
      }
    }
  }
  return 0;
}


int coreToPreempt(job_t** array, job_t* newJob, scheme_t scheme)
{

  if(scheme == PPRI)
  {
      int maxPri = array[0]->priority;
      int valIndex = 0;
      for(int i=0; i<numCores; i++)
      {
        if(array[i]->priority > maxPri)
        {
          maxPri = array[i]->priority;
          valIndex = i;

        }
      }

      int newestJob = array[valIndex]->lastTimeScheduled;
      for(int i=0; i<numCores; i++)
      {
        if(array[i]->priority == maxPri)
        {
          if(array[i]->lastTimeScheduled > newestJob)
          {
            newestJob == array[i]->lastTimeScheduled;
          }
        }
      }
      for(int i=0; i<numCores; i++)
      {
        if(array[i]->priority == maxPri)
        {
          if(array[i]->lastTimeScheduled == newestJob)
          {
            return i;
          }

        }
      }
  }
  else
  {
    return -1;
  }


//   if(scheme == PPRI)
//   {
//
//   int maxPri = array[0]->priority;
//   for(int i=0; i<numCores; i++)
//   {
//     if(array[i]->priority > maxPri)
//     {
//       maxPri = array[i]->priority;
//     }
//   }
//   for(int i=0; i<numCores; i++)
//   {
//     if(array[i]->priority == maxPri)
//     {
//       return i;
//     }
//   }
//
//
//
//
//     return -1;
// /*
//
//     int minPri = array[0]->priority;
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i]->priority > minPri)
//       {
//         minPri = array[i]->priority;
//       }
//     }
//
//     int newestJob = array[0]->arrivalTime;
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i]->arrivalTime > newestJob)
//       {
//         if(array[i]->priority = minPri)
//         {
//           newestJob = array[i]->arrivalTime;
//         }
//       }
//     }
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i]->priority == minPri)
//       {
//         if(array[i]->arrivalTime = newestJob)
//         {
//           return i;
//         }
//       }
//     }
// */
//
//   }
//   else
//   {
//     int maxRunTime = array[0]->runTime;
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i] ->runTime > maxRunTime)
//       {
//         maxRunTime = array[i]->runTime;
//       }
//     }
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i] ->runTime > maxRunTime)
//       {
//         maxRunTime = array[i]->runTime;
//       }
//     }
//     for(int i=0; i<numCores; i++)
//     {
//       if(array[i]->runTime == maxRunTime)
//       {
//         return i;
//       }
//
//     }
//   }
}


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
      preemptFlag=0;
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
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
      job_t* newJob = malloc(sizeof(job_t));
      newJob-> beenScheduled=0;
      newJob-> jobid=job_number;
      newJob-> arrivalTime=time;
      newJob-> runTime=running_time;
      newJob-> priority=priority;
      newJob-> waitTime = 0;
      newJob-> lastPutinQueue=0;


      for(int i = 0; i< numCores; i++)
      {
        if(jobsArray[i] != NULL)
        {
          jobsArray[i]->runTime -= time - jobsArray[i]->lastTimeScheduled;
        }
      }

      if(!preemptFlag)
      {
        if(coresAvailable(jobsArray, numCores))
        {
          int nextCore = nextAvailableCore(jobsArray, numCores);
          schedule(jobsArray, newJob, nextCore);
          newJob->firstTimeScheduled = time;
          newJob-> lastTimeScheduled = time;
          newJob-> beenScheduled = 1;
          return nextCore;
        }
        else
        {
          priqueue_offer(&queue, newJob);
          newJob->lastPutinQueue = time;
          return -1;
        }
      }
      else
      {
        if(coresAvailable(jobsArray, numCores))
        {
          int nextCore = nextAvailableCore(jobsArray, numCores);
          newJob->firstTimeScheduled = time;
          newJob->lastTimeScheduled = time;
          newJob->beenScheduled = 1;
          schedule(jobsArray, newJob, nextCore);
          return nextCore;
        }
        else
        {
          if(preemptNeeded(jobsArray, newJob, currentScheme))
          {
            newJob->lastTimeScheduled = time;
            newJob->firstTimeScheduled = time;
            newJob->beenScheduled = 1;

            int nextCore = coreToPreempt(jobsArray,newJob, currentScheme);
            priqueue_offer(&queue, jobsArray[nextCore]);
            jobsArray[nextCore]->lastPutinQueue = time;
            schedule(jobsArray, newJob, nextCore);

            return nextCore;
          }
          else
          {
            priqueue_offer(&queue, newJob);
            newJob->lastPutinQueue = time;
            return -1;
          }
        }
      }

  return -1;
}

/**
  Called when a job has completed execution.

  The core_id, job_number and time parameters are provided for convenience.
   You may be able to calculate the values with your own data structure.
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
        job_t* lastJob = jobsArray[core_id];

        if(priqueue_empty(&queue))
        {
          jobsArray[core_id]=NULL;
          totalTurnAroundTime += time - lastJob->arrivalTime;
          totalResponseTime += lastJob->firstTimeScheduled - lastJob->arrivalTime;
          totalWaitTime+= lastJob->waitTime;
          free(lastJob);
          return -1;
        }
        else
        {
          job_t* nextJob = priqueue_poll(&queue);
          jobsArray[core_id] = nextJob;
          nextJob->lastTimeScheduled = time;
          nextJob->waitTime += time - nextJob->lastPutinQueue;

          if(!nextJob->beenScheduled)
          {
            nextJob->firstTimeScheduled = time;
            nextJob-> beenScheduled = 1;
          }


          totalTurnAroundTime += time - lastJob->arrivalTime;
          totalResponseTime += lastJob->firstTimeScheduled - lastJob->arrivalTime;
          totalWaitTime+= lastJob->waitTime;

          free(lastJob);
          return jobsArray[core_id]->jobid;
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

  if(priqueue_empty(&queue))
  {
    return jobsArray[core_id]->jobid;
  }
  else
  {
    job_t* lastJob = jobsArray[core_id];
    lastJob->lastPutinQueue = time;
    priqueue_offer(&queue, lastJob);
    job_t* newJob = priqueue_poll(&queue);

    jobsArray[core_id] = newJob;
    newJob->waitTime += time - newJob->lastPutinQueue;
    newJob->lastTimeScheduled=time;
    if(!newJob->beenScheduled)
    {
      newJob->firstTimeScheduled = time;
      newJob->beenScheduled=1;
    }


    return jobsArray[core_id]->jobid;
  }

}

/*
  if(priqueue_empty(&queue) && jobsArray[0] == NULL)
  {
    return -1;
  }
  else if(priqueue_empty(&queue) && jobsArray[0] != NULL)
  {
    return jobsArray[0]->jobid;
  }
  else
  {
    priqueue_offer(&queue, jobsArray[0]);
    jobsArray[0]->lastPutinQueue = time;
    jobsArray[0]=priqueue_poll(&queue);
    jobsArray[0] -> waitTime += time - jobsArray[0]->lastPutinQueue;
    if(!jobsArray[0]->beenScheduled)
    {
      jobsArray[0] ->firstTimeScheduled= time;
      jobsArray[0] -> beenScheduled = 1;
    }
    return jobsArray[0]->jobid;
  }
}
*/

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

       printf(" %d, " , curid);
    }
  }

}
