#include "list.h"



// FUNCTIONS

void list_init(GMPF_List *list)
{
    /*
        Init a new list.
        The list is already in the element.
    */
    list->prev = NULL;
    list->next = NULL;
}

void list_add_at_pos(GMPF_List *list, GMPF_List *add, int pos)
{
    GMPF_List *getlist = list_get_at_pos(list, pos);
    if (getlist != NULL)
        list_add_after(getlist, add);
}

void list_add_after(GMPF_List *list, GMPF_List *add)
{
    GMPF_List *nextlist = list->next;
    list->next = add;
    add->prev = list;
    add->next = nextlist;
    if (nextlist != NULL)
        nextlist->prev = add;
}

void list_swap(GMPF_List *a, GMPF_List *b)
{
    GMPF_List *prevList = a->prev;
    GMPF_List *nextList = b->next;

    if (prevList != NULL)
        prevList->next = b;
    if (nextList != NULL)
        nextList->prev = a;

    b->prev = prevList;
    a->next = nextList;

    a->prev = b;
    b->next = a;
}

int list_move_up(GMPF_List *list)
{
    if (!list->next)
        return 0;

    list_swap(list, list->next);
    return 1;
}

int list_move_down(GMPF_List *list)
{
    if (!list->prev || !list->prev->prev)
        return 0;

    list_swap(list->prev, list);
    return 1;
}

void list_append(GMPF_List *list, GMPF_List *add)
{
    while (list->next != NULL)
        list = list->next;
    list_add_after(list, add);
}

void list_remove_at_pos(GMPF_List *list, int pos)
{
    GMPF_List *getlist = list_get_at_pos(list, pos);
    if (getlist != NULL)
        list_remove(getlist);
}

void list_remove(GMPF_List *list)
{
    GMPF_List *prevlist = list->prev;
    GMPF_List *nextlist = list->next;
    if (prevlist != NULL)
        prevlist->next = list->next;
    if (nextlist != NULL)
        nextlist->prev = list->prev;
    list->prev = NULL;
    list->next = NULL;
}

GMPF_List * list_get_at_pos(GMPF_List *list, int pos)
{
    if (pos < 0)
        return NULL;
    GMPF_List *getlist = list;
    while (getlist != NULL && pos > 0)
    {
        getlist = getlist->next;
        pos--;
    }
    return getlist;
}

GMPF_List * list_get_last(GMPF_List *list)
{
    GMPF_List *getlist = list;
    while (getlist != NULL)
    {
        getlist = getlist->next;
    }
    return getlist;
}
