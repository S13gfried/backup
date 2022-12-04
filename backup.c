#define MAX_NAME_LENGTH 256
#define TIMESTAMP_LENGTH 12
#define MAX_TREE_DEPTH 5

#define ROSTER_PATH "_ROSTER.txt"
#define BACKUP_DATA_PATH "_BACKUP_DATA.txt"
#define BACKUP_FOLDER "backup"

#include <stdio.h>

#include "ptrlist.h"
#include "dtree.h"

int main()
{
    // generate tree
    // get table
    // find dirs
    // compare dates/absence with old roster

    struct dtree old = {};
    struct dtree new = {};
    
    struct ptrlist* deletions = newlist();
    struct ptrlist* additions = newlist();

    generatetree(&old);
    printf("GENERATED 1\n");

    int obama_balls;
    scanf("%d", &obama_balls);
    generatetree(&new);

    int added = treedifference(&new, &old, additions); //no. of new entries
    int deleted = treedifference(&old, &new, deletions); //no. of deleted entries

    printf("\n\n\n%d %d\n", created, deleted);

    return 0;
}
/*
// Returns a pointer to dtree node specified by branch[] index path
struct dtree *followpath(struct dtree* tree, int *path, int length)
{
    for (int step = 0; step < length; step++)
    {

    }
    return 0;
}
*/