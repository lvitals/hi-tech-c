#include "cgen.h"

/*********************************************************
 * sub_808 OK++ PMO				 Used in: sub_B19
 * code identical except for where the original code had
 * l = l4b->bOffset immediately followed
 * by hl = 0. The useless l = l4b->b-b6 code is no longer there
 * Note there appears to be a problem with hitech code generation
 * around << of a long value
 * if the rhs is uint8_t then the calculation is done incorrectly
 *
 *********************************************************/

int sub_808(register member_t *sb, node_t *p2a)
{
    int16_t l1;
    int16_t l2;
    member_t *l3b;
    member_t *l4b;
    int l5;
    node_t *l6a FORCEINIT; /* */

    l1 = 0;
    if (p2a->a_c0 == COLON_U)
    {
        sub_475C(p2a);
        l5 = 0;
        if (sb->b_class == STRUCT)
        {
            l3b = sb;
            while ((p2a = parseExpr())->a_c0 != DOT_DOT)
            {
                if ((l2 = sub_1CC4(l4b = l3b->b_memb.mlist->vals[l5++])) != 0 && l1 % ++l2 != 0)
                {
                    l2 -= l1 % l2;
                    l1 += l2;
                    prDefb0s(l2); /* emit defb 0 to pad to item boundary */
                }
                if (l4b->b_sloc & 0x10)
                {
                    p2a = sub_43EF(BAND, p2a, sub_415E((1L << (int8_t)(l4b->bWidth)) - 1L));
                    if (l4b->bOffset != 0)
                        /* PMO: bug fix, replaced 0L arg to sub_415E with bit offset l4b->bOffset*/
                        l6a = sub_43EF(BOR, l6a, sub_43EF(LSHIFT, p2a, sub_415E(l4b->bOffset)));
                    else
                        l6a = p2a;

                    if (l5 != l3b->b_memb.mlist->cnt &&
                        (l3b->b_memb.mlist->vals[l5]->b_sloc & 0x10) &&
                        l3b->b_memb.mlist->vals[l5]->b_off == l4b->b_off)
                        continue;
                    p2a = l6a;
                }
                l1 += sub_808(l4b, p2a);
            }

            sub_475C(p2a);
        }
        else
        {
            l3b = sb->b_type;
            for (;;)
            {
                if ((p2a = parseExpr())->a_c0 == COLON_U)
                    l1 += sub_808(l3b, p2a);
                else if (p2a->a_c0 == DOT_DOT)
                {
                    sub_475C(p2a);
                    break;
                }
                else
                {
                    l1 += nodesize(p2a);
                    sub_2BD0(sub_600E(sub_43EF(ATGIGN, p2a, 0)));
                }
            }
        }
    }
    else
    {
        p2a = sub_600E(sub_43EF(ATGIGN, p2a, 0));
        l1 = nodesize(p2a);
        sub_2BD0(p2a);
    }
    if (l1 < sb->b_size)
    {
        prDefb0s(sb->b_size - l1);
        l1 = sb->b_size;
    }
    else if (sb->b_size < l1 && sb->b_size != 0)
        prError("Too many initializers");

    return l1; /* m15: */
}

/*********************************************************
 * sub_B19 OK++	PMO			 Used in: sub_6D1
 *********************************************************/
void parseInit()
{
    register member_t *sb;
    int l1;
    int l2;

    sb = sub_265(getToken());
    prPsect(P_DATA);
    sub_CAB(sb); /* Emit "symbolic_name:" (identifier label) */
    l1 = sub_808(sb, parseExpr());
    l2 = sb->b_refl ? 2 : sb->b_type->b_size;
    if (sb->b_nelem == 0)
    { /* make sure nelem is set */
        sb->b_nelem = l1 / l2;
        sb->b_size = varSize(sb);
    }
    expect(']');
}

/*********************************************************
 * sub_B96 OK++ PMO				Used in: sub_2BD0
 * emit_call_ncsv
 *********************************************************/
void prFrameHead(int fId)
{
    static bool frameGlobalsEmitted; /* First call ncsv	   */

    prPsect(P_TEXT);
    if (frameGlobalsEmitted == false) /* if first call	*/
        printf("global\tncsv, cret, indir\n");

    printf("call\tncsv\n");
    frameGlobalsEmitted = true; /* first call is done	*/
    printf("defw\tf%d\n", fId);
}

/*********************************************************
 * sub_BCC OK++ PMO				 Used in: leaveBlock
 *********************************************************/
void prFrameTail(int fId, int fSize)
{

    prPsect(P_TEXT);
    printf("jp\tcret\n"
           "f%d\tequ\t%d\n",
           fId, fSize);
}

/*********************************************************
 * sub_BEE OK++ PMO				 Used in: leaveBlock
 *********************************************************/
void sub_BEE(int kId, int size)
{

    prPsect(P_TEXT);
    printf("ld\tde,k%d\n"
           "ld\tbc,%d\n"
           "ldir\n"
           "ld\thl,k%d\n",
           kId, size, kId);
    prPsect(P_BSS);
    printf("k%d:defs\t%d\n", kId, size);
}

/*********************************************************
 * sub_C36 OK++ PMO       Used in: sub_C57, sub_CAB, sub_4192
 *********************************************************/
void prGlobalDef(register member_t *sb)
{

    printf("global\t%s\n", sb->b_name);
    sb->b_sloc |= 2;
}

/*********************************************************
 * sub_C57 OK++ PMO				Used in: sub_5CF5
 *********************************************************/
void sub_C57(register member_t *sb)
{

    if ((sb->b_sloc & 1) == 0 && sb->b_nelem != 0 && (sb->b_refl & 2) == 0)
    {
        prPsect(P_BSS);
        if (sb->b_flag == 3 && (sb->b_sloc & 2) == 0)
            prGlobalDef(sb); /* Emit "global name" */
        sub_CAB(sb);         /* Emit "symbolic_name:" (identifier label) */
        printf("\tdefs\t%u\n", sb->b_size);
    }
}

/*********************************************************
 * sub_CAB OK++ PMO			Used in: sub_B19, sub_C57
 *
 * Emit "symbolic_name:" (identifier label)
 *********************************************************/
void sub_CAB(register member_t *sb)
{

    if ((sb->b_sloc & 2) == 0 && sb->b_flag == 3)
        prGlobalDef(sb); /* Emit "global name" */
    sb->b_sloc |= 1;
    printf("%s:\n", sb->b_name);
}

/*********************************************************
 * sub_CDF OK++ PMO				 Used in: sub_808
 * Emit "defb 0, ..." (num bytes)
 *********************************************************/
void prDefb0s(int num)
{
    char cnt;

    cnt = 0; /* Reset counter bytes printed	   */
    while (num-- != 0)
    { /* While data is available	   */
        if (cnt == 0)
            printf("defb\t0"); /* Initially output "defb 0",	   */
        else
            printf(",0"); /* later ",0"			   */
        cnt++;            /* and update number bytes output  */
        if (cnt == 16)
        {                   /* If number bytes in string is 16 */
            cnt = 0;        /* Reset counter and		   */
            fputchar('\n'); /* continue output in next line	   */
        }
    } /* continue processing		   */
    if (cnt != 0)
        fputchar('\n'); /* If line is incomplete, new line */
}

/*********************************************************
 * prPsect OK++ PMO     Used in: sub_6D1,  sub_793,  sub_B19,
 * 			      prFrameHead,  prFrameTail,  sub_BEE,
 * Select psect		      sub_C57,  sub_E43
 *********************************************************/
void prPsect(int section)
{
    static int curPsect;
    static char *psectNames[] = {"", "bss", "text", "data"};

    if (section != curPsect) /* Only when changing section */
        printf("psect\t%s\n", psectNames[curPsect = section]);
}

/*********************************************************
 * sub_D66 OK++ PMO				 Used in: sub_E43
 *
 *********************************************************/
void sortCaseLabels(int *pCase, int *pLabel, int nCase)
{
    bool changed;
    int *pl;
    int cnt;
    int tCase;
    int tLabel;
    register int *pc;

    do
    {
        changed = false;
        pc = pCase;
        pl = pLabel;
        for (cnt = nCase; --cnt > 0; pc++, pl++)
        {
            if (pc[1] < pc[0])
            {
                changed = true;
                tCase = pc[0];
                pc[0] = pc[1];
                pc[1] = tCase;
                tLabel = pl[0];
                pl[0] = pl[1];
                pl[1] = tLabel;
            }
            else if (pc[0] == pc[1])
            {
                prError("Duplicate case label");
                return;
            }
        }
    } while (changed != 0);
    return;
}

/*********************************************************
 * sub_E43 OK++	PMO		 Used in: sub_6D1
 * Only minor code differences due to the declaration of
 * functions with char / uint8_t parameters rather than
 * defaulting to int.
 * One other change is where the compiler saves l to the high
 * and low byte, but as the optimiser detected that h and l
 * were both 0 this is not a problem
 *********************************************************/
void parseSwitch()
{
    node_t *l1a;
    int codeLabel, swTableLabel, caseRange, caseCnt, defaultCodeLabel;
    int16_t l7, l8;
    bool loTest, hiTest;
    int caseVals[255], codeLabels[255];
    register node_t *sa;

    l1a = parseExpr();
    caseCnt = 0;
    for (;;)
    {
        sa = sub_600E(parseExpr());
        codeLabel = atoi(getToken());
        if (sa->a_c0 == DOT_DOT)
        { /* end of switch block */
            defaultCodeLabel = codeLabel;
            freeNode(sa);
            expect(']');
            if (caseCnt != 0)
                break;
            prWarning("No case\tlabels");
            sub_475C(l1a);
            printf("jp\tl%d\n", defaultCodeLabel);
            return;
        }
        if (sa->a_c0 == CONST)
        { /* only constants allowed */
            caseVals[caseCnt] = sa->info.l;
            codeLabels[caseCnt++] = codeLabel;
        }
        else
            prError("Non-constant case label");
        sub_475C(sa);
    }
    sortCaseLabels(caseVals, codeLabels, caseCnt); /* m6:  */
    caseRange = caseVals[caseCnt - 1] - caseVals[0];
    if (0 <= caseRange && caseRange < 16000 &&
        caseRange * 2 + 20 < caseCnt * 5)
    { /* if jmp table is shorter */
        /* jump table option is smaller so use it
           note the alternative cmp/jp option may be slower
           even if it is shorter
        */
        sa = allocNode();
        sa->a_c0 = TYPE;
        sa->pm = l1a->pm;
        sa->a_i15 = l1a->a_i15;
        sa = sub_43EF(CONV, sub_415E(caseVals[0]), sa);
        sa = sub_43EF(SUB, l1a, sa);
        if (nodesize(sa) != 2)
        {
            l1a = allocNode();
            l1a->a_c0 = TYPE;
            l1a->pm = sub_265("us");
            sa = sub_43EF(CONV, sa, l1a);
        }
        prPsect(P_TEXT);
        sub_3DC9(sub_43EF(RPAREN, sa, 0));
        swTableLabel = newLocal(); /* swTableCnt++ */
        /*
            with the switch value in hl, the code
            emitted is effectively
            ld  a,.high. caseRange
            cp  h
            jp  c,l{defaultCodeLabel}
            jp  nz,1f
            ld  a,.low. caseRange
            cp  l
            jp  c,l{defaultCodeLabel}
         1: add hl,hl
            ld  de,S{swTableLabel}
            add hl,de
            ld  a,(hl)
            inc hl
            ld  h,(hl)
            ld  l,a
            jp  (hl)
            where {xxx} indicates the respective label number of xxx
        */
        printf("ld\ta,%u\n"
               "cp\th\n"
               "jp\tc,l%d\n"
               "jp\tnz,1f\n"
               "ld\ta,%u\n"
               "cp\tl\n"
               "jp\tc,l%d\n"
               "1:add\thl,hl\n"
               "ld\tde,S%u\n"
               "add\thl,de\n"
               "ld\ta,(hl)\n"
               "inc\thl\n"
               "ld\th,(hl)\n"
               "ld\tl,a\n"
               "jp\t(hl)\n",
               (unsigned)(caseRange >> 8), defaultCodeLabel, 
               (unsigned)(caseRange & 0xff), defaultCodeLabel, 
               (unsigned)swTableLabel);
        prPsect(P_DATA);
        printf("S%d:\n", swTableLabel);
        codeLabel = 0;
        caseRange = caseVals[0];
        do
        {
            if (caseRange == caseVals[codeLabel])
            {
                printf("defw\tl%d\n", codeLabels[codeLabel]);
                codeLabel++;
            }
            else
                printf("defw\tl%d\n", defaultCodeLabel); /* fill holes with default */

            caseRange++;
        } while (codeLabel < caseCnt);
        return;
    }
    if (nodesize(l1a) == 2)
    {
        prPsect(P_TEXT);
        sub_3DC9(sub_43EF(RPAREN, l1a, 0));
        l7 = -1;
        l8 = (int)(loTest = hiTest = false);

        for (codeLabel = 0; codeLabel < caseCnt; codeLabel++)
        {
            l7 &= caseVals[codeLabel];
            l8 |= caseVals[codeLabel];
            if (l7 >> 8 != l8 >> 8 && (l7 & 0xFF) != (l8 & 0xFF))
            {
                if (hiTest | loTest)
                    printf("1:\n"); /* target of previous hi/lo test */
                loTest = hiTest = false;
                l7 = l8 = caseVals[codeLabel];
            }
            if (l7 >> 8 == l8 >> 8)
            {
                if (!hiTest)
                {
                    printf("ld\ta,h\n");
                    sub_1420((int)(l7 >> 8));
                    printf("jp\tnz,1f\n"
                           "ld\ta,l\n");
                    hiTest = true;
                }
                caseVals[codeLabel] &= 0xFF;
            }
            else
            {
                if (!loTest)
                { /* m16: */
                    if (hiTest)
                        printf("1:\n");
                    printf("ld\ta,l\n"); /* m17: */
                    sub_1420((int)(l7 & 0xFF));
                    printf("jp\tnz,1f\n"
                           "ld\ta,h\n");
                    loTest = true;
                }
                caseVals[codeLabel] >>= 8;
                caseVals[codeLabel] &= 0xFF;
            }
            sub_1420((int)caseVals[codeLabel]); /* m19: */
            printf("jp\tz,l%d\n", codeLabels[codeLabel]);
        }
        if (hiTest | loTest)
            printf("1:\n");
        printf("jp\tl%d\n", defaultCodeLabel); /* m21: */
        return;
    }

    prPsect(P_TEXT);
    sub_3DC9(sub_43EF(INAREG, l1a, 0));
    for (codeLabel = 0; codeLabel < caseCnt; codeLabel++)
    {
        if (caseVals[codeLabel] < 256 && caseVals[codeLabel] >= -128)
        {
            sub_1420(caseVals[codeLabel]);
            printf("jp\tz,l%d\n", codeLabels[codeLabel]);
        }
    }
    printf("jp\tl%d\n", defaultCodeLabel);
}

/*********************************************************
 * sub_1420 OK++ PMO			 Used in: sub_E43
 *********************************************************/
void sub_1420(int par)
{

    printf((uint16_t)par ? "cp\t%d\n" : "or\ta\n", par);
}

/*********************************************************
 * sub_143F OK++ PMO			Used in: sub_35E6
 *********************************************************/
void prPush(uint8_t reg)
{

    if (reg == REG_DEHL)
    {
        printf("push\thl\n"
               "push\tde\n");
        return;
    }
    if (reg <= REG_H)
        reg = reg / 2 + REG_AF;
    printf("push\t%s\n", regNames[reg]);
}

/*********************************************************
 * sub_1489 OK++ PMO			Used in: sub_35E6
 *********************************************************/
void prPop(uint8_t reg)
{

    if (reg == REG_DEHL)
    {
        printf("pop\tde\n"
               "pop\thl\n");
        return;
    }
    if (reg <= REG_H)
        reg = reg / 2 + REG_AF;
    printf("pop\t%s\n", regNames[reg]);
}

/*********************************************************
 * sub_14D3 OK++ PMO			Used in: sub_1EDF
 *
 * Assigning register "IY" value formal parameter with
 * type register
 *********************************************************/
void prIXnPush(register member_t *sb)
{

    printf("ld\tl,(ix+%d)\n"
           "ld\th,(ix+%d)\n"
           "push\thl\n"
           "pop\tiy\n",
           sb->b_off, sb->b_off + 1);
}

/*********************************************************
 * sub_14F3 OK++ PMO   Used in: sub_153A, sub_2D09, sub_39CA,
 *			    sub_3A79, sub_3EAA, sub_47B2,
 *			    sub_4FCE, sub_508A, sub_54B6,
 *			    sub_60A8, sub_6246, sub_628F
 *********************************************************/
uint8_t sub_14F3(register node_t *sa)
{

    if (sa->a_i15 & 2)
        return 0;
    if (sa->a_i15 != 0)
        return 2;
    if (sa->pm->b_nelem > 1)
        return 0;
    return sa->pm->b_flag;
}

/*********************************************************
 * sub_153A OK++ PMO			Used in: sub_2D09
 *********************************************************/
void sub_153A(register node_t *sa)
{

    static char array_A542[] = {0, 'a', 'l', 'f'};

    fputchar(array_A542[sub_14F3(sa)]);
}

/*********************************************************
 * sub_155D OK++ PMO			 Used in: sub_793
 *
 * Emit "defb byte1, ..." (from ptr num bytes)
 *********************************************************/
void prDefb(register char *ptr, int num)
{
    char cnt;

    cnt = 0; /* Reset counter bytes printed	   */
    while (num-- != 0)
    { /* While data is available	   */
        if (cnt == 0)
            printf("defb\t"); /* Initially output "defb",	   */
        else
            fputchar(',');             /* later "," 			   */
        printf("%d", (uint8_t)*ptr++); /* Output byte and advance pointer */
        cnt++;                         /* update number bytes output 	   */
        if (cnt == 16)
        { /* max 16 bytes per line */
            fputchar('\n');
            cnt = 0;
        }
    }
    if (cnt != 0)
        fputchar('\n'); /* If line is incomplete, new line */
}

/*********************************************************
 * xx1	 OK++ PMO		Used in: Explicit calls are absent
 *********************************************************/
void prJmpLabel(int p)
{
    printf("j%d:\n", p);
}

/*********************************************************
 * xx2	 OK++ PMO		Used in: Explicit calls are absent
 *********************************************************/
void prJump(int p)
{
    printf("jp\tj%d\n", p);
}

/*********************************************************
 * sub_15FB OK++ PMO			Used in: sub_1B0C
 *********************************************************/
void sub_15FB(register member_t *sb, int p2, int p3)
{

    if (p2 >= 0)
    {
        sb->b_flag = 2;
        if (p3 <= 255)
            sb->b_size = 1;
        else
            sb->b_size = 2;
    }
    else
    {
        sb->b_flag = 1;
        if (p3 <= 127 && p2 >= -128)
            sb->b_size = 1;
        else
            sb->b_size = 2;
    }
}

/*********************************************************
 * sub_1659 OK++ PMO			Used in: sub_1CEF
 * Find maximum between two numbers
 *********************************************************/
int max(int num1, int num2)
{
    return (num1 > num2) ? num1 : num2;
}

#define NVARS 14

struct tType
{
    char *t_str;
    int t_size;
    int t_alig;
    char t_flag;
};

/*********************************************************
 * sub_1680 OK++ PMO			    Used in: main
 *
 * first_init
 *********************************************************/
void sub_1680()
{
    member_t *sb;
    int16_t cnt;
    register struct tType *tp;

    /*
     *	Initializaion of type pointers
     */
    static struct tType vars[NVARS] = {/* sub_1680 */
                                       {"i", 2, 0, 1},
                                       {"s", 2, 0, 1},
                                       {"c", 1, 0, 1},
                                       {"l", 4, 0, 1},
                                       {"ui", 2, 0, 2},
                                       {"us", 2, 0, 2},
                                       {"uc", 1, 0, 2},
                                       {"ul", 4, 0, 2},
                                       {"f", 4, 0, 3},
                                       {"d", 4, 0, 3},
                                       {"x", 2, 0, 1},
                                       {"ux", 2, 0, 2},
                                       {"b", 0, 0, 0},
                                       {"v", 0, 0, 0}};

    /* Clear hash table */

    blkclr((char *)hashtab, sizeof(hashtab));

    /* Create a hash table of templates for standard types */

    cnt = NVARS;
    tp = vars;
    do
    {
        sb = sub_265(tp->t_str);
        sb->b_class = TYPE;
        sb->b_off = tp->t_alig;
        sb->b_size = tp->t_size;
        sb->b_flag = tp->t_flag;
        tp++;
    } while (--cnt != 0);

    /* Additional patterns for types */

    typeLong = sub_265("l");   /* long	  	*/
    typeDouble = sub_265("d"); /* double 	*/
    typeB = sub_265("b");      /* b   	  	*/
    typeVar = sub_265("v");    /* variable 	*/
    typeChar = sub_265("c");   /* char   	*/
    typeUChar = sub_265("uc"); /* uint8_t  	*/
    typeX = sub_265("x");      /* x      	*/
    lvlidx = -1;
}

/*********************************************************
 * sub_174C OK++ PMO    Used in: sub_E43,  sub_17E0, sub_39CA,
 *			     sub_4192, sub_508A
 *********************************************************/
int newLocal()
{
    static int localLabelCnt;
    return ++localLabelCnt;
}

/*********************************************************
 * sub_1754 OK++ PMO    Used in: sub_17E0, sub_19C1, sub_1B0C
 *********************************************************/
member_t *sub_1754(char *token, uint8_t cls)
{
    member_t **l1b;
    register member_t *sb;

    sb = sub_265(token); /* Set pointer to struct associated with   */
                         /* pointer to token.			   */
    if (sb->b_ndpth != nstdpth)
    {                                             /* If nesting depth not correspond current,*/
        l1b = gethashptr(token);                  /* Get pointer to pointer to struct from   */
        sb = allocMem(sizeof(member_t));          /* hash table. Create new struct and save  */
        sb->b_next = *l1b;                        /* pointer to struct from hash table in    */
                                                  /* struct as associated with it. 	   */
        *l1b = sb;                                /* Save pointer to this struct in current  */
                                                  /* position hash table.			   */
        sb->b_name = allocMem(strlen(token) + 1); /* Get memory address allocated for token  */
                                                  /* and assign it to corresponding member.  */
        strcpy(sb->b_name, token);                /* Copy specified token to this address.   */
        sb->b_ndpth = (uint8_t)nstdpth;           /* Save current nesting depth in struct    */
                                                  /*
                                                        if(++symcnt > dynmem)   dynmem = symcnt;
                                                  */
    }
    sb->b_class = cls;
    return sb;
}
/* end of code.c */
/*
 * File - sym1.c
 */
/*********************************************************
 * sub_17E0 OK++ PMO			Used in: sub_6D1, sub_4192
 *
 * Two related Basic Blocks moved and one optimisation
 * loading a from l rather then sb->b_size
 *********************************************************/
void parseVariable()
{
    node_t *l1a;
    int l2;
    register member_t *sb;

    sb = sub_1754(getToken(), VAR);
    sb->b_type = sub_627(getToken(), &sb->b_refl);
    l1a = sub_600E(parseExpr());
    if (l1a->a_c0 != CONST)
        fatalErr("Bad element count expr");
    sb->b_nelem = (uint16_t)l1a->info.l;
    sub_475C(l1a);
    l2 = *getToken();
    expect(']');

    switch (l2)
    {
    case 'A': /* auto		*/
    case 'a':
        sb->b_flag = 1;
        break;
    case 'R': /* register	*/
    case 'r':
        sb->b_sloc |= 8;
        /* fall through */
    case 'P': /* pointer	*/
    case 'p':
        sb->b_flag = 5;
        break;
    case 'S': /* static 	*/
    case 's':
        sb->b_flag = 4;
        break;
    case 'E': /* extern	*/
    case 'e':
        sb->b_flag = 3;
        break;
    case 'T': /* typedef	*/
    case 't':
        sb->b_class = TYPE;
        if (sb->b_refl != 0)
        {
            sb->b_off = 0;
            sb->b_flag = 2;
        }
        else
        {
            sb->b_off = sb->b_type->b_off;
            sb->b_flag = sb->b_type->b_flag;
        }
        if (sb->b_nelem != 1)
            sb->b_flag = 0;
        break;
    default:
        fatalErr("Bad storage class");
    }
    sb->b_size = varSize(sb);
    if (sb->b_class != TYPE)
    {
        if (sb->b_refl == 0 && sb->b_size == 0 && sb->b_type->b_size == 0)
            prError("Bad dimensions");

        if (sb->b_flag == 5 || sb->b_flag == 1)
            sub_5CF5(sb, l2);
        else
            sb->b_off = newLocal(); /* swTableCnt++ */

        if ((sb->b_refl & 0x2) && sb->b_nelem != 0)
        {
            if (++lvlidx == MAXFUN)
                fatalErr("Functions nested too deep");

            array_AE57[lvlidx] = nstdpth; /* Save current nesting depth */
            array_AE97[lvlidx] = 6;
            array_AE13[lvlidx] = 0;
            array_AEDB[lvlidx] = sb;
        }
    }
}

typedef struct
{
    uint16_t cnt;
    member_t *vals[257];
} memberb_t;

typedef struct
{
    uint16_t cnt;
    int16_t vals[257];
} memberi_t;

/*********************************************************
 * sub_19C1 OK++ PMO	       Used in: sub_6D1, sub_4192
 * One minor optimisation as sub_1754 now declared as taking
 * a uint8_t rather than int for last parameter
 *********************************************************/
void parseMembers(int p1)
{

    size_t size;
    member_t *l2b;
    memberb_t *pMember;
    node_t *l4a;
    char *l5;
    memberb_t members;
    register member_t *sb;

    l2b = sub_1754(getToken(), p1);
    pMember = &members;
    pMember->cnt = 0;
    for (;;)
    {
        l5 = getToken();
        if (*l5 == ']')
            break;
        sb = (member_t *)allocMem(sizeof(member_t)); /* Create member_t */
        sb->b_class = MEMBER;
        if (*l5 == ':')
        {
            sb->bWidth = atoi(l5 + 1);
            sb->b_sloc |= 0x10;
            l5 = getToken();
        }
        sb->b_type = sub_627(l5, &sb->b_refl);
        l4a = sub_600E(parseExpr());
        if (l4a->a_c0 != CONST)
            fatalErr("Strucdecl - bad\tnelem");
        sb->b_nelem = (uint16_t)l4a->info.l;
        sub_475C(l4a);
        sb->b_size = varSize(sb);
        pMember->vals[pMember->cnt++] = sb;
    }
    /* note in the size calculation below an extra member_t pointer is reserved
     * in the decompilation I have assumed struct _memb (defined in struct bbb)
     * used a dummy array size of [1] for the vals, this gives the extra pointer
     */
    size = (pMember->cnt - 1) * sizeof(member_t *) + sizeof(struct _memb);
    pMember = allocMem(size);
    bmove(&members, pMember, size);
    l2b->b_memb.mlist = (struct _memb *)pMember;
    sub_1CEF(l2b);
}

/*********************************************************
 * sub_1B0C OK++ PMO		Used in: sub_6D1, sub_4192
 *
 * There are three minor differences of equivalent code
 * 1) optimisation of last parameter passed to sub_1754 as it
 *    is declared a uint8_t
 * 2) Slightly inferior code to calculate (pMember->cnt - 1)
 *    using explicity add -1 rather than dec hl
 * 3) code generated following expect includes popping the
 *    passed in paramater, before jp cret. As cret will
 *    clean up the stack this isn't strictly necessary
 *
 *********************************************************/
void parseEnum()
{
    node_t *l1a;
    memberi_t *pMember;
    int lobnd;
    int hibnd;
    size_t size;
    memberi_t members;
    register member_t *sb;

    lobnd = MININT; /* arithmetic overflow in constant expression */
    hibnd = MAXINT;
    sb = sub_1754(getToken(), ENUM);
    pMember = &members;
    pMember->cnt = 0;
    for (;;)
    {
        l1a = parseExpr();
        if (l1a->a_c0 == DOT_DOT)
        {
            sub_475C(l1a);
            size = (pMember->cnt - 1) * sizeof(uint16_t) + sizeof(struct _memi);
            pMember = allocMem(size);
            bmove(&members, pMember, size);
            sb->b_memb.ilist = (struct _memi *)pMember;
            sub_15FB(sb, hibnd, lobnd);
            expect(']');
            return;
        }
        l1a = sub_600E(l1a);

        pMember->vals[pMember->cnt++] = (uint16_t)l1a->info.l;

        if (lobnd < l1a->info.l)
            lobnd = (int)l1a->info.l;
        if (l1a->info.l < hibnd)
            hibnd = (int)l1a->info.l;
        sub_475C(l1a);
    }
}

/*********************************************************
 * sub_1C6D OK++ PMO			Used in: sub_1CEF
 *********************************************************/
int sub_1C6D(int p1, int p2)
{

    return (p1 + p2) & ~p2;
}

/*********************************************************
 * sub_1C8E OK++ PMO   Used in: sub_B19, sub_17E0, sub_19C1
 *********************************************************/
int varSize(register member_t *sb)
{

    if (sb->b_refl & 1)
        return sb->b_nelem * 2;
    return sb->b_type->b_size * sb->b_nelem;
}

/*********************************************************
 * sub_1CC4 OK++ PMO	       Used in: sub_808, sub_1CEF
 *********************************************************/
int sub_1CC4(register member_t *sb)
{

    if (sb->b_refl != 0)
        return 0;
    return sb->b_type->b_off;
}

/*********************************************************
 * sub_1CEF OK++ PMO			Used in: sub_19C1
 *********************************************************/
void sub_1CEF(register member_t *sb)
{
    int l1;
    int l2;
    int l3;
    member_t *l4b;
    l1 = 0;

    for (l2 = l3 = 0; l2 != sb->b_memb.mlist->cnt; l2++)
    {

        l4b = sb->b_memb.mlist->vals[l2];
        if (sb->b_class == UNION)
        {
            l1 = max(l1, l4b->b_size);
            l4b->b_off = 0;
        }
        else
        {
            l1 = l4b->b_off = sub_1C6D(l1, sub_1CC4(l4b));
            if (l4b->b_sloc & 0x10)
            {
                if (16 < l3 + l4b->bWidth || l4b->bWidth == 0)
                {
                    if (l3 != 0)
                        l4b->b_off = (l1 += 2);
                    l3 = 0;
                }
                l4b->bOffset = (uint8_t)l3;
                l3 += l4b->bWidth;
                if (l3 == 32)
                {
                    l3 = 0;
                    l1 += 2;
                }
            }
            else
            {
                if (l3 != 0)
                {
                    l3 = 0;
                    l4b->b_off = (l1 += 2);
                }
                l1 += l4b->b_size;
            }
        }
    }
    if (l3 != 0)
    {
        l3 = 0;
        l1 += 2;
    }
    sb->b_size = sub_1C6D(l1, 0); /* m10: */
    sb->b_off = 0;
    sb->b_class = STRUCT;
}

/*********************************************************
 * sub_1EDF OK++ PMO			Used in: sub_2BD0
 *********************************************************/
void sub_1EDF()
{
    member_t **l1b;
    register member_t *sb;

    l1b = hashtab;
    do
    {
        for (sb = *l1b; sb != 0 && sb->b_ndpth == nstdpth; sb = sb->b_next)
            if (sb->b_class == VAR && sb->b_flag == 2 && (sb->b_sloc & 4))
                prIXnPush(sb);
    } while (++l1b != &hashtab[HASHTABSIZE]);
}

/* end of file sym1.c */

/*
 * File - sym2s.c
 */

/* macros to make reading the code easier */
/* clang-format off */
/* don't reformat as hitech cpp includes the extra spaces
   which can cause line overflow
*/
#ifndef TOPBIT
#define TOPBIT 0x8000
#define TopBitSet(n) ((n)&TOPBIT)
#define MapVal(n) (TopBitSet(n)?~(n):array_AAE8[n])
#endif
/* clang-format on */
static int array_A162[] = {
    0, 9, -10, 25, -28, 21, 25, -28, 7, 21, 25, 28, -29, 11, 12,
    13, 14, -15, 4, 5, 21, 25, -28, 6, 21, 25, -28, 1, 21, -28,
    17, 20, 21, 22, 25, 26, 28, -29, 3, 21, 25, -28, 4, 5, 20,
    21, 25, -28, 2, 21, 25, -28, 19, 23, 24, -27, 20, -28, 239, -240,
    133, -134, 46, -47, 43, 44, 46, -47, 43, 44, 46, 47, -48, 43, 44,
    -47, 43, 44, 45, 46, 47, -48, 43, 44, 45, 46, -47, 45, -47, 116,
    117, 118, 119, 120, 121, 122, 123, 124, 127, 130,

    131, -132, 117, 118, 120, 125, 128, 131, -132, 117, 118, 120, 126, 129, 131,
    -132, 245, -246, 212, 214, -217, 218, -221, 228, 229, 230, 231, 232, 233, 234,
    235, -236, 211, -227, 224, 225, -226, 214, -219, 213, -214, 135, -136, 71, 72,
    73, -97, 93, -94, 77, 78, 79, 80, 81, -82, 85, -86, 75, -83, 73,
    -91, 87, -88, 30, -31, 145, 146, -147, 143, -144, 175, -178, 161, -207, 185,
    -204, 162, 163, 182, 183, 184, -185, 161, 198, 199, 200, -201, 173, -180, 172,
    -181, 161, 185, 186, 187, 205, 206, 208, 209, -210,

    169, -171, 159, 165, 185, 194, 195, 196, -203, 174, -179, 163, 165, 188, 189,
    191, -193, 190, -192, 176, -179, 168, -171, 177, -178, 162, -185, 161, -163, 248,
    -249, 40, 41, -42, 38, 39, 40, 41, -42, 40, -41, 37, 38, 39, 40,
    41, -42, 38, 39, 40, -41, 109, -110, 243, -244, 51, -52, 152, -153, 53,
    55, -56, 54, -55, 107, -108, 59, 60, -61, 58, -61, 58, 59, -61, 105,
    -106, 101, -102, 101, 102, -103, 156, -157, 138, -140};

/*********************************************************
 * sub_1F4B OK++ PMO	Used in: sub_2BD0
 * one small block of code moved by optimiser otherwise code
 * identical
 * Note needs standalone ported optimiser to optimise
 *********************************************************/
int sub_1F4B(node_t *p1a, int p2, int p3, int p4, int *p5)
{
    int l1, l2, l3, l4, l5, l6;
    uint16_t l7, l8;
    register struct codeFrag_t *sz;
    static int i; /* afed */
    static char *s;
    static char *t;
    static char ch;
    static int m;
    static int n;

    if (p1a->a_c1 == 0)
        p1a->a_c2 = 0;
    if ((l1 = sub_13D(p2, p1a->a_c0)) == 0)
        return (-1);

    if (p4 != 0 && TopBitSet(p4) == 0 && sub_6589(p3, p4) == 0 &&
        (p1a->a_c0 != USEREG || sub_6589(array_AAE8[p1a->info.l], p4) == 0))
        return (-1);

    do
    {
        if (l1 > 0)
            i = l5 = array_A162[l1++];
        else
            i = l5 = l1;

        if (i < 0)
            i = -i;

        sz = &codeFrag[i];

        if (sz->c_2 != 0 && sub_6589(p3, sz->c_2) == 0)
            continue;

        if ((uint8_t)sz->c_3 < 'H' && sz->c_4 != 0 &&
            (sub_13D((uint8_t)sz->c_4, p1a->info.np[0]->a_c0) == 0 ||
             (sz->c_5 != 0 && sub_13D(sz->c_5, p1a->info.np[1]->a_c0) == 0)))
            continue;
        if (sz->c_0 != 0 && sub_47B2(p1a, sz->c_0) == 0)
            continue;

        l6 = sz->c_1;
        l7 = 0;
        if (p4 == 0 && sz->c_3 == 'E')
            p4 = sz->c_1;

        if (p4 != 0 && (!TopBitSet(p4) || l6 == 0 || sub_6589(p4 & 0x7fff, l6) != 0))
        {
            if ((s = sz->p_8) != 0)
            {
                if (s[0] == 'L' && s[1] == 0)
                    s = "GL";
                while (*s)
                {
                    ch = *s;
                    if (ch == 'X' || ch == 'G' || (ch == 'S' && p1a->a_c0 == USEREG))
                    {
                        if (isdigit(*++s))
                            t = s;
                        else
                            t = 0;
                        while (*s < 'A')
                            s++;
                        switch (*s)
                        {
                        case 'L':
                            if ((l6 = sub_66BC(l6, p4, p3, t)) == 0)
                                l6 = -1;
                            break;
                        case 'R':
                            l7 = p4;
                            break;
                        case 'N':
                            if (ch == 'S')
                            {
                                if (p1a->a_c0 == USEREG &&
                                    sub_66BC(l6, p4, array_AAE8[p1a->info.l], t) == 0)
                                    l6 = -1;
                            }
                            else if (sub_63B8(p3, p4, sz->c_2) == 0 &&
                                     (!TopBitSet(p4) || sub_66BC(sz->c_2, p4, p3, t) == 0))
                                l6 = -1;
                            break;
                        default:
                            continue;
                        }
                        break;
                    }
                    else
                        s++;
                }
            }
            else if ((l6 = sub_66BC(l6, p4, p3, 0)) == 0)
                l6 = -1;
        }

        if (l6 == -1)
            continue;
        l4 = 0;
        if ((uint8_t)sz->c_3 >= 'H')
        {
            p1a->a_c1 = 0;
            if (sub_1F4B(p1a, (uint8_t)sz->c_3, p3, l6, &l2) < 0)
            {
                p1a->a_c1 = 0;
                continue;
            }
        }
        else if (sz->c_4 != 0)
        {
            p1a->info.np[0]->a_c1 = 0;
            if (sub_1F4B(p1a->info.np[0], (uint8_t)sz->c_4, p3, l6, &l2) < 0)
                continue;
            if (sz->c_5 != 0)
            {
                p1a->info.np[1]->a_c1 = 0;
                l8 = sub_2B2A(p1a->info.np[0]);
                if (l7 == 0)
                {
                    if (sz->c_2 != 0)
                        l7 = array_AAE8[(uint8_t)sub_63B8(p3, p4, sz->c_2)];
                    else
                        l7 = 0;
                    l7 = (l8 | l7) ? (l8 | l7 | 0x8000) : 0;
                }
                if (sub_1F4B(p1a->info.np[1], sz->c_5, p3, l7, &l3) < 0)
                    continue;
                l4 = sub_2B2A(p1a->info.np[1]);
                if (l4 & l8)
                {
                    p1a->info.np[1]->a_c1 = 0;
                    if (sub_1F4B(p1a->info.np[1], sz->c_5, p3 & ~l8, l7, &l3) <= 0)
                        continue;
                    l4 = sub_2B2A(p1a->info.np[1]);
                }
                if ((dopetab[p1a->a_c0] & 0x100) || (l4 & l2))
                {
                    if (l8 & l3)
                    {
                        if (dopetab[p1a->a_c0] & 0x100)
                            continue;
                        p1a->a_c2 = 2;
                        l4 = 0;
                    }
                    else
                        p1a->a_c2 = 1;
                }
                else
                    p1a->a_c2 = 0;
            }
        }
        else if (l6 != 0)
            p4 = l6;

        if (sz->c_2 != 0)
        {
            m = p3;
            if (!(sz->c_2 & 0x40) || (n = sub_63B8(sub_2B2A(p1a), p4, sz->c_2)) == 0)
            {
                if ((sz->c_2 & 0x40) == 0)
                    m &= ~sub_2B2A(p1a);
                m &= ~l4;
                n = sub_63B8(m, p4, sz->c_2);
                if (n == 0 && (n = sub_6589(m, sz->c_2)) == 0)
                    continue;
            }
            p1a->a_uc9[p1a->a_c1] = (uint8_t)n;
        }
        else
            p1a->a_uc9[p1a->a_c1] = 0;

        p1a->a_c3[p1a->a_c1++] = (uint8_t)(sz - codeFrag);
        n = (uint8_t)sub_2B79(p1a);
        
        if (p1a->a_c1 > 0 && p1a->a_c1 <= sizeof(p1a->a_reg)) {
            p1a->a_reg[p1a->a_c1 - 1] = (uint8_t)n;
        }
        
        if (n == 0 && p1a->a_c0 == USEREG)
            p1a->a_reg[p1a->a_c1 - 1] = n = (uint8_t)p1a->info.l;
        /* clang-format off */
        if ((p4 != 0 && !TopBitSet(p4) && (n == 0 || sub_6589(MapVal(p4), n) != n)) ||
            (TopBitSet(p4) && (MapVal(p4) & MapVal(n)) != MapVal(n))) {
                p1a->a_c1--;
                continue;
            }
        /* clang-format on */
        *p5 = array_AAE8[p1a->a_uc9[p1a->a_c1 - 1]];

        if ((uint8_t)sz->c_3 >= 'H' || sz->c_4 != 0)
            *p5 |= l2;
        if (sz->c_5 != 0)
            *p5 |= l3;
        return 1;
    } while (l5 >= 0);
    return -1;
}

/* end of file sym2s.c */
/*
 * File - sym3.c
 */

/*********************************************************
 * sub_283E OK++ PMO				 Used in: sub_2B2A
 *
 * apart from one minor optimisation difference to save
 * a pop bc, the code is identical
 *********************************************************/
int sub_283E(register node_t *sa, int par)
{
    char *s;
    struct codeFrag_t *l2z;
    node_t *l3a;
    char *t;
    uint8_t ch;
    int l6;
    int l7;

    l2z = &codeFrag[sa->a_c3[par]];
    l7 = 0;

    for (s = l2z->p_8; *s;)
    {
        switch (ch = *(s++))
        {
        case 'X':
        case 'G':
            if (isdigit(*s))
            {
                t = s;
                while (isdigit(*s))
                    s++;
            }
            else
                t = 0;

            switch (*s++)
            {
            case 'N':
                l6 = par;
                l3a = sa;
                break;
            case 'L':
                if (par > 0)
                {
                    l3a = sa;
                    l6 = par - 1;
                }
                else
                {
                    l3a = sa->info.np[0];
                    l6 = l3a->a_c1 - 1;
                }
                break;
            case 'R':
                l3a = sa->info.np[1];
                l6 = l3a->a_c1 - 1;
                break;
            default:
                fprintf(stderr, "%s: line %d - internal error\n", "cgen.c", __LINE__);
                exit(1);
            }
            if (ch == 'S')
            {
                if (l3a->a_c0 == USEREG)
                    ch = (uint8_t)l3a->info.l;
                else
                    ch = 0;
            }
            else
            {
                ch = ch == 'G' ? l3a->a_reg[l6] : l3a->a_uc9[l6];
                if (t)
                    while (isdigit(*t) != 0)
                        ch = array_AB24[ch * 2 + (*t++ - '0')];
            }
            l7 |= array_AAE8[ch];
            if (ch != 0)
                byte_AFFA = (char)ch;
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'F':
        case 'M':
        case 'O':
        case 'P':
        case 'Q':
        case 'S':
        case 'T':
        case 'V':
        case 'W':
        case 'Z':
            while (*s && *s != 'N' && *s != 'L' && *s != 'R')
                s++;
            break;
        case '~':
            while (*s && *s != 'L' && *s != 'R')
                s++;
            break;
        case 'L':
            if (par > 0)
                l7 |= sub_283E(sa, par - 1);
            else
                l7 |= sub_2B2A(sa->info.np[0]);

            break;
        case 'R':
            l7 |= sub_2B2A(sa->info.np[1]);
            break;
        case 'D':
        case 'U':
            break;
        }
    }
    return l7 & word_B017;
}

/*********************************************************
 * sub_2B2A OK++ PMO   Used in: sub_1F4B, sub_283E, sub_2B79
 * optimiser improves on code for sa->a_c1 - 1
 *********************************************************/
int sub_2B2A(register node_t *sa)
{

    if (sa->a_c1 == 0)
    {
        if ((dopetab[sa->a_c0] & 0xC) == 0)
            return 0;
        return sub_2B2A(sa->info.np[0]); /* LOGOP & QUEST */
    }
    return sub_283E(sa, sa->a_c1 - 1);
}

/*********************************************************
 * sub_2B79 OK++ PMO			 Used in: sub_1F4B
 *********************************************************/
uint8_t sub_2B79(register node_t *sa)
{
    uint8_t l1;
    int l2;

    l1 = sa->a_uc9[sa->a_c1 - 1];
    byte_AFFA = 0;
    l2 = sub_2B2A(sa);
    if (l1 != 0 && sub_6589(l2, (int)l1) == l1)
        return l1;
    return (uint8_t)byte_AFFA;
}
/* end of file sym3.c */
/*
 * File - cgen1.c
 */

/*********************************************************
 * sub_2BD0 OK++ PMO      Used in: sub_793, sub_808, sub_3CDF
 *			       sub_3DC9
 *********************************************************/
void sub_2BD0(register node_t *sa)
{
    int tmp;

    sa->a_c1 = 0;

    if (sub_1F4B(sa, 0x48, word_B017, 0, &tmp) <= 0)
        fatalErr("Expression too complicated");

    sub_35E6(sa);
    if (sa->a_c0 == COLON_U)
    {
        if (sa->a_i15 & 2)
        {
            prFrameHead(array_AEDB[lvlidx]->b_off); /* emit_call_ncsv */
            sub_1EDF();
        }
        if (sa->info.np[0]->a_c0 == IDOP)
            sa->info.np[0]->info.mp[0]->b_sloc |= 1; /* may ba struct aaa a_c3[0] */
    }
    sub_475C(sa);
}

/*********************************************************
 * sub_2C5E OK++ PMO   	      Used in: sub_2CE0, sub_47B2
 *
 *********************************************************/
long sub_2C5E(node_t *sa, long p2)
{
    char loc;

    if ((loc = (char)nodesize(sa) * 8) >= 32)
        return p2;
    if (p2 & (1L << (loc - 1)))
        p2 |= ~((1L << loc) - 1L);
    return p2;
}

/*********************************************************
 *  sub_2CE0 OK++ PMO			Used in: sub_2D09
 *********************************************************/
void sub_2CE0(node_t *p1a, long p2)
{

    printf("%ld", sub_2C5E(p1a, p2));
}
/* end of cgen1.c */

/*
 * File - cgen2s.c	BIG size - problems compiling v3.09
 */

/* compiling this function with Hitech C runs out of memory
   so here the external functions are declared explicitly
   so that the normal function declarations and some of the
   unused include files are not processed.
*/
#ifndef _CGEN_H
#define MINI
#include "cgen.h"

int atoi(char *);
uint8_t sub_14F3(node_t *);
uint16_t sub_387A(node_t *);
uint16_t nodesize(node_t *);
uint8_t sub_46F7(long);
#endif
/*.......................................................*
 *  sub_2D09_v2 OK++	Used in: sub_3564
 * minor differences
 * 1) block of 5 instructions has different order but same effect
 * 2) one optimisation missed on loading a from lc10, possibly
 *    related to note 1
 * 3) optimisation because p3 is passed recursively as a char
 *.......................................................*/
void sub_2D09(register node_t *sa, char *p2, char p3)
{
    struct codeFrag_t *lz1 FORCEINIT; /* avoid uninitialised warning */
    node_t *la2 FORCEINIT;            /* avoid uninitialised warning */
    char *li3 FORCEINIT;              /* avoid uninitialised warning */
    uint8_t lc4;
    char lc5 = 0; /* avoid uninitialised warning */
    char lc6;
    char lc7;
    int ch;
    long ll9;
    uint8_t lc10 = 0; /* avoid uninitialised warning */
    char lc11;
    char *li12;
    uint32_t ll13 = 0; /* avoid uninitalised warning */
    bool lc14;

    li12 = p2;
    lc11 = lc14 = false;
    for (;;)
    {
        if ((lc7 = *p2++) == 0)
            return;
        switch (lc7)
        {
        case ' ':
            if (!lc14)
            {
                fputchar('\t');
                lc14 = true;
            }
            else
                fputchar(lc7);
            continue;
        case ';':
            return;
        case '\n':
        case '\r':
            fputchar('\n');
            lc14 = false;
            continue;
        case 'G':
        case 'X':
            if (isdigit(*p2))
            {
                li3 = p2;
                while (isdigit(*p2))
                    p2++;
            }
            else
                li3 = 0;
            lc6 = *p2++;
            ch = 'G';
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'F':
        case 'M':
        case 'O':
        case 'P':
        case 'Q':
        case 'S':
        case 'T':
        case 'V':
        case 'W':
        case 'Z':
        case '~':
            if ((ch = *p2) == '-' || ch == '+')
            {
                if (isdigit(*++p2))
                {
                    lc5 = (char)atoi(p2);
                    if (ch == '-')
                        lc5 = -lc5;
                    ch = 0;
                }
                else
                    lc5 = 0;
                while (isdigit(*p2) || *p2 == '+' || *p2 == '-')
                    p2++;
            }
            else if (ch == '>')
            {
                for (lc5 = 0; *p2 == '>'; p2++, lc5++)
                    ;
            }
            else
            {
                ch = 0;
                lc5 = 0;
            }

            lc6 = *p2++;
            break;
        case 'L':
        case 'R':
        case 'U':
            lc6 = lc7;
            lc7 = '~';
            ch = 0;
            lc5 = 0;
            break;
        case 'D':
            lc6 = lc7;
            ch = 0;
            lc5 = 0;
            break;

        default:
            fputchar(lc7);
            continue;
        }

        switch (lc6)
        {
        case 'L':
            if (p3 > 0)
            { /* m27: */
                la2 = sa;
                lc10 = (uint8_t)(p3 - 1);
                lz1 = &codeFrag[sa->a_c3[lc10]];
            }
            else
            {
                la2 = sa->info.np[0];
                lc10 = (uint8_t)(la2->a_c1 - 1);
                lz1 = &codeFrag[la2->a_c3[lc10]];
            }
            break;
        case 'R':
            la2 = sa->info.np[1];
            lc10 = (uint8_t)(la2->a_c1 - 1);
            lz1 = &codeFrag[la2->a_c3[lc10]];
            break;
        case 'N':
            la2 = sa;
            lc10 = (uint8_t)p3;
            break;
        }
        switch (lc7)
        {
        case 'D':
            if (word_AFF8 != 0)
            {
                printf("%d", word_AFF8);
                word_AFF8 = 0;
            }
            break;
        case 'S':
            if (la2->a_c0 == USEREG)
                printf("%s", regNames[la2->info.l]);
            else if (la2->a_c0 == CONST)
                sub_2CE0(la2, la2->info.l);
            else if (la2->a_c0 == FCONST)
                printf("%s", la2->info.sv.s);
            else
                printf("%s", la2->info.mp[0]->b_name);
        proff:
            if (ch == 0)
            {
                lc5 += (char)word_AFF8;
                word_AFF8 = 0;
                if (lc5 > 0)
                    printf("+%d", (int)lc5);
                else if (lc5 < 0)
                    printf("%d", (int)lc5);
            }
            break;
        case 'F':
            printf("%d", la2->info.sv.v);
            goto proff;

        case 'Z':
            ll9 = (long)nodesize(la2);
            goto m46;
        case 'B':
            ll9 = (long)sub_387A(la2);
        m46:
            if (ch == '-')
            {
                if (lc11 == 0)
                {
                    ll13 = ll9 + (long)word_AFF8;
                    word_AFF8 = 0;
                    lc11 = 1;
                }
                goto m54;
            }
            printf("%lu", ll9);
            break;
        case 'O':
            printf("%s", otherFrag[la2->a_c0]);
            break;
        case 'A':
            sub_153A(la2);
            break;
        case 'V':
            if (ch == '-' || ch == '+')
            {
                if (lc11 == 0)
                {
                    ll13 = la2->info.l + (long)word_AFF8;
                    word_AFF8 = 0;
                    lc11 = 1;
                }
            m54:
                ll13 += ch == '-' ? -1L : 1L;
                if (sub_14F3(la2) == 2 && nodesize(la2) < 4)
                    ll13 &= (1 << (int)(nodesize(la2) * 8)) - 1;

                if (ll13 != 0)
                {
                    while (li12 < p2 && p2[-1] != '\n')
                        p2--;
                    fputchar('\n');
                }
                else
                    lc11 = 0;
            }
            else
            {
                lc5 += (char)word_AFF8;
                word_AFF8 = 0;
                sub_2CE0(la2, la2->info.l >> (lc5 * 8));
            }
            break;

        case 'C':
            if (la2->a_c0 == BFIELD)
                printf("%d", la2->info.mp[1]->bOffset);
            else
            {
                ll9 = la2->info.l;
                if (sub_46F7(ll9) == 0)
                    ll9 = ~ll9;
                word_AFF8 += (sub_46F7(ll9) - 1) / 8;
            }
            break;
        case 'T':
            if (la2->a_c0 == BFIELD)
                printf("%d", la2->info.mp[1]->bWidth);
            else
            {
                ll9 = la2->info.l;
                if (sub_46F7(ll9) == 0)
                    ll9 = ~ll9;
                printf("%d", sub_46F7(ll9) - 1);
            }
            break;
        case 'G':
        case 'X':
            lc4 = lc7 == 'G' ? la2->a_reg[lc10] : la2->a_uc9[lc10];

            if (li3 != 0)
                while (isdigit(*li3))
                    lc4 = array_AB24[lc4 * 2 + (*li3++ - '0')];

            if (*p2 != '+')
            {
                lc4 += (uint8_t)word_AFF8;
                word_AFF8 = 0;
            }
            printf("%s", regNames[lc4]);
            break;
        case '~':
            if (lc6 == 'U')
            {
                lc5 += (char)word_AFFB;
                if ((dopetab[sa->a_c0] & 0x200) && p3 != 0)
                    lc5--;
                printf("%d", (int)lc5);
            }
            else
            {
                word_AFF8 += (int)lc5;
                sub_2D09(la2, lz1->p_8, (char)lc10);
            }
            break;
        }
    }
}

/* end of file cgen2s.c */
/*
 * File - cgen3.c Created 09.03.2019 Last Modified 30.05.2020
 */

/*********************************************************
 * sub_3564 OK++ PMO			Used in: sub_35E6
 * minor optimisation difference due to last parameter
 * of sub_2D09 being declared as char
 *********************************************************/
void sub_3564(register node_t *sa)
{
    uint8_t l1;
    struct codeFrag_t *l2z;

    word_AFF8 = 0;
    for (l1 = 0; l1 != sa->a_c1; l1++)
    {
        if ((l2z = &codeFrag[sa->a_c3[l1]])->p_6)
        {
            sub_2D09(sa, l2z->p_6, (char)l1);
            fputchar('\n');
        }
    }
}

/*********************************************************
 * sub_35E6 OK++ PMO				Used in: sub_2BD0
 *
 *********************************************************/
void sub_35E6(register node_t *sa)
{
    int l1;

    if (dopetab[sa->a_c0] & 0x200)
        word_AFFB++;
    l1 = dopetab[sa->a_c0] & 0xC;
    if (8 == l1)
    {
        if (sa->a_c2 & 1)
        {
            sub_35E6(sa->info.np[0]);
            sub_35E6(sa->info.np[1]);
        }
        else
        {
            sub_35E6(sa->info.np[1]);
            if (sa->a_c2 & 2)
                prPush(sa->info.np[1]->a_reg[sa->info.np[1]->a_c1 - 1]);
            sub_35E6(sa->info.np[0]);
        }
    }
    if (l1 == 4)
        sub_35E6(sa->info.np[0]);
    if (sa->a_c2 & 2)
        prPop(sa->info.np[1]->a_reg[sa->info.np[1]->a_c1 - 1]);
    sub_3564(sa);
    if (dopetab[sa->a_c0] & 0x200)
        word_AFFB--;
}

/*********************************************************
 * sub_36E0 OK++ PMO   Used in: sub_E43,  sub_39CA, sub_3CDF,
 *			    sub_3DC9, sub_3EAA, sub_43EF,
 *			    sub_475C, sub_4C8B, sub_4FA8,
 *			    sub_508A, sub_53EE, sub_54B6,
 *			    sub_5DF6, sub_5F52
 *********************************************************/
void freeNode(register node_t *sa)
{

    byte_B013 = true;
    if (sa->a_c0 == FCONST)
        free(sa->info.np[0]);
    sa->pm = (member_t *)nodeFreeList;
    nodeFreeList = sa;
}

/*********************************************************
 * sub_3712 OK++ PMO    Used in: sub_E43,  sub_377A, sub_415E,
 *			     sub_4192, sub_43EF, sub_4DA3,
 *			     sub_54B6
 *********************************************************/
node_t *allocNode(void)
{
    register node_t *sa;

    byte_B013 = true;
    if (nodeFreeList)
    {
        sa = (node_t *)nodeFreeList;
        nodeFreeList = sa->pm;
        blkclr((char *)sa, sizeof(node_t));
    }
    else
    { /* create node_t */
        sa = (node_t *)allocMem(sizeof(node_t));
    }

    return sa;
}

/*********************************************************
 *  sub_374C OK++ PMO			Used in: allocMem
 *********************************************************/
bool releaseNodeFreeList(void)
{
    register node_t *sa;

    if (nodeFreeList == 0)
        return false;
    while ((sa = (node_t *)nodeFreeList))
    {
        nodeFreeList = sa->pm;
        free(sa);
    }
    return true;
}

/*********************************************************
 *  sub_377A OK++ PMO			Used in: sub_3CDF
 *********************************************************/
node_t *sub_377A(node_t *p1a)
{
    register node_t *sa;

    sa = allocNode();
    *sa = *p1a;

    if (dopetab[sa->a_c0] & 0xC)
        sa->info.np[0] = sub_377A(sa->info.np[0]);
    if ((dopetab[sa->a_c0] & 0xC) == 8)
        sa->info.np[1] = sub_377A(sa->info.np[1]);
    return sa;
}

/*********************************************************
 * sub_37ED v1 OK++ OK	      Used in: sub_387A, sub_43EF
 *
 * missed optimisation on reloading l1b but otherwise
 * identical
 *********************************************************/
void sub_37ED(register node_t *sa)
{
    member_t *l1b;

    l1b = sa->pm;
    if (sa->a_i15 != 0)
        sa->a_i15 >>= 2;
    else
    {
        while (l1b->b_refl == 0)
            l1b = l1b->b_type;
        sa->pm = l1b->b_type;
        sa->a_i15 = l1b->b_refl >> 2;
    }
}

/*********************************************************
 * sub_385B OK++ PMO    Used in: sub_387A, sub_43EF, sub_4DA3,
 *			     sub_4E8D
 *********************************************************/
void sub_385B(register node_t *sa)
{
    sa->a_i15 = (sa->a_i15 * 4) | 1;
}

/*********************************************************
 * sub_387A OK++ PMO			Used in: sub_2D09
 *********************************************************/
uint16_t sub_387A(register node_t *sa)
{
    uint8_t loc;

    if (sa->a_i15 == 0 && sa->pm->b_type == 0 && sa->pm->b_refl == 0)
        return 1;

    sub_37ED(sa);
    loc = (uint8_t)nodesize(sa);
    sub_385B(sa);
    return (uint16_t)loc;
}

/*********************************************************
 * sub_38CA OK++ PMO	      Used in: sub_393C, sub_3952
 * minor difference as function declared as bool
 *********************************************************/
bool sub_38CA(node_t *p1a, int p2)
{
    register member_t *sb;

    if (p1a->a_i15 & p2)
        return true;
    if (p1a->a_i15 != 0)
        return false;
    sb = p1a->pm;
    do
    {
        if (sb->b_refl & p2)
            return true;
        if (sb->b_refl != 0)
            return false;
    } while ((sb = sb->b_type) != 0);
    return false;
}

/*********************************************************
 * sub_393C OK++ PMO			Used in: Not used
 *********************************************************/
bool sub_393C(node_t *sa)
{
    return sub_38CA(sa, 1);
}

/*********************************************************
 * sub_3952 OK++ PMO			Used in: Not used
 *********************************************************/
bool sub_3952(node_t *sa)
{
    return sub_38CA(sa, 2);
}

/*********************************************************
 * sub_3968 OK++ PMO			Used in: sub_47B2
 *********************************************************/
bool sub_3968(register node_t *sa)
{
    return sa->pm->b_class == STRUCT && (sa->a_i15 & 1) == 0;
}

/*********************************************************
 * nodesize OK++ PMO    Used in: sub_808,  sub_E43,  sub_2C5E,
 *			     sub_2D09, sub_387A, sub_3A79,
 *			     sub_43EF, sub_47B2, sub_508A,
 *			     sub_54B6, sub_60A8, sub_6246,
 *			     sub_62BE
 *********************************************************/
uint16_t nodesize(register node_t *sa)
{

    if (sa->a_i15 & 2)
        prError("can\'t take sizeof func");

    if (sa->a_i15 & 1)
        return 2;
    return sa->pm->b_size;
}

/*********************************************************
 * sub_39CA OK++ PMO			Used in: sub_3EAA
 *********************************************************/
node_t *sub_39CA(register node_t *sa)
{
    char buf[50];

    freeNode(sa->info.np[1]);
    if (sub_14F3(sa) == 3)
    {
        sprintf(buf, "%ld.", sa->info.np[0]->info.l);
        freeNode(sa->info.np[0]);
        sa->info.sv.s = (char *)allocMem(strlen(buf) + 1); /* create string */
        strcpy(sa->info.sv.s, buf);
        sa->info.sv.v = newLocal();
        sa->a_c0 = FCONST;
        return sa;
    }
    sub_3A79(sa, sa->info.np[0]);
    freeNode(sa);
    return sa->info.np[0];
}

/*********************************************************
 * sub_3A79 OK++ PMO	              Used in: sub_3EAA, sub_39CA
 *
 * Compiler generates code that performs expected action,
 * but differs from original image
 *********************************************************/
void sub_3A79(register node_t *sa, node_t *p2a)
{
    int l1;

    l1 = nodesize(sa);
    if (l1 != 0 && l1 < 4)
    {
        l1 <<= 3;
        p2a->info.l &= (1L << l1) - 1L;
        if (sub_14F3(sa) == 1)
        {
            if (p2a->info.l & (long)(1 << (l1 - 1)))
            { /* sign extend */
                p2a->info.l |= ~((1L << l1) - 1L);
            }
        }
    }
    p2a->pm = sa->pm;
    p2a->a_i15 = sa->a_i15;
}

/*********************************************************
 * sub_3B65 OK++ PMO			Used in: sub_3EAA
 *********************************************************/
void sub_3B65(register unsigned long *ptr, long p2, int operator)
{

    switch (operator)
    {
    case DIV:
        *ptr /= p2;
        break;
    case MOD:
        *ptr %= p2;
        break;

    case RSHIFT:
        *ptr >>= p2;
        break;
    }
}

/*********************************************************
 * sub_3BC6 v1 OK++ PMO			Used in: sub_3EAA
 *
 *********************************************************/
void sub_3BC6(register long *ptr, long p2, int operator)
{

    switch (operator)
    {
    case ADD:
        *ptr += p2;
        break;
    case SUB:
        *ptr -= p2;
        break;
    case MUL:
        *ptr *= p2;
        break;
    case DIV:
        *ptr /= p2;
        break;
    case MOD:
        *ptr %= p2;
        break;
    case BOR:
        *ptr |= p2;
        break;
    case BAND:
        *ptr &= p2;
        break;
    case BXOR:
        *ptr ^= p2;
        break;
    case LSHIFT:
        *ptr <<= p2;
        break;
    case RSHIFT:
        *ptr >>= p2;
        break;
    }
}

/*********************************************************
 * sub_3CDF OK++ PMO			Used in: sub_3DC9
 *********************************************************/
node_t *sub_3CDF(register node_t *sa)
{
    int loc;

    if (sa->a_c0 == SCOLON)
    {
        sub_2BD0(sa->info.np[0]);
        freeNode(sa);
        sa = sa->info.np[1];
    }
    if (byte_B014 != 10)
    {
        if (dopetab[sa->a_c0] & 0x4000)
            return sa;
        if ((sa->a_c0 == INCR || sa->a_c0 == DECR) && sub_4C6D(sa))
        {
            array_AFFD[byte_B014++] = sa;
            sa->a_c0 = sa->a_c0 == INCR ? ASADD : ASSUB;
            sa = sub_377A(sa->info.np[0]);
        }
        loc = dopetab[sa->a_c0];
        if (loc & 0xC)
            sa->info.np[0] = sub_3CDF(sa->info.np[0]);
        if ((loc & 0xC) == 8)
            sa->info.np[1] = sub_3CDF(sa->info.np[1]);
    }
    return sa;
}

/* end of file cgen3.c */
/*
 * File - tree1.c Created 09.03.2019 Last Modified 17.06.2020
 */
/*********************************************************
 * sub_3DC9 OK++ PMO		Used in: sub_6D1, sub_D66
 *********************************************************/
void sub_3DC9(register node_t *sa)
{
    uint16_t loc;

    sa = sub_600E(sa);
    if (sa->a_c0 == DOLLAR && sa->info.np[0]->a_c0 == CONST)
    {
        prWarning("constant conditional branch");
        if (sa->info.np[0]->info.l != 0)
        {
            sa->a_c0 = DOLLAR_U;
            freeNode(sa->info.np[0]);
            sa->info.np[0] = sa->info.np[1];
        }
        else
        {
            sub_475C(sa);
            return;
        }
    }
    byte_B014 = 0;
    if (sa->a_c0 == INCR)
        sa->a_c0 = ASADD;
    else if (sa->a_c0 == DECR)
        sa->a_c0 = ASSUB;

    sa = sub_3CDF(sa);
    if (byte_B014 != 0)
        sa = sub_600E(sa);
    sub_2BD0(sa);
    for (loc = 0; loc < byte_B014; loc++)
        sub_2BD0(array_AFFD[loc]);
}

/*********************************************************
 * sub_3EAA OK++ PMO				Used in: sub_600E
 *********************************************************/

node_t *sub_3EAA(register node_t *sa)
{
    node_t *l1a;
    int l2;

    if ((l2 = dopetab[sa->a_c0] & 0xC))
        sa->info.np[0] = sub_3EAA(sa->info.np[0]);
    if (l2 == 8)
        sa->info.np[1] = sub_3EAA(sa->info.np[1]);
    if (sa->a_c0 == CONV && sa->info.np[0]->a_c0 == FCONST && sub_14F3(sa) == 3)
    {
        l1a = sa->info.np[0];
        l1a->pm = sa->pm;
        freeNode(sa->info.np[1]);
        freeNode(sa);
        return l1a;
    }
    if (l2 == 0 || sa->info.np[0]->a_c0 != CONST)
        return sa;

    if (sa->a_c0 == CONV)
        return sub_39CA(sa);

    if (l2 == 8 && sa->info.np[1]->a_c0 != CONST)
        return sa;

    if (sub_14F3(sa) == 2)
    {
        switch (sa->a_c0)
        {
        case LT:
        case LEQ:
        case GT:
        case GEQ:
            warningMsg = "constant relational expression";
            return sa;
        case MOD:
        case DIV:
        case RSHIFT:
            sub_3B65(&sa->info.np[0]->info.ul, sa->info.np[1]->info.l, sa->a_c0);
        finish:
            if (l2 == 8)
                sub_475C(sa->info.np[1]);
            sub_3A79(sa, sa->info.np[0]);
            freeNode(sa);
            return sa->info.np[0];
        }
    }
    switch (sa->a_c0)
    { /* m10: */
    case NEQL:
    case LT:
    case LEQ:
    case EQL:
    case GT:
    case GEQ:
        warningMsg = "constant relational expression";
        return sa;
    case BAND:
    case LAND:
    case SUB:
    case BXOR:
    case BOR:
    case LOR:
    case ADD:
    case MUL:
    case DIV:
    case MOD:
    case RSHIFT:
    case LSHIFT:
        sub_3BC6(&sa->info.np[0]->info.l, sa->info.np[1]->info.l, sa->a_c0);
        goto finish;
    case MINUS_U:
        sa->info.np[0]->info.l = -sa->info.np[0]->info.l;
        goto finish;
    case NOT:
        sa->info.np[0]->info.l = sa->info.np[0]->info.l == 0;
        goto finish;
    case BNOT:
        sa->info.np[0]->info.l = ~sa->info.np[0]->info.l;
        goto finish;
    default:
        return sa;
    }
}

/*********************************************************
 * sub_415E OK   Used in: sub_808,  sub_E43,  sub_4192,
 *			    sub_4DA3, sub_54B6
 *********************************************************/
node_t *sub_415E(long number)
{
    register node_t *sa;
    sa = allocNode();
    sa->a_c0 = CONST;
    sa->pm = typeLong;
    sa->info.l = number;
    return sa;
}

/* end of file tree1.c */
/*
 * File - tree2.c Created 09.03.2019 Last Modified 17.06.2020
 */

/* when compiling with the Hitech C compiler, to avoid a compiler crash
   use a subset of the cgen.h and declare only used functions
*/

#ifndef _CGEN_H
#define MINI
#include "cgen.h"

char sub_1B2(char *);
member_t *sub_265(char *);
member_t *sub_627(char *, uint16_t *);
void parseVariable();
void parseCType(int);
void parseEnum();
node_t *parseExpr();
node_t *sub_43EF(uint8_t, node_t *, node_t *);
long atol(char *str);
void *allocMem(size_t);
char *getToken();
int newLocal();
node_t *sub_415E(long);
node_t *allocNode();

void prGlobalDef(member_t *);
#endif
/*********************************************************
 * sub_4192 OK++ PMO     Used in: sub_6D1,  sub_793,  sub_808,
 *			     sub_B19,  sub_E43,  sub_17E0,
 *			     sub_19C1, sub_1B0C
 * minor variation due to paramater 1 of sub_43EF being declared
 * uint8_t, otherwise identical
 *********************************************************/
node_t *parseExpr()
{
    node_t *l1a;
    char *l2;
    int l3;
    int l4;
    register node_t *sa;

    for (;;)
    {
        l2 = getToken();
        if (isdigit(*l2) || (*l2 == '-' && isdigit(l2[1])))
        {
            if (*l2 == '-')
            {
                l4 = 1;
                l2++;
            }
            else
                l4 = 0;

            sa = sub_415E(atol(l2));
            if (l4 != 0)
                sa->info.l = -sa->info.l;
            return sa;
        }

        if (*l2 == '_' || isalpha(*l2))
        {
            (sa = allocNode())->a_c0 = IDOP; /* m8:  */
            sa->info.mp[0] = sub_265(l2);
            sa->pm = sa->info.mp[0]->b_type;
            sa->a_i15 = sa->info.mp[0]->b_refl;
            if (sa->info.mp[0]->b_flag == 3 && (sa->info.mp[0]->b_sloc & 2) == 0)
            {
                prGlobalDef(sa->info.mp[0]); /* Emit "global name" */
            }
            return sa;
        }
        if (*l2 == '`')
        {
            sa = allocNode();
            sa->pm = sub_627(l2, &sa->a_i15);
            sa->a_c0 = TYPE;
            return sa;
        }
        if (*l2 == '.' && l2[1] && l2[1] != '.')
        {
            sa = allocNode();
            sa->pm = typeDouble;                          /* "d" - double */
            sa->info.sv.s = (char *)allocMem(strlen(l2)); /* create string */
            strcpy(sa->info.sv.s, l2 + 1);
            sa->info.sv.v = newLocal(); /* swTableCnt++ */
            sa->a_c0 = FCONST;
            return sa;
        }
        l4 = sub_1B2(l2); /* Token search */ /* m12: */
        l3 = dopetab[l4] & 0xC;
        switch (l3)
        {
        case 0xc:
            switch (l4)
            {
            case '9':
                parseVariable();
                continue;
            case '7':
            case '8':
                parseMembers(l4);
                continue;
            case '4':
                parseEnum();
                continue;
            default:
                fatalErr("Expression error");
                break;
            }
            /* fall through */
        case 0:
            return sub_43EF((uint8_t)l4, 0, 0);
        case 4:
            return sub_43EF((uint8_t)l4, parseExpr(), 0);
        case 8:
            l1a = parseExpr();
            return sub_43EF((uint8_t)l4, l1a, parseExpr());
        }
    }
}

/* end of file tree2.c */
/*
 * File - tree3.c Created 09.03.2019 Last Modified 17.06.2020
 */

/*********************************************************
 * sub_43EF OK++ PMO      Used in: sub_793,  sub_808,  sub_E43,
 *                            sub_4192, sub_4C8B, sub_4DA3,
 *                            sub_4E8D, sub_508A, sub_54B6,
 *                            sub_5DF6

 *********************************************************/
node_t *sub_43EF(uint8_t p1, node_t *p2a, node_t *p3a)
{
    member_t *l1b;
    long l2;
    register node_t *sa;

    sa = allocNode();
    sa->a_c0 = p1;
    sa->info.np[0] = p2a;
    sa->info.np[1] = p3a;
    switch (dopetab[sa->a_c0] & 0x30)
    {
    case 0x10:
        sa->pm = typeLong; /* long	  */
        break;
    case 0x20:
        sa->pm = typeB; /* b	  */
        break;

    case 0x30:
        sa->pm = typeVar; /* variable */
        break;

    default:
        sa->pm = p2a->pm;
        sa->a_i15 = p2a->a_i15;
    }
    switch (sa->a_c0)
    {
    case DOT:
        if (p2a->a_c0 == TYPE)
        {
            l1b = p2a->pm;
            freeNode(sa);
            freeNode(p2a);
            sa = p3a;
            sa->pm = l1b;
            p3a->info.l = (long)l1b->b_memb.ilist->vals[sa->info.l];
            break;
        }
        l1b = sa->pm->b_memb.mlist->vals[p3a->info.l];
        sa->pm = l1b->b_type;
        sa->a_i15 = l1b->b_refl;
        p3a->info.l = (long)l1b->b_off;
        p3a->pm = typeX; /* x      */
        if (l1b->b_sloc & 0x10)
        {
            p2a = allocNode();
            *p2a = *sa;
            p2a->info.np[0] = sa;
            sa = p2a;
            sa->a_c0 = BFIELD;
            sa->info.mp[1] = l1b;
        }
        break;
    case CONV:
    case SCOLON:
    case QUEST:
        sa->pm = sa->info.np[1]->pm;
        sa->a_i15 = sa->info.np[1]->a_i15;
        break;
    case GADDR:
        if (sa->a_i15 == 0 && sa->pm->b_nelem > 1)
        {
            sa->a_i15 = sa->pm->b_refl;
            sa->pm = sa->pm->b_type;
        }
        sub_385B(sa);
        break;
    case LPAREN:
    case MUL_U:
        sub_37ED(sa);
        break;
    case COLON_S:
        sa->pm = typeChar; /* char   */
        sa->a_i15 = 1;
        break;
    case HASHSIGN:
        if (sa->info.np[0]->a_c0 == IDOP)
            l2 = sa->info.np[0]->info.mp[0]->b_size;
        else
            l2 = (uint16_t)nodesize(sa->info.np[0]);
        if (l2 == 0)
            prWarning("Sizeof yields 0");
        sub_475C(sa->info.np[0]);
        sa->a_c0 = CONST;
        sa->pm = typeLong; /* long	  */
        sa->a_i15 = 0;
        sa->info.l = l2;
        break;
    }
    return sa;
}

/*********************************************************
 * sub_46F7 OK++ PMO     Used in: sub_2D09, sub_47B2, sub_4FCE
 *			      sub_54B6
 *********************************************************/
uint8_t sub_46F7(long p1)
{
    uint8_t l1;

    if ((p1 & (p1 - 1)) || p1 < 1)
        return 0;
    for (l1 = 0; p1 >>= 1; l1++)
        ;

    return (l1 + 1);
}

/*********************************************************
 * sub_475C OK++ PMO    Used in: sub_808,  sub_E43,  sub_17E0,
 *			     sub_19C1, sub_1B0C, sub_2BD0,
 *			     sub_3DC9, sub_3EAA, sub_43EF,
 *			     sub_475C, sub_4DA3, sub_4FA8,
 *			     sub_508A, sub_54B6
 *********************************************************/
void sub_475C(register node_t *sa)
{
    int loc;

    if ((loc = dopetab[sa->a_c0] & 0xC))
    {
        sub_475C(sa->info.np[0]);
        if (loc == 8)
            sub_475C(sa->info.np[1]);
    }
    freeNode(sa);
}

/*********************************************************
 * sub_47B2 OK++ PMO				Used in: sub_1F4B
 * The code optimisers generate many differences in the code
 * however the vast majority relate to the choice of the code
 * fragments to share for returning true/false and generating
 * true/false from != condition
 * I have tried multiple variants of code to get better matches
 * for the current code, the other differences are
 * 1) at m49dc:, code for sub_14FE(sa) != 3 is moved
 * 2) case SUB: nodesize(sa) == 2 code moved and missed sharing
 *    of code for or a ! sbc hl,de ! jp nz false
 * 3) case DECR: similar to (2)
 * 4) case GADDR: similar to (2)
 * 5) an instance of missed sharing of code
 * 6) instances of inversion of jp conditions with labels also
 *    flipped
 * Note removing the gotos was initially tried, but the optimiser
 * match was poorer. Similarly using compound returns rather
 * than if / break, also produced a poorer match.
 *********************************************************/
uint8_t sub_47B2(register node_t *sa, int p2)
{
    long l1;

    switch (p2)
    {
    case MUL_U:
        sa = sa->info.np[1];
        /* fall through */
    case T_SCOLON:
        return (uint8_t)(sa->info.l >= 1L && sa->info.l < 3L);
    case RPAREN:
        sa = sa->info.np[0];
        /* fall through */
    case MOD:
        if (nodesize(sa) == 2 && !sub_3968(sa))
            return 1;
        break;
    case BAND:
        if (sub_14F3(sa) == 3)
            return 0;
        goto m485f;
    case MUL:
        sa = sa->info.np[0];
        /* fall through */
    case DOLLAR_U:
    m485f:
        if (nodesize(sa) == 4 && !sub_3968(sa))
            return 1;
        break;
    case SCOLON:
        sa = sa->info.np[0];
        /* fall through */
    case DOLLAR:
        return (uint8_t)sub_3968(sa);
    case ADD:
        return (uint8_t)((uint8_t)sa->info.l == 8 || (uint8_t)sa->info.l == 9);
    case INCR:
        l1 = sub_2C5E(sa, sa->info.l);
        return (uint8_t)(-128L <= l1 && l1 < 125L);
    case COLON_U:
        return (uint8_t)(1L <= sa->info.l && sa->info.l < 5L);
    case COLON_S:
        l1 = sa->info.l;
        if (sub_14F3(sa) == 2 && (l1 & (uint16_t)(1 << ((int16_t)nodesize(sa) << 3))))
            l1 |= (uint16_t)~((1 << ((int16_t)nodesize(sa) << 3)) - 1);
        return (uint8_t)(l1 < 0 && l1 >= -4L);
    case LPAREN:
        sa = sa->info.np[1];
        /* fall through */
    case HASHSIGN:
        return (uint8_t)(nodesize(sa) == 1 && !sub_3968(sa));
    case MINUS_U:
        if (nodesize(sa) == 4)
            goto m49dc;
        break;
    case COMMA:
        if (nodesize(sa) == 2)
        m49dc:
            if (sub_14F3(sa) != 3 && sub_14F3(sa->info.np[0]) != 3)
                return (uint8_t)(sub_14F3(sa) == 2 || sub_14F3(sa->info.np[0]) == 2);
        break;
    case LT:
        return (uint8_t)(sa->info.l == 0);
    case CONV:
        if (nodesize(sa) == 4)
        m4a48:
            return (uint8_t)(sub_14F3(sa) == 1 && sub_14F3(sa->info.np[0]) == 1);
        return 0;

    case SUB:
        if (nodesize(sa) == 2)
            goto m4a48;
        break;
    case PLUS_U:
        return (uint8_t)(1L == sa->info.l);
    case NOT:
        return (uint8_t)(sub_46F7(sa->info.np[1]->info.l));
    case NEQL:
        return (uint8_t)(sub_46F7(~sa->info.np[1]->info.l));
    case COLON:
        if (nodesize(sa) == 4)
            goto m4b02;
        break;
    case DECR:
        if (nodesize(sa) == 2)
            goto m4b02;
        break;

    case GADDR:
        if (nodesize(sa) != 1)
            return 0;
    m4b02:
        return (uint8_t)((sub_14F3(sa) == 1 || sub_14F3(sa) == 2) && sub_14F3(sa->info.np[0]) == 3);
    case DIV:
        if (nodesize(sa->info.np[0]) == 1)
            goto dotp;
        break;
    case DOT_DOT:
        if (nodesize(sa->info.np[0]) == 2)
            goto dotp;
        break;
    case DOT:
        if (nodesize(sa->info.np[0]) == 4)
        {
        dotp:
            if (sub_14F3(sa) != 3)
                return 0;
            else
                return (uint8_t)(sub_14F3(sa->info.np[0]) == 1 || sub_14F3(sa->info.np[0]) == 2);
        }
        break;
    case LAND:
        return (uint8_t)(sub_14F3(sa) == 3);
    }
    return 0;
}

/*********************************************************
 * sub_4B89 OK++ PMO		      Used in: sub_4C12, sub_54B6

 *********************************************************/
bool sub_4B89(register node_t *sa)
{
    /* Any operator except "#", "..", "CONST" */
    return (dopetab[sa->a_c0] & 0x10) || (sa->a_c0 == GADDR && sa->info.np[0]->a_c0 == IDOP) ||
           (sa->a_c0 == ADD && sa->info.np[1]->a_c0 == CONST && sub_4B89(sa->info.np[0]) != 0);
}

/*********************************************************
 * sub_4BE5 OK++ PMO			Used in: sub_54B6
 *********************************************************/
bool sub_4BE5(register node_t *sa)
{

    return sa->a_c0 == CONST && sa->info.l == 0;
}

/*********************************************************
 * sub_4C12++ PMO			Used in: sub_54B6
 *********************************************************/
int sub_4C12(register node_t *sa)
{
    if (sa->a_c0 == GADDR && sa->info.np[0]->a_c0 == IDOP)
        return 1;
    if (dopetab[sa->a_c0] & 0x10)
        return 0; /* if "#", "..", "CONST"  */
    if (sub_4B89(sa))
        return 1;
    return dopetab[sa->a_c0] & 0xF;
}

/*********************************************************
 * sub_4C6D OK++ PMO				Used in: sub_3CDF
 * bool causes optimiser to use ld l, rather than ld hl
 *********************************************************/
bool sub_4C6D(node_t *p1a)
{
    return p1a->info.np[0]->a_c0 == USEREG;
}

/*********************************************************
 * sub_4C8B OK++ PMO			Used in: sub_54B6
 * sub_43EF using bool as first parameter is optimised
 * but misses ex (sp),hl rather than pop bc ! push hl
 *********************************************************/
node_t *sub_4C8B(register node_t *sa)
{
    node_t *l1;

    l1 = sa->info.np[0];
    if (sa->info.np[0]->a_c0 == NOT)
    {
        l1 = l1->info.np[0];
        freeNode(sa->info.np[0]);
        freeNode(sa);
        return l1;
    }
    if ((dopetab[l1->a_c0] & 0x12C) == 0x28)
    {
        freeNode(sa);
        l1->a_c0 = (uint8_t)invertTest(l1->a_c0);
        return l1;
    }
    if (dopetab[l1->a_c0] & 0x20)
    {
        l1->info.np[0] = sub_43EF(NOT, l1->info.np[0], (node_t *)0);
        l1->info.np[1] = sub_43EF(NOT, l1->info.np[1], (node_t *)0);
        freeNode(sa);
        l1->a_c0 = l1->a_c0 == LAND ? LOR : LAND;
        return l1;
    }
    return sa;
}

/*********************************************************
 * sub_4DA3 OK++ PMO			Used in: sub_54B6
 * sub_43EF using bool as first parameter is optimised
 * but misses ex (sp),hl rather than pop bc ! push hl
 *********************************************************/
node_t *sub_4DA3(register node_t *sa)
{
    node_t *l1a;

    if (sa->info.mp[0]->b_flag == 1)
    {
        l1a = sub_415E(sa->info.mp[0]->b_off);
        l1a->pm = sa->pm;
        l1a->a_i15 = sa->a_i15;
        sub_475C(sa);
        sub_385B(l1a);
        sa = allocNode();
        *sa = *l1a;
        sa->a_c0 = USEREG;
        sa->info.l = 8L;
        sa = sub_43EF(ADD, sa, l1a);
        return sub_43EF(MUL_U, sa, 0);
    }
    if (sa->info.mp[0]->b_flag == 2)
    {
        sa->a_c0 = USEREG;
        sa->info.l = sa->info.mp[0]->b_memb.i;
    }
    return sa;
}

/*********************************************************
 * sub_4E8D OK++ PMO 			Used in: sub_54B6
 * sub_43EF using bool as first parameter is optimised
 * but misses ex (sp),hl rather than pop bc ! push hl
 *********************************************************/
node_t *sub_4E8D(register node_t *sa)
{
    node_t *l1a;

    if (dopetab[(l1a = sa->info.np[0])->a_c0] & 0x1000)
    {
        l1a->pm = sa->pm;
        l1a->a_i15 = sa->a_i15;
        sa->info.np[0] = l1a = sub_43EF(GADDR, l1a, 0);
    }
    else
    {
        l1a = allocNode();
        *l1a = *sa;
        l1a->a_c0 = TYPE;
        sub_385B(l1a);
        sa->info.np[0] = sub_43EF(CONV, sub_43EF(GADDR, sa->info.np[0], 0), l1a);
    }
    sa->pm = l1a->pm;
    sa->a_i15 = l1a->a_i15;
    sa->info.np[1]->a_i15 = sa->a_i15;
    sa->info.np[1]->pm = sa->pm;
    sa->a_c0 = ADD;
    return sub_43EF(MUL_U, sa, 0);
}

/*********************************************************
 * sub_4FA8 OK++ PMO			Used in: sub_54B6
 *********************************************************/
node_t *sub_4FA8(register node_t *sa)
{

    sub_475C(sa->info.np[1]);
    freeNode(sa);
    return sa->info.np[0];
}

/*********************************************************
 * sub_4FCE OK++ OK				Used in: sub_54B6
 *
 *********************************************************/
node_t *sub_4FCE(register node_t *sa)
{

    switch (sa->a_c0)
    {
    case MUL:
        sa->a_c0 = LSHIFT;
        break;
    case ASMUL:
        sa->a_c0 = ASLSHIFT;
        break;
    case DIV:
        if (sub_14F3(sa) != 2)
            return sa;
        sa->a_c0 = RSHIFT;
        break;
    case ASDIV:
        if (sub_14F3(sa) != 2)
            return sa;
        sa->a_c0 = ASRSHIFT;
        break;
    case ASMOD:
        sa->a_c0 = ASAND;
        sa->info.np[1]->info.l--;
        return sa;
    case MOD:
        sa->a_c0 = BAND;
        sa->info.np[1]->info.l--;
        return sa;
    }
    byte_B013 = true;
    sa->info.np[1]->info.l = sub_46F7(sa->info.np[1]->info.l) - 1;
    return sa;
}

/*********************************************************
 * sub_508A OK++ PMO			Used in: sub_54B6
 * optimiser optimises code for parameter 1 of sub_43EF
 *********************************************************/
node_t *sub_508A(register node_t *sa)
{
    node_t *l1a;
    node_t *l2a;
    char buf[15];
    long l3;

    if (sub_14F3(sa) == 3 && sa->info.np[0]->a_c0 == CONST)
    {
        l3 = sa->info.np[0]->info.l;
        freeNode(sa->info.np[0]);
        freeNode(sa->info.np[1]);
        sprintf(buf, "%ld", l3);
        sa->info.sv.s = (char *)allocMem(strlen(buf) + 1); /* create string */
        strcpy(sa->info.sv.s, buf);
        sa->info.sv.v = newLocal();
        sa->a_c0 = FCONST;
        return sa;
    }
    if ((dopetab[(l1a = sa->info.np[0])->a_c0] & 0x1000) && nodesize(l1a) >= nodesize(sa) &&
        sub_14F3(sa) != 3 && sub_14F3(l1a) != 3)
    {
        l1a->pm = sa->pm;
        l1a->a_i15 = sa->a_i15;
        freeNode(sa->info.np[1]);
        freeNode(sa);
        return l1a;
    }
    if (sub_6246(sa, l1a))
    {
        sub_475C(sa->info.np[1]);
        l1a->pm = sa->pm;
        l1a->a_i15 = sa->a_i15;
        freeNode(sa);
        return l1a;
    }
    if (nodesize(l1a) < nodesize(sa))
        return sa;
    if (l1a->a_c0 == CONV && nodesize(l1a->info.np[0]) >= nodesize(sa))
    {
        if (sub_6246(sa, l1a->info.np[0]) ||
            (sub_14F3(sa) == sub_14F3(l1a) && sub_14F3(sa) == sub_14F3(l1a->info.np[0])))
        {
            sa->info.np[0] = l1a->info.np[0];
            freeNode(l1a->info.np[1]);
            freeNode(l1a);
            return sa;
        }
    }
    if ((dopetab[l1a->a_c0] & 0xC00) == 0 || sub_14F3(l1a) == 3)
        return sa;

    if ((dopetab[l1a->a_c0] & 0x400) &&
        (l1a->info.np[0]->a_c0 != CONV || nodesize(l1a->info.np[0]->info.np[0]) >= nodesize(l1a)))
        return sa;

    l2a = sub_43EF(CONV, l1a->info.np[0], sa->info.np[1]);
    *sa = *sa->info.np[1];
    if (dopetab[l1a->a_c0] & 8)
    {
        sa = sub_43EF(CONV, l1a->info.np[1], sa);
    }
    else
    {
        freeNode(sa);
        sa = 0;
    }
    l2a = sub_43EF((uint8_t)l1a->a_c0, l2a, sa);
    freeNode(l1a);
    return l2a;
}

/*********************************************************
 * sub_53EE  OK++ PMO			Used in: sub_54B6
 *********************************************************/
node_t *sub_53EE(register node_t *sa)
{
    node_t *l1a;

    if (dopetab[(l1a = sa->info.np[0])->info.np[0]->a_c0] & 0x1000)
    {
        l1a = l1a->info.np[0];
        l1a->pm = sa->pm;
        l1a->a_i15 = sa->a_i15;
        freeNode(sa->info.np[0]);
        freeNode(sa);
        return l1a;
    }
    sa->a_c0 = CONV;
    sa->info.np[1] = l1a;
    sa->info.np[0] = l1a->info.np[0];
    l1a->a_c0 = TYPE;
    l1a->pm = sa->pm;
    l1a->a_i15 = sa->a_i15;
    byte_B013 = true;
    return sa;
}

/* end of file tree3.c */
/*
 * File - local1s.c
 */

#ifndef _CGEN_H
#define MINI
#include "cgen.h"

node_t *allocNode(void);
uint16_t nodesize(node_t *);
uint8_t sub_14F3(node_t *);
node_t *sub_415E(long);
node_t *sub_43EF(uint8_t, node_t *, node_t *);
uint8_t sub_46F7(long);
bool sub_4B89(node_t *);
bool sub_4BE5(node_t *);
int sub_4C12(node_t *);
node_t *sub_4C8B(node_t *);
node_t *sub_4DA3(node_t *);
node_t *sub_4E8D(node_t *);
node_t *sub_4FA8(node_t *);
node_t *sub_4FCE(node_t *);
node_t *sub_508A(node_t *);
node_t *sub_53EE(node_t *);
bool sub_60A8(node_t *, node_t *);
bool sub_6246(node_t *, node_t *);
bool sub_62BE(node_t *, node_t *);
#endif

/*********************************************************
 * sub_54B6 - OK++ PMO	Used in: sub_600E
 * needs ported optimiser to build
 * Optimiser differences
 * 1) in the GT ? GEQ : LT the GEQ assignment is moved
 * 2) in MUL_U, the optimiser uses bc rather than hl, then
 *    does hl = bc
 *********************************************************/
node_t *sub_54B6(register node_t *sa)
{
    node_t *l1a;
    node_t *l2a;
    int l3;
    uint32_t l4;

    l3 = dopetab[sa->a_c0] & 0xC;
    if (l3 != 0)
        sa->info.np[0] = sub_54B6(sa->info.np[0]);

    if (l3 == 8)
        sa->info.np[1] = sub_54B6(sa->info.np[1]);

    if (dopetab[sa->a_c0] & 0x40)
    {
        if (sub_4C12(sa->info.np[0]) < sub_4C12(sa->info.np[1]))
        {
            byte_B013 = true;
            l1a = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l1a;
        }
        if ((dopetab[sa->a_c0] & 0x2000) && sa->a_c0 == sa->info.np[0]->a_c0)
        {
            if (sub_4B89(sa->info.np[1]) && sub_4B89((l1a = sa->info.np[0])->info.np[1]))
            {
                byte_B013 = true;
                sa->info.np[0] = l1a->info.np[1];
                l1a->info.np[1] = sa;
                sa = l1a;
            }
            else if (sub_4C12(sa->info.np[0]->info.np[1]) < sub_4C12(sa->info.np[1]))
            {
                byte_B013 = true;
                l1a = sa->info.np[1];
                sa->info.np[1] = sa->info.np[0]->info.np[1];
                sa->info.np[0]->info.np[1] = l1a;
            }
        }
    }

    if ((dopetab[sa->a_c0] & 0xC00) == 0x800 && sa->info.np[0]->a_c0 == CONV &&
        nodesize(sa->info.np[0]->info.np[0]) < nodesize(sa) &&
        sub_60A8(sa->info.np[1], sa->info.np[0]->info.np[0]))
    {

        l2a = sa->info.np[0];
        sa->info.np[0] = l2a->info.np[0];
        sa->pm = sa->info.np[0]->pm;
        sa->a_i15 = sa->info.np[0]->a_i15;
        sa->info.np[1]->pm = sa->pm;
        sa->info.np[1]->a_i15 = sa->a_i15;
        l2a->info.np[0] = sa;
        sa = l2a;
    }

    if ((dopetab[sa->a_c0] & 0x420) == 0x420 && (l1a = sa->info.np[0])->a_c0 == CONV &&
        nodesize(l1a->info.np[0]) != 0 && nodesize(l1a->info.np[0]) < nodesize(l1a))
    {
        if (sa->info.np[1]->a_c0 == CONST &&
            (sub_14F3(l1a) == 1 || sub_14F3(l1a->info.np[0]) == 2))
        {
            if (sub_62BE(sa->info.np[0], sa->info.np[1]))
            {
                freeNode(sa->info.np[0]);
                freeNode(sa->info.np[0]->info.np[1]);
                sa->info.np[0] = sa->info.np[0]->info.np[0];
                sa->info.np[1]->pm = sa->info.np[0]->pm;
                sa->info.np[1]->a_i15 = sa->info.np[0]->a_i15;
            }
            else
                warningMsg = "mismatched comparision"; /* m8: */
        }
        else if (sa->info.np[1]->a_c0 == CONV &&
                 sub_6246(sa->info.np[0]->info.np[0], sa->info.np[1]->info.np[0]))
        {
            l1a = sa->info.np[0];
            sa->info.np[0] = l1a->info.np[0];
            freeNode(l1a->info.np[1]);
            freeNode(l1a);
            l1a = sa->info.np[1];
            sa->info.np[1] = l1a->info.np[0];
            freeNode(l1a->info.np[1]);
            freeNode(l1a);
        }
    }

    switch (sa->a_c0)
    {
    case IDOP:
        return sub_4DA3(sa);
    case DOT:
        return sub_4E8D(sa);
    case NOT:
        return sub_4C8B(sa);
    case LT:
    case GEQ:
        if (sub_14F3(sa->info.np[0]) == 2 && sub_4BE5(sa->info.np[1]))
        {
            warningMsg = "degenerate unsigned comparision";
            sub_475C(sa);
            sa = sub_415E(sa->a_c0 == GEQ ? 1L : 0L);
        }
        break;
    case LEQ:
    case GT:
        if (sub_14F3(sa->info.np[0]) == 2 && sub_4BE5(sa->info.np[1]))
            sa->a_c0 = sa->a_c0 == GT ? NEQL : EQL;
        else if (sa->info.np[1]->a_c0 == CONST)
        {
            sa->info.np[1]->info.l += 1;
            sa->a_c0 = sa->a_c0 == GT ? GEQ : LT;
        }
        else
        {
            l1a = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l1a;
            sa->a_c0 = sa->a_c0 != GT ? GEQ : LT;
        }
        break;
    case ASEOR:
    case BOR:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == -1L)
        {
            sub_475C(sa->info.np[0]);
            freeNode(sa);
            return sa->info.np[1];
        }
        /* fall through */
    case ADD:
    case SUB:
    case LSHIFT:
    case ASADD:
    case ASSUB:
    case ASLSHIFT:
    case ASRSHIFT:
    case RSHIFT:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 0L)
            return sub_4FA8(sa);

        if ((dopetab[sa->a_c0] & 0x80) && nodesize(sa->info.np[1]) != 1)
        {
            l1a = allocNode();
            l1a->pm = sub_14F3(sa->info.np[1]) == 2 ? typeUChar : typeChar;
            l1a->a_c0 = TYPE;
            sa->info.np[1] = sub_43EF(CONV, sa->info.np[1], l1a);
        }
        break;
    case MUL:
    case DIV:
    case ASMUL:
    case ASDIV:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 1L)
            return sub_4FA8(sa);
        /* fall through */
    case MOD:
    case ASMOD:
        if (sa->info.np[1]->a_c0 == CONST && sub_46F7(sa->info.np[1]->info.l) != 0)
            return sub_4FCE(sa);
        /* fall through */
    case BAND:
    case ASAND:
        if (sa->info.np[1]->a_c0 == CONST && sa->info.np[1]->info.l == 0L)
            switch (sa->a_c0)
            {
            case MOD:
            case DIV:
            case ASMOD:
            case ASDIV:
                prWarning("Division by zero");
                return sa;
            case BAND:
            case MUL:
                sub_475C(sa->info.np[0]);
                sub_475C(sa->info.np[1]);
                sa->a_c0 = CONST;
                sa->info.l = 0;
                return sa;
            case ASAND:
            case ASMUL:
                sa->a_c0 = ASSIGN;
                return sa;
            }
        l4 = nodesize(sa) >= 4 ? -1L : (1L << (nodesize(sa) * 8)) - 1;
        if ((sa->a_c0 == BAND || sa->a_c0 == ASAND) && sa->info.np[1]->a_c0 == CONST &&
            l4 == (sa->info.np[1]->info.l & l4))
            return sub_4FA8(sa);
        break;
    case CONV:
        return sub_508A(sa);
    case PLUS_U:
        freeNode(sa);
        sa = sa->info.np[0];
        break;
    case GADDR:
        if (sa->info.np[0]->a_c0 == MUL_U)
            return sub_53EE(sa);
        break;
    case MUL_U:
        if (sa->info.np[0]->a_c0 == GADDR)
        {
            l1a = sa->info.np[0]->info.np[0];
            if (l1a->a_c0 != LPAREN && l1a->a_c0 != ASSIGN && l1a->a_c0 != QUEST)
                return sub_53EE(sa);
        }
        break;
    }
    return sa;
}

/* end of file local1s.c */
/*
 * File - local2.c
 */

/*********************************************************
 * sub_5CF5 OK++ PMO			Used in: leaveBlock, sub_17E0
 *
 *********************************************************/
void sub_5CF5(register member_t *sb, int p2)
{

    switch (sb->b_flag)
    {
    case 1:
        if (sub_61AA(sb, p2) == 0)
        {
            array_AE13[lvlidx] -= sb->b_size;
            if ((sb->b_off = array_AE13[lvlidx]) < -128 && sb->b_size <= 4)
                prWarning("%s: large offset", sb->b_name);
        }
        break;
    case 5:
        sb->b_off = array_AE97[lvlidx];
        array_AE97[lvlidx] += sb->b_size;
        if (sb->b_size == 1 && sb->b_type->b_class != STRUCT && sb->b_type->b_class != UNION)
            array_AE97[lvlidx]++;

        if (!sub_61AA(sb, p2))
            sb->b_flag = 1;
        break;
    case 3:
    case 4:
        sub_C57(sb);
        break;
    }
}

/*********************************************************
 * sub_5DF6 OK++ PMO			Used in: sub_600E
 * parameter 1 of sub_43EF optimised
 *********************************************************/
node_t *sub_5DF6(register node_t *sa)
{
    char l1;
    char *l2;

    l1 = (char)(dopetab[sa->a_c0] & 0xC);
    if (l1 == 8)
        sa->info.np[1] = sub_5DF6(sa->info.np[1]);
    if (l1 != 0)
        sa->info.np[0] = sub_5DF6(sa->info.np[0]);

    switch (sa->a_c0)
    {
    case SUB:
    case ASSUB:
        if (sa->info.np[1]->a_c0 == CONST)
            sa->info.np[1]->info.l = -sa->info.np[1]->info.l;
        else
            sa->info.np[1] = sub_43EF(MINUS_U, sa->info.np[1], 0);
        sa->a_c0 = sa->a_c0 == SUB ? ADD : ASADD;
        break;
    case MINUS_U:
        if (sa->info.np[0]->a_c0 == MINUS_U)
        {
            freeNode(sa);
            sa = sa->info.np[0];
            freeNode(sa);
            sa = sa->info.np[0];
        }
        if (sa->info.np[0]->a_c0 == FCONST)
        {
            freeNode(sa);
            sa = sa->info.np[0];
            l2 = (char *)allocMem(strlen(sa->info.sv.s) + 2);
            strcat(strcpy(l2, "-"), sa->info.sv.s);
            free(sa->info.sv.s);
            sa->info.sv.s = l2;
        }
    }
    return sa;
}

/*********************************************************
 * sub_5F52 OK++ PMO			Used in: sub_600E
 *********************************************************/
node_t *sub_5F52(register node_t *sa)
{
    char l1;
    node_t *l2;

    l1 = (char)(dopetab[sa->a_c0] & 0xC);
    if (l1 == 8)
        sa->info.np[1] = sub_5F52(sa->info.np[1]);
    if (l1 != 0)
        sa->info.np[0] = sub_5F52(sa->info.np[0]);

    switch (sa->a_c0)
    {
    case ADD:
        if (sa->info.np[0]->a_c0 == MINUS_U)
        {
            l2 = sa->info.np[0];
            sa->info.np[0] = sa->info.np[1];
            sa->info.np[1] = l2;
        }
        /* fall through */
    case ASADD:
        if (sa->info.np[1]->a_c0 == MINUS_U)
        {
            freeNode(sa->info.np[1]);
            sa->info.np[1] = sa->info.np[1]->info.np[0];
            sa->a_c0 = sa->a_c0 == ASADD ? ASSUB : SUB;
        }
        /* fall through */
    }

    return sa;
}

/*********************************************************
 * sub_600E OK++ PMO     Used in: sub_793,  sub_808,  sub_E43,
 *                            sub_17E0, sub_19C1, sub_1B0C,
 *			      sub_3DC9
 *********************************************************/
node_t *sub_600E(register node_t *sa)
{

    /* int l1; Not used */

    warningMsg = 0;
    sa = sub_5DF6(sa);
    /* l1    = 0; Not used */
    do
    {
        byte_B013 = false;
        /*
              Excluded part optimization code
              present in the DOS version (3.06)
        */
        sa = sub_3EAA(sub_54B6(sa));
    } while (byte_B013);

    if (warningMsg)
        prWarning(warningMsg);

    return sub_5F52(sa);
}

/*********************************************************
 * sub_605E OK++ PMO				Used in: sub_4C8B
 *
 *********************************************************/

int invertTest(int op)
{

#ifdef DEBUG
    printf("\tsub_605E(%d)\n", op);
#endif

    switch (op)
    {
    case EQL:
        return NEQL;
    case NEQL:
        break;
    case LT:
        return GEQ;
    case LEQ:
        return GT;
    case GEQ:
        return LT;
    case GT:
        return LEQ;
    }
    return EQL;
}

/*********************************************************
 * sub_60A8 OK++ PMO				Used in: sub_54B6
 *
 *********************************************************/
bool sub_60A8(register node_t *sa, node_t *p2a)
{
    long l1;

    if (sa->a_c0 != 'C')
        return 0;
    if (nodesize(p2a) >= 4)
        return (bool)(sa->info.l >= 0 || sub_14F3(p2a) == 1);

    l1 = (uint16_t)(1 << (nodesize(p2a) * 8));
    if (sub_14F3(p2a) == 2)
    {
        if (sa->info.l < 0)
            return 0;
    }
    else
    {
        l1 /= 2;
        if (sa->info.l < 0)
            return (bool)(sa->info.l >= -l1);
    }
    return (bool)(sa->info.l < l1);
}

/*********************************************************
 * sub_61AA OK++ PMO			Used in: sub_5CF5
 *********************************************************/
bool sub_61AA(register member_t *sb, int p2)
{

    if (!rflag && isupper(p2) && sb->b_refl == 1 && sb->b_nelem <= 1)
    {
        if (sub_6589(word_B017, 9) == 0)
            return false;
        if (sb->b_flag == 5)
            sb->b_sloc |= 4;
        sb->b_flag = 2;
        sb->b_memb.i = (int16_t)sub_6589(word_B017, 9);
        word_B017 &= ~array_AAE8[sb->b_memb.i];
        return true;
    }

    return false;
}

/*********************************************************
 * sub_6246 OK++ PMO	      Used in: sub_508A, sub_54B6
 *********************************************************/
bool sub_6246(node_t *p1a, node_t *p2a)
{

    return nodesize(p1a) == nodesize(p2a) && sub_14F3(p1a) == sub_14F3(p2a);
}

/*********************************************************
 * sub_628F OK++ PMO
 *********************************************************/
bool sub_628F(node_t *p1a, node_t *p2a)
{
    return sub_14F3(p1a) == 1 && sub_14F3(p2a) == 1;
}

/*********************************************************
 * sub_62BE OK++ PMO			Used in: sub_54B6
 *********************************************************/
bool sub_62BE(register node_t *sa, node_t *p2a)
{
    long l1, l2;
    char l3;

    l3 = (char)(nodesize(sa->info.np[0]) * 8);
    if (l3 >= 32)
        return true;
    l1 = 1L << l3;
    l2 = 0;
    if (sub_628F(sa, sa->info.np[0]))
    {
        l1 /= 2;
        l2 = -l1;
    }
    return (bool)(l2 <= p2a->info.l && p2a->info.l < l1);
}

/* macros to make reading the code easier */
/* clang-format off */
/* don't reformat as hitech cpp includes the extra spaces
   which can cause line overflow
*/
#ifndef TOPBIT
#define TOPBIT 0x8000
#define TopBitSet(n) ((n)&TOPBIT)
#define MapVal(n) (TopBitSet(n)?~(n):array_AAE8[n])
#endif
/* clang-format on */
/*********************************************************
 * sub_63B8 OK++ PMO				Used in: sub_1F4B
 *
 *********************************************************/
uint8_t sub_63B8(int p1, int p2, int p3)
{
    uint8_t l1;
    uint8_t l0;
    int l2;
    register uint8_t *st;

    p3 &= ~0x40;
    l1 = 0;

    if (p2 == 0)
        l1 = sub_6589(p1, p3);
    else if (p2 & 0x8000)
        l1 = sub_6589(MapVal(p2) & p1, p3);
    else if (p3 < 24)
    {
        if ((l1 = sub_6589(MapVal(p2) & p1, p3)) == 0 && (l2 = sub_6589(p1, p3)) != 0 &&
            sub_6589(MapVal(l2), p2) != 0)
            l1 = (uint8_t)l2;
    }
    else
    {
        l2 = 6;
        st = &array_AB54[(p3 - 24) * 6];
        while (*st && l2-- != 0)
            if ((l0 = sub_6589(p1, *st++)) != 0 &&
                (sub_6589(array_AAE8[l0], p2) != 0 || sub_6589(array_AAE8[p2], l0) != 0))
            {
                l1 = l0;
                break;
            }
    }
    return l1;
}

/*********************************************************
 * sub_6589 OK++ PMO    Used in: sub_1F4B, sub_2B79, sub_61AA,
 *			     sub_63B8, sub_66BC
 * Optimiser improves on
 *      ld a,(ix-1) ! add a,255 ! ld (ix-1),a ! or a
 * with dec (ix-1)
 *********************************************************/
uint8_t sub_6589(int p1, int p2)
{
    char l1;
    uint8_t *l2;

    p2 &= ~0x40; /* Clear bit 6 */
    if (p2 < 24)
    {
        if ((p1 & array_AAE8[p2]) == array_AAE8[p2])
            return (uint8_t)p2;
        else
            return 0;
    }
    p2 -= 24;
    l1 = 6;
    l2 = &array_AB54[p2 * 6];
    do
    {
        if (*l2 != 0 && (p1 & array_AAE8[*l2]) == array_AAE8[*l2])
            return *l2;
    } while (*++l2 && --l1 != 0);
    return 0;
}

/*********************************************************
 * sub_665B OK++ PMO			Used in: sub_66BC
 *********************************************************/
uint16_t sub_665B(uint16_t p1, uint8_t p2)
{
    uint16_t loc;

    if (p1 == 0)
        return 0;
    p2 -= 48;
    for (loc = 1; loc < 24; loc++)
        if (array_AB24[loc * 2 + p2] == p1)
            return loc;
    return 0;
}

/*********************************************************
 * sub_66BC OK++ PMO			Used in: sub_1F4B
 * Optimiser passes param 2 as uint8_t rather than promoting
 * to int
 *********************************************************/

int sub_66BC(int p1, int p2, int p3, char *p4)
{
    int16_t l1, retval;
    char *l3;

    retval = 0;
    if ((p2 & 0x8000) != 0)
    {
        if (p1 == 0)
            retval = p2;
        else if ((l1 = sub_6589(((p2 & 0x8000) ? ~p2 : array_AAE8[p2]) & p3, p1)) != 0)
            retval = l1;
        else
            retval = sub_6589(p3, p1);
    }
    else if (p1 == 0)
    {
        if (p2 == 0 || p4 == 0)
            retval = p2;
        else
        {
            for (l3 = p4; isdigit(l3[1]); l3++)
                ;
            if (p2 >= 24 && (p2 = sub_6589(p3, p2)) == 0)
                retval = 0;
            else
            {
                do
                {
                    p2 = sub_665B((uint16_t)p2, (uint8_t)*l3);
                } while (l3-- != p4);
                retval = p2;
            }
        }
    }
    else if (p2 == 0)
        retval = p1;
    else
    {
        l1 = (int16_t)(((p1 & 0x8000) ? ~p1 : array_AAE8[p1]) & ((p2 & 0x8000) ? ~p2 : array_AAE8[p2]));
        if (((p1 & 0x8000) ? ~p1 : array_AAE8[p1]) == l1)
            retval = p1;
        else if (((p2 & 0x8000) ? ~p2 : array_AAE8[p2]) == l1)
            retval = p2;
        else
            retval = sub_6589(l1, p1);
    }
    return retval;
}
/* end of local2.c*/

