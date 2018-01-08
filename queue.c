# include <stdlib.h>

# include "queue.h"

/**
 * Creates a new queue of the passed size.
 * @param size -the size of the new queue
 * @return q -the newly created queue
 */
queue *newQueue(uint32_t size)
{
	queue *q = (queue *) malloc(sizeof(queue));
	q->size = size;
	q->head = q->tail = 0;
	q->Q = (item *) calloc(size, sizeof(item));
	return q;
}

/**
 * Deletes the passed queue by freeing all the elements,
 * and then the queue itself.
 * @param q -the queue to be freed
 */
void delQueue(queue *q) 
{
	free(q->Q);
	free(q); 
	return; 
}

/**
 * Checks if the passed queue is empty, and returns a boolean accordingly.
 * @param q -check if this queue is empty
 * @return -true if the head and tail are in the same place, otherwise, returns false
 */
bool emptyQueue(queue *q) 
{ 
	return q->head == q->tail; 
}

/**
 * Checks if the passed queue is full by checking the position
 * of the head and tail of the queue.
 * @param q -queue passed for fullness check
 * @return -returns true if head and tail are in the same spot, otherwise, returns false
 */
bool fullQueue(queue *q)
{ 
	return (q->head + 1) % q->size == q->tail % q->size; 
}

/**
 * Takes in a queue and a tree node, and inserts the tree node into
 * the queue based on its frequency count. 
 * @param q -the node will be inserted into this queue
 * @param i -the tree node to be inserted
 * @return -returns true if the node is successfully inserted; return false is it fails
 */
bool enqueue(queue *q, item i)
{
	int index = 1; //no need to sort only one node, so start at 1
	if(fullQueue(q))
	{
		return false;
	}
	else if(q && q->Q) //check if the queue is valid
	{
		/*Just slot in the node if the queue is empty*/
		if(q->Q[q->head] == NULL && q->Q[q->tail] == NULL)
		{
			q->Q[q->tail] = i;
			q->head = (q->head + 1) % q->size;
		}
		else
		{
			/*Insert the node based on frequency*/
			while(i->count < q->Q[q->head - index]->count && index <= q->head)
			{
				q->Q[q->head - index + 1] = q->Q[q->head - index];
				q->Q[q->head - index] = i;
				index++;
				if(index > q->head) //prevents running off the end of array
				{
					break;
				}
			}
			if(index == 1) //the node has the highest count; slot it in place
			{
				q->Q[q->head] = i;
			}
			q->head = (q->head + 1) % q->size;
		}
		return true;
	}
	else
	{
		return false; //if the queue passed is somehow not valid
	}
}

/**
 * Takes in a queue, takes a node off the queue, and then assigns the taken off node
 * to the passed tree node item. The nodes of the queue are then pushed downwards
 * to close the gap.
 * @param q -the node is taken off this queue
 * @param i -the taken off node is assigned here
 * @return -returns false if there's nothing to dequeue, otherwise return true
 */ 
bool dequeue(queue *q, item *i)
{
	int index = 1; //start at 1 in order to move nodes down
	if (emptyQueue(q))
	{
		return false;
	}
	else
	{
		*i = q->Q[q->tail]; 
		q->Q[q->tail] = NULL;
 
		/*Move down the nodes until there's nothing left to move down*/
		while(q->Q[index] != NULL) 
		{
			q->Q[index - 1] = q->Q[index];
			index++;
		}

		q->Q[index - 1] = NULL; //there's an extra node leftover at the end; remove it 
		q->head--; //move the head down by one since a node is gone
		return true;
	}
}
