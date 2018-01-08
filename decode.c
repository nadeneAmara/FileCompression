# include <stdio.h>
# include <stdlib.h>
# include <stdint.h>
# include <errno.h>
# include <unistd.h>
# include <string.h>
# include <fcntl.h>
# include <ctype.h>
# include <sys/stat.h>
# include <sys/types.h>

# include "bv.h"
# include "stack.h"
# include "queue.h"
# include "huffman.h"
# include "code.h"

int main(int argc, char **argv)
{
	char *inputFile = NULL;

	char *outputFile = NULL;

	int fileIn = 0;

	int fileOut = 1;

	int verbose = 0; //truth value for printing out statistics

	int printHuffman = 0; //truth value for printing the tree

	int depth; //depth of printed huffman tree

	int s; //used for command line and switch statement

	uint16_t treeSize = 0; //huffman tree's size

	uint8_t byte; //stores read in bytes

	uint8_t savedTree[treeSize]; //array of the huffman tree's bytes

	uint32_t magicNumber; //magic number for encoded file

	uint64_t fileSize; //original input file size

	treeNode *root; //root of the constructed huffman tree

	treeNode *temp; //copy of the root for stepTree() traversal

	while ((s = getopt(argc, argv, "-vi:o:p:")) != -1)
	{
		switch(s)
		{
			case 'i':
				{
					inputFile = strdup(optarg);
					break;
				}
			case 'o':
				{
					outputFile = strdup(optarg);
					break;
				}
			case 'p':
				{
					printHuffman = 1;
					depth = atoi(optarg);
					break;
				}
			case 'v':
				{
					verbose = 1;
					break;
				}
			default:
				{
					break;
				}
		}
	}

	/*Check for an input file*/
	if(inputFile)
	{
		char z[1000] = {0};
		if(open(inputFile, O_RDONLY, EOF) == -1)
		{
			perror(z);
			exit(1);
		}
		fileIn = open(inputFile, O_RDONLY, EOF);
	}
	else
	{
		char z[1000] = {0};
		printf("No Input File\n");
		perror(z);
		exit(1);
	}

	/*Check for an output file*/
	if(outputFile)
	{
		if(open(outputFile, O_RDONLY, EOF) == -1)
		{
			fileOut = open(outputFile, O_CREAT|O_RDWR, 0644);
		}
		fileOut = open(outputFile, O_RDWR, EOF);
	}
	else
	{
		fileOut = 1;
	}

	read(fileIn, &magicNumber, 4); //read in the magic number for an encoded file

	if(magicNumber != 0xdeadd00d) //if there is no magic number, it's not a compressed file
	{
		char z[1000] = {0};
		perror(z);
		exit(1);
	}

	read(fileIn, &fileSize, 8); //read in the original file size

	read(fileIn, &treeSize, 2); //read in the huffman tree's size 

	for(int i = 0; i < treeSize; i++)
	{
		read(fileIn, &savedTree[i], 1); //read in the huffman tree's bytes
	}

	root = loadTree(savedTree, treeSize); //reconstruct the tree
	temp = root; //set a temporary tree node equal to the root for stepTree()

	//read in a byte at a time
	int len = fileSize;
	while(read(fileIn, &byte, 1) != 0)
	{ 
		for(int bit = 0; bit < 8; bit++) 
		{
			int valBit = (byte & (0x1 << (bit % 8))) >> (bit % 8);
			if(len != 0)
			{
				int32_t bits = stepTree(&temp, valBit);
				if(bits != -1) //reset it and write the symbol
				{
					temp = root;
					write(fileOut, &bits, sizeof(char));
					len--;
				}
			}
		}

	}

	if(printHuffman == 1) //prints out the tree
	{
		printTree(root, depth);
	}

	/*Statistics*/
	if(verbose)
	{
		printf("Original %lu bits: ", fileSize * 8);
		printf("tree (%d)\n", treeSize);
	}

	delTree(root);
	free(inputFile);
	free(outputFile);
	close(fileIn);
	close(fileOut);
	return 0;
}
