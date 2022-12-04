#pragma once

#include <string.h>

char *revcat(char *to, char *from);
char *gotoword(char *src, int row, int col);
int strmov(char *to, char *from, int length);
int strcom(char *one, char *two, int length);
int linecount(char *str);
int ridofpostfix(char *src);

// Reverse concatenation
char *revcat(char *to, char *from)
{
    char buf[MAX_NAME_LENGTH * 2] = {};
    // static char buf[MAX_NAME_LENGTH*2];

    // for(int i = 0; i < MAX_NAME_LENGTH*2; i++)
    //     buf[i] = '\0'; // buffer cleanup

    strcat(buf, from);
    strcat(buf, to);
    strcpy(to, buf);

    return to;
}

// Get pointer to given word in given column (skip Nrow '\n's and Ncolumn ' 's)
char *gotoword(char *src, int row, int col)
{
    int index = 0;

    int rowc = 0;
    int colc = 0; // encountered so far

    for (; rowc < row; index++)
    {
        if (src[index] == '\0')
            return (src + index); // out-of-range case

        if (src[index] == '\n')
            rowc++;
    } // find row

    for (; colc < col; index++)
    {
        if ((src[index] == '\0') || (src[index] == '\n'))
            return (src + index); // out-of-range case

        if (src[index] == ' ')
        {
            colc++;
            while (src[index + 1] == ' ')
                index++; // skip multiple
        }
    }
    return (src + index);
}

// Move strings with known length regardless of '\0's placed
int strmov(char *to, char *from, int length)
{
    int i = 0;

    for (; i < length; i++)
        to[i] = from[i];

    return i;
}

// Comparison of two strings with known length
int strcom(char *one, char *two, int length)
{
    for (int i = 0; i < length; i++)
        if (one[i] != two[i])
            return 0;
        else if ((one[i] == '\0') && (two[i] == '\0')) // forced end of string
            break;

    return 1;
}

// Count lines in a string
int linecount(char *str)
{
    if (str[0] == '\0')
        return 0; // empty string

    int count = 1;
    for (int i = 0;; i++)
        if (str[i] == '\0')
            break;
        else if (str[i] == '\n')
            count++;

    return count;
}

int ridofpostfix(char *src)
{
    int index = 0;
    while (1)
    {
        if (src[index] == '\0')
            return index;
        if (src[index] == '.')
        {
            src[index] = '\0';
            return index;
        }
    }
}