/* Copyright (c) 2003, 2004, 2005 Jakub Wilk <ubanus@users.sf.net>
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License version 2 as published
 * by the Free Software Foundation.
 */

#include <assert.h>
#include <string.h>

#include "common.h"
#include "memory.h"
#include "nonogram.h"
#include "queue.h"

static inline void update_queue_enq(Queue *queue, unsigned int i)
{
  assert(i < queue->size);
  queue->enqueued[queue->elements[i].id] = i;
}

static void heapify_queue(Queue *queue)
{
  unsigned int i, l, r, max;
  QueueItem ivalue;
  
  i = 0;
  while (true)
  {
    ivalue = queue->elements[i];
    l = 2 * i + 1;
    r = l + 1;
    max = (l < queue->size && queue->elements[l].factor > ivalue.factor) ? l : i;
    if (r < queue->size && queue->elements[r].factor > queue->elements[max].factor)
      max = r;
    if (max != i)
    {
      queue->elements[i] = queue->elements[max];
      queue->elements[max] = ivalue;
      update_queue_enq(queue, i);
      update_queue_enq(queue, max);
      i = max;
    }
    else 
      return;
  }
}

Queue *alloc_queue(void)
// Synopsis:
// | allocates a queue
{
  Queue *tmp =
    alloc(
      offsetof(Queue, space) +
      xpysize * (sizeof(unsigned int*) + sizeof(QueueItem)) );
  tmp->size = 0;
  tmp->enqueued = (unsigned int*)tmp->space;
  memset(tmp->enqueued, -1, sizeof(unsigned int*) * xpysize);
  tmp->elements = (QueueItem*)(tmp->space + xpysize * sizeof(unsigned int));
  return tmp;
}

void free_queue(Queue *queue)
// Synopsis:
// | frees `queue'
{
  free(queue);
}

bool is_queue_empty(Queue *queue)
// Synopsis:
// | checks if a queue (which `queue' points to) is empty
{
  return queue->size == 0;
}

bool put_into_queue(Queue *queue, unsigned int id, int factor)
// Synopsis:
// | pushes a number (`i') to a queue (which `queue' points to)
// | if the number has been already queued, the queue might be renumbered
{
  unsigned int i, j;

  factor = -factor;

  assert(id < xpysize);
  i = queue->enqueued[id];
  if (i == (unsigned int)-1)
    i = queue->size++;
  else
  {
    assert(i < queue->size);
    if (factor <= queue->elements[i].factor)
      return false;
  }

  while (i > 0 && queue->elements[j = (i-1)/2].factor < factor)
  {
    queue->elements[i] = queue->elements[j];
    update_queue_enq(queue, i);
    i = j;
  }

  queue->elements[i].id = id;
  queue->elements[i].factor = factor;
  update_queue_enq(queue, i);

  return true;
}

unsigned int get_from_queue(Queue *queue)
// Synopsis:
// | pops a number from a queue (which `queue' points to)
// | if the queue is empty, the result might be strange
{
  unsigned int resultid, last;
  assert(queue->size > 0);
  resultid = queue->elements[0].id;
  last = --queue->size;
  if (queue->size > 0)
  {
    queue->elements[0] = queue->elements[last];
    update_queue_enq(queue, 0);
    heapify_queue(queue);
  }
  queue->enqueued[resultid] = (unsigned int)-1;
  return resultid;
}

/* vim:set ts=2 sw=2 et: */