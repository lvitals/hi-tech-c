#include "cgen.h"

#ifdef CPM

extern int pack(void);
extern void *sbrk(int);
extern uint8_t *setSize(register uint8_t *p, short size);

#ifndef Link
#define Link(p) (*(uint8_t **)(p))
#endif

#ifndef Len
#define Len(p) (*(uint16_t *)((p) - ( ((uint8_t *)(p))[-1] & 0x80 ? 3 : 1 )))
#endif

#ifndef BlkAddr
#define BlkAddr(p) ((uint8_t *)(p) - ( ((uint8_t *)(p))[-1] & 0x80 ? 3 : 1 ))
#endif

static uint8_t *freeList;

void *malloc(size_t size)
{
    uint8_t *l1;
    size_t l2;
    int done;  /* bool não parece estar definido */
    register uint8_t *sst;

    if (size < sizeof(uint8_t *))
        size = sizeof(uint8_t *);
    done = 0;  /* false não definido */
    
    do
    {
        for (sst = (uint8_t *)&freeList; (l1 = Link(sst)) != 0; sst = l1)
        {
            if (Len(l1) >= size)
            {
                Link(sst) = Link(l1);
                sst = l1;
                /* if splitting the block would leave sufficient space
                 * for a new block i.e. header + data/pointer then split it */
                if ((l2 = Len(sst)) > size + 2 + sizeof(uint8_t *))
                {
                    l1 = sst + size;
                    free(setSize(l1, (short)(l2 - size)));
                    sst = BlkAddr(sst);
                    return setSize(sst, (short)size);
                }
                else
                    return sst;
            }
        }
    } while (!done && (done = pack()));
    
    l2 = size < 128 ? 1 : 3; /* size of block header */
    if ((sst = sbrk((int)(size + l2))) == (uint8_t *)-1)
        return 0;

    return setSize(sst, (short)(size + l2));
}

void free(void *p)
{
    register uint8_t *st;

    for (st = (uint8_t *)&freeList; 
         Link(st) != 0 && Len(Link(st)) < Len((uint8_t *)p); 
         st = Link(st))
        ;
    
    Link(p) = Link(st);
    Link(st) = (uint8_t *)p;
}

#endif /* CPM */