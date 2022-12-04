#pragma once

#include <fcntl.h>
#include <unistd.h>
#include <malloc.h>
#include <assert.h>

int getsize(int fd);
char *loadfile(int fd, int size);

// Get file size in characters
int getsize(int fd)
{
    int size = lseek(fd, 0, SEEK_END);
    lseek(fd, 0, SEEK_SET);
    return size;
}

// Allocate memory and load file contents (+ '\0')
char *loadfile(int fd, int size)
{
    char *mem = malloc(size + 1);
    assert(mem); // errcheck
    assert(read(fd, mem, size));
    mem[size] = '\0';

    return mem;
}
