#include "cgen.h"

int fgetchar()
{
    int c;
    while ((c = fgetc(stdin)) == '\r')
        ;
    return c;
}
int fputchar(int c)
{
    if (c == '\n')
        fputc('\r', stdout);
    return fputc(c, stdout);
}

/* end of file getchar.c */