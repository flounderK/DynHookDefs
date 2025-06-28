#ifndef INTRUSIVE_LIST_H
#define INTRUSIVE_LIST_H

#include <stddef.h>

/* Basic intrusive doubly-linked list node */
struct list_head {
    struct list_head *next, *prev;
};

/* Initialize a list head to point to itself */
#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
    struct list_head name = LIST_HEAD_INIT(name)

/* Initialize a list head dynamically */
static inline void INIT_LIST_HEAD(struct list_head *list) {
    list->next = list;
    list->prev = list;
}

/* Insert a new_node entry between two known consecutive entries */
static inline void __list_add(struct list_head *new_node,
                              struct list_head *prev,
                              struct list_head *next)
{
    next->prev = new_node;
    new_node->next = next;
    new_node->prev = prev;
    prev->next = new_node;
}

/* Add a new_node entry after the specified head */
static inline void list_add(struct list_head *new_node, struct list_head *head) {
    __list_add(new_node, head, head->next);
}

/* Add a new_node entry before the specified head (tail insertion) */
static inline void list_add_tail(struct list_head *new_node, struct list_head *head) {
    __list_add(new_node, head->prev, head);
}

/* Delete a list entry by making the prev/next point to each other */
static inline void __list_del(struct list_head *prev, struct list_head *next) {
    next->prev = prev;
    prev->next = next;
}

/* Delete an entry from the list */
static inline void list_del(struct list_head *entry) {
    __list_del(entry->prev, entry->next);
    entry->next = entry->prev = NULL;
}

/* Check if the list is empty */
static inline int list_empty(const struct list_head *head) {
    return head->next == head;
}

/* Iterate over the list */
#define list_for_each(pos, head) \
    for (pos = (head)->next; pos != (head); pos = pos->next)

/* Iterate over the list safely against removal */
#define list_for_each_safe(pos, n, head) \
    for (pos = (head)->next, n = pos->next; pos != (head); \
         pos = n, n = pos->next)

/* Get the struct for this entry */
#define list_entry(ptr, type, member) \
    ((type *)((char *)(ptr)-(unsigned long)(&((type *)0)->member)))

#endif // INTRUSIVE_LIST_H
