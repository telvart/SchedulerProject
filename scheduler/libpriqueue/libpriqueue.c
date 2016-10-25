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

//       int* myPtr=malloc(sizeof(int));
//       myPtr=ptr;
//       /* if (q->first == NULL) */
//       /* { */
//       /*   struct node temp = {NULL, ptr}; */
//       /*   q->first=&temp; */
//       /* } */
//       /* else */
//       /* { */
//         struct node* traverse = q->first;
//       /*   while (traverse->next != NULL) */
//       /*   { */
//       /*     traverse = traverse->next; */
//       /*   } */
//       /*   struct node temp = {NULL, ptr}; */
//       /*   traverse->next=&temp; */
//
//       /* } */
//       //      return 1;
//
//       if(q->first==NULL && q->last == NULL)//where q is empty
//       {
//             struct node* temp=malloc(sizeof(temp));
//             temp->next=NULL;
//             temp->content=ptr;
//             q->first=temp;
//             q->last=temp;
//             q->size++;
//             return 0;
//             /*
//             struct node temp;// = {NULL,ptr};
//             temp.next=NULL;
//             temp.content=ptr;
//             q->first = &temp;
//             q->last = &temp;
//             q->size++;
//             return 0;*/
//       }
//       else if(q->first == q->last) // case size = 1
//       {
//         struct node* temp2 = malloc(sizeof(temp2));
//         temp2->next = NULL;
//         temp2->content =ptr;
//
//         /*
//           if(priority ptr > priority q->first)
//           {
//             insert before first
//           }
//           else
//           {
//           insert after first
//           }
//         */
//          if(q->cmp(q->first->content, myPtr) > 0)
//          {
//            temp2->next=q->first;
//            q->first=temp2;
//          }
//          else
//          {
//           q->first->next = temp2;
//           q->last = temp2;
//          }
//         q->size++;
//         return 1;
//       }
//       else
//       {/*
//         struct node* temp3 = malloc(sizeof(temp3));
//         temp3->next=NULL;
//         temp3->content=ptr;
//         struct node* traverse= q->first;
//         while (q->cmp(traverse->content,temp3->content) < 0 && traverse->next != NULL)
//         {
//           traverse=traverse->next;
//         }
//         temp3->next = traverse->next;
//         traverse->next=temp3;
//         q->size++;
//         return q->size-1;
// */
//         struct node* temp3 =malloc(sizeof(temp3));
//         temp3->next = NULL;
//         temp3->content =myPtr;
//
//         if(q->cmp(q->first->content,temp3->content)<0)
//         {
//           struct node* temp4;
//           temp4=q->first->next;//save for later
//           q->first=temp3;
//           temp3->next=temp4;
//           q->size++;
//           // return priqueue_size(q)-1;
//         }
//         else
//         {
//           while (q->cmp(traverse->content,temp3->content) < 0 && traverse->next != NULL)
//           {
//             traverse=traverse->next;
//           }
//           temp3->next = traverse->next;
//           traverse->next=temp3;
//   //        q->last->next=temp3;
//     //      q->last = temp3;
//           q->size++;
//           return priqueue_size(q)-1;
//         }
//       }

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
          //free(q->first);TODO FIX MEMLEEK!
            q->first=NULL;
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
      int number = 0;

      while(current != 0)
      {
        if (q->cmp(current->content, ptr) == 0)
        {
          node* temp = current->next;
          parent->next = temp;
          number++;
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
      return number;

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
  /*
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
      }*/
}
