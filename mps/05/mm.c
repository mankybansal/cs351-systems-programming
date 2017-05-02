/*
 *  MPS/05 - DYNAMIC MEMORY ALLOCATOR
 *
 *  NAME: Mayank Bansal
 *  EMAIL: mbansal5@hawk.iit.edu
 *
 *  WARNING: The code in this repo can potentially make you cry,
 *  Wilbur the safety pig is provided below for your benefit.
 *  Copy Wilbur the safety pig, wrap him in a comment and hope all goes well.
 *                             _
 *     _._ _..._ .-',     _.._(`))
 *    '-. `     '  /-._.-'    ',/
 *       )         \            '.
 *      / _    _    |             \
 *     |  a    a    /              |
 *     \   .-.                     ;
 *      '-('' ).-'       ,'       ;
 *         '-;           |      .'
 *            \           \    /
 *            | 7  .__  _.-\   \
 *            | |  |  ``/  /`  /
 *           /,_|  |   /,_/   /
 *              /,_/      '`-'
 *
 */

/*****************************
 * DMA WITH BEST-FIT DESIGN
 * AND SEGRAGATED STORAGE USING
 * DOUBLY LINKED LIST
 * PERF/INDEX = 90%
 *****************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "mm.h"
#include "memlib.h"

//ALIGNMENT HELPER & MACROS
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)
#define BLK_HDR_SIZE ALIGN(sizeof(blockHDR))
#define BLK_FTR_SIZE ALIGN(sizeof(blockFTR))
#define BLK_META_SIZE BLK_HDR_SIZE + BLK_FTR_SIZE
#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

//LINKED LIST HELPER MACROS
#define PNEXT(p, points) (p->next = points)
#define PPREV(p, points) (p->prev = points)
#define PSIZE(p, newSize) (p->size = newSize)
#define APNEXT(p, points) (p.next = points)
#define APPREV(p, points) (p.prev = points)
#define APSIZE(p, newSize) (p.size = newSize)
#define PCOMP(p) (p->size & ~1)
#define PHEAD(p, points) (((blockFTR *) ((char *) p + PCOMP(p) - BLK_FTR_SIZE))->head = points)
#define PDELT(p) {PNEXT(p->prev, p->next); PPREV(p->next, p->prev); }

typedef struct header blockHDR;
typedef struct footer blockFTR;

blockHDR *find_fit(size_t size);

struct header {
    size_t size;
    blockHDR *next;
    blockHDR *prev;
};

struct footer {
    blockHDR *head;
};

//BIN INFO & HELPERS
#define BIN_COUNT 16
struct header *bins;
int i, *binSize;

int getBin(size_t size) {
    for (i = 0; i < BIN_COUNT; i++)
        if (size <= binSize[i]) return i;
    return BIN_COUNT - 1;
}

/***********************
 * INIT MEMORY PACKAGE *
 ***********************/

int mm_init(void) {

    bins = mem_sbrk(BIN_COUNT * BLK_HDR_SIZE);
    binSize = mem_sbrk(BIN_COUNT * sizeof(int));

    binSize[0] = 8;
    for (i = 1; i < BIN_COUNT; i++)
        binSize[i] = binSize[i - 1] * 2;

    blockHDR *bp = mem_sbrk(BLK_META_SIZE);
    blockFTR *fp = (blockFTR *) ((char *) bp + BLK_HDR_SIZE);

    PNEXT(bp, PPREV(bp, bp));
    PSIZE(bp, (BLK_META_SIZE + 1));

    fp->head = bp;

    for (i = 0; i < BIN_COUNT; i++) {
        APNEXT(bins[i], APPREV(bins[i], &bins[i]));
        APSIZE(bins[i], BLK_META_SIZE + 1);
    }

    return 0;
}

/*************************
 * MALLOC USING FIND FIT *
 *************************/

void *mm_malloc(size_t size) {

    size_t newSize = ALIGN(size + SIZE_T_SIZE + BLK_FTR_SIZE);
    blockHDR *p;

    if ((p = find_fit(newSize))) {
        if (PCOMP(p) - newSize >= 7 + BLK_META_SIZE) {

            size_t oldSize = p->size;
            PSIZE(p, newSize);
            PHEAD(p, p);

            blockHDR *split = (blockHDR *) ((char *) p + PCOMP(p));
            PSIZE(split, oldSize - p->size);
            PHEAD(split, split);

            i = getBin(split->size - SIZE_T_SIZE - BLK_FTR_SIZE);
            PNEXT(split, bins[i].next);
            bins[i].next = PPREV(split->next, split);
            PPREV(split, &bins[i]);
        }

        PDELT(p);
        p->size |= 1;

    } else {
        blockHDR *last = (((blockFTR *) ((char *) mem_heap_hi() + 1 - BLK_FTR_SIZE))->head);
        if (!(last->size & 1)) {
            mem_sbrk(newSize - last->size);
            PSIZE(last, newSize | 1);
            PHEAD(last, last);
            PDELT(last);
            p = last;
        } else {
            if (!(p = mem_sbrk(newSize))) return NULL;
            else {
                PSIZE(p, newSize | 1);
                blockFTR *fp = (blockFTR *) ((char *) p + PCOMP(p) - BLK_FTR_SIZE);
                fp->head = p;
            }
        }
    }

    return (void *) ((char *) p + sizeof(size_t));
}

/********************
 * MM FREE FUNCTION *
 ********************/

void mm_free(void *ptr) {

    blockHDR *bp = (blockHDR *) ((char *) ptr - SIZE_T_SIZE),
            *prev = ((blockFTR *) ((char *) bp - BLK_FTR_SIZE))->head;

    PSIZE(bp, PCOMP(bp));

    if (!((prev->size) & 1)) {
        PDELT(prev);
        prev->size += bp->size;
        PHEAD(bp, prev);
        bp = prev;
    }

    blockHDR *next = (blockHDR *) ((char *) bp + PCOMP(bp));

    if (((void *) ((char *) bp + bp->size) <= mem_heap_hi()) && !((next->size) & 1)) {
        bp->size += next->size;
        PHEAD(bp, bp);
        PDELT(next);
    }

    blockHDR *front = &bins[getBin(bp->size - SIZE_T_SIZE - BLK_FTR_SIZE)];

    PPREV(bp, front);
    PNEXT(bp, front->next);
    PPREV(front->next, bp);
    PNEXT(front, bp);
}

/******************
 * REALLOC MEMORY *
 ******************/

void *mm_realloc(void *ptr, size_t size) {
    size_t newSize = ALIGN(size + SIZE_T_SIZE + BLK_FTR_SIZE);
    blockHDR *bp = (blockHDR *) ((char *) ptr - sizeof(size_t)),
            *next = (blockHDR *) ((char *) bp + (bp->size &= ~1));

    if (PCOMP(bp) > newSize)
        return ptr;

    if (((void *) ((char *) bp + bp->size) <= mem_heap_hi())) {
        if (!((next->size) & 1) && (next->size + PCOMP(bp) > newSize)) {
            PSIZE(bp, (bp->size + next->size) | 1);
            PHEAD(bp, bp);
            PDELT(next);
            return ptr;
        }
    } else {
        mem_sbrk(newSize - bp->size);
        PSIZE(bp, newSize);
        PHEAD(bp, bp);
        return ptr;
    }

    void *newPtr = mm_malloc(size);
    memcpy(newPtr, ptr, bp->size - SIZE_T_SIZE - BLK_FTR_SIZE);
    mm_free(ptr);

    return newPtr;
}

/************************
 * FIND FIT HELPER FUNC *
 ************************/

blockHDR *find_fit(size_t size) {
    blockHDR *bp;
    for (i = getBin(size), bp = bins[i].next; i < BIN_COUNT; i++, bp = bins[i].next) {
        while ((bp != &bins[i]) && (bp->size < size + SIZE_T_SIZE + BLK_FTR_SIZE))
            bp = bp->next;
        if (bp != &bins[i]) return bp;
    }
    return NULL;
}
