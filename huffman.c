# include <stdlib.h>
# include <stdbool.h>
# include <ctype.h>
# include <unistd.h>

# include "stack.h"
# include "huffman.h"

/**
 * Creates a new tree node representing a symbol, its frequency, and whether or
 * not it is a leaf or a branch of the huffman tree.
 * @param s -the symbol used to represent the node
 * @param l -defines if the node is a leaf or an internal node
 * @param c -frequency of the symbol
 * @return t -the new node created
 */
treeNode *newNode(uint8_t s, bool l, uint64_t c)
{
	treeNode *t = (treeNode *) calloc(10, sizeof(treeNode));
	t->symbol = s;
	t->leaf = l;
	t->count = c;
	t->left = t->right = (treeNode *) 0;
	return t;
}

/**
 * Takes the entire tree and writes it to the passed file using post-traversal.
 * If the node is a leaf, an 'L' is written, followed by the symbol.
 * If the node is internal, only a 'I' is written.
 * @param t -tree to be dumped onto file
 * @param file -file used to store the dumped tree
 */
void dumpTree(treeNode *t, int file)
{
	if(t->leaf == 1)
	{
		write(file, "L", sizeof(uint8_t));
		write(file, &t->symbol, sizeof(t->symbol));
	}
	else
	{
		dumpTree(t->left, file);
		dumpTree(t->right, file);
	}
	if(t->leaf == 0)
	{
		write(file, "I", sizeof(uint8_t));
	}
	return;
}

/**
 * Takes in the saved bytes from an array, and reconstructs the tree of
 * the passed treeBytes size.
 * @param savedTree -array holding all the bytes of the huffman tree
 * @param treeBytes -size of the huffman tree
 * @return root -returns the root of the tree, or the whole tree
 */
treeNode *loadTree(uint8_t savedTree[], uint16_t treeBytes)
{
	stack *s = newStack();
	for(uint16_t i = 0; i < treeBytes; i++)
	{
		if(savedTree[i] == 'L')
		{
			i++;
			push(s, newNode(savedTree[i], 1, 0));
		}
		else if(savedTree[i] == 'I')
		{
			treeNode *r = pop(s);
			treeNode *l = pop(s);
			treeNode *joined = join(l, r);
			push(s, joined);
		}
	}
	treeNode *root = pop(s);
	delStack(s);
	return root;
}

int32_t stepTree(treeNode **t, uint32_t code)
{
	if(code == 0)
	{
		*t = (*t)->left;
	}
	else
	{
		*t = (*t)->right;
	}

	if((*t)->leaf == 1)
	{
		return (*t)->symbol;
	}
	else
	{
		return -1;
	}
}

/**
 * This takes in a huffman tree, a code, and a table of codes and constructs codes
 * for all the leaves. The code is moved through the tree; going left pushes a 0, while
 * going right pushes a 1. The code is saved in the table every time it hits a leaf.
 * @param t -tree to be encoded
 * @param s -code to be altered throughout encoding
 * @param table -stores all the codes for all the leaves
 */
void buildCode(treeNode *t, code s, code table[256])
{
	uint32_t popped; //stores the popped values

	if(t->leaf == 1)
	{
		table[t->symbol] = s;
	}
	else 
	{
		pushCode(&s, 0);
		buildCode(t->left, s, table);
		popCode(&s, &popped);
		pushCode(&s, 1);
		buildCode(t->right, s, table);
		popCode(&s, &popped);
	}
	return;
}

/**
 * Takes in a huffman tree, and deletes the tree recursively, node by node.
 * @param t -tree to be deleted
 */
void delTree(treeNode *t)
{
	if(t == NULL)
	{
		return;
	}
	delTree(t->left);
	delTree(t->right);
	delNode(t);
	return;
}

/**
 * Takes two leaf nodes and joins them together by creating an internal node that branches
 * out to both nodes. The internal node is represented by the '$' symbol, and has the
 * sum of the counts of the joined nodes.
 * @param l = left leaf of the new internal node
 * @param r = right leaf of the new internal node
 * @return t = return joined tree node
 */
treeNode *join(treeNode *l, treeNode *r)
{
	treeNode *t = newNode('$', 0, (l->count + r->count));
	t->left = l; 
	t->right = r;
	return t;
}

/**
 * Code from DDEL
 * Takes a huffman tree and prints out the tree accordingly. The depth passed makes the tree
 * displayed that many spaces to the right of the output screen.
 * @param t = tree to be printed
 * @param depth = spaces right to print the tree
 */
void printTree(treeNode *t, int depth)
{
	if (t) 
	{
		printTree(t->left, depth + 1);

		if (t->leaf)
		{
			if (isalnum(t->symbol))
			{
				spaces(4 * depth); printf("'%c' (%llu)\n", t->symbol, t->count);
			}
			else
			{
				spaces(4 * depth); printf("0x%X (%llu)\n", t->symbol, t->count);
			}
		}
		else
		{
			spaces(4 * depth); printf("$ (%llu)\n", t->count);
		}

		printTree(t->right, depth + 1); 
	}

	return;
}
