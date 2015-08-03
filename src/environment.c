#include "trooth/environment_internal.h"
#include "trooth/util_internal.h"
#include <stdio.h>

/********************/
/* Callback defines */
/********************/
void TR_free(void* ptr)
{
      // Remove from our in-use list
      free(ptr);
      return;
}

void* TR_alloc(size_t size)
{
      void* result =  malloc(size);
      if (!result)
      {
        fprintf(stderr,"TROOTH: Failed to allocate memory");
        abort();
      }

      return result;
}

/*****************/
/* Alloc/Dealloc */
/*****************/
TR_Environment* TR_Environment_alloc()
{
	TR_Environment* env =  malloc(sizeof(TR_Environment));
	env->allocator = &TR_alloc;
	env->deallocator = &TR_free;

	return env;
}

void TR_Environment_free(TR_Environment* env)
{
	free(env);
}
