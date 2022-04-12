// plist.h

// declares functions in plist.c

// This prevents double-inclusions
#ifndef PLIST
#define PLIST

// Some useful librarys
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Implement booleans

#define TRUE 1
#define FALSE 0

typedef struct plist_struct *plist;

plist pl_new();

void pl_free(plist l);

void pl_print(plist l);

void pl_append(plist l, void *x);

void pl_extend(plist l1, plist l2);

void pl_insert(plist l, uint64_t i, void *x);

void pl_remove(plist l, void *x);

void *pl_pop(plist l, uint64_t i);

void pl_clear(plist l);

uint64_t pl_index(plist l, void *x);

uint64_t pl_count(plist l, void *x);

void pl_reverse(plist l);

plist pl_copy(plist l);

void pl_tail_remove(plist l);

void pl_tail_append(plist l, void *x);


#endif
