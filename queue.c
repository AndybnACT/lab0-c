#include "queue.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "strnatcmp.h"

/*
 * Create empty queue.
 * Return NULL if could not allocate space.
 */
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (!q) {
        perror("malloc");
        return NULL;
    }
    q->head = NULL;
    q->tail = NULL;
    q->nr = 0;
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    /* Free queue structure */
    list_ele_t *ptr;
    if (!q)
        return;

    ptr = q->head;
    while (ptr) {
        list_ele_t *self;
        self = ptr;
        ptr = ptr->next;
        if (self->value) {
            free(self->value);
        }
        free(self);
    }

    free(q);
}

/*
 * Attempt to insert element at head of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    list_ele_t *newh;
    /* TODO: What should you do if the q is NULL? */
    if (!q) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        perror("malloc, cannot allocate space for new element");
        return false;
    }
    /* Don't forget to allocate space for the string and copy it */
    /* What if either call to malloc returns NULL? */
    // printf("%lx\n", *((unsigned long*)s));
    newh->value = strdup(s);
    if (!newh->value) {
        perror("strdup, cannot allocate space for value");
        free(newh);
        return false;
    }
    newh->next = q->head;
    q->head = newh;
    if (!q->tail) {
        q->tail = newh;
    }
    q->nr++;
    return true;
}

/*
 * Attempt to insert element at tail of queue.
 * Return true if successful.
 * Return false if q is NULL or could not allocate space.
 * Argument s points to the string to be stored.
 * The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    list_ele_t *newh;
    if (!q) {
        return false;
    }

    newh = malloc(sizeof(list_ele_t));
    if (!newh) {
        perror("malloc, cannot allocate space for new element");
        return false;
    }

    newh->value = strdup(s);
    if (!newh->value) {
        perror("strdup, cannot allocate space for value");
        free(newh);
        return false;
    }
    newh->next = NULL;

    if (!q->tail) {
        q->tail = newh;
        q->head = newh;
    } else {
        q->tail->next = newh;
        q->tail = newh;
    }

    q->nr++;
    return true;
}

/*
 * Attempt to remove element from head of queue.
 * Return true if successful.
 * Return false if queue is NULL or empty.
 * If sp is non-NULL and an element is removed, copy the removed string to *sp
 * (up to a maximum of bufsize-1 characters, plus a null terminator.)
 * The space used by the list element and the string should be freed.
 */
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    list_ele_t *rm;
    if (!q)
        return false;
    if (!q->head)
        return false;

    rm = q->head;
    if (sp) {
        memset(sp, 0, bufsize);
        if (rm->value) {
            strncpy(sp, rm->value, bufsize - 1);
        }
    }
    free(rm->value);

    q->head = rm->next;
    q->nr--;
    if (rm == q->tail)
        q->tail = NULL;

    free(rm);
    return true;
}

/*
 * Return number of elements in queue.
 * Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (!q) {
        return 0;
    }
    return q->nr;
}

/*
 * Reverse elements in queue
 * No effect if q is NULL or empty
 * This function should not allocate or free any list elements
 * (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
 * It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    list_ele_t *cur;
    list_ele_t *prev, *tmp;
    if (!q)
        return;
    if (!q->head)
        return;

    cur = q->head;
    prev = NULL;
    while (cur->next) {
        tmp = cur->next;
        cur->next = prev;
        prev = cur;
        cur = tmp;
    }
    cur->next = prev;

    q->tail = q->head;
    q->head = cur;
    return;
}

/*
 * Sort elements of queue in ascending order
 * No effect if q is NULL or empty. In addition, if q has only one
 * element, do nothing.
 */
void q_sort(queue_t *q)
{
    list_ele_t *head, *tail, *e;
    if (!q)
        return;
    if (!q->head)
        return;

    head = q->head;
    tail = q->head;

    e = head->next;
    head->next = NULL;

    while (e) {
        list_ele_t *insert = e;
        list_ele_t **haystack = &head;
        list_ele_t *tmp;

        e = e->next;
        insert->next = NULL;

        while (*haystack) {
            if (strnatcmp(insert->value, (*haystack)->value) <= 0)
                break;
            haystack = &((*haystack)->next);
        }
        if (!*haystack)
            tail = insert;

        tmp = *haystack;
        *haystack = insert;
        insert->next = tmp;
    }

    q->head = head;
    q->tail = tail;
    return;
}
