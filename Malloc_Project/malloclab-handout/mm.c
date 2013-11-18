/*
 * mm-naive.c - The fastest, least memory-efficient malloc package.
 * 
 * In this naive approach, a block is allocated by simply incrementing
 * the brk pointer.  A block is pure payload. There are no headers or
 * footers.  Blocks are never coalesced or reused. Realloc is
 * implemented directly using mm_malloc and mm_free.
 *
 * NOTE TO STUDENTS: Replace this header comment with your own header
 * comment that gives a high level description of your solution.
 * Name: Punit Mehta
 * ID  : 201101061
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>
#include <string.h>

#include "mm.h"
#include "memlib.h"

#define WSIZE 4 /* word size is equal to 4 bytes */
#define DSIZE 8 /* double word alignment so 8 bytes bouandry convention is followed */
#define CHUNKSIZE (1<<12) /* Maximum available Memory 2^12 bytes */
#define OVERHEAD 8 /* HEADER + FOOTER size = 8 bytes */

#define PACK(size, alloc)   ((size) | (alloc)) /* Setting the header or footer data */
#define GET(p)              (*(size_t *)(p)) /* Get the word from the addrees pointed by p */
#define PUT(p, val)         (*(size_t *)(p) = (val)) /* Puts the val in the word pointed by p */
#define GET_SIZE(p)         (GET(p) & ~0x7) /* Getting the size of the block whose header is pointed by p (just mask the last three bits)*/
#define GET_ALLOC(p)        (GET(p) & 0x1) /* Getting the allocation status (1 for allocated and 0 for free) of the block */
#define HDRP(bp)            ((char *)(bp) - WSIZE) /* Header pointer from the block pointer */
#define FTRP(bp)            ((char *)(bp) + GET_SIZE(HDRP(bp)) - DSIZE) /* footer pointer from  the block pinter */

#define NEXT_BLKP(bp) ((char *)(bp) + GET_SIZE(((char *)(bp) -WSIZE))) /* Pointer to the next block of p */
#define PREV_BLKP(bp) ((char *)(bp) - GET_SIZE(((char *)(bp) - DSIZE))) /* Pointer to the previous block to p */
#define MAX(a,b) ((a>b)?a:b) /* Getting maximum of a and b */

/* Macros used for explicit free list */
#define GET_PREV_FREE_BLKP(bp) (*(void **) (bp)) /* Pointer to the Previous free block of bp in the explicit free list */
#define GET_NEXT_FREE_BLKP(bp) (*(void **) (bp + WSIZE)) /* Pointer to the Next free block of bp in the explicit free list */
#define SET_PREV_FREE_BLKP(bp, prev) (*((void **)(bp)) = prev) /* Set the previous free block of the block pointed by bp in free list */
#define SET_NEXT_FREE_BLKP(bp, next) (*((void **)(bp + WSIZE)) = next) /* Set the next free block of the block pointed by bp in free list */

static void *extend_heap(size_t words);
static void *coalesce(void *bp);
static void place(void *bp, size_t asize);
static void *find_fit(size_t asize);
static void removeBlock(void *bp);
static void addBlock(void *bp);

/*********************************************************
 * NOTE TO STUDENTS: Before you do anything else, please
 * provide your team information in the following struct.
 ********************************************************/
team_t team = {
    /* Team name */
    "punit9462",
    /* First member's full name */
    "Punit Mehta (201101061)",
    /* First member's email address */
    "201101061@daiict.ac.in",
    /* Second member's full name (leave blank if none) */
    "",
    /* Second member's email address (leave blank if none) */
    ""
};

/* single word (4) or double word (8) alignment */
#define ALIGNMENT 8

/* rounds up to the nearest multiple of ALIGNMENT */
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~0x7)

#define SIZE_T_SIZE (ALIGN(sizeof(size_t)))

void *heap_start_ptr; /* Points to the starting address of the allocated heap just for initial requirement */
void* head_FreeList=NULL; /* Points to the head of the explicit free list , will be null when the heap is not created */

/* 
 * mm_init - initialize the malloc package.
 *
 */

int mm_init(void)
{
    if ((heap_start_ptr = mem_sbrk(4*WSIZE)) == (void *)-1) /* returns the pointer which will be the strating point of heap */
            return -1;
        PUT(heap_start_ptr, 0); /* Indicates 4 bytes padding at the starting of the heap */
        PUT(heap_start_ptr+WSIZE, PACK(OVERHEAD, 1)); /* Prologue (1) */
        PUT(heap_start_ptr+DSIZE, PACK(OVERHEAD, 1)); /* Prologue (2) */
        PUT(heap_start_ptr+WSIZE+DSIZE, PACK(0, 1)); /* Epilogue */
        heap_start_ptr += DSIZE;
        head_FreeList=NULL; /* Initial Free list Pointer NULL because no memory for payload is available at this point */
        if (extend_heap(CHUNKSIZE/WSIZE) == (void *)-1) /* Extends the heap to have blocks of memory (argument is number of words) */
                return -1;
        return 0;
}

/*
 * extend_heap - Extends the heap when the desired memory is not available
 *
 */
static void *extend_heap(size_t words) {
    char *bp;
    size_t size=words*WSIZE;
    size = ALIGN(size+SIZE_T_SIZE); /* Set the size of the block to follow double word Allignment */
 \
    if ((int)(bp = mem_sbrk(size)) == -1)
        return NULL;

    PUT(HDRP(bp), PACK(size, 0)); /* Sets the Header of the Block */
    PUT(FTRP(bp), PACK(size, 0)); /* Sets the Footer of the Block */
    PUT(HDRP(NEXT_BLKP(bp)), PACK(0, 1)); /* Epilogue has to be created at the end */
    return coalesce(bp); /* Coalescing the Free blocks (Immediate Coalescing) */
}

/*
 * addBlock - adds the block in the freelist according to its address
 *
 */
static void addBlock(void *bp) {
    void *current = head_FreeList;
    void *temp = current;
    void *prev = NULL;
    while (current != NULL && bp < current) { /* checks untill the free block to be added gets its appropriate position in the free list */
        prev = GET_PREV_FREE_BLKP(current); /* prev free block of the current freeblock */
        temp = current;                    /* saves the current free block pointer */
        current = GET_NEXT_FREE_BLKP(current); /* gets the next free block pointer */
    }

    /* Inserts bp in the Explicit Free list */
    SET_PREV_FREE_BLKP(bp, prev);
    SET_NEXT_FREE_BLKP(bp, temp);

    if (prev != NULL) {
        SET_NEXT_FREE_BLKP(prev, bp);
    }
    else { /* Insert bp before current free list head*/
        head_FreeList = bp;
    }
    if (temp != NULL) {
        SET_PREV_FREE_BLKP(temp, bp);
    }
}


/*
 * removeBlock - removes the block in the freelist (Adjustment of Pointers)
 *
 */
static void removeBlock(void *bp) {

    void *next = (void *) GET_NEXT_FREE_BLKP(bp);/* next free block of the current freeblock */
    void *prev = (void *) GET_PREV_FREE_BLKP(bp); /* previous free block of the current freeblock */
    if (prev == NULL) { /* List is not started yet */
        head_FreeList = next;
    } else {
        SET_NEXT_FREE_BLKP(prev, next);
    }

    if (next != NULL) { /* Not the end of list */
        SET_PREV_FREE_BLKP(next, prev);
    }
}

/*
 * coalesce - Merges two free blocks
 *
 */
static void *coalesce(void *bp) {
    size_t prev_alloc = GET_ALLOC(FTRP(PREV_BLKP(bp))); /* status of the previous block */
    size_t next_alloc = GET_ALLOC(HDRP(NEXT_BLKP(bp))); /* status of the next block */
    size_t size = GET_SIZE(HDRP(bp));
    if (prev_alloc && next_alloc) {
        addBlock(bp); /* Just add the free block in the free list */
        return bp;
    }
    else if (prev_alloc && !next_alloc) {
        removeBlock(NEXT_BLKP(bp)); /* remove the next free block */
        size += GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(bp), PACK(size, 0));
        PUT(FTRP(bp), PACK(size, 0));
        addBlock(bp); /* add the new merged free block in the list */
    }
    else if (!prev_alloc && next_alloc) {
        removeBlock(PREV_BLKP(bp));/* remove the previous free block */
        size += GET_SIZE(HDRP(PREV_BLKP(bp)));
        PUT(FTRP(bp), PACK(size, 0));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        addBlock(bp); /* add the new merged free block in the list */
    }
    else { /* If next and previous both blocks are free */
        /* remove the both free block */
        removeBlock(PREV_BLKP(bp));
        removeBlock(NEXT_BLKP(bp));

        size+=GET_SIZE(HDRP(PREV_BLKP(bp)));
        size+=GET_SIZE(HDRP(NEXT_BLKP(bp)));
        PUT(HDRP(PREV_BLKP(bp)), PACK(size, 0));
        PUT(FTRP(NEXT_BLKP(bp)), PACK(size, 0));
        bp = PREV_BLKP(bp);
        addBlock(bp);/* add the new merged block in the list */
    }
    return bp; /* Pointer to the newly merged free block */
}

/* 
 * mm_malloc - Allocate a block by incrementing the brk pointer.
 *     Always allocate a block whose size is a multiple of the alignment.
 */
void *mm_malloc(size_t size){

        size_t asize, extendsize; 	 /* adjusted block size */
        char *bp;                  /* amount to extend heap if no fit */
        if (size <= 0) return NULL;  /* not applicable */
        if (size <= DSIZE)
             asize = DSIZE+OVERHEAD; /* 8+8=16 bytes block where minimum payload=8 bytes */
        else
             asize = ALIGN(size+SIZE_T_SIZE);
        if ((bp = find_fit(asize)) != NULL) { /* finds the first fit block in the heap */
            place(bp, asize); /* Places headers and footers in the block also checks whether splitting is needed or not */
            return bp;
        }

        /* Extends the Size of the current heap if no enough memory is available to satisfy the request */
        extendsize = MAX(asize,CHUNKSIZE);
        if ((bp = extend_heap(extendsize/WSIZE)) == NULL)
            return NULL;
        place(bp, asize);
        return bp;
}

/*
 * find_fit - Implements the routine to find the first fit block
 *
 */
static void *find_fit(size_t asize) {
    void *bp;

    /* Traverse throught the whole free list untill the first appropriate free block is found */
    for (bp = head_FreeList; bp!=NULL ; bp = GET_NEXT_FREE_BLKP(bp))
        if (asize <= GET_SIZE(HDRP(bp)))
        return bp;
    return NULL;    /* Returns NULL if it can not find the block of enough memory */
}

/*
 * place - Places the Headers and footers in the block , also performs spilliting if neccessary
 *
 */
static void place(void *bp, size_t asize) {
    size_t csize = GET_SIZE(HDRP(bp));

    /* Split the Block if the new created block size is greater than minimum block size(16 bytes) */
     if ((csize - asize) >= (DSIZE + OVERHEAD)) {
        removeBlock(bp);
        PUT(HDRP(bp), PACK(asize, 1));
        PUT(FTRP(bp), PACK(asize, 1));
        bp = NEXT_BLKP(bp);
        PUT(HDRP(bp), PACK(csize-asize, 0));
        PUT(FTRP(bp), PACK(csize-asize, 0));
        addBlock(bp);
   }

    /* Do not split , just allocate the block's header ans footer */
    else { /* Causes internal fragmentation */
         PUT(HDRP(bp), PACK(csize, 1));
         PUT(FTRP(bp), PACK(csize, 1));
         removeBlock(bp);
     }
}


/*
 * mm_free - Freeing a block does nothing.
 */
void mm_free(void *ptr)
{
    if(heap_start_ptr==0) mm_init(); /* If the heap is not intialised and free occurs */

    size_t size = GET_SIZE(HDRP(ptr));
    PUT(HDRP(ptr), PACK(size, 0));
    PUT(FTRP(ptr), PACK(size, 0));
    coalesce(ptr); /* Merge the block to the previous and/or next free block */
}

/*
 * mm_realloc - Implemented simply in terms of mm_malloc and mm_free
 */
void *mm_realloc(void *ptr, size_t size)
{
   
 if(ptr==NULL && size!=0){ /* If the ptr is NULL and size is nonzero then just do malloc */
        void* payload=mm_malloc(size);
        return payload;
    }

    if(size==0){ /* For zero size , perfoms only free */
        if(ptr==NULL) return NULL; /* Not applicable */
        mm_free(ptr);
        return ptr;
    }

    /* Realloc Implementation */
    void *old_bp = ptr;
    void *new_bp;
    size_t old_size;
    
    new_bp = mm_malloc(size); /* New block of new size */
    if (new_bp == NULL) /* If no enough memory available */
      return NULL;
    old_size = *(size_t *)((char *)old_bp - WSIZE); /* Old block Size */

    if (size < old_size)
      old_size = size;

    memcpy(new_bp, old_bp, old_size);
    mm_free(old_bp); /* Frees the old ptr (Block)*/

    return new_bp;

}	
