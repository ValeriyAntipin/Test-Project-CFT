#ifndef _TYPES_
#define _TYPES_

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <search.h>

typedef struct StatData {
    long id;
    int count;
    float cost;
    unsigned int primary:1;
    unsigned int mode:3;
} StatData;

typedef enum {
    NO_ERROR = 0,
    OPEN_FILE_ERROR = 1,
    CLOSE_FILE_ERROR = 2,
    WRITE_HEADER_ERROR = 3,
    WRITE_DATA_ERROR = 4,
    READ_HEADER_ERROR = 5,
    READ_DATA_ERROR = 6,
    MEMORY_ALLOC_ERROR = 7,
    EMPTY_BUFFER = 8,
    ZERO_POINET = 9,
    ERROR_LOAD = 10,
    ERROR_JOIN = 11,
    ERROR_WRITE = 12
} ERROR_CODE_TYPE;

#endif