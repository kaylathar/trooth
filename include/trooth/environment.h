#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H

/**
 * Environments are used in the construction of all other Trooth objects
 * 
 * An environment is used to define the allocator, deallocator, and other utilities - overriding the implementation will let you change how these function
 * Please keep in mind, you may not use objects across environment boundaries.  It is perfectly acceptable to use an environment at the top level.
 * All constituent objects must be deallocated prior to an environment being freed.
 */
struct TR_Environment;
typedef struct TR_Environment TR_Environment;

/**
 * Allocates a new environment object of the default typedef
 */
TR_Environment* TR_Environment_alloc();

/**
 * Frees memory used by a given environment object
 */
void TR_Environment_free(TR_Environment* env);
#endif
