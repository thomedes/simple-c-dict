/*****************************************************************************/
/**

@file

@brief Djb2 hash function

@date Aug 2013

@par License
I'dont really know what to put here. This code is copied (and very slightly
modifed) from [Hash Functions](http://www.cse.yorku.ca/~oz/hash.html) where I
couldn't find any mention to any license. My guess is that it's in the public
domain, but I'm not a lawyer so please do as you see fit.

*/
/*****************************************************************************/
#include "djb2.h"

#include <assert.h>
#include <stdio.h>

unsigned long
djb2(const char *str)
{
    unsigned long hash = 5381;
    int c;

    assert(str != NULL);

    while ((c = *str++))
	hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

    return hash;
}
