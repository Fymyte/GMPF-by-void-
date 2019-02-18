#ifndef LIST_H
#define LIST_H

#include <stdlib.h>

/*
    Implementation of intrusive lists.
*/

// DEFINES - default on linux
#ifndef offsetof
#define offsetof(st, m) ((size_t)&(((st *)0)->m))
#endif

#ifndef container_of
#define container_of(ptr, type, member)  \
    (type *)((char *)ptr - offsetof(type,member));
#endif


// STRUCTURES

typedef struct GMPF_List GMPF_List;

struct GMPF_List {
    GMPF_List *prev;
    GMPF_List *next;
};


// FUNCTIONS DECLARATIONS

void list_init(GMPF_List *list);
void list_add_at_pos(GMPF_List *list, GMPF_List *add, int pos);
void list_add_after(GMPF_List *list, GMPF_List *add);
void list_append(GMPF_List *list, GMPF_List *add);
void list_remove_at_pos(GMPF_List *list, int pos);
void list_remove(GMPF_List *list);
GMPF_List * list_get_at_pos(GMPF_List *list, int pos);

#endif //LIST_H
