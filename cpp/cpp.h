#define VERSION "2.3"
#define VERSION_DATE "2021/07/16"

#ifndef _CPP_H
#define _CPP_H
#define _CRT_SECURE_NO_WARNINGS 1
#define _CRT_DECLARE_NONSTDC_NAMES 1
#define _CRT_NONSTDC_NO_WARNINGS 1

#define LOCAL static
#define EXPORT

#ifdef CPM
int _doprnt();
#define vfprintf _doprnt
#define UConst
#define signed
#define SMALL
#else
#define UConst const
#endif
#ifndef STDIN_FILENO
#define STDIN_FILENO 0
#endif

/* support unix variants */
#define NULDEV "/dev/null"
#define DIRSEP "/"
#define ISDIRSEP(ch) ((ch) == '/')
#define HOST_OS "Unix"

#define HOST_CPU "Z80"
#define HOST_VENDOR "Hi-Tech"

struct symtab
{
    char *name;
    char *value;
    struct symtab *next;
};

extern char *skipbl(char *);
extern void pperror(char *fmt, ...);
extern void yyerror(char *fmt, ...);
extern int yyparse(void);
extern int yylex(void);
extern struct symtab *lookup(char *, int);

/* get args support */
extern int _argc_;
#ifdef USE_GETARGS
extern int _argc_;
char **_getargs(char *_str, const char *_name);
#endif
char *fname(char *name);

#endif /* _CPP_H */
