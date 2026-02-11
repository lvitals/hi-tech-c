#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdarg.h>

/* Apenas para HI-TECH C, ignorado pelo GCC */
#ifdef z80
extern size_t strlen(char *);
extern char *strchr(char *, int);
#endif

#ifdef z80
#define bool char
#define true 1
#define false 0
#else
#include "dirent.h"
#include <stdbool.h>
#endif

extern int isatty(int);

#ifndef _MAX_PATH
#ifdef PATH_MAX
#define _MAX_PATH PATH_MAX
#else
#define _MAX_PATH 255
#endif
#endif

#define mapcase(c) tolower(c)

char *fname(char *name);

#define MAXARGS 512
#define MAXLEN _MAX_PATH
#define QUOTE -128
#define CHUNK 256
#define isterminator(c) ((c) == 0)
#define look() (*str)

#ifdef z80
extern int _argc_;
#else
int _argc_;
#endif

static char *name, *str, *bp;
static size_t bpSize;
static char interactive;
static void error(char *, ...);
#ifdef CPM
static void *alloc(size_t n);
#endif
static char nxtch(void);
static char iswild(char c);
static char isseparator(char c);

char match(char *regexp, char *text);
char matchstar(char *regexp, char *text);

/* Função auxiliar para verificar wildcards - elimina chamadas strchr */
static char has_wildcard(char *s)
{
    char *p = s;
    while (*p)
    {
        if (*p == '*' || *p == '?')
            return 1;
        p++;
    }
    return 0;
}

char **_getargs(char *_str, char *_name)
{
    char **argv;
    char *ap;
    char *cp;
    short argc;
    char c, quote;
    char *argbuf[MAXARGS + 1];
    char buf[MAXLEN];

    bp = NULL;
    bpSize = 0;
    quote = 0;
    name = _name;
    str = _str;
    
    if ((interactive = (str == NULL) ? 1 : 0))
        str = "\\";
    else
    {
        while (*str == ' ' || *str == '\t')
            str++;
        cp = str + strlen(str);
        while (--cp >= str && isseparator(*cp))
            ;
        cp[1] = '\0';
    }
    
    argbuf[0] = name;
    argc = 1;

    while (look())
    {
        if (argc >= MAXARGS)
            error("too many arguments", 0);
            
        while (isseparator(c = nxtch()))
            continue;
        if (c == '\0')
            break;
            
        ap = buf;

        if (c == '\'' || c == '"')
            quote = c;
        else
            *ap++ = c;

        while ((c = nxtch()) && (quote || !isseparator(c)))
        {
            if (ap >= &buf[MAXLEN])
                error("argument too long", 0);
            if (c == quote)
                quote = 0;
            else if (!quote && (c == '\'' || c == '"'))
                quote = c;
            else
            {
                *ap++ = c;
            }
        }

        *ap = '\0';

        /* Verifica wildcards usando função auxiliar */
        if (iswild(buf[0]) || has_wildcard(buf))
        {
#ifdef z80
#ifndef CPM
            argbuf[argc] = malloc((size_t)(ap - buf + 1));
            if (argbuf[argc])
                strcpy(argbuf[argc], buf);
            argc++;
#else
            argbuf[argc] = alloc((size_t)(ap - buf + 1));
            if (argbuf[argc])
                strcpy(argbuf[argc], buf);
            argc++;
#endif
#else
            char pattern[_MAX_PATH];
            char dirpath[_MAX_PATH];
            char *p;
            
            p = fname(buf);
            strcpy(pattern, p);
            *p = '\0';
            strcpy(dirpath, *buf ? buf : ".");
            
            DIR *dir = opendir(dirpath);
            if (dir)
            {
                struct dirent *entry;
                while ((entry = readdir(dir)))
                {
                    if (strcmp(entry->d_name, ".") && strcmp(entry->d_name, ".."))
                    {
                        if (match(pattern, entry->d_name))
                        {
#ifndef CPM
                            argbuf[argc] = malloc((size_t)(p - buf + strlen(entry->d_name) + 1));
#else
                            argbuf[argc] = alloc((size_t)(p - buf + strlen(entry->d_name) + 1));
#endif
                            if (argbuf[argc])
                            {
                                strcpy(argbuf[argc], buf);
                                strcat(argbuf[argc++], entry->d_name);
                            }
                        }
                    }
                }
                closedir(dir);
            }
            else
            {
                error(buf, pattern, ": no match", NULL);
            }
#endif
        }
        else
        {
#ifndef CPM
            argbuf[argc] = malloc((size_t)(ap - buf + 1));
#else
            argbuf[argc] = alloc((size_t)(ap - buf + 1));
#endif
            if (argbuf[argc])
            {
                strcpy(argbuf[argc], buf);
                argc++;
            }
        }
    }

    _argc_ = argc;
    argbuf[argc] = NULL;

#ifndef CPM
    argv = malloc((size_t)(argc + 1) * sizeof(char *));
#else
    argv = alloc((size_t)(argc + 1) * sizeof(char *));
#endif

    if (argv)
        memcpy(argv, argbuf, (size_t)(argc + 1) * sizeof(char *));

    return argv;
}

static char nxtch(void)
{
    int cnt = 0;
    int c;

    if (interactive && *str == '\\' && str[1] == '\0')
    {
        if (isatty(0))
            fprintf(stderr, "%s> ", name);
        cnt = 0;
        bpSize = 0;
        bp = NULL;
        
        while ((c = getchar()) != '\n' && c != EOF)
        {
            if ((size_t)(cnt + 1) >= bpSize)
            {
                size_t newSize = bpSize + CHUNK;
                char *newBp = realloc(bp, newSize);
                if (newBp == NULL)
                    error("no room for arguments", NULL);
                bp = newBp;
                bpSize = newSize;
            }
            if (bp)
                bp[cnt++] = (char)c;
        }
        if (bp && cnt > 0)
        {
            bp[cnt] = '\0';
            str = bp;
        }
        else
            str = "";
    }
    if (*str)
        return *str++;

    return '\0';
}

static void error(char *s, ...)
{
    va_list args;
    va_start(args, s);
    while (s != NULL)
    {
        fputs(s, stderr);
        s = va_arg(args, char *);
    }
    fputc('\n', stderr);
    va_end(args);
    exit(-1);
}

#ifdef CPM
static void *alloc(size_t n)
{
    void *p;

    if ((p = malloc(n)) == NULL)
        error("no room for arguments", NULL);
    return p;
}
#endif

static char iswild(char c)
{
    return c == '*' || c == '?';
}

static char isseparator(char c)
{
    return c == ' ' || c == '\t' || c == '\n';
}

char match(char *regexp, char *text)
{
    if (regexp == NULL || text == NULL)
        return 0;
    if (*regexp == '\0')
        return *text == '\0';
    if (*regexp == '*')
        return matchstar(regexp + 1, text);

    if (*text != '\0' && (mapcase(*regexp) == mapcase(*text) || *regexp == '?'))
        return match(regexp + 1, text + 1);
    return 0;
}

char matchstar(char *regexp, char *text)
{
    do
    {
        if (match(regexp, text))
            return 1;
    } while (*text++ != '\0');
    return 0;
}