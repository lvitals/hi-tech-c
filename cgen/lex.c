#include "cgen.h"

/* clang-format on */

/* offsets into character array below */
static int16_t array_987D[] = {0, 0, 4, 9, 0x0A, 0x0C, 0x0E, 0x1C, 0x2D,
                               0x31, 0x3E, 0x52, 0x54, 0x62, 0x7C, 0x8E, 0x0A0, 0x0A1,
                               0x0A3, 0x0B3, 0x0C3, 0x0D7, 0x0E7, 0x0E8, 0x0EB, 0x0F9, 0x0FB,
                               0x0FD, 0x10B, 0x119, 0x11A, 0x11F, 0x12F, 0x130, 0x147, 0x155,
                               0x165, 0x173, 0x18B, 0x19B, 0x1A9, 0x1B9, 0x1BE, 0x1CE, 0x1DE,
                               0x1EE, 0x1EF, 0x1F4, 0x204, 0x214, 0x216, 0x217, 0x218, 0x219,
                               0x21A, 0x21B, 0x21C, 0x21D, 0x21E, 0x22E, 0x22F, 0x23F, 0x243,
                               0x244, 0x254, 0x255, 0x256, 0x26F, 0x28D, 0x2A6, 0x2B6, 0x2B8};

static uint8_t array_990D[] = {
    "Qhi\0"                           /* [0 or 1] 000h */
    "PQhi\0"                          /* [2] 004h */
    "\0"                              /* [3] 009h */
    "H\0"                             /* [4] 00Ah */
    "H\0"                             /* [5] 00Ch */
    "HNSTZ]^_djprs\0"                 /* [6] 00Eh */
    "HNSTUVZ]^_dgjprs\0"              /* [7] 01Ch */
    "Qhi\0"                           /* [8] 02Dh */
    "HNTYZ]^djprs\0"                  /* [9] 031h */
    "HNRSTUVZ[]^_`djnprs\0"           /* [10] 03Eh */
    "H\0"                             /* [11] 052h */
    "HNSTZ]^_djprs\0"                 /* [12] 054h */
    "HIMNRSTUVZ[\\]^_`abcdfjprs\0"    /* [13] 062h */
    "HNSTUVYZ]^_djprsw\0"             /* [14] 07Ch */
    "HNSTUVZ]^_djklprs\0"             /* [15] 08Eh */
    "\0"                              /* [16] 0A0h */
    "N\0"                             /* [17] 0A1h */
    "HNSTUVZ]^_djprs\0"               /* [18] 0A3h */
    "HNSTUVZ]^_djprs\0"               /* [19] 0B3h */
    "HNRSTUVYZ[]^_`djprs\0"           /* [20] 0C3h */
    "HNSTUVZ]^_djprs\0"               /* [21] 0D7h */
    "\0"                              /* [22] 0E7h */
    "Nt\0"                            /* [23] 0E8h */
    "HNSTZ]^_djprs\0"                 /* [24] 0EBh */
    "m\0"                             /* [25] 0F9h */
    "H\0"                             /* [26] 0FBh */
    "HNOTYZ]^djprs\0"                 /* [27] 0FDh */
    "HNTUVZ]^djprs\0"                 /* [28] 10Bh */
    "\0"                              /* [29] 119h */
    "PQhi\0"                          /* [30] 11Ah */
    "HNSTUVZ]^_djprs\0"               /* [31] 11Fh */
    "\0"                              /* [32] 12Fh */
    "HNRSTUVWZ[]^_`cdfjprsw\0"        /* [33] 130h */
    "HNSTZ]^_djprs\0"                 /* [34] 147h */
    "HNSTUVZ]^_djprs\0"               /* [35] 155h */
    "HNSTZ]^_djprs\0"                 /* [36] 165h */
    "HINSTUVWZ]^_abcdfjprsvw\0"       /* [37] 173h */
    "HNSTUVZ]^_djprs\0"               /* [38] 18Bh */
    "HNSTZ]^_djprs\0"                 /* [39] 19Bh */
    "HNSTUVZ]^_djprs\0"               /* [40] 1A9h */
    "PQhi\0"                          /* [41] 1B9h */
    "HNSTUVZ]^_djprs\0"               /* [42] 1BEh */
    "HNSTUVZ]^_djprs\0"               /* [43] 1CEh */
    "HNSTUVZ]^_djprs\0"               /* [44] 1DEh */
    "\0"                              /* [45] 1EEh */
    "PQhi\0"                          /* [46] 1EFh */
    "HNSTUVZ]^_djprs\0"               /* [47] 1F4h */
    "HNSTUVZ]^_djprs\0"               /* [48] 204h */
    "H\0"                             /* [49] 214h */
    "\0"                              /* [50] 216h */
    "\0"                              /* [51] 217h */
    "\0"                              /* [52] 218h */
    "\0"                              /* [53] 219h */
    "\0"                              /* [54] 21Ah */
    "\0"                              /* [55] 21Bh */
    "\0"                              /* [56] 21Ch */
    "\0"                              /* [57] 21Dh */
    "HNSTUVZ]^_djprs\0"               /* [58] 21Eh */
    "\0"                              /* [59] 22Eh */
    "HNSTUVZ]^_djprs\0"               /* [60] 22Fh */
    "Qhi\0"                           /* [61] 23Fh */
    "\0"                              /* [62] 243h */
    "HNSTUVZ]^_djprs\0"               /* [63] 244h */
    "\0"                              /* [64] 254h */
    "\0"                              /* [65] 255h */
    "HMNORSTUVZ[\\]^_`acdfjprs\0"     /* [66] 256h */
    "HIJKLNOSTUVYZ]^_abcdfjopqrsux\0" /* [67] 26Fh */
    "HMNRSTUVXZ[\\]^_`acdfjprs\0"     /* [68] 28Dh */
    "HNTWZ]^cdfjprsw\0"               /* [69] 2A6h */
    "H\0"                             /* [70] 2B6h */
    "HNTZ]^cdefjprs"                  /* [71] 2B8h */
};

static int16_t array_9BD4[] = {
    -66, -65, -68, 0, 89, -69, -65, -68, 0, 0, 1, 0, -8, 0, 3,
    62, -221, 141, -167, 231, 251, -151, 264, -70, -106, 276, -100, 0, 5, 64,
    -221, 141, -90, 163, 170, 234, 251, -151, 264, -63, -70, 274, 278, -100, 0,
    -64, -65, -68, 0, -28, -47, 141, -33, 172, 239, 251, 264, -70, -106, -102,
    -100, 0, 8, 68, -245, -215, 141, -84, 163, 174, -250, 234, 251, -151, -243,
    264, -70, -98, 274, 278, -100, 0, 13, 0, 3, 62, -221, 141, -167, 231,
    251, -151, 264, -70, -106, 276, -100, 0, 9, 58,

    60, 68, 116, 118, 141, 143, 163, 176, 229, -111, 241, 251, -151, 253, 255,
    257, 259, 264, 266, -70, 274, 278, -100, 0, 5, 64, 121, 141, -89, 163,
    -36, 182, 234, 251, -151, 264, -70, 274, 278, -100, 283, 0, 18, 64, -220,
    141, 147, 163, 187, 234, 251, -151, 264, -70, -141, -137, 274, 278, -100, 0,
    0, -49, 0, 5, 64, -221, 141, -90, 163, -197, 234, 251, -151, 264, -70,
    274, 278, -100, 0, 23, 64, -220, 141, 147, 163, 189, 234, 251, -151, 264,
    -70, 274, 278, -100, 0, 9, 68, -245, 123, 141,

    149, 163, -35, 191, -251, 234, 251, -151, -243, 264, -70, 274, 278, -100, 0,
    5, 64, 132, 141, -96, 163, -202, 234, 251, -151, 264, -70, 274, 278, -100,
    0, 0, -50, -114, 0, 3, 62, -221, 141, -167, 231, 251, -151, 264, -70,
    -106, 276, -100, 0, -99, 0, -16, 0, -28, -47, -148, 141, -32, -161, 239,
    251, 264, -70, -106, -102, -100, 0, 27, 73, 141, -74, 163, -164, 247, 251,
    264, -70, 274, 279, -100, 0, 0, 102, -69, -65, -68, 0, 5, 64, -223,
    141, 155, 163, 200, 234, 251, -151, 264, -70, 274,

    278, -100, 0, 0, 30, 76, -245, 134, 141, 157, 163, 165, 202, -247, 234,
    251, -151, -243, -54, 264, 269, -70, 274, 278, -100, -139, 0, 3, 62, -219,
    141, -166, 231, 251, -151, 264, -70, -106, 276, -100, 0, 38, 64, -219, 141,
    -92, 163, 209, 234, 251, -151, 264, -70, 274, 278, -100, 0, 3, 62, -219,
    141, -166, 231, 251, -151, 264, -70, -106, 276, -100, 0, 42, -238, 82, 137,
    141, 159, 163, 168, 211, 241, 251, -151, -51, -152, 262, 264, 271, -70, 274,
    278, -100, 281, -139, 0, 23, 64, -219, 141, -92,

    163, 217, 234, 251, -151, 264, -70, 274, 278, -100, 0, 3, 62, -219, 141,
    -166, 231, 251, -151, 264, -70, -106, 276, -100, 0, 5, 64, -222, 141, 161,
    163, -170, 234, 251, -151, 264, -70, 274, 278, -100, 0, 89, -69, -65, -68,
    0, 5, 64, -222, 141, 161, 163, -170, 234, 251, -151, 264, -70, 274, 278,
    -100, 0, 5, 64, -219, 141, -92, 163, -179, 234, 251, -151, 264, -70, 274,
    278, -100, 0, 48, 64, -219, 141, -92, 163, 219, 234, 251, -151, 264, -70,
    274, 278, -100, 0, 0, 109, -69, -65, -68, 0,

    5, 64, -223, 141, 155, 163, 221, 234, 251, -151, 264, -70, 274, 278, -100,
    0, 5, 64, -216, 141, -76, 163, -160, 234, 251, -151, 264, -70, 274, 278,
    -100, 0, 52, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 64, -221,
    141, -90, 163, -178, 234, 251, -151, 264, -70, 274, 278, -100, 0, 0, 5,
    64, -221, 141, -90, 163, 223, 234, 251, -151, 264, -70, 274, 278, -100, 0,
    -67, -65, -68, 0, 0, 5, 64, -227, 141, -95, 163, -202, 234, 251, -151,
    264, -70, 274, 278, -100, 0, 0, 0, 9, -133,

    68, -149, -245, -212, 141, -97, 163, 225, -248, -112, 234, 251, -151, -243, -52,
    -53, 264, -61, -70, 274, 278, -100, 0, 5, -237, -104, -252, -242, 64, -150,
    139, 141, -73, 163, -34, 227, 241, 251, -151, -51, -152, -55, 264, 272, -70,
    -241, 274, -155, 278, -100, -253, -154, 0, 9, -133, 68, -245, -212, 141, -97,
    163, -115, 225, -248, -113, 234, 251, -151, -243, -52, -53, 264, -61, -70, 274,
    278, -100, 0, 56, 87, 141, -142, -165, 239, 251, -54, 264, 269, -70, -106,
    -102, -100, -139, 0, -18, 0, -28, -47, 141, -158,

    239, 251, -57, 264, -62, -61, -70, -106, -102, -100, 0};

/*********************************************************
 * sub_13D	Ok++ PMO		Used in: sub_1F4B
 * This function wasn't optimised in original code hence
 * it is split out separately
 *********************************************************/
int sub_13D(int a, int b)
{
    int i;

    for (i = array_987D[b]; array_990D[i] != 0 && array_990D[i] != a; i++)
        ;
    return array_9BD4[i];
}

/* end of file lex1u.c */
/*
 * File - lex2.c
 */

/*********************************************************
 * char sub_1B2 OK++ PMO	       Used in: sub_6D1, sub_4192
 *
 * Token search		(Binary search algorithm is used)
 *********************************************************/
char sub_1B2(register char *target)
{
    uint8_t hi, lo, mid;
    char cmp;

    /* clang-format off */
    static char *tnames[] = {
        "",    "!",  "!=",  "#",  "$",  "$U", "%",  "&",  "&&", "&U",
        "(",   ")",  "*",   "*U", "+",  "++", "+U", ",",  "-",  "--",
        "->",  "-U", ".",   "..", "/",  ":",  ":U", ":s", ";",  ";;",
        "<",   "<<", "<=",  "=",  "=%", "=&", "=*", "=+", "=-", "=/",
        "=<<", "==", "=>>", "=^", "=|", ">",  ">=", ">>", "?",  "@",
        "[\\", "[a", "[c",  "[e", "[i", "[s", "[u", "[v", "^",  "{",
        "|",   "||", "}",   "~", "RECIP", "TYPE",
        "ID", "CONST", "FCONST", "REG", "INAREG", "BITFIELD"
    };
    /* clang-format on */

    static char lastTok = 65;

    lo = 0;
    hi = lastTok;
    do
    {
        mid = (hi + lo) / 2;
        if ((cmp = strcmp(target, tnames[mid])) == 0)
            return mid;
        if (cmp < 0)
            hi = mid - 1;
        else
            lo = mid + 1;
    } while (hi >= lo);
    return (-1); /* Search terminates as unsuccessful */
}

/*********************************************************
 * gethashptr OK++ PMO		Used in: sub_265, sub_1754
 *
 * Convert name to a hash table ptr (sub_21F)
 *
 * Returns pointer to pointer to structure associated with
 * pointer to token.  for example
 *
 * Input	Hash key  Destination
 * string  	dec hex
 *   "f"  	 1	- float
 *   "i"  	 4	- int
 *   "l"  	 7	- long
 *   "s"  	14 0eh	- short
 *   "v"  	17 11h	- void
 *   "x"  	19 13h	- x ?
 *   "uc" 	30 1eh	- uint8_t
 *   "ui" 	36 24h	- uint16_t
 *   "ul" 	39 27h	- uint32_t
 *   "us" 	46 2eh	- uint16_t short
 *   "ux" 	51 33h	- uint16_t ?
 *   "b"  	98 62h	- b ?
 *   "c"  	99 63h	- char
 **********************************************************/

#define HASHTABSIZE 101

member_t **gethashptr(register char *str)
{
    uint16_t key;

    /* calculate hash */
    for (key = 0; *str; str++)
        key = key * 2 + *(uint8_t *)str;

    /* Hash table index is determined by    */
    /* hash function using division method  */
    return &hashtab[key % HASHTABSIZE];
}

/*********************************************************
 * sub_265 OK++ PMO	     Used in: sub_627,  sub_B19,  sub_E43,
 *			      sub_1680, sub_1754, sub_4192
 *
 * Return pointer to struct associated with pointer to token
 *********************************************************/
member_t *sub_265(char *str)
{
    member_t **l1b;
    register member_t *sb;

    /* In hash table, we look for pointer to    */
    l1b = gethashptr(str); /* entry in symbol table associated with    */
    /* scan for the matching name on the hash list */
    for (sb = *l1b; sb; sb = sb->b_next)
        if (strcmp(sb->b_name, str) == 0)
            return sb;
    /* not found so create new record & insert at head of list */
    sb = allocMem(sizeof(member_t)); /* 22 bytes */
    sb->b_next = *l1b;
    sb->b_name = allocMem(strlen(str) + 1); /* effectively strdup */
    strcpy(sb->b_name, str);
    sb->b_ndpth = (uint8_t)nstdpth;
    *l1b = sb;

    return sb; /* position and return pointer to record.   */
}

/*********************************************************
 * sub_306 v1 OK++ PMO    Used in: sub_6D1,  sub_793,  sub_B19,
 *			      sub_E43,  sub_17E0, sub_19C1,
 *			      sub_4192
 * get_token
 * Optimiser one basic blocked moved between original code
 *  and new code. Behaviour unchanged
 *********************************************************/
char *getToken()
{
    int ch, expectName;
    register char *ptr;
    static char buffer[MAXBUF];

    expectName = false;
    for (;;)
    {
        ptr = buffer;

        while ((ch = fgetchar()) != EOF && isspace(ch))
            ;
        if (ch == EOF)
            return (char *)~0;
        *ptr++ = ch;
        do
        {
            *ptr++ = ch = fgetchar();
        } while (ch != EOF && !isspace(ch));
        *--ptr = 0;
        if (buffer[0] == '"')
        {
            lineno = atoi(buffer + 1);
            expectName = ch != '\n';
        }
        else if (expectName)
        {
/* GCC moans about truncating the string, which is what we actually want */
#ifdef __GNUC__
#pragma GCC diagnostic push
/* #pragma GCC diagnostic ignored "-Wstringop-truncation" */
#endif
            strncpy(progname, buffer, sizeof(progname) - 1);
            progname[sizeof(progname) - 1] = '\0';
#ifdef __GNUC__
#pragma GCC diagnostic pop
#endif
            expectName = false;
        }
        else if (buffer[0] == ';' && buffer[1] == ';')
        {
            do
            {
                fputchar(ch = fgetchar());
            } while (ch != '\n');
        }
        else
            return buffer;
    }
}

/*********************************************************
 * sub_406 v3 OK++ PMO			 Used in: sub_6D1
 *
 * Notes:
 *   Added 0 parameter to sub_5CF5 function call to
 *   match number and types actual and formal parameters
 *   this function.
 *   Modified code to get sb->b_next before freeing sb
 *   Other than code changes to handle the above, the code is identical
 *********************************************************/
/* remove and free up items which have got out of scope */

void leaveBlock()
{
    member_t **pHashEntry;
    int l2;
    member_t *newlist;
    member_t *next;
    member_t **l5;
    int l6;
    register member_t *sb;

    l2 = HASHTABSIZE;
    pHashEntry = hashtab;
    do
    {
        newlist = 0;
        /* note with the fixed handling of free, this could be converted to a for loop */
        sb = *pHashEntry;
        while (sb && sb->b_ndpth == nstdpth)
        {
            if (sb->b_class == VAR)
            {
                if (sb->b_flag == 2)
                    word_B017 |= array_AAE8[sb->b_memb.i];
                if (sb->b_flag == 4 || sb->b_flag == 3)
                    sub_5CF5(sb, 0); /* Add parameter 0! */
                if (0 < nstdpth && sb->b_flag == 3)
                {
                    next = sb->b_next;
                    sb->b_next = newlist;
                    newlist = sb;
                    sb->b_ndpth--;
                    sb = next;
                    continue;
                }
            }

            free(sb->b_name);

            if (sb->b_class == UNION || sb->b_class == STRUCT)
            {
                l6 = sb->b_memb.mlist->cnt;
                l5 = sb->b_memb.mlist->vals;
                while (l6-- != 0)
                    free(*l5++);
                free(sb->b_memb.mlist);
            }
            /*  code modified to get sb->b_next before freeing sb !!!*/
            next = sb->b_next;
            free(sb);
            /* sb = sb->b_next original code relied on b_next not being at start of block !!!*/
            sb = next;
        }
        if (newlist)
        {                              /* if l3b list exists add to end */
            *(pHashEntry++) = newlist; /* set hash table to the l3b list */
            while (newlist->b_next)
                newlist = newlist->b_next; /* m14: */
            newlist->b_next = sb;
        }
        else
            *(pHashEntry++) = sb; /* else initialise hash table entry with new item  */
    } while (--l2 != 0);

    nstdpth--;
    if (lvlidx >= 0)
    {
        if (array_AE57[lvlidx] == nstdpth)
        {
            sb = array_AEDB[lvlidx];
            if (sb->b_type->b_class == STRUCT && (sb->b_refl & 4) == 0)
                sub_BEE(sb->b_off, sb->b_size);
            prFrameTail(sb->b_off, array_AE13[lvlidx]);
            lvlidx--;
            word_B017 = 0x17F;
        }
    }
}

/*********************************************************
 * sub_627 OK++ PMO     Used in: sub_17E0, sub_19C1, sub_4192
 *********************************************************/
/* encode the accessor prefix */
member_t *sub_627(register char *ch, uint16_t *par)
{
    uint16_t loc;

    loc = 0;
    ch++;
    do
    {
        loc <<= 2;
        if (*ch == '(')
        {
            loc |= 2;
            ch++;
        }
        else if (*ch == '*')
        {
            loc |= 1;
            ch++;
        }
    } while (!isalnum(*ch) && *ch != '_');
    /* invert them */
    *par = 0;
    while (loc != 0)
    {
        *par <<= 2;
        *par |= (loc & 3);
        loc >>= 2;
    }
    return sub_265(ch);
}

/*********************************************************
 * sub_6C8 OK++ PMO			Used in: sub_6D1, expect
 *********************************************************/
void badIntCode()
{

    fatalErr("Bad int. code");
}

/*********************************************************
 * sub_6D1 OK++				    Used in: main
 *
 * Compiling intermediate code
 *
 * note optimiser uses hl in jmp table calculations
 * then exchanges with de. The original code uses de
 * the effect is the same and the rest of the code is identical
 *********************************************************/
void parseStmt()
{
    register char *ch;
    int tok;

    word_B017 = 0x17F;
    while ((ch = getToken()) != (char *)~0)
    { /* get_token    */
        tok = sub_1B2(ch);
        switch (tok)
        {              /* search_token */
        case LBRACE:   /* "{" opening block */
            nstdpth++; /* Increase the current nesting depth */
            break;
        case RBRACE: /* "}" end of block 	*/
            leaveBlock();
            break;
        case EXPR: /* "[e" expression	*/
            prPsect(P_TEXT);
            sub_3DC9(parseExpr());
            expect(']');
            break;
        case VAR: /* "[v" variable	*/
            parseVariable();
            break;
        case STRUCT: /* "[s" struct	*/
        case UNION:  /* "[u" union	*/
            parseMembers(tok);
            break;
        case CASE: /* "[\\" */
            parseSwitch();
            break;
        case ENUM: /* "[c" enum		*/
            parseEnum();
            break;
        case INIT: /* "[i" initialization	*/
            parseInit();
            break;
        case UNKNOWN: /* "[a" 		*/
            parseData();
            break;
        case BXOR: /* "^" */
        case BOR:  /* "|" */
        case LOR:  /* "||" */
        default:
            badIntCode();
        }
    }
    leaveBlock();
}

/*********************************************************
 * sub_76F OK	      Used in: sub_6D1,  sub_B19, sub_E43,
 * 			       sub_17E0, sub_1B0C
 ;
 * Search for expected character (closing square bracket)
 *********************************************************/
void expect(char par)
{
    char ch;

    do
    {
        ch = fgetchar();
    } while (isspace(ch)); /* Skip white-space characters */

    if (ch != par)
        badIntCode();
}

/*********************************************************
 * sub_793 OK++ 			 Used in: sub_6D1
 * minor variance in code as sub_43EF the first parameter
 * is explicitly declared uint8_t, rather than the default int
 *********************************************************/
void parseData()
{
    char *loc1;
    char buf[1024];
    register char *ptr;

    prPsect(P_DATA);
    sub_2BD0(sub_600E(sub_43EF(COLON_U, sub_43EF(COLON_S, parseExpr(), 0), 0)));
    ptr = buf;
    for (;;)
    {
        if (*(loc1 = getToken()) == ']') /* parse non-terminating tokens */
            break;
        *ptr = atoi(loc1); /* Convert string to int value and place it in buffer */
        ptr++;
    }

    prDefb(buf, (int)(ptr - buf)); /* Emit data "defb byte1, ..." ("ptr-buf" bytes from "buf")*/
}
