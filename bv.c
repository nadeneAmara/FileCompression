/** 
 * This .c file implements all the functions declared in the header file
 * bv.h. Three functions, specifically setBit, clrBit, and valBit contain
 * code from: Darrell Long.
 */

# include "bv.h"
# include <stdlib.h>

/*Returns a new vector of a specified length, and allocates memory for it*/
bitV *newVec(uint32_t length)
{
	bitV* vector = (bitV *) malloc(sizeof(bitV));
	vector->v = (uint8_t *) calloc(length/8+1, sizeof(uint8_t));
	vector->l = length;
	return vector;
}

/*Deletes a vector by freeing the contents of the vector and then the wrapper*/
void delVec(bitV *v)
{
        free(v->v); 
        free(v); 
	return;
}

/*Sets all the bits in a bit vector to 1, or true*/
void oneVec(bitV *v)
{
	for(uint32_t i = 0; i < lenVec(v); i++)
	{
		setBit(v,i); //sets all the bits to 1
	}
	return;
}

/*Sets a bit at a specified location*/
void setBit(bitV *v, uint32_t l) //Code from DDEL 4/15/17
{
	v->v[l >> 3] |= (0x1 << (l % 8));
	return;
}

/*Clears a bit at a specified location*/
void clrBit(bitV *v, uint32_t l) //Code from DDEL 4/15/17
{
	v->v[l >> 3] &= ~(0x1 << (l % 8)); 
	return;
}

/*Returns the value of a bit at a given location*/
uint8_t valBit(bitV *v, uint32_t l) //Code from DDEL 4/15/17
{
	return (v->v[l >> 3] & (0x1 << l % 8)) >> (l % 8);
}

/*Returns the length of the passed bit vector*/
uint32_t lenVec(bitV *v)
{
	return v->l;
}
