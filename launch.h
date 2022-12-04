#pragma once

#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

pid_t launch(char *command);

// Run a bash command
pid_t launch(char *command)
{
    pid_t parent = fork();

    if (parent) // self-explanatory
        return waitpid(parent, NULL, 0);

    execl("/bin/bash", "bash", "-c", command, NULL); // child
    return 0;
}