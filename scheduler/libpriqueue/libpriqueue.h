/** @file libpriqueue.h
 */

#ifndef LIBPRIQUEUE_H_
#define LIBPRIQUEUE_H_

//will be used to insert into the queue based on priority
typedef int(*comparer)(const void *, const void *);

/**
  Priqueue Data Structure
*/

typedef struct node
{
      struct node *next;
      void* content;
} node;

typedef struct _priqueue_t
{
  int size;
  struct node* first;
  comparer cmp;
} priqueue_t;


void   priqueue_init     (priqueue_t *q, comparer cmp);

int    priqueue_offer    (priqueue_t *q, void *ptr);
void * priqueue_peek     (priqueue_t *q);
void * priqueue_poll     (priqueue_t *q);
void * priqueue_at       (priqueue_t *q, int index);
int    priqueue_remove   (priqueue_t *q, void *ptr);
void * priqueue_remove_at(priqueue_t *q, int index);
int    priqueue_size     (priqueue_t *q);

void   priqueue_destroy  (priqueue_t *q);

#endif /* LIBPQUEUE_H_ */
