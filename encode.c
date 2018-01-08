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

	struct stat fileStat; //size of the input file

	int fileIn = 0;

	int fileOut = 1;

	int bitsSet = 0; //how many bits are encoded

	int bitIndex = 0; //index for bits in code table

	int verbose = 0; //truth value for printing out statistics

	int printHuffman = 0; //truth value for printing the tree

	int depth; //depth of printed huffman tree

	int s; //used for command line and switch statement

	uint8_t buffer; //stores the bytes read in from the input file

	uint16_t leafCount = 0; //amount of leaves in huffman tree

	uint16_t treeSize; //size of huffman tree

	uint32_t magicNumber = 0xdeadd00d; //magic number to determine if file is encoded

	uint32_t histogram[256]; //used to hold frequency of bytes

	uint32_t bSize; //bit vector size

	queue *q = newQueue(257); //priority queue; sacrificed one slot to keep track of head

	treeNode *leftNode, *rightNode; //used for storing and joining dequeue'd nodes

	treeNode *joined; //joined node of left and right nodes from queue

	treeNode *tree; //the fully joined tree

	while((s = getopt(argc, argv, "-vi:o:p:")) != -1)
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

	/*Check for an output file, if it doesn't exist, make one*/
	if(outputFile)
	{
		if(open(outputFile, O_RDONLY, EOF) == -1)
		{
			fileOut = open(outputFile, O_CREAT|O_RDWR, 0644); 
		}
		else
		{
			fileOut = open(outputFile, O_RDWR, EOF);
		}
	}
	else
	{	
		fileOut = 1;
	}

	/*Make the histogram*/
	for(int i = 0; i < 256; i++)
	{
		histogram[i] = 0;
	}

	histogram[0] = histogram[255] = 1; //quick fix for files with no bytes

	while(read(fileIn, &buffer, 1) != 0)
	{
		histogram[buffer]++;
	}

	/*Order the nodes made from the histogram in a priority queue*/
	for(int i = 0; i < 256; i++)
	{
		if(histogram[i] > 0)
		{
			enqueue(q, newNode(i, 1, histogram[i]));
			leafCount++;
		}
	}

	treeSize = (3 * leafCount) - 1; //calculate how big the tree is

	/*Dequeue and join nodes over and over until you only have the root*/
	while(q->Q[1] != NULL)
	{
		dequeue(q, &leftNode);
		if(!emptyQueue(q))
		{
			dequeue(q, &rightNode);
			joined = join(leftNode, rightNode);
			enqueue(q, joined);
		}
	}

	dequeue(q, &tree);

	/*Build the code for each symbol and store them in a table*/
	code c = newCode();

	code table[256];

	for(int i = 0; i < 256; i++)
	{
		table[i] = newCode();
	}
	buildCode(tree, c, table);

	/*Encode the file into a bit vector*/
	fstat(fileIn, &fileStat); //read in the size of the file
	bSize = fileStat.st_size * 8; //bit vector size

	bitV *bv = newVec(bSize);

	close(fileIn); //close and reopen to re-read
	fileIn = open(inputFile, O_RDONLY, EOF);

	/*Appending code to a bit vector*/
	while(read(fileIn, &buffer, 1) != 0)
	{
		bitIndex = 0;
		for(int y = 0; y < table[buffer].l; y++)
		{
			if(y != 0 && y % 8 == 0)
			{
				bitIndex++;
			}
			if((table[buffer].bits[bitIndex] & (0x1 << y % 8)) >> (y % 8) == 1)
			{
				setBit(bv, bitsSet);
			}
			bitsSet++;	
		}
	}

	/*Prints the tree out*/
        if(printHuffman == 1)
        {
                printTree(tree, depth);
        }

	/*Statistics*/
	if(verbose)
	{
		printf("Original %lld bits: ", 8 * fileStat.st_size);
		printf("leaves %d (bytes %d) ", leafCount, (leafCount * 3) - 1);
		printf("encoding %u bits (%f%)\n", bitsSet, 
		(float)bitsSet/(fileStat.st_size * 8) * 100);
	}

	write(fileOut, &magicNumber, sizeof(magicNumber)); //write the magic number

	write(fileOut, &fileStat.st_size, sizeof(fileStat.st_size)); //write file size

	write(fileOut, &treeSize, sizeof(treeSize)); //write the huffman tree size

	dumpTree(tree, fileOut); //write the huffman tree to the file

	write(fileOut, bv->v, bv->l/8); //write the bit vector to the file

	delVec(bv);

	delTree(tree);

	delQueue(q);

	free(inputFile);

	free(outputFile);

	close(fileIn);

	close(fileOut);

	return 0;
}
