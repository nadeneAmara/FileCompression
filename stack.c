# include <stdlib.h>
# include <stdbool.h>

# include "stack.h"

# define MAX 10 

// Creates a new stack and returns it.
stack *newStack()
{
	stack *s = (stack *) calloc(MAX, sizeof(stack));
	s->size = MAX;
	s->top = 0;
	s->entries = (stackItem *) calloc(MAX, sizeof(stackItem));
	return s;
}

/**
 * Takes a stack and deletes the entries, and then the stack itself.
 * @param s -stack to be deleted
 */
void delStack(stack *s)
{
	free(s->entries);
	free(s);
	return;
}

/**
 * Takes an item on the stack and pops it off. If there's nothing on
 * the stack, returns an error.
 * @param s -the item pops off of this stack
 * @return -returns the item at the top of the stack
 */
stackItem pop(stack *s)
{
	if(s->top > 0)
	{
		s->top--;
		return s->entries[s->top];
	}
}

/**
 * Takes a stack and pushes the passed item onto the top of the stack.
 * Grows the stack if the stack is too small.
 * @param s -the item will be pushed onto this stack
 * @param i -the item to be pushed onto the stack
 */
void push(stack *s, stackItem i)
{
	if(s->top == s->size)
	{
		s->size *= 2;
		s->entries = (stackItem *) realloc(s->entries, s->size*sizeof(stackItem));
	}
	s->entries[s->top] = i;
	s->top++;
	return;
}

/**
 * Checks if the stack passed is empty based on the position of the top of the
 * stack.
 * @param s -stacked checked for emptiness
 */
bool empty(stack *s)
{
	return s->top == 0;
}

/**
 * Checks the passed stack for fullness by checking if the stack's top is not
 * at 0.
 * @param s -stack checked for fullness
 */
bool full(stack *s)
{
	return s->top != 0;
}
