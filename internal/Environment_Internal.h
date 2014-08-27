#ifndef ENVIRONMENT_INTERNAL_H
#define ENVIRONMENT_INTERNAL_H
#include "Environment.h"
#include "BigInt.h"
#include "stdlib.h"


union TR_Data
{
        TR_BigInt* bigint;
};


// Callback defs
typedef TR_Data (*TR_Internal_TaskCallback)(TR_Data);
typedef void (*TR_Internal_Dispatch)(TR_Internal_TaskCallback callback,TR_Data* data);
typedef void* (*TR_Internal_Alloc)(size_t size);
typedef void (*TR_Internal_Free)(void* block);

// Environment defines important callbacks for parallelism and other platform
// specific features
struct TR_Environment
{
        TR_Internal_Dispatch dispatch;
        TR_Internal_Alloc allocator;
        TR_Internal_Free deallocator;
};

#endif
