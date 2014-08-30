#include "Environment_Internal.h"

TR_Environment* TR_Environment_alloc()
{
	TR_Environment* env =  malloc(sizeof(TR_Environment));
	env->allocator = &malloc;
	env->deallocator = &free;

	return env;
}

void TR_Environment_free(TR_Environment* env)
{
	free(env);
}


