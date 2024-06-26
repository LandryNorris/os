/*             Author: Benjamin David Lunt
 *                     Forever Young Software
 *                     Copyright (c) 1984-2022
 *
 *  This code is donated to the Freeware communitee.  You have the
 *   right to use it for learning purposes only.  You may not modify it
 *   for redistribution for any other purpose unless you have written
 *   permission from the author.
 *
 *  You may modify and use it in your own projects as long as they are
 *   for non-profit only and if distributed, have these same requirements.
 *  Any project for profit that uses this code must have written
 *   permission from the author.
 *
 *  For more information:
 *    http://www.fysnet.net/osdesign_book_series.htm
 *  Contact:
 *    fys [at] fysnet [dot] net
 *
 * Last update:  4 May 2022 (May the 4th be with you!)
 *  (What is the core temperature of a Tauntaun?  --  Luke warm)
 *
 */

// NOLINTBEGIN

/*
 * Modified for use with this OS. Main modifications include removing spinlock (this is
 * a single-threaded OS currently) and changing ctypes (this OS uses stdint).
 *
 * I also added the NOLINT comments for the linter
 */

#include <string.h>
#include <stdio.h>
#include "bucket.h"
#include "pmm.h"
#include "paging.h"

#define MODNAME "malloc.cpp"

void* kernel_heap = NULL;

// initialize the heap by simply allocating a single Bucket
//  with a single Pebble in it.  The pebble will be free.
void* initializeKernelHeap(size_t size) {
    struct S_MEMORY_BUCKET *bucket = create_bucket(size);

    return bucket;
}

// allocates a linear block of memory, in 'size' bytes, and creates
//  a Bucket for this block, with one (free) Pebble.
struct S_MEMORY_BUCKET *create_bucket(size_t size) {

    // do we allocate a minimum?
#ifdef ALLOC_MIN
    if (size < ALLOC_MIN)
    size = ALLOC_MIN;
#endif

    // size must be an even number of pages
    size = (size + (PAGE_SIZE - 1)) & ~(PAGE_SIZE - 1);

    struct S_MEMORY_BUCKET *bucket = (struct S_MEMORY_BUCKET *) mmap(NULL, size / PAGE_SIZE);
    if (bucket != NULL) {
        bucket->magic = MALLOC_MAGIC_BUCKET;
        bucket->lflags = BUCKET_FLAG_FIRST;
        bucket->size = size / PAGE_SIZE;  // count of pages used
        bucket->largest = size - sizeof(struct S_MEMORY_BUCKET) - sizeof(struct S_MEMORY_PEBBLE);

        bucket->prev = NULL;  // these will be assigned by the insert_bucket() call
        bucket->next = NULL;

        struct S_MEMORY_PEBBLE *first = (struct S_MEMORY_PEBBLE *) ((uint8_t *) bucket + sizeof(struct S_MEMORY_BUCKET));
        bucket->first = first;

        first->magic = MALLOC_MAGIC_PEBBLE;
        first->sflags = 1;
        first->lflags = PEBBLE_FLAG_FREE;
        first->padding = 0;
        first->size = bucket->largest;
#ifdef MEM_USE_DEBUGNAME
        memset(first->name, 0, MAX_DEBUGNAME);
#endif
        first->parent = bucket;
        first->prev = NULL;
        first->next = NULL;
    }

    return bucket;
}

// insert a bucket at destination
void insert_bucket(struct S_MEMORY_BUCKET *bucket, void *destination) {
    struct S_MEMORY_BUCKET *dest = (struct S_MEMORY_BUCKET *) destination;

    if (bucket && dest)
        UPDATE_NODE(bucket, dest);
}

// remove a bucket
void remove_bucket(struct S_MEMORY_BUCKET *bucket) {

    // don't remove the initial bucket
    if (bucket && (bucket != kernel_heap)) {
        if (bucket->prev)
            bucket->prev->next = bucket->next;
        if (bucket->next)
            bucket->next->prev = bucket->prev;
        munmap(bucket, bucket->size);
    }
}

// run through the bucket and get the (possibly) new largest size
size_t bucket_update_largest(struct S_MEMORY_BUCKET *bucket) {
    struct S_MEMORY_PEBBLE *p = bucket->first;
    size_t ret = 0;

    while (p != NULL) {
        if (p->size > ret)
            ret = p->size;
        p = p->next;
    }

    // update the value
    bucket->largest = ret;

    return ret;
}

// this takes an already created pebble and tries to place it in a bucket
// it is assumed that the caller has already checked that this bucket
//  isn't full and can hold the pebble, though we check anyway.
struct S_MEMORY_PEBBLE *place_pebble(struct S_MEMORY_BUCKET *bucket, struct S_MEMORY_PEBBLE *pebble) {
    struct S_MEMORY_PEBBLE *start = bucket->first;
    struct S_MEMORY_PEBBLE *best = NULL;
    size_t best_size = -1;

    if (bucket->lflags & BUCKET_FLAG_BEST) {
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
        // BEST FIT method
        // scroll through all the pebbles until we find a free one
        //  large enough to insert our pebble, but the least sized free
        //  entry that satisfies our request.
        while (start != NULL) {
            if (PEBBLE_IS_FREE(start) && (start->size <= pebble->size)) {
                if (start->size < best_size) {
                    best = start;
                    best_size = start->size;
                }
            }
            start = start->next;
        }
        // did we find one? Do we need to split it?
        if (best != NULL) {
            split_pebble(best, pebble->size);
            best->sflags = pebble->sflags;
            best->lflags = pebble->lflags;
#ifdef MEM_USE_DEBUGNAME
            memcpy(best->name, pebble->name, MAX_DEBUGNAME + 1);
#endif
        }
        start = best;
    } else {
        // -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
        // FIRST FOUND method
        // scroll through the pebbles until we find a free one
        //  large enough to insert our pebble in.  First one found, we use.
        while (start != NULL) {
            if (PEBBLE_IS_FREE(start) && (start->size >= pebble->size)) {
                // we found one to use.  Do we need to split it?
                split_pebble(start, pebble->size);
                start->sflags = pebble->sflags;
                start->lflags = pebble->lflags;
#ifdef MEM_USE_DEBUGNAME
                memcpy(start->name, pebble->name, MAX_DEBUGNAME + 1);
#endif
                break;
            }
            start = start->next;
        }
    }

    return start;
}

// if the current pebble is large enough, will split a pebble into two
// else it returns NULL
struct S_MEMORY_PEBBLE *split_pebble(struct S_MEMORY_PEBBLE *pebble, size_t size) {
    struct S_MEMORY_PEBBLE *new_pebble = NULL;
    size_t new_size;

    if (SPLIT_PEBBLE(pebble->size, size)) {
        new_size = (size + (PEBBLE_MIN_ALIGN - 1)) & ~(PEBBLE_MIN_ALIGN - 1);
        new_pebble = (struct S_MEMORY_PEBBLE *) ((uint8_t *) pebble + sizeof(struct S_MEMORY_PEBBLE) + new_size);
        memcpy(new_pebble, pebble, sizeof(struct S_MEMORY_PEBBLE));
        new_pebble->size = pebble->size - new_size - sizeof(struct S_MEMORY_PEBBLE);
        new_pebble->prev = pebble;
        pebble->size = new_size;
        pebble->next = new_pebble;
    }

    return new_pebble;
}

// if this pebble is empty *and* if present, the next one is empty,
//  then absorb the next one, into this one.
struct S_MEMORY_PEBBLE *absorb_next(struct S_MEMORY_PEBBLE *pebble) {
    if (pebble && pebble->next) {
        if (PEBBLE_IS_FREE(pebble) && PEBBLE_IS_FREE(pebble->next)) {
            if (pebble->parent->first == pebble->next)  // don't "delete" the Bucket->first pebble before we update it
                pebble->parent->first = pebble;
            pebble->size += pebble->next->size + sizeof(struct S_MEMORY_PEBBLE);
            pebble->next = pebble->next->next;
            if (pebble->next)
                pebble->next->prev = pebble;
            bucket_update_largest(pebble->parent);
        }
    }
    return pebble;
}

// if this pebble is empty, *and* if present the last one is empty,
//  then let the last one absorb this one.
struct S_MEMORY_PEBBLE *melt_prev(struct S_MEMORY_PEBBLE *pebble) {
    if (pebble && pebble->prev) {
        if (PEBBLE_IS_FREE(pebble) && PEBBLE_IS_FREE(pebble->prev)) {
            if (pebble->parent->first == pebble)  // don't "delete" the Bucket->first pebble before we update it
                pebble->parent->first = pebble->prev;
            pebble->prev->size += pebble->size + sizeof(struct S_MEMORY_PEBBLE);
            pebble->prev->next = pebble->next;
            if (pebble->next)
                pebble->next->prev = pebble->prev;
            pebble = pebble->prev;
            bucket_update_largest(pebble->parent);
        }
    }
    return pebble;
}

// shrink the pebble from the current size to a new smaller size
//  if the size is now small enough to split the pebble, we do it
struct S_MEMORY_PEBBLE *shrink_pebble(struct S_MEMORY_PEBBLE *pebble, size_t size) {
    struct S_MEMORY_PEBBLE *ret = NULL;

    if (pebble) {
        split_pebble(pebble, size);
        ret = pebble;
    }

    return ret;
}

#ifdef MALLOC_DEBUG
void malloc_dump(void* bucket) {
  struct S_MEMORY_BUCKET *start = (struct S_MEMORY_BUCKET *) bucket;
  struct S_MEMORY_PEBBLE *pebble, *prev;
  int i, j;

  printf("\n\n");
  i = 0;
  while (start != NULL) {
    printf("Bucket: #%d:  0x%x\n", i, start);
    printf("         Magic: 0x%x\n", start->magic);
    if (start->magic != MALLOC_MAGIC_BUCKET)
      break;
    printf("   local flags: 0x%x\n", start->lflags);
    printf("  size (pages): %'d\n", start->size);
    printf("largest pebble: %'d\n", start->largest);
    printf("   prev bucket: %'x\n", start->prev);
    printf("   next bucket: %'x\n", start->next);
    printf("  first pebble: %'x\n", start->first);

    j = 0;
    prev = NULL;
    pebble = start->first;
    while (pebble) {
      printf("Pebble: #%d: 0x%x\n", j, pebble);
      printf("           Magic: 0x%x\n", pebble->magic);
      if (pebble->magic != MALLOC_MAGIC_PEBBLE)
        break;
      printf("    source flags: 0x%x\n", pebble->lflags);
      printf("     local flags: 0x%x\n", pebble->lflags);
      printf("         padding: 0x%x\n", pebble->padding);
      printf("    size (bytes): %d\n", pebble->size);
#ifdef MEM_USE_DEBUGNAME
      printf("            name: %s\n", pebble->name);
#endif
      printf("          parent: 0x%x  (%s)\n", pebble->parent, (pebble->parent == start) ? "good" : "error");
      printf("        previous: 0x%x  (%s)\n", pebble->prev, (pebble->prev == prev) ? "good" : "error");
      printf("            next: 0x%x\n", pebble->next);
      j++;
      prev = pebble;
      pebble = pebble->next;
    }

    start = start->next;
    i++;
  }
}
#endif

void *kmalloc(size_t size, uint64_t alignment, uint32_t flags, char *name) {
    void *ret = NULL;

    // minimum amount of memory we allocate to the caller
    if (size < PEBBLE_MIN_SIZE)
        size = PEBBLE_MIN_SIZE;

    struct S_MEMORY_PEBBLE pebble;
    pebble.magic = MALLOC_MAGIC_PEBBLE;
    pebble.sflags = flags;
    pebble.lflags = PEBBLE_FLAG_IN_USE;
    pebble.padding = 0;
    pebble.size = (size + (PEBBLE_MIN_ALIGN - 1)) & ~(PEBBLE_MIN_ALIGN - 1);
#ifdef MEM_USE_DEBUGNAME
    strcpy(pebble.name, name); //modified to use strcpy, since this kernel doesn't support strncpy yet.
#endif

    struct S_MEMORY_BUCKET *bucket = (struct S_MEMORY_BUCKET *) kernel_heap;
    while (bucket != NULL) {
        if (bucket->largest >= pebble.size) {
            ret = place_pebble(bucket, &pebble);
            bucket_update_largest(bucket);
            if (ret != NULL)
                ret = (uint8_t *) ret + sizeof(struct S_MEMORY_PEBBLE);
            break;
        }
        bucket = bucket->next;
    }

    // if ret == NULL, we didn't find a bucket large enough, or with enough empty space.
    //  so allocate another bucket
    if (ret == NULL) {
        size_t new_size = pebble.size + (sizeof(struct S_MEMORY_BUCKET) + sizeof(struct S_MEMORY_PEBBLE));
        bucket = create_bucket(new_size);
        if (bucket) {
            insert_bucket(bucket, kernel_heap);
            ret = place_pebble(bucket, &pebble);
            bucket_update_largest(bucket);
            if (ret != NULL)
                ret = (uint8_t *) ret + sizeof(struct S_MEMORY_PEBBLE);
        }
    }

    // if we are to clear the memory, do it now
    if (ret && (flags & (1 << 2)))
        memset(ret, 0, size);

    return ret;
}

void *realloc(void *ptr, size_t size) {
    struct S_MEMORY_PEBBLE *pebble;
    void *ret = NULL;

    if (size == 0) {
        mfree(ptr);
        return NULL;
    }

    if (ptr == NULL)
        return kmalloc(size, 0 /* not used */, 0, MODNAME);

    pebble = (struct S_MEMORY_PEBBLE *) ((uint8_t *) ptr - sizeof(struct S_MEMORY_PEBBLE));

    if (size <= pebble->size)
        ret = shrink_pebble(pebble, size);
    else {
        if (pebble->sflags & (1 << 2))
            ret = NULL;
        else {
            // the new requested size is larger than the current pebble, so allocate a new space
            ret = kmalloc(size, 0 /* not used */, pebble->sflags, MODNAME);
            if (ret)
                memcpy(ret, ptr, size);
            mfree(ptr);
        }
    }

    return ret;
}

// free a pebble, possibly merging it with a neighbor(s), and possible removing this
//  now empty Bucket.
void mfree(void *ptr) {

    if (ptr == NULL)
        return;

    struct S_MEMORY_PEBBLE *pebble = (struct S_MEMORY_PEBBLE *) ((uint8_t *) ptr - sizeof(struct S_MEMORY_PEBBLE));

    // check that it actually is a pebble
    if (pebble->magic != MALLOC_MAGIC_PEBBLE) {
        return;
    }

    // mark it as free
    pebble->lflags = PEBBLE_FLAG_FREE;

    // see if we can absorb any of the neighbors
    pebble = melt_prev(pebble);
    absorb_next(pebble);

    // if this empties the bucket, shall we remove the bucket?
    struct S_MEMORY_BUCKET *bucket = pebble->parent;
    if (PEBBLE_IS_FREE(bucket->first) && (bucket->first->prev == NULL) && (bucket->first->next == NULL))
        remove_bucket(bucket);
    else
        bucket_update_largest(bucket);
}

// NOLINTEND
