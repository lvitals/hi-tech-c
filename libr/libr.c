
/*
 * Program to combine multiple object files into one file.
 *
 * File libr3.c Created 18.07.2021 Last Modified 03.08.2021
 *
 * The C source code was RESTORED by disassembling the original executable
 * file LIBR.COM from the Hi-Tech v3.09 compiler.
 *
 * This file with some warning messages is compiled by Hi-Tech C compiler
 * v3.09 and the resulting executable file performs all the functions
 * provided. To compile, use the following command:
 *
 *    cc -o libr3.c
 *
 * The created executable file almost completely matches the original image.
 *
 * Not a commercial goal of this laborious work is to popularize
 * among potential fans of 8-bit computers the old HI-TECH C compiler
 * V3.09 (HI-TECH Software) and extend its life, outside of the
 * CP/M environment (Digital Research, Inc), for full operation in a
 * Unix-like operating system UZI-180 without using the CP/M emulator.
 *
 *	Andrey Nikitin 03.08.2021
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>
#include <stdint.h>
#include "libr.h"

/*
 *	These definitions are not present in the header files of the compiler.
 */
#if CPM
#define SEEK_SET 0 /* from beginning of file */
#define SEEK_CUR 1 /* from current position */
#define SEEK_END 2 /* from end of file */

#define USE_GETARGS 1
#endif

typedef unsigned char uint8_t;

#ifdef USE_GETARGS
extern int _argc_;
char **_getargs(char *_str, const char *_name);
#endif

/*
 *	Used macros
 */
#define bittst(var, bitno) ((var) & 1 << (bitno))

/*
 *	Initialized variables
 */
char symbolTypes[] = "D?C???U";
int width = 80;            // Output width		word_4256
char commands[] = "rdxms"; // Program Keys
char usageMsg[] = "Usage: libr [-w][-pwidth] key [subkeys symbol] file.lib [modules ...]";
char arry_42f0[] = {0};
char order32[] = {0, 1, 2, 3}; // 4358
uchar order16[] = {0, 1};      // 435c

/*
 *	Descriptions of uninitialized variables and arrays
 */
int num_ofiles;      // Number of object files		word_4745
char **cmdLineNames; //
char *moduleFlags;   // Pointer to an area of modsize num_ofiles
int *moduleSizes;    // Pointer to an area of modsize num_ofiles * 2
int tempFileSize;
int num_modules; // Number of modules in library file	word_474f
char moduleRead;
int size_symbols; //
long longZero;
char *libraryName; // filename
int newModuleCnt;
int newSymSize;
char symbolsRead;
int symCnt;
FILE *libraryFp; // file pointer
long moduleSize;
uint8_t moduleBuf[512];
FILE *tempFp; // file pointer
uint8_t libBuf[512];
long unusedLong;
FILE *libContentFp; // file pointer
int symSize;
uchar listModuleType;
uchar listDefinedOpt;
uchar listUndefinedOpt;
char *listModuleName; //
char listModuleFound;
int columns;
int curColumn;
char ban_warn; // byte_4b7b
int err_num;   // word_4b7c
int cmdIndex;
char haveIdent;
uchar recbuf[512];
uchar rectyp; // byte_4d81
int length;   // word_4d82

struct sym
{
    uchar *name;
    uchar flags;
};

struct sym *curSymPtr; // ptr to struct sym
ulong reclen;
char hasNonExtern;
FILE *moduleFp; // file pointer
struct sym symbols[500];
char *moduleName; // file name

/****************************************************************
 * Prototype functions are located in sequence of being in
 * original binary image of LIBR.COM
 *
 * ok++ - Full binary compatibility with code in original file;
 * ok   - Code generated during compilation differs slightly,
 *        but is logically correct;
 ****************************************************************/

#if CPM
char *strrchr(char *, int);
int fputc(int c, FILE *stream);
int fgetc(FILE *stream);
int fseek(FILE *stream, long offs, int wh);
int unlink(char *name);
char *index(char *s, char c);
#endif

void (*dispatch[])() = {
    replaceModule,  // Replace modules
    deleteModule,   // Delete modules
    extractModules, // Extract modules
    listModules,    // List modules
    listWithSymbols // List modules with symbols
};

/**************************************************************************
 1	cmpNames	ok++	String comparison
 **************************************************************************/
/* could use strcasecmp */
int cmpNames(register char *st, char *p2)
{
    char l1, l2;

    while (*st != 0)
    {
        l1 = tolower(*st);
        l2 = tolower(*p2);
        if (l1 != l2)
            break;
        ++st;
        ++p2;
    }
    return *st - *p2;
}

/**************************************************************************
 2	allocModuleArrays	ok++
 **************************************************************************/
void allocModuleArrays(int p1, char **p2)
{

    cmdLineNames = p2;
    if ((num_ofiles = p1) != 0)
    {
        moduleFlags = xalloc(num_ofiles);
        moduleSizes = (int *)xalloc(num_ofiles * sizeof(int));
    }
}

/**************************************************************************
 3	lookupName	ok++
 **************************************************************************/
uchar lookupName(uint8_t *p1)
{
    uint8_t l1;

    if (num_ofiles == 0)
        return 1;
    l1 = num_ofiles;
    do
    {
        if (l1-- == 0)
            return 0;
    } while (cmpNames(cmdLineNames[l1], (char *)p1) != 0);

    moduleFlags[l1] = 1;
    return l1 + 1;
}

/**************************************************************************
 4	processUnmatched	ok++
 **************************************************************************/
void processUnmatched(void (*fun)(uint8_t *, uint))
{
    int l1;

    l1 = 0;
    while (l1 != num_ofiles)
    {
        if (moduleFlags[l1] == 0)
            fun((uint8_t *)cmdLineNames[l1], l1);
        ++l1;
    }
}

/**************************************************************************
 5	copyUnchangedSymbols	ok++
 **************************************************************************/
void copyUnchangedSymbols(uint8_t *name, long dummy)
{
    (void)dummy;
    if (lookupName(name) == 0)
    {
        copySymbolsToTemp();
    }
}

/**************************************************************************
 6	copyUnchangedModules	ok++
 **************************************************************************/
void copyUnchangedModules(uint8_t *name, long dummy)
{
    (void)dummy;
    if (lookupName(name) == 0)
    {
        copyModuleToTemp();
    }
}

/**************************************************************************
 7	deleteModule	Delete modules		ok++
 **************************************************************************/
void deleteModule()
{

    if (num_ofiles == 0)
    {
        fatal_err("delete what ?");
    }

    openContent();
    openTemp();
    visitModules(copyUnchangedSymbols);
    processUnmatched(noModule); // Module not found
    rewindLibrary();
    visitModules(copyUnchangedModules);
    commitNewLibrary();
}

/**************************************************************************
 8	extractNamedModule	ok++
 **************************************************************************/
void extractNamedModule(uint8_t *p1, long dummy)
{
    (void)dummy;
    if (lookupName(p1) != 0)
    {
        extractOneModule((char *)p1);
    }
}

/**************************************************************************
 9	extractModules	Extract modules		ok++
 **************************************************************************/
void extractModules()
{

    openContent();
    visitModules(extractNamedModule);
    processUnmatched(noModule); // Module not found
}

/**************************************************************************
 10	openTemp	ok++
 **************************************************************************/
void openTemp()
{

    if ((tempFp = fopen("libtmp.$$$", "wb")) == 0)
    {
        open_err("libtmp.$$$");
    }
}

/**************************************************************************
 11	writeFile	ok++
 **************************************************************************/
int writeFile(uint8_t *buf, uint size, uint count, FILE *fp)
{

    if ((size = (uint)fwrite(buf, size, count, fp)) != count)
    {
        fatal_err("Write error on %s file", (fp == tempFp) ? "temp" : "output");
    }
    return size;
}

/**************************************************************************
 12	closeTemp() sub_03c7hh	ok++
 **************************************************************************/
void closeTemp()
{

    if (tempFp != 0)
    {
        fclose(tempFp);
        unlink("libtmp.$$$");
    }
}

/**************************************************************************
 13	openLibrary	ok++
 **************************************************************************/
void openLibrary(char *name)
{
    register char *st;

    libraryName = name;
    libraryFp = fopen(libraryName, "rb");

    if (libraryFp == 0)
    {
        if (cmdIndex != 0)
        {
            open_err(libraryName);
        }

        st = strrchr(libraryName, '.');

        if (st == 0 || (strcmp(st, ".lib") && strcmp(st, ".LIB")))
        {
            warning("library file names should have .lib extension: %s", libraryName);
        }

        size_symbols = 0;
        num_modules = 0;
        return;
    }

    if (fread(libBuf, 4, 1, libraryFp) != 1)
    {
        unexp_eof();
    }

    size_symbols = conv_btou16(libBuf);

    //  Get the number of modules in a library file
    num_modules = conv_btou16(libBuf + 2);
}

/**************************************************************************
 14	openContent	ok++
 **************************************************************************/
void openContent()
{

    if (libraryFp == 0)
    {
        return;
    }
    if ((libContentFp = fopen(libraryName, "rb")) != 0)
    {
        if (fseek(libContentFp, ((long)size_symbols + 4), SEEK_SET) != -1)
        {
            return;
        }
    }
    open_err(libraryName);
}

/**************************************************************************
 15	rewindLibrary	ok++
 **************************************************************************/
void rewindLibrary()
{

    if (err_num != 0)
    {
        finish(5);
    }
    if (libraryFp == 0)
    {
        return;
    }
    if (fseek(libraryFp, 4, SEEK_SET) != -1)
    {
        if (fseek(libContentFp, ((long)size_symbols + 4), SEEK_SET) != -1)
        {
            return;
        }
    }
    seek_err(libraryName);
}

/**************************************************************************
 16	commitNewLibrary	ok++
 **************************************************************************/
/*
    close original library and replace with the new library created in
    libtmp.$$$, the symbol directory sizing information and number of
    modules is also added
*/
void commitNewLibrary()
{
    int l1;

    fclose(tempFp);
    if (libraryFp != 0)
    {
        fclose(libraryFp);
        fclose(libContentFp);
    }
    if ((tempFp = fopen("libtmp.$$$", "rb")) == 0)
    {
        open_err("libtmp.$$$");
    }
    if ((libraryFp = fopen(libraryName, "wb")) == 0)
    {
        open_err(libraryName);
    }

    conv_u16tob(newSymSize, libBuf);
    conv_u16tob(newModuleCnt, libBuf + 2);

    writeFile(libBuf, 1, 4, libraryFp);

    while ((l1 = (int)fread(libBuf, 1, 512, tempFp)) != 0)
    {
        writeFile(libBuf, 1, l1, libraryFp);
    }
    fclose(libraryFp);
}

/**************************************************************************
 17	visitModules	ok++
 **************************************************************************/
void visitModules(void (*funptr)(uint8_t *, long))
{
    int cntModulesLeft;

    for (cntModulesLeft = num_modules; cntModulesLeft; --cntModulesLeft)
    {
        if (fread(libBuf, 12, 1, libraryFp) != 1)
        {
            unexp_eof();
        }

        symSize = conv_btou16(libBuf);        // +0 Offset in the read buffer
        symCnt = conv_btou16(libBuf + 2);     // +2
        moduleSize = conv_btou32(libBuf + 4); // +4
        unusedLong = conv_btou32(libBuf + 8); // +8

        readName(libBuf + 12);

        symbolsRead = moduleRead = 0;

        funptr(libBuf + 12, unusedLong);

        if (!symbolsRead && fseek(libraryFp, symSize, SEEK_CUR) == -1)
        {
            seek_err(libraryName);
        }

        if (libContentFp && !moduleRead &&
            fseek(libContentFp, moduleSize, SEEK_CUR) == -1)
        {
            seek_err(libraryName);
        }
    }
}

/**************************************************************************
 18	sub_0727	ok++
 **************************************************************************/
void visitSymbols(void (*action)(uint8_t *, int))
{
    int cnt;
    int tType;

    for (cnt = symCnt; cnt; cnt--)
    {
        if ((tType = fgetc(libraryFp)) == -1)
        {
            unexp_eof();
        }
        readName(moduleBuf);
        action(moduleBuf, tType);
    }
    symbolsRead = 1;
}

/**************************************************************************
 19	copySymbolsToTemp	ok++
 **************************************************************************/
void copySymbolsToTemp()
{
    int l1, l2;

    writeFile(libBuf, 1, (l1 = (int)strlen((char *)libBuf + 12) + 13), tempFp);
    tempFileSize += l1;
    newSymSize += (l1 + symSize);
    l1 = symSize;
    while (l1 != 0)
    {
        l2 = (l1 < 0x200) ? l1 : 0x200;
        if ((int)fread(moduleBuf, 1, l2, libraryFp) != l2)
        {
            unexp_eof();
        }
        writeFile(moduleBuf, 1, l2, tempFp);
        tempFileSize += l2;
        l1 -= l2;
    }
    ++newModuleCnt;
    symbolsRead = 1;
}

/**************************************************************************
 20	copyModuleToTemp	ok++
 **************************************************************************/
void copyModuleToTemp()
{
    int l1, l2;

    l1 = moduleSize;
    while (l1 != 0)
    {
        l2 = (l1 < 512) ? l1 : 512;
        if ((int)fread(moduleBuf, 1, l2, libContentFp) != l2) {
            unexp_eof();
        }
        writeFile(moduleBuf, 1, l2, tempFp);
        tempFileSize += l2;
        l1 -= l2;
    }
    moduleRead = 1;
}

/**************************************************************************
 21	extractOneModule	ok++
 **************************************************************************/
void extractOneModule(char *name)
{
    int l1, l2;
    register FILE *st;

    if ((st = fopen(name, "wb")) == 0)
    {
        open_err(name);
    }
    l1 = moduleSize;
    while (l1 != 0)
    {
        l2 = (l1 < 512) ? l1 : 512;
        if ((int)fread(moduleBuf, 1, l2, libContentFp) != l2)
        {
            unexp_eof();
        }
        writeFile(moduleBuf, 1, l2, st);
        l1 -= l2;
    }
    fclose(st);
    moduleRead = 1;
}

/**************************************************************************
 22	copyNewModule	ok++	used when replacing modules
 **************************************************************************/
void copyNewModule(uint8_t *p1, uint moduleId)
{
    unsigned int l1, l2;
    FILE *st;

    if ((st = fopen((char *)p1, "rb")) == NULL)
    {
        open_err((char *)p1);
    }
    l1 = moduleSizes[moduleId];

    while ((l2 = (unsigned int)fread(moduleBuf, 1, l1 > 512 ? 512 : l1, st)) != 0)
    {
        writeFile(moduleBuf, 1, l2, tempFp);
        tempFileSize += l2;
        l1 -= l2;
    }
    fclose(st);
}

/**************************************************************************
 23	copyNewSymbols	ok++	used when replacing modules
 **************************************************************************/
void copyNewSymbols(uint8_t *moduleName, uint moduleId)
{
    int modsize, simsize, cnt;
    register struct sym *st;

    moduleSizes[moduleId] = (modsize = scanModule((char *)moduleName));

    cnt = (int)(curSymPtr - symbols);
    simsize = cnt * 2; /* flag & trailing 0 */

    for (st = symbols; st != curSymPtr; ++st)
        simsize += (int)strlen((char *)st->name); /* add in the symbol lengths */

    newSymSize += (int)(strlen((char *)moduleName) + simsize +
                        13); /* 13 -> header size + trailing 0 of module name */
    conv_u16tob(simsize, libBuf);
    conv_u16tob(cnt, libBuf + 2);
    conv_u32tob((unsigned long)modsize, libBuf + 4);
    conv_u32tob(longZero, libBuf + 8);
    writeFile(libBuf, 1, 12, tempFp);
    tempFileSize += 12;
    writeName(moduleName);

    for (st = symbols; st != curSymPtr; ++st)
    {
        fputc(st->flags, tempFp);
        ++tempFileSize;
        writeName(st->name);
    }
    ++newModuleCnt;
}

/**************************************************************************
 24	conv_u32tob	sub_0bd5h	ok++
 **************************************************************************/
void conv_u32tob(unsigned long p1, uint8_t *p2)
{

    *p2 = (uint8_t)p1;
    *(p2 + 1) = (uint8_t)(p1 >> 8);
    *(p2 + 2) = (uint8_t)(p1 >> 0x10);
    *(p2 + 3) = p1 >> 0x18;
}

/**************************************************************************
 25	conv_u16tob	sub_0c31h	ok++
 **************************************************************************/
void conv_u16tob(uint p1, uint8_t *p2)
{

    *p2 = p1;
    *(p2 + 1) = p1 >> 8;
}

/**************************************************************************
 26	conv_btou16	sub_0c53h	ok++
 **************************************************************************/
uint conv_btou16(register uchar *p1)
{
    return *p1 + (*(p1 + 1) << 8);
}

/**************************************************************************
 27	conv_btou32	sub_0c73h	ok++
 **************************************************************************/
long conv_btou32(register uchar *p1)
{
    return *p1 + (*(p1 + 1) << 8) + (*(p1 + 2) << 0x10) + (*(p1 + 3) << 0x18);
}

/**************************************************************************
 28	readName	ok++
 **************************************************************************/
void readName(register uint8_t *p1)
{
    int l1;

    do
    {
        if ((l1 = fgetc(libraryFp)) == -1)
            unexp_eof();
        *(p1++) = l1;
    } while (l1 != 0);
}

/**************************************************************************
 29	writeName	ok++
 **************************************************************************/
void writeName(register uint8_t *p1)
{
    do
    {
        ++tempFileSize;
    } while (fputc(*(p1++), tempFp) != 0);
}

/**************************************************************************
 30	unexp_eof	sub_0d14h	ok++
 **************************************************************************/
void unexp_eof()
{
    badFormat(libraryName, "unexpected end of file");
}

/**************************************************************************
 31	checkToList	ok++
 **************************************************************************/
void checkToList(uint8_t *p1, int tType)
{
    if (((char)tType == 0 ? listDefinedOpt : listUndefinedOpt) == 0)
    {
        return;
    }
    if (strcmp(listModuleName, (char *)p1) != 0)
    {
        if (*p1 != '_' || strcmp(listModuleName, (char *)p1 + 1) != 0)
        {
            return;
        }
    }
    listModuleFound = 1;
    listModuleType = tType;
}

/**************************************************************************
 32	listOneModule	Print obj name from library	ok++
 **************************************************************************/
void listOneModule(uint8_t *p1, long dummy)
{
    (void)dummy;
    if (lookupName(p1) == 0)
    {
        return;
    }
    if (listModuleName)
    {
        listModuleFound = 0;
        visitSymbols(checkToList);
        if (!listModuleFound)
        {
            return;
        }
    }
    printf("%-15.15s", (char *)p1);
    if (listModuleName != 0)
    {
        printf(" %c", (listModuleType >= 7) ? '?' : symbolTypes[listModuleType]);
    }
    putchar('\n');
}

/**************************************************************************
 33	listModules	List modules	ok++
 **************************************************************************/
void listModules(char *key, char *name)
{

    if (*key)
    {
        listModuleName = name;
        do
        {
            switch (*key)
            {
            case 'd':
                listDefinedOpt = 1;
                break;
            case 'u':
                listUndefinedOpt = 1;
                break;
            default:
                fatal_err("Subkeys: d(defined) u(ndefined)");
            }
        } while (*++key);
    }
    visitModules(listOneModule); // Print name obj name from library with the key m
}

/**************************************************************************
 34	printSymbol	ok++	Print symbol name with the key s
 **************************************************************************/
void printSymbol(uint8_t *name, int tType)
{

    if (curColumn >= columns)
    {
        printf("\t\t");
        curColumn = 0;
    }
    printf("%c %-12.12s", ((tType >= 7) ? '?' : symbolTypes[tType]), (char *)name);
    if (++curColumn >= columns)
    {
        printf("\n");
        return;
    }
    printf("  ");
}

/**************************************************************************
 35	printObjAndSymbols	ok+	Print obj name and symbol names
 **************************************************************************/
void printObjAndSymbols(uint8_t *p1, long dummy)
{
    (void)dummy;
    if (lookupName(p1) == 0)
    {
        return;
    }
    printf("%-16.15s", (char *)p1); // Print obj name from library with the key s
    curColumn = 0;
    visitSymbols(printSymbol); // Print symbol name with the key s
    if (curColumn == 0)
    {
        return;
    }
    if (curColumn >= columns)
    {
        return;
    }
    putchar('\n');
}

/**************************************************************************
 36	listWithSymbols	List modules with symbols	ok++
 **************************************************************************/
void listWithSymbols()
{

    if ((columns = (width - 16) / 16) == 0)
    {
        columns = 1;
    }
    visitModules(printObjAndSymbols); // Print obj name and symbol names
    processUnmatched(noModule);       // Module not found
}

/**************************************************************************
 37	main	sub_0f38h	ok+
 **************************************************************************/
int main(int argc, char **argv)
{
    char *l1;
    char *l2;

    if (signal(SIGINT, SIG_IGN) != SIG_IGN)
    {
        signal(SIGINT, sigintHandler);
    }
#if USE_GETARGS
    if (argc == 1)
    {
        argv = _getargs(0, "libr");
        argc = _argc_;
    }
#endif

    fclose(stdin);
    for (--argc, ++argv; argc && **argv == '-'; --argc, ++argv)
    {
        l1 = *argv + 1;
        while (*l1)
        {
            switch (*l1++)
            {
            case 'W': // Suppress non-fatal errors
            case 'w':
                ban_warn = 1; // Disable warning messages
                break;
            case 'P': // Specify page width
            case 'p':
                if (isdigit(*l1))
                {
                    width = atoi(l1); // Assigning a new width value
                    l1 = "";
                    break;
                }
            default:
                fatal_err(usageMsg); // "Usage: ...]"
                break;
            }
        }
    }

    if (argc < 2)
    {
        fatal_err(usageMsg); // "Usage: ...]"
    }
    l2 = *argv;
    --argc;
    ++argv;
    if ((l1 = index(commands, tolower(*l2))) == NULL)
    {
        fatal_err("Keys: r(eplace), d(elete), (e)x(tract), m(odules), s(ymbols)");
    }

    cmdIndex = (int)(l1 - commands);

    if (*(l2 + 1) != 0)
    {
        l1 = *argv;
        --argc;
        ++argv;
    }
    if (argc == 0)
    {
        fatal_err(usageMsg); // "Usage: ...]"
    }

    openLibrary(*argv);
    allocModuleArrays(argc - 1, argv + 1);

    (dispatch[cmdIndex])();

    finish(err_num != 0); // Terminate the program with the appropriate error code
}

/**************************************************************************
 38	fatal_err	sub_117ch	ok++
 **************************************************************************/
#ifndef CPM
void vfatal_err(char *fmt, va_list args)
{
    vfprintf(stderr, fmt, args);
    fputc('\n', stderr);
    finish(6);
}
void fatal_err(char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);
    vfatal_err(fmt, args);
    va_end(args);
}
#else
void fatal_err(p1, p2, p3, p4, p5)
{

    fprintf(stderr, (char *)p1, p2, p3, p4, p5);
    fputc('\n', stderr);
    finish(6);
}
#endif

/**************************************************************************
 39	open_err	sub_11c4h	ok++
 **************************************************************************/
_Noreturn void open_err(char *p1)
{

    fprintf(stderr, "Can't open %s\n", p1);
    finish(2);
}

/**************************************************************************
 40	seek_err	sub_11e5h	ok++
 **************************************************************************/
void seek_err(char *p1)
{

    fprintf(stderr, "Seek error on %s\n", p1);
    finish(3);
}

/**************************************************************************
 41	Simple error	sub_1206h	ok++
 **************************************************************************/
#if CPM
void simpl_err(p1, p2, p3, p4, p5) char *p1, *p2;
{

    fprintf(stderr, p1, p2, p3, p4, p5);
    fputc('\n', stderr);
    ++err_num;
}
#else
void simpl_err(char *p1, char *p2)
{

    fprintf(stderr, p1, p2);
    fputc('\n', stderr);
    ++err_num;
}
#endif

/**************************************************************************
 42	warning		sub_124dh	ok++
 **************************************************************************/
#if CPM
void warning(p1, p2, p3, p4, p5) char *p1, *p2;
{

    if (ban_warn != 0)
        return;
    fprintf(stderr, p1, p2, p3, p4, p5);
    fprintf(stderr, " (warning)\n");
}
#else
void warning(char *p1, char *p2)
{

    if (ban_warn != 0)
    {
        return;
    }
    fprintf(stderr, p1, p2);
    fprintf(stderr, " (warning)\n");
}
#endif

/**************************************************************************
 43	badFormat		sub_1294h	ok++
 **************************************************************************/
#if CPM
void badFormat(name, fmt, p3, p4, p5, p6) char *name, *fmt;
uint p3, p4;
{
    fprintf(stderr, "bad file format: %s\n", name);
    fatal_err(fmt, p3, p4, p5, p6);
}
#else
void badFormat(char *name, char *fmt, ...)
{
    va_list args;
    va_start(args, fmt);

    fprintf(stderr, "bad file format: %s\n", name);
    vfatal_err(fmt, args);
    va_end(args);
}
#endif
/**************************************************************************
 44	noModule	ok++	module not found
 **************************************************************************/
void noModule(uint8_t *p1, uint dummy)
{
    (void)dummy;
    simpl_err("no such module: %s", (char *)p1);
}

/**************************************************************************
 45	finish	ok++
 **************************************************************************/
_Noreturn void finish(int p1)
{

    closeTemp();
    exit(p1);
}

/**************************************************************************
 46	sigintHandler	ok++
 **************************************************************************/
void sigintHandler(int p1)
{
    (void)p1;
    finish(4);
}

/**************************************************************************
 47	allocmem	sub_1304h	ok
 **************************************************************************/
void *xalloc(int len)
{
    void *p;

    if ((p = malloc(len)) == 0)
    {
        fatal_err("Cannot get memory");
    }
    return memset(p, 0, len);
}

/**************************************************************************
 48	parseIdentRec	ok
 **************************************************************************/
void parseIdentRec()
{
    uchar l1;

    if ((int)fread(recbuf, 1, length, moduleFp) != length)
    {
        badFormat(moduleName, "incomplete ident record");
    }

    l1 = 0;
    if (haveIdent != 0)
    {

        while (l1 < 4)
        {
            if (recbuf[l1] != order32[l1])
            {
                badFormat(moduleName, "ident records do not match"); // bug fix
            }
            l1++;
        }
        l1 = 0;
        while (l1 < 2)
        {
            if ((char)(recbuf + 4)[l1] != (char)order16[l1])
            {
                badFormat(moduleName, "ident records do not match"); // bug fix
            }
            l1++;
        }
        return;
    }
    while (l1 < 4)
    { /* read the 32bit byte order */
        order32[l1] = recbuf[l1];
        l1++;
    }
    l1 = 0;
    while (l1 < 2)
    { /* read the 16bit byte order */
        order16[l1] = recbuf[l1 + 4];
        l1++;
    }
    haveIdent = 1;
}

/**************************************************************************
 49	get_modu16	sub_1429h	ok++
 **************************************************************************/
uint get_modu16(uchar *p1)
{

    return (p1[order16[1]] << 8) + p1[order16[0]];
}

/**************************************************************************
 50	addSymbol	ok+
 **************************************************************************/

void addSymbol(uchar *name, int flags)
{
    register struct sym *st;

    if (curSymPtr == &symbols[500])
    {
        fatal_err("Too many symbols in %s", moduleName);
    }
    st = curSymPtr;
    if ((st->flags = flags) != 6)
    {
        hasNonExtern = 1;
    }
    strcpy((char *)(curSymPtr->name = xalloc((int)strlen((char *)name) + 1)), (char *)name);

    ++curSymPtr;
}

// clang-format off
// Object record types:   | Length (16 bits)     | Record type (8 bits) | Data (Length*8 bits) |
#define TEXT_RECORD  1
#define PSECT_RECORD 2
#define RELOC_RECORD 3
#define SYM_RECORD   4 // | Value (32 bits)      | flags (16 bits)      | psect name           | symbol name |
#define START_RECORD 5
#define END_RECORD   6 // | flags (16 bits)      |
#define IDENT_RECORD 7 // | byte order (32 bits) | byte order (16 bits) | machine name         | version number (16 bits) |
#define XPSECT_RECORD  8
#define SEGMENT_RECORD 9
// clang-format on

/**************************************************************************
 51	scanModule	ok+
 **************************************************************************/
int scanModule(char *name)
{

    moduleFp = fopen((moduleName = name), "rb");
    if (moduleFp == 0)
    {
        open_err(moduleName);
    }
    hasNonExtern = 0;
    reclen = 0;
    curSymPtr = symbols;

    for (;;)
    {
        getRecord(); // Get type of record

        switch (rectyp)
        {
        case IDENT_RECORD:
            parseIdentRec();
            break;
        case SYM_RECORD:
            parseSymbolRec();
            break;
        default:
            skipRecord();
            break;
        case END_RECORD:
            fclose(moduleFp);
            if (reclen >= 0x10000)
            {
                fatal_err("file too big ");
            }
            if (hasNonExtern == 0)
            {
                warning(" module %s defines no symbols ", moduleName);
            }
            return reclen;
        }
    }
}

/**************************************************************************
 52	conv_btou16a	sub_1548h	ok++
 **************************************************************************/
uint conv_btou16a(register uchar *st)
{

    return st[0] + (st[1] << 8);
}

/**************************************************************************
 53	getRecord	ok++	Get type of record
 **************************************************************************/
void getRecord()
{

    if (fread(recbuf, 3, 1, moduleFp) != 1)
    {
        badFormat(moduleName, "no end record found");
    }

    rectyp = recbuf[2];

    if (rectyp == 0 || rectyp >= 9)
    {
        badFormat(moduleName, "unknown record type: %d", rectyp);
    }
    if ((512 - 3) < (length = conv_btou16a(recbuf)))
    {
        badFormat(moduleName, "record too long: %d+%d", 3, length);
    }

    reclen += (length + 3);
}

/**************************************************************************
 54	skipRecord	ok++
 **************************************************************************/
void skipRecord()
{

    if (fseek(moduleFp, length, SEEK_CUR) != -1)
    {
        return;
    }
    badFormat(moduleName, "incomplete record");
}

/**************************************************************************
 55	parseSymbolRec	ok++
 **************************************************************************/
void parseSymbolRec()
{
    uchar *name;
    uint l2;
    uchar flags;
    register uchar *st;

    if ((int)fread(recbuf, 1, length, moduleFp) != length)
    {
        badFormat(moduleName, "incomplete symbol record");
    }

    st = recbuf;

    while (st < (recbuf + length))
    {
        l2 = get_modu16(st + 4); /* read flags */
        flags = l2 & 0xF;        /* mask off low 4 bits */
        name = st + 6;           /* skip to psect name */

        while (*name++) /* skip psect name */
            ;
        /* only process PUBLIC, COMM and EXTERN entries */
        if ((bittst(l2, 4) && flags == 0) || flags == 2 || flags == 6)
        {
            addSymbol(name, flags);
        }
        st = name; // m6:

        while (*(st++) != 0) /* skip symbol name */
            ;
    }
}

/**************************************************************************
 56	copyMatchedSymbols	ok++
 **************************************************************************/
void copyMatchedSymbols(uint8_t *p1, long dummy)
{
    (void)dummy;
    if (lookupName(p1) != 0)
    {
        copyNewSymbols(p1, lookupName(p1) - 1);
        return;
    }
    copySymbolsToTemp();
}

/**************************************************************************
 57	copyMatchedModules	ok++
 **************************************************************************/
void copyMatchedModules(uint8_t *p1, long dummy)
{
    (void)dummy;
    if (lookupName(p1) != 0)
    {
        copyNewModule(p1, lookupName(p1) - 1);
        return;
    }
    copyModuleToTemp();
}

/**************************************************************************
 58	replaceModule	Replace modules		ok++
 **************************************************************************/
void replaceModule()
{
    if (num_ofiles == 0)
    {
        fatal_err("replace what ?");
    }
    openContent();
    openTemp();
    visitModules(copyMatchedSymbols);
    processUnmatched(copyNewSymbols);
    rewindLibrary();
    visitModules(copyMatchedModules);
    processUnmatched(copyNewModule);
    commitNewLibrary();
}
