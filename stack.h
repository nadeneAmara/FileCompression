# ifndef _STACK_H
# define _STACK_H
# include <stdint.h>
# include <stdbool.h>

# include "huffman.h"

typedef treeNode *stackItem;

typedef struct stack
{
	uint32_t size; // How big?
	uint32_t top; // Where’s the top?
	stackItem *entries; // Array to hold it (via calloc)
} stack;

// Constructor
stack *newStack(); 
// Destructor
void delStack(stack *);

// Returns the top item
stackItem pop(stack *); 
// Adds an item to the top
void push(stack *, stackItem);

// Is it empty?
bool empty(stack *); 
// Is it full?
bool full(stack *); 

# endif
