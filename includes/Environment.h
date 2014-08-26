struct TR_Environment;
union TR_Data;

typedef struct TR_Environment TR_Environment;
typedef union TR_Data TR_Data; 

TR_Environment* TR_Environment_alloc();
void TR_Environment_free(TR_Environment* env);
