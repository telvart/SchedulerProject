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
void priqueue_init(priqueue_t *q, comparer new_cmp)
{
  q->size=0;
  q->first = NULL;
  q->last = NULL;
  q->cmp=new_cmp;
  //jamie saidwork on this fuction, its missing some stuff, specifically compare function
}


/**
  Inserts the specified element into this priority queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @param ptr a pointer to the data to be inserted into the priority queue
  @return The zero-based index where ptr is stored in the priority queue, where 0 indicates that ptr was stored at the front of the priority queue.
 */
int priqueue_offer(priqueue_t *q, void *ptr)
{


      /* if (q->first == NULL) */
      /* { */
      /*   struct node temp = {NULL, ptr}; */
      /*   q->first=&temp; */
      /* } */
      /* else */
      /* { */
      /*   struct node* traverse = q->first; */
      /*   while (traverse->next != NULL) */
      /*   { */
      /*     traverse = traverse->next; */
      /*   } */
      /*   struct node temp = {NULL, ptr}; */
      /*   traverse->next=&temp; */

      /* } */
      //      return 1;

      if(q->first==NULL && q->last == NULL)//where q is empty
      {
            struct node* temp=malloc(sizeof(temp));
            temp->next=NULL;
            temp->content=ptr;
            q->first=temp;
            q->last=temp;
            q->size++;
            return 0;
            /*
            struct node temp;// = {NULL,ptr};
            temp.next=NULL;
            temp.content=ptr;
            q->first = &temp;
            q->last = &temp;
            q->size++;
            return 0;*/
      }
      else if(q->first == q->last) // case size = 1
      {
        struct node* temp2 = malloc(sizeof(temp2));
        temp2->next = NULL;
        temp2->content =ptr;
        q->first->next = temp2;
        q->last = temp2;
        q->size++;
        return 1;
      }
      else
      {/*
        struct node* temp3 = malloc(sizeof(temp3));
        temp3->next=NULL;
        temp3->content=ptr;
        struct node* traverse= q->first;
        while (q->cmp(traverse->content,temp3->content) < 0 && traverse->next != NULL)
        {
          traverse=traverse->next;
        }
        temp3->next = traverse->next;
        traverse->next=temp3;
        q->size++;
        return q->size-1;
*/

        struct node* temp3 =malloc(sizeof(temp3));
        temp3->next = NULL;
        temp3->content =ptr;
        q->last->next=temp3;
        q->last = temp3;
        q->size++;
        return priqueue_size(q)-1;
      }

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
	if (q->first==NULL && q->last==NULL)
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
      else if(q->first == q->last)//Case where queue is size==1
      {
            struct node *temp = q->first;
            void* pop = q->first->content;
          //free(q->first);TODO FIX MEMLEEK!
            q->first=NULL;
            q->last=NULL;
            q->size--;
            return pop;
            //q->first = NULL;//Memory leak? need to free rather than reallocate?
            return temp->content;
      }
      else
      {
        struct node* temp4 = q->first->next;
        void* pop = q->first->content;
      //free(q->first);TODO FIX MIMLEEK!
        q->first = temp4;
        q->size--;
        return pop;
      }
      // else
      // {
      //       struct node *temp = q->first;//save the first node that we'll return later
      //       q->first = q->first->next;//makes the front of the queue point at the second node of the queue
      //       return temp->content;
      // }

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
            for(int i=0;i<index; i++)//maybe a one off error here?
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
/*
      if (q->first == NULL)
      {
        return 0;
      }
      if (q->size == 1 && q->first->content==ptr)
      {
          free(q->first);
          q->first=NULL;
          q->last=NULL;
          q->size--;
          return 1;
      }

      else
      {
        int numOccurances=0;
        int numDeleted=0;
        for(int i=0; i <q->size; i++) // get the number of occurances in the queue
        {
          if (priqueue_at(q, i) == ptr)
          {
            numOccurances++;
          }
        }
        for(int i=0; i < numOccurances; i++ ) // for every occurance
        {
          for(int j=0; (j<q->size && priqueue_at(q,j) != NULL); j++) // go through list and delete it
          {
            if(priqueue_at(q, j) == ptr)
            {
              priqueue_remove_at(q,i);
              numDeleted++;
            }
          }
        }
        return numDeleted;

      }*/

      if(q->first == NULL)//Handles the empty case
      {
            return 0;
      }
      struct node *temp = q->first;
      struct node *temp2;
      int numDeleted  = 0;
      for(int i=0; i<priqueue_size(q);i++)//maybe an off one error here?
      {
            if(temp->content==NULL)
            {
                  return numDeleted;
            }
            if(temp->content==ptr)
            {
                  temp2=temp->next;
                  free(temp->next);
                  temp=temp2;
                  q->size--;
                  numDeleted++;
            }
      }

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
      if(index>priqueue_size(q))//handles the case where index is beyond scope
      {
            return NULL;
      }
      struct node *temp = q->first;
      for(int i=1; i<index; i++)//begins at one because temp is set to first
      {
            temp=temp->next;
      }//temp is now looking at the node before the node we want to delete
      if(temp->next==NULL)//case where there is nothing to remove
      {
            //free(temp);
            return NULL;
      }
      if(temp->next->next==NULL)//case where we're removing the last node
      {
            struct node *temp2=temp->next;//an actual node to hold return valu
//            struct node *temp3=*(temp2);//a pointer to look at that node
            free(temp->next);//free/delete the space
            return temp2;//return the pointer
      }//now we are at the general case where there is something after
      //the node we're deleting
      struct node *temp2 = temp->next->next;//store the tail in temp2
      struct node *temp3=temp->next;
//      struct node *temp4=*(temp3);
      free(temp->next);//cut of the head
      temp->next = temp2;//replace the head with the tail
      return temp3;
}


/**
  Returns the number of elements in the queue.

  @param q a pointer to an instance of the priqueue_t data structure
  @return the number of elements in the queue
 */
int priqueue_size(priqueue_t *q)
{
  return q->size;
      // int size=0;
      // struct node *temp = q->first;
      // if(q->first==NULL)
      // {
      //       free(temp);
      //       return size;
      // }
      // else
      // {
      //       while(temp->next!=NULL)
      //       {
      //             size++;
      //             temp=temp->next;
      //       }
      //       return size;
      // }
	//return 0;
}


/**
  Destroys and frees all the memory associated with q.

  @param q a pointer to an instance of the priqueue_t data structure
 */
void priqueue_destroy(priqueue_t *q)
{
      if(q->first==NULL)//handles case where its already empty
      {
            return;
      }
      if(q->first->next==NULL)//handles the case where size==1
      {
            free(q->first);
            return;
      }
      struct node *secondEl;//holds the second element while we delete the first
//      struct node *temp2=temp->next;
      for(int i=1;i<priqueue_size(q);i++)
      {
            if(q->first->next==NULL)
            {
                  free(q->first);
                  return;
            }
            secondEl=q->first->next;
            free(q->first);
            q->first=secondEl;
      }
}
