#pragma once

#include <malloc.h>

#include "errcheck.h"

struct ptrlist
{
    struct ptrlist* next;
    void* element; //first element points to int - length
};

struct ptrlist* newlist();
int listlength(struct ptrlist* list);
struct ptrlist* plget(struct ptrlist* list, int index);
struct ptrlist* append(struct ptrlist* list, void* elem);
struct ptrlist* seekend(struct ptrlist* list);
struct ptrlist* destroy(struct ptrlist* list, int index);
int pseek(struct ptrlist* list, void* lookfor, int size);

struct ptrlist* newlist()
{
    struct ptrlist* new = calloc(1, sizeof(struct ptrlist));

    new->element = calloc(1, sizeof(int));
    *(int*)(new->element) = 0;

    new->next = NULL;
    return new;
}
// get & update length of a list
int listlength(struct ptrlist* list)
{
    struct ptrlist* last = list;
    int length;

    for(length = 0; last->next != NULL; length++)
        last = last->next;

    *(int*)(list->element) = length;
    return length;
}

// get pointer to an element via index
struct ptrlist* plget(struct ptrlist* list, int index)
{
    if(*(int*)(list->element) == 0)
        return NULL;

    index = index % *(int*)(list->element); //cycle indices
    if(index < 0)
        index = index + *(int*)(list->element);

    struct ptrlist* result = NULL;

    for(int i = 0; (i <= index) && (list != NULL); i++)
    {
        result = list->next;
        list = result;
    }
    return result;
}


// get pointer to last element
struct ptrlist* seekend(struct ptrlist* list)
{
    while(list->next != NULL)
        list = list->next;
    return list;
}

// add new element to list end
struct ptrlist* append(struct ptrlist* list, void* elem)
{
    struct ptrlist* first = list;
    list = seekend(list);

    list->next = calloc(1, sizeof(struct ptrlist));
    list = list->next;

    list->next = NULL;
    list->element = elem;

    *(int*)(first->element) = *(int*)(first->element) + 1;
    
    return list;
}

// destroy element with matching index
struct ptrlist* destroy(struct ptrlist* list, int index)
{
    if(*(int*)(list->element) == 0 || list->next == NULL)
        return NULL;

    index = index % *(int*)(list->element); //cycle indices
    if(index < 0)
        index = index + *(int*)(list->element);

    struct ptrlist* jointo = list;

    for(int i = 0; i < index; i++)
    {
        jointo = jointo->next;
        if(jointo == NULL)
            return NULL;
    }

    if(jointo->next == NULL)
        return NULL;

    struct ptrlist* joinfrom = jointo->next->next;

    free(jointo->next);
    jointo->next = joinfrom;

    return joinfrom;
}

// find first element matching the query. object size has to be specified. return -1 if no entries are found
int pseek(struct ptrlist* list, void* lookfor, int size)
{
    int index = 0;
    while(list->next != NULL)
    {
        list = list->next;

        int match = 1;
        for(int i = 0; i < size; i++)
            if(((char*)list->element)[i] != ((char*)lookfor)[i])
            {
                match = 0;
                break;
            }
        if(match)
            return index;
        index++;
    }
    return -1;
}