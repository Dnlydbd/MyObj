#include "type.h"
#include "mmpool.h"
#include "mmalloc.h"


/* base type */
struct cocol_base_type g_cocol_type =	
{	
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
    .cocol_malloc = NULL,   
	.cocol_free = NULL,	
	
	.cocol_objinit = NULL,		
	.cocol_objdup = NULL,
	.cocol_objcmp = NULL,
	.cocol_objdestroy = NULL,
	.cocol_objhash = NULL,
	.cocol_objprint = NULL,
};

struct cocol_base_type g_cocol_integer =	
{	
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "int",	
	.encocolType = COCOL_TYPE_INT,	
	
    .cocol_malloc = cocol_malloc,   
	.cocol_free = cocol_free,	
	
	.cocol_objinit = cocol_obj_init,		
	.cocol_objdup = cocol_obj_dup,
	.cocol_objcmp = cocol_obj_cmp,
	.cocol_objdestroy = cocol_obj_destroy,
	.cocol_objhash = NULL,
	.cocol_objprint = NULL,
};


struct cocol_base_type g_cocol_character = 
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "char",	
	.encocolType = COCOL_TYPE_CHAR,	
	
    .cocol_malloc = cocol_malloc,   
	.cocol_free = cocol_free,	
	
	.cocol_objinit = cocol_obj_init,		
	.cocol_objdup = cocol_obj_dup,
	.cocol_objcmp = cocol_obj_cmp,
	.cocol_objdestroy = cocol_obj_destroy,
	.cocol_objhash = NULL,
	.cocol_objprint = NULL,
};

struct cocol_base_type g_cocol_string = 
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "string",	
	.encocolType = COCOL_TYPE_STRING,	
	
    .cocol_malloc = cocol_malloc,   
	.cocol_free = cocol_free,	
	
	.cocol_objinit = cocol_obj_init,		
	.cocol_objdup = cocol_obj_dup,
	.cocol_objcmp = cocol_obj_cmp,
	.cocol_objdestroy = cocol_obj_destroy,
	.cocol_objhash = NULL,
	.cocol_objprint = NULL,
};

struct cocol_base_type g_cocol_array =
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)pe)
	.name = "array",
	.encocolType = COCOL_TYPE_ARRAY,
	.pcocolType = NULL,
	.cocol_init = NULL,
	.cocol_print = NULL,
	.cocol_malloc = NULL,
	.cocol_free = NULL,
	.cocol_dup = NULL,
};

struct cocol_base_type g_cocol_double =
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "double",
	.encocolType = COCOL_TYPE_DOUBLE,
	.pcocolType = NULL,
	.cocol_init = NULL,
	.cocol_print = NULL,
	.cocol_malloc = NULL,
	.cocol_free = NULL,
	.cocol_dup = NULL,
};

struct cocol_base_type g_cocol_float =
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "float",
	.encocolType = COCOL_TYPE_FLOAT,
	.pcocolType = NULL,
	.cocol_init = NULL,
	.cocol_print = NULL,
	.cocol_malloc = NULL,
	.cocol_free = NULL,
	.cocol_dup = NULL,
};

struct cocol_base_type g_cocol_unknown =
{
    CO_HEADER_OBJINIT(&g_cocol_type,NULL,NULL)
	.name = "unknown",
	.encocolType = COCOL_TYPE_FLOAT,
	.pcocolType = NULL,
	.cocol_init = NULL,
	.cocol_print = NULL,
	.cocol_malloc = NULL,
	.cocol_free = NULL,
	.cocol_dup = NULL,
};


#if 0
#define COCOL_BASE_TYPE_DEF(type)	\
struct cocol_base_type g_cocol_##type =	\
{	\
    COCOL_HEADER_OBJINIT(g_cocol_type)	\
    
	.name = #type,	\
	.encocolType = COCOL_TYPE_##type,	\
	.cocol_init = cocol_##type##_init,		\
	.cocol_print = cocol_##type##_print,	\
	.cocol_malloc = cocol_##type##_malloc,	\
	.cocol_free = cocol_##type##_free,	\
	.cocol_dup = cocol_##type##_dup,	\
};


COCOL_BASE_TYPE_DEF(int)
COCOL_BASE_TYPE_DEF(char)
COCOL_BASE_TYPE_DEF(string)
//COCOL_BASE_TYPE_DEF(array)
//COCOL_BASE_TYPE_DEF(dict)
//COCOL_BASE_TYPE_DEF(float)
//COCOL_BASE_TYPE_DEF(double)
//COCOL_BASE_TYPE_DEF(unknown)
#endif





#define OBJ_REG()

VOID OBJ_REG_BASETYPE()
{
	strcpy(g_cocol_type.name,"type");
	
}

