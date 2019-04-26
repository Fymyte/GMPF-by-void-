#include "list.h"


/*
 * Init a list with all parameters to NULL
 (The list is already in the element)
 */
void list_init(GMPF_List *list)
{
    list->prev = NULL;
    list->next = NULL;
}


/*
 * Add a new GMPF_List element at the given position in the list
 * (Do nothing if the position is invalid)
 */
void list_add_at_pos(GMPF_List *list, GMPF_List *add, int pos)
{
    GMPF_List *getlist = list_get_at_pos(list, pos);
    if (getlist != NULL)
        list_add_after(getlist, add);
}


/*
 * Add a new GMPF_List element after the given element
 * (Do nothing if the list element is invalid)
 */
void list_add_after(GMPF_List *list, GMPF_List *add)
{
    GMPF_List *nextlist = list->next;
    list->next = add;
    add->prev = list;
    add->next = nextlist;
    if (nextlist != NULL)
        nextlist->prev = add;
}


/*
 * Swap the two elements "a" and "b" in the list
 * (Do nothing if one of the element is invalid)
 */
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


/*
 * Move the given element one place before in the list
 * (Do nothing if the element is invalid or it is already at the first place)
 */
int list_move_up(GMPF_List *list)
{
    if (!list->next)
        return 0;

    list_swap(list, list->next);
    return 1;
}


/*
 * Move the given element one place after in the list
 * (Do nothing if the element is invalid or it is already at the last place)
 */
int list_move_down(GMPF_List *list)
{
    if (!list->prev || !list->prev->prev)
        return 0;

    list_swap(list->prev, list);
    return 1;
}


/*
 * Add an element at the end of the list
 * (Do nothing if the element is invalid)
 */
void list_append(GMPF_List *list, GMPF_List *add)
{
    while (list->next != NULL)
        list = list->next;
    list_add_after(list, add);
}


/*
 * Remove the element at the given position in the list
 * (Do nothing if the position or the list is invalid)
 */
void list_remove_at_pos(GMPF_List *list, int pos)
{
    GMPF_List *getlist = list_get_at_pos(list, pos);
    if (getlist != NULL)
        list_remove(getlist);
}


/*
 * Remove the element of the list and free it
 * (Do nothing if the element is invalid)
 */
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


/*
 * Return the element at the given position in the list
 * (Return: the element, or NULL if the position is invalid)
 */
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


/*
 * Return the last element of the list
 * (Return: the element (itself if it is the last element), or NULL if the
 * list is invalid)
 */
GMPF_List * list_get_last(GMPF_List *list)
{
    GMPF_List *getlist = list;
    while (getlist->next != NULL)
    {
        getlist = getlist->next;
    }
    return getlist;
}
