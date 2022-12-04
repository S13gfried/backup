#pragma once

#ifndef MAX_NAME_LENGTH
#define MAX_NAME_LENGTH 256
#endif

#ifndef TIMESTAMP_LENGTH
#define TIMESTAMP_LENGTH 16
#endif

#ifndef ROSTER_PATH
#define ROSTER_PATH "_ROSTER.txt"
#endif

#ifndef MAX_TREE_DEPTH
#define MAX_TREE_DEPTH 5
#endif

#include "stringproc.h"
#include "dirparse.h"
#include "launch.h"
#include "ptrlist.h"

#include <malloc.h>
#include <fcntl.h>
#include <string.h>

struct dtree
{
    char name[MAX_NAME_LENGTH];
    char date[TIMESTAMP_LENGTH];
    int branches; //-1 for regular file, 0+ for directories
    struct dtree *branch;
    struct dtree *root;
};

char *getfullpath(struct dtree *targetnode, char *buffer);
int createroster(struct dtree *targetnode);
int appenddirs(struct dtree *targetnode);
int generatetree(struct dtree *root);
int treedifference(struct dtree *templ, struct dtree *eval, struct ptrlist* result);
int checkpathvalidity(struct dtree *destination, struct dtree *eval);

// get path to directory from its dtree node pointer
char *getfullpath(struct dtree *targetnode, char *buffer)
{
    while (targetnode->root) // ascending to root
    {
        revcat(buffer, "/");
        revcat(buffer, targetnode->name); // append name to the beginning of path
        targetnode = targetnode->root;    // get one step higher
    }
    return buffer; // placeholder
}

// generate a roster text file which contains file data for target node
int createroster(struct dtree *targetnode)
{
    char buf[MAX_TREE_DEPTH * (MAX_NAME_LENGTH + 1) + 1];
    char command[(MAX_TREE_DEPTH * (MAX_NAME_LENGTH + 1)) * 2 + 10];

    for (int i = 0; i < MAX_TREE_DEPTH * (MAX_NAME_LENGTH + 1); i++)
        buf[i] = '\0'; // buffer cleanup

    for (int i = 0; i < (MAX_TREE_DEPTH * (MAX_NAME_LENGTH + 1)) * 2 + 10; i++)
        command[i] = '\0';

    strcat(command, "ls -l ");
    strcat(command, buf);
    strcat(command, " > ");
    strcat(command, buf);
    strcat(command, ROSTER_PATH); // build command

    launch(command); // run via bash

    return 0;
}

// Append directories to a branch (ASSUMING A ROSTER FILE AND A TREE NODE ARE PRESENT)
int appenddirs(struct dtree *targetnode)
{
    char path[MAX_TREE_DEPTH * (MAX_NAME_LENGTH + 1)] = {};
    getfullpath(targetnode, path);

    strcat(path, ROSTER_PATH);
    int rosterdesc = open(path, O_RDONLY | 0666);

    int size = getsize(rosterdesc);
    char *roster = loadfile(rosterdesc, size);
    close(rosterdesc); // store roster in memory instead of disk access

    int entries = linecount(roster) - 3; // count entries; last is empty
    int directories = 0;

    targetnode->branches = entries;
    targetnode->branch = calloc(entries, sizeof(struct dtree)); // allocate branch array
    int bias = 0;                                               // used for skipping roster

    for (int i = 0; i < entries + 1; i++) // init branches by pasting strings from designated places of file
    {
        char *nameptr = gotoword(roster, i + 1, 8); // 0th line is "total X"

        if (strcom(nameptr, ROSTER_PATH, strlen(ROSTER_PATH)))
        {
            bias = -1;
            continue; // do not include the roster file itself
        }

        for (int k = 0; (nameptr[k] != '\n') && (nameptr[k] != '\0'); k++)
            targetnode->branch[i + bias].name[k] = nameptr[k]; // set name

        char *timeptr = gotoword(roster, i + 1, 5);
        strmov(targetnode->branch[i + bias].date, timeptr, TIMESTAMP_LENGTH); // set timestamps (copying str of fixed length due to multiple spaces)

        targetnode->branch[i + bias].root = targetnode; // set root

        // printf("%s appended to %s\n", targetnode->branch[i + bias].name, targetnode->name);

        if (*gotoword(roster, i + 1, 0) == 'd')
        {
            targetnode->branch[i + bias].branches = 0; // mark as (currently) empty directory.
            directories++;
        }
        else
            targetnode->branch[i + bias].branches = -1; // mark as regular file
    }
    free(roster);
    return directories;
}

// Generate directory tree and rosters recursively
int generatetree(struct dtree *root)
{
    int dirs = 1;
    createroster(root);

    /*//~
    char path[MAX_NAME_LENGTH * (MAX_TREE_DEPTH + 2) + 1] = {};
    getfullpath(root, path);
    strcat(path, ROSTER_PATH);

    int filecheck;
    while ((filecheck = open(path, O_RDONLY | 0666)) < 0)
        ; // wait for file to be created
    char sample;
    while (read(filecheck, &sample, 1) == 0)
        ; // wait for characters to appear in file

    close(filecheck);
    *///~
    appenddirs(root); // init roster and next layer

    for (int i = 0; i < root->branches; i++)
    {
        if (root->branch[i].branches == 0)
            dirs += generatetree(&(root->branch[i]));
    }
    return dirs;
}

// Get paths of nodes present in first tree and absent in the second.
int treedifference(struct dtree *templ, struct dtree *eval, struct dtree **result) // can't be used for maxdepth = 0
{
    int *path = (int *)calloc(MAX_TREE_DEPTH, sizeof(int)); // last element not used
    struct dtree *position = templ;

    // there are available nodes && maxdepth for iteration not reached - go down
    // there are available nodes && maxdepth reached - check for similar node in eval
    // node count exceeded - process node, clear level index, go up, step level index,
    // node count exceeded at level 0 - process node and finish
    int layer = 0;
    while (1)
    {
        if (path[layer] >= position->branches || layer == MAX_TREE_DEPTH) // when all branches out are processed, process the node itself.
        {
            if (layer == 0)
                break;

            if (!checkpathvalidity(position, eval))
                append(result, (void*)position);

            path[layer] = 0;   // begin new layer
            path[layer - 1]++; // shift upper layer

            layer--; // go up
            position = position->root;
            printf("goto %s (%d)\n", position->name, layer); // nig
            continue;
        }
        position = &(position->branch[path[layer]]);
        layer++;                                         // go down
        printf("goto %s (%d)\n", position->name, layer); // nig
    }
    free(path);
    return listlength(result);
}

// check if a node with same address is present
int checkpathvalidity(struct dtree *destination, struct dtree *eval)
{
    char *path = (char *)calloc(MAX_TREE_DEPTH * MAX_NAME_LENGTH, sizeof(char));

    int length = 0;
    for (; destination->root != 0 && length < MAX_TREE_DEPTH; length++)
    {
        strmov(path + MAX_NAME_LENGTH * length, destination->name, MAX_NAME_LENGTH);
        printf("%d %s (%s)\n", length, (char *)(path + MAX_NAME_LENGTH * length), destination->name); // nig
        destination = destination->root;
    }
    printf("\n"); // nig
    for (int i = 0; i < length; i++)
    {
        int match = 0;
        for (int k = 0; k < eval->branches; k++)
        {
            printf("%d - %s v. %s\n", i, (char *)(path + MAX_NAME_LENGTH * (length - 1 - i)), eval->branch[k].name); // nig
            if (strcom(path + MAX_NAME_LENGTH * (length - 1 - i), eval->branch[k].name, MAX_NAME_LENGTH))
            {
                match = 1;
                eval = &(eval->branch[k]);
                break;
            }
        }
        if (!match)
        {
            free(path);
            printf("FAILURE!\n");
            return 0;
        }
    }
    free(path);
    return 1;
}