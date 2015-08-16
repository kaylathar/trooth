#ifndef ENVIRONMENT_INTERNAL_H
#define ENVIRONMENT_INTERNAL_H
#include "trooth/environment.h"
#include "trooth/bigint.h"
#include <stdlib.h>

/* Callback defs */
typedef void* (*TR_Internal_Alloc)(size_t size);
typedef void (*TR_Internal_Free)(void* block);

/* Environment defines important callbacks for parallelism and other platform
 * specific features
 */
struct TR_Environment
{
        TR_Internal_Alloc allocator;
        TR_Internal_Free deallocator;
};

#endif
