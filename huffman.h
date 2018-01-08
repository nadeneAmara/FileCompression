# ifndef _HUFFMAN_H
# define _HUFFMAN_H

# include <stdio.h>
# include <stdint.h>
# include <stdbool.h>
# include <stdlib.h>

# include "code.h"

# ifndef NIL
# define NIL (void *) 0
# endif

typedef struct DAH treeNode;

struct DAH
{
	uint8_t symbol;
	uint64_t count;
	bool leaf;
	treeNode *left, *right;
};

// New node, with symbols, leaf or not, a count associated with it
treeNode *newNode(uint8_t, bool, uint64_t);

// Dump a Huffman tree onto a file
void dumpTree(treeNode *, int);

// Build a tree from the saved tree
treeNode *loadTree(uint8_t [], uint16_t);

// Step through a tree following the code
//int32_t stepTree(treeNode *, treeNode **, uint32_t);
int32_t stepTree(treeNode **, uint32_t);

// Parse a Huffman tree to build codes
void buildCode(treeNode *, code, code table[256]);

// Delete a tree
void delTree(treeNode *);

// Prints a tree
void printTree(treeNode *, int);

// Join two subtrees
treeNode *join(treeNode *, treeNode *); 

static inline void delNode(treeNode *h) 
{ 
	free(h); 
	return; 
}

static inline int8_t compare(treeNode *l, treeNode *r)
{
	return l->count - r->count; // l < r if negative, l = r if 0, ...
}

static inline void spaces(int c) //used for printing huffman trees
{
	for(int i = 0; i < c; i += 1)
	{ 
		putchar(' ');
	} 
	return;
}

# endif
