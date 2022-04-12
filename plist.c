// plist.c

// implements functions in plist.h
#include "plist.h"

struct plist_struct
{
    int size;
    int len;
	void **elements;
};


plist pl_new()
{
    plist new = (plist) malloc (sizeof(struct plist_struct));
    new->len = 0;
    new->size = 4;
    new->elements = (void **) malloc (sizeof(void*) * new->size);
	return new;
}

void pl_free(plist l)
{
    free(l);
}

void pl_print(plist l)
{
    int i;
    if (l->len == 0){
        printf("[]\n");
    } else {
        printf("[");
        for (i = 0; i < l->len - 1 ; i++)
        {
            int * val = (void *)l->elements[i];
            printf("%d, ", val);
        }
        int * val2 = (void *)l->elements[i];
        printf("%d]\n", val2);
        return;
    }
    
}

void pl_append(plist l, void *x)
{
	return pl_insert(l, l->len, x);
}

void pl_extend(plist l1, plist l2)
{
	for (int i = 0; i < l2->len; i++)
    {
        pl_append(l1, l2->elements[i]);
    }
    return;
}

void swap (int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}

void pl_insert(plist l, uint64_t i, void *x)
{
    if (l->len == 0){
        l->elements[0] = x;
        l->len++;
    } else {

        if (i == l->len)
        {
            l->elements[i] = x;
            l->len++;
        } else {

            int end = l->len;
            l->elements[end] = x;

            while (end > i) {
                swap((void *)&l->elements[end - 1], (void *)&l->elements[end]);
                end--;
            }
            l->len++;
        }
    }
}

void pl_remove(plist l, void *x)
{
    int i = 0;
    for (i; i < l->len; i++)
    {
        if (l->elements[i] == x){
            int j = i;
            while (j < l->len - 1)
            {
                swap((void *)&l->elements[j], (void *)&l->elements[j + 1]);
                j++;
            }
            l->elements[l->len - 1] = NULL;
            l->len--;
            break;
        }  
    }
    return;
}

void *pl_pop(plist l, uint64_t i)
{
	if (i > l->len){
        exit(1);
    } else {
        int j = i;
        while (j < l->len - 1)
        {
            swap((void *)&l->elements[j], (void*)&l->elements[j + 1]);
            j++;
        }
        l->elements[l->len - 1] = NULL;
        l->len--;
    }
}

void pl_clear(plist l)
{
	while (l->len != 0)
    {
        l->elements[l->len] = NULL;
        l->len--;
    }
}

uint64_t pl_index(plist l, void *x)
{
    for (int j = 0; j < l->len; j++)
    {
        if (l->elements[j] == x)
        {
            return j;
        }
    }
	return -1;
}

uint64_t pl_count(plist l, void *x)
{
    int counter = 0;
    for (int i = 0; i < l->len; i++)
    {
        if (l->elements[i] == x)
        {
            counter += 1;
        }
    }
    return counter;
}

void pl_reverse(plist l)
{
	int end = l->len - 1;
    int i = 0;

    while (end >= i)
    {
        swap((void *)&l->elements[i], (void *)&l->elements[end]);
        end--;
        i++;
    }
}

plist pl_copy(plist l)
{
    plist l2 = pl_new();
    l2->size = l->size;
    int i = 0;pl_print(l);

    while (i < l->len)
    {
        pl_append(l2, l->elements[i++]);
    }

	return l2;
}


/* Functions Used for SNEK implementation */
void pl_tail_append(plist l, void *x)
{
    l->elements[l->len] = x;
    l->len++;
}

void pl_tail_remove(plist l)
{
    l->elements[l->len - 1] = NULL;
    l->len--;
}

