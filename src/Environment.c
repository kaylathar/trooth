#include "Environment_Internal.h"


void TR_free(void* ptr)
{
      // Remove from our in-use list
      free(ptr);
      return;
}

void* TR_alloc(size_t size)
{
      void* result =  malloc(size);
      
      // Add to our in-use list
      
      return result;
}

void *TR_stack_alloc(size_t size)
{
  if (0) // Switch to using __builtin_frame_address to figure out what frame and can be done safely
  {
    // use platform stack allocator if size is 
  }
  
  return TR_alloc(size);
  
}

TR_Environment* TR_Environment_alloc()
{
	TR_Environment* env =  malloc(sizeof(TR_Environment));
	env->allocator = &TR_alloc;
	env->stack_allocator = &TR_stack_alloc;
	env->deallocator = &TR_free;

	return env;
}

TR_Environment* TR_Environment_fromEnvironment(TR_Environment* env)
{
  
}

void TR_Environment_free(TR_Environment* env)
{
	free(env);
}


