#include "cgen.h"

/*********************************************************
 * main OK++ PMO
 *
 *********************************************************/
int main(int argc, char **argv)
{

#ifdef CPM
    baseHeap = sbrk(0); /* Current highest memory */
#endif
    --argc, ++argv;
    while (argc > 0 && **argv == '-')
    { /* Parsing options */
        switch (argv[0][1])
        {
        case 'W':
        case 'w':
            wflag = true;
            break; /* Displaying warnings */
        case 'R':
            rflag = true;
            break;
        default:
            fatalErr("Illegal\tswitch %c", argv[0][1]);
            break;
        }
        argv++, argc--;
    }
#ifdef CPM
    if (argc-- > 0)
    {
        if (freopen(*argv, "r", 0) == 0) /* Open input file */
            fatalErr("Can't open %s", *argv);
        else if (argc > 0 && freopen(argv[1], "w", 1) == 0) /* Open output file */
            fatalErr("Can't create %s", argv[1]);
    }
    sub_1680();  /* First_init */
    parseStmt(); /* Compiling intermediate code */
    if (fclose(1) == (-1))
    { /* Close output file */
        prError("Error closing output file");
    }
#else
    if (argc-- > 0)
    {
        if (freopen(*argv, "r", stdin) == NULL) /* Open input file */
            fatalErr("Can't open %s", *argv);
        else if (argc > 0 && freopen(argv[1], "w", stdout) == NULL) /* Open output file */
            fatalErr("Can't create %s", argv[1]);
    }
    sub_1680();  /* First_init */
    parseStmt(); /* Compiling intermediate code */
    if (fclose(stdout) == EOF)
    { /* Close output file */
        prError("Error closing output file");
    }
#endif
    /* Exit with error code */
    exit(errcnt != 0); /* Generated code is not significantly different */
}

/*********************************************************
 * sub_6AA2 OK PMO     Used in: badIntCode,  sub_17E0, sub_19C1,
 *			      sub_2BD0, sub_4192, sub_6B1D,
 * fatalErr error  	      sub_6B9B,
 * Difference due to change to use stdarg
 *********************************************************/
_Noreturn void fatalErr(char *fmt, ...)
{
#ifdef CPM
    prMsg(fmt, (char *)&((&fmt)[1]));
    fclose(1);
    exit(2);
#else
    va_list args;
    va_start(args, fmt);
    prMsg(fmt, args);
    va_end(args);
    fclose(stdout);
    exit(2);
#endif
}

/*********************************************************
 * sub_6AD0  OK PMO  Used in: sub_E43,  sub_3DC9, sub_43EF,
 * Warning message	      sub_54B6, sub_5CF5, sub_600E
 * Difference due to change to use stdarg
 *********************************************************/
void prWarning(char *fmt, ...)
{
    if (wflag == 0)
    {
#ifdef CPM
        fprintf(2, "%s:%d:\t", progname, lineno);
        _doprnt(2, fmt, (char *)&((&fmt)[1]));
        fprintf(2, " (warning)\n");
#else
        va_list args;
        fprintf(stderr, "%s:%d:\t", progname, lineno);
        va_start(args, fmt);
        vfprintf(stderr, fmt, args);
        va_end(args);
        fprintf(stderr, " (warning)\n");
#endif
    }
}

/*********************************************************
 * prError OK PMO
 * Nonfatal error
 * Difference due to use of stdarg
 *********************************************************/
void prError(char *fmt, ...)
{
#ifdef CPM
    prMsg(fmt, (char *)&((&fmt)[1]));
#else
    va_list args;
    va_start(args, fmt);
    prMsg(fmt, args);
    va_end(args);
#endif
    if (++errcnt >= MAXERR)
        fatalErr("Too many errors");
}

/*********************************************************
 * prMsg OK PMO	      	  Used in: ferror, prError
 * Difference due to use of stdarg
 *********************************************************/
#ifdef CPM
void prMsg(char *fmt, char *args)
{
    fprintf(2, "%s:%d:\t", progname, lineno);
    _doprnt(2, fmt, args);
    fputc('\n', 2);
}
#else
void prMsg(char *fmt, va_list args)
{
    fprintf(stderr, "%s:%d:\t", progname, lineno);
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
}
#endif

/*********************************************************
 * allocMem OK    Used in: sub_265,  sub_1754, sub_19C1,
 *			     sub_1B0C, allocNode, sub_39CA,
 * alloc_mem & return ptr    sub_4192, sub_508A, sub_5DF6
 *********************************************************/
void *allocMem(size_t size)
{
    register char *ptr;

    do
    {
        if ((ptr = malloc(size)))
            goto done;
    } while (releaseNodeFreeList());
    fatalErr("No room");
done:
    blkclr(ptr, size);
    return ptr;
}
