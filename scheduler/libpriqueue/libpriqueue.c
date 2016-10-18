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
  @param comparer a function pointer that compares two elements.
  See also @ref comparer-page
 */
void priqueue_init(priqueue_t *q, int(*comparer)(const void *, const void *))
{
  q->first=NULL;
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{
      if(q->first==NULL)
      {
            q->first->content = ptr;
            return -1;
      }
      struct node *temp;
      temp = q->first;
      while(temp->next!=NULL)
      {
            temp=temp->next;
      }
      temp->content = ptr;
      return priqueue_size(q)-1;
//	return -1;
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
	return q->first;
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
      else if(priqueue_size(q)==1)//Case where queue is size==1
      {
            struct node *temp = q->first;
            q->first = NULL;//Memory leak? need to free rather than reallocate?
            return temp->content;
      }
      else
      {
            struct node *temp = q->first;//save the first node that we'll return later
            q->first = q->first->next;//makes the front of the queue point at the second node of the queue
            return temp->content;
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
      if(priqueue_size(q)<index)//case where the index goes beyond the bounds
      {
            return NULL;
      }
	else
      {
            struct node *temp = q->first;
            for(int i=0;i<index; i++)//maybe a one off error here?
            {
                  temp=temp->next;
            }
            return temp;
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
      if(q->first == NULL)//Handles the empty case
      {
            return 0;
      }
      struct node *temp = q->first;
      int numDeleted  = 0;
      for(int i=0; i<priqueue_size(q);i++)//maybe an off one error here?
      {
            //TODO FIX THIS
      //       if(temp->next->content==NULL)//takes care of the edge case
      //       {
      //             if(temp->content==ptr)
      //             {
      //                   temp->content==NULL;
      //
      //             }
      //       }
      //       if(temp->next->content==ptr)
      //       {
      //             temp
      //       }
      }
	 return 0;
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
      //TODO: THIS
	return 0;
}


/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
      int size=0;
      struct node *temp = q->first;
      if(q->first==NULL)
      {
            free(temp);
            return size;
      }
      else
      {
            while(temp->next!=NULL)
            {
                  size++;
                  temp=temp->next;
            }
            return size;
      }
	//return 0;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{

}
