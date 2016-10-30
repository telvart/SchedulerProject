/** @file libpriqueue.c
 */

#include <stdlib.h>
#include <stdio.h>

#include "libpriqueue.h"


/**
  Initializes the priqueue_t data structure.

  Assumtions
    - You may assume this function will only be called once per instance of priqueue_t
    - You may assume this function will be the first function called using an instance of priqueue_t.
  @param q a pointer to an instance of the priqueue_t data structure
  @param new_cmp a function pointer that compares two elements.
  See also @ref comparer-page
 */



void priqueue_init(priqueue_t *q, comparer new_cmp)
{
  q->size=0;
  q->first = NULL;
  q->cmp=new_cmp;

}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priorits quantum expire. Core 0 is now running job 12.
  Queue: Currently Running: 12ity queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_not_empty(priqueue_t* q)
{
  if (q->size > 0)
  {
    return 1;
  }
  else
  {
    return 0;
  }
}

int priqueue_empty(priqueue_t* q)
{
  if (q->size == 0)
  {
    return 1;
  }
  return 0;
}

int priqueue_offer(priqueue_t *q, void *ptr)
{
        node* newNode = malloc(sizeof(node));
        newNode->content=ptr;
        newNode->next=0;

        if(q->size==0)
        {
          q->size++;
          q->first=newNode;
          return 0;
        }

        node* temp=q->first;
        node* parent = 0;
        int indexInserted=0;

        while((temp!=0) && q->cmp(temp->content, ptr) < 0)
      //  while((temp!=0) && q->cmp(temp->content, ptr) > 0)
        {
          parent=temp;
          temp=temp->next;
          indexInserted++;
        }

        if (indexInserted == 0)
        {
          q->size++;
          newNode->next=q->first;
          q->first=newNode;
          return 0;
        }

        parent->next=newNode;
        newNode->next=temp;
        q->size++;
        return indexInserted;

}


/**
  Retrieves, but does not remove, the head of this queue, returning NULL if
  this queue is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return pointer to element at the head of the queue
  @return NULL if the queue is empty
 */
void *priqueue_peek(priqueue_t *q)
{
	if (q->size==0)
  {
    return NULL;
  }
  else
  {
    return q->first->content;
  }
}


/**
  Retrieves and removes the head of this queue, or NULL if this queue
  is empty.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the head of this queue
  @return NULL if this queue is empty
 */
void *priqueue_poll(priqueue_t *q)
{
      if(q->first == NULL)//Case where queue is empty
      {
            return NULL;
      }
      else if(q->size==1)//Case where queue is size==1
      {
            struct node *temp = q->first;
            void* pop = q->first->content;
            free(q->first);
            q->first=0;
            q->size--;
            return pop;
      }
      else // general case
      {
            struct node* temp = q->first;
            void* contentRemoved = temp->content;
            q->first=temp->next;
            free(temp);
            q->size--;
            return contentRemoved;
      }
}


/**
  Returns the element at the specified position in this list, or NULL if
  the queue does not contain an index'th element.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of retrieved element
  @return the index'th element in the queue
  @return NULL if the queue does not contain the index'th element
 */
void *priqueue_at(priqueue_t *q, int index)
{
      if(priqueue_size(q) < index || index > q->size)//case where the index goes beyond the bounds
      {
            return NULL;
      }
	    else
      {
            struct node *temp = q->first;
            for(int i=0;i<index && temp->next != 0; i++)//maybe a one off error here?
            {
                  temp=temp->next;
            }
            return temp->content;
      }
}


/**
  Removes all instances of ptr from the queue.

  This function should not use the comparer function, but check if the data contained in each element of the queue is equal (==) to ptr.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr address of element to be removed
  @return the number of entries removed
 */
int priqueue_remove(priqueue_t *q, void *ptr)
{
      if (q->size < 1)
      {
        return 0;
      }
      if (q->cmp(ptr, q->first->content) == 0)
      {
        q->size--;
        node* temp = q->first;
        q->first=q->first->next;
        free (temp);
        return (priqueue_remove(q, ptr) + 1);
      }
      node* current = q->first->next;
      node* parent = q->first;
      int numRemoved = 0;

      while(current != 0)
      {
        if (q->cmp(current->content, ptr) == 0)
        {
          node* temp = current->next;
          parent->next = temp;
          numRemoved++;
          free(current);
          current=temp;
          q->size--;
        }
        else
        {
          parent=current;
          current=current->next;

        }
      }
      return numRemoved;
}




/**
  Removes the specified index from the queue, moving later elements up
  a spot in the queue to fill the gap.

  @param q a pointer to an instance of the priqueue_t data structure
  @param index position of element to be removed
  @return the element removed from the queue
  @return NULL if the specified index does not exist
 */
void *priqueue_remove_at(priqueue_t *q, int index)
{

      node* traverse = q->first;
      node* traverseNext=0;
      int i = 1;
      while(i < (index))
      {
        traverse=traverse->next;
        i++;
      }
      traverseNext = traverse;
      traverse=traverse->next;
      if(traverse->next == 0)
      {
        void* contentDeleted = traverse->content;
        free(traverse);
        q->size--;
        traverseNext->next=0;
        traverse=0;
        return contentDeleted;
      }
      else
      {
        traverseNext->next= traverse->next;
        void* contentDeleted = traverse->content;
        free(traverse);
        q->size--;
        traverse=0;
        return contentDeleted;
      }

}

/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  return q->size;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{

      node* current=q->first;
      while(current != 0)
      {
        node* nextNode = current->next;
        free(current);
        current = nextNode;
      }
      q->first=0;

}
void priqueue_insert_back(priqueue_t* q, void* ptr)
{
  node* newJob = malloc(sizeof(node));
  newJob->content = ptr;
  if( q->size == 0)
  {
    q->first = newJob;
    newJob->next=0;
  }
  else
  {
    node* traverse = q->first;
    while(traverse->next != 0)
    {
      traverse=traverse->next;
    }
    traverse->next = newJob;
    newJob->next = 0;
  }
  q->size++;
}

void priqueue_insert_front(priqueue_t* q, void* ptr)
{
  node* newJob = malloc(sizeof(node));
  newJob->content = ptr;
  // newJob -> next = q->first;
  // q->first = newJob;

  if (q->size == 0)
  {
    q->first =newJob;
    newJob -> next =0;
  }
  else
  {
    newJob->next = q->first;
    q->first = newJob;
  }
  q->size++;
}

/*
void priqueue_insert_after(priqueue_t *q, void* newJob, int index)
{
  node* traverse = q->first;
  for(int i=0;i<index;i++)
  {
    traverse = traverse->next;
  }
  node* holder = traverse->next;
  node inserter = malloc(sizeof(node));
  inserter->content=newJob;
  traverse->next = inserter;
  inserter->next = holder;
}


int priqueue_index_by_priority(priqueue_t *q, int priority)
{
  node*traverse = q->first;
  int count =0;  l
  }
  if(q->first->next==NULL)
  {
    return -1;
  }
  //case where the front of the queue is what were looking for;
  // if((q->first->priority == priority)&& (q->first->next->priority > priority))
  // {
  //   return count;
  // }
  while(traverse->next !=NULL)
  {
    if((traverse->content->priority == priority) && (q->first->next->priority >priority))
    {
      return count;
    }
    else
    {
      traverse=traverse->next;
      count++;
    }
  }
  return -1;*/
