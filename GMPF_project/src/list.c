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


