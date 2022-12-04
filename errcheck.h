#pragma once

#include <stdio.h>

#define ERRCHECK (expr, msg)          \
    if (!((expr)))                      \
    {                              \
        perror((msg));                 \
        return(-1);                    \
    }                               