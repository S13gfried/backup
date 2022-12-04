#include <stdio.h>
#include <malloc.h>

#include "ptrlist.h"

enum commands
{
    TERM, \
    APPD, \
    DEST, \
    FIND, \
    SKND, \
    LGTH, \
    SEEK, \
};

int main()
{
    struct ptrlist* list = newlist();
    int command, arg;
    while(1)
    {
        scanf("%d", &command);

        switch(command)
        {
        case TERM:
            return 0;

        case APPD:
            printf("APPEND: ");
-            scanf("%d", &arg);

            int* newval = malloc(sizeof(int));
            *newval = arg;

            struct ptrlist* new = append(list, newval);
            printf("RETURNED %ld\n", (long)new);
            *((int*)new->element) = arg;
            break;
        
        case DEST:
            printf("DESTROY: ");
            scanf("%d", &arg);

            printf("RETURNED %ld\n", (long)destroy(list, arg));
            break;

        case FIND:
            printf("INDEX: ");
            scanf("%d", &arg);

            printf("VALUE IS %d\n", *((int*)plget(list, arg)->element));
            break;

        case SKND:
            printf("END: %d\n", *(int*)(seekend(list)->element));
            break;
        
        case LGTH:
            printf("LENGTH: %d\n", listlength(list));
            break;

        case SEEK:
            printf("FIND: ");
            scanf("%d", &arg);  

            printf("INDEX: %d\n", pseek(list, &arg, sizeof(int)));
            break;
        default:

            printf("unknown\n");
            break;
        }
    }

    return 0;
}