# ifndef _QUEUE_H
# define _QUEUE_H
# include <stdint.h>
# include <stdbool.h>

# include "huffman.h"

// treeNode defined in huffman.h
typedef treeNode *item; 

typedef struct queue
{
	uint32_t size; // How big is it?
	uint32_t head, tail; // Front and rear locations
	item *Q; // Array to hold it (via calloc)
} queue;

// Constructor
queue *newQueue(uint32_t size);
// Deconstructor 
void delQueue(queue *q); 

// Is it empty?
bool emptyQueue(queue *q);
// Is it full?
bool fullQueue(queue *q); 

// Add an item
bool enqueue(queue *q, item i); 
// Remove from the rear
bool dequeue(queue *q, item *i); 

# endif
