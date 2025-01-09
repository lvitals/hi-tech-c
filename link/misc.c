#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef __GNUC__
#include <sys.h>
#endif

#include "link.h"
#ifndef __GNUC__
static void clrbuf(register char *p, size_t size);
#endif
/**************************************************************************
40	xalloc	sub_1265h	ok++
**************************************************************************/
void *xalloc(size_t size)
{
    register char *p;

    if ((p = malloc(size)) == 0)
        fatalErr("Cannot get memory");
    clrbuf(p, size);
    return p;
}

/**************************************************************************
 41	clrbuf	ok++ (PMO) (use macro for newer code)
 as latest definition of memcpy uses restrict and gcc complains of overlap
 also memset is usually inlined
 **************************************************************************/
#ifndef __GNUC__
static void clrbuf(register char *p, size_t size)
{

    if (size == 0)
        return;
    *p = 0;
    if (--size == 0)
        return;
    bmove(p, p + 1, size); /* use bmove to copy 0 to remaining locations*/
}
#endif
