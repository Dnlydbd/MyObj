/*
 *  dnly_func/start.c
 *
 *  Copyright (C) 2018, 2020  cocol - dnlydbd
 */

#include "init.h"
#include "type.h"
struct cocol_global g_cocol_global;
#if 0
struct cocol_type g_cocol_int
{
	.name = "int",
	._type = COCOL_TYPE_INT;
	.pcocolType = NULL;
};
#endif


int parse_argc_param(int argc,char ** argv)
{
	return 0;
}

int get_current_workdir(char *pdir)
{
	char curExcDir[64] = {0};
	assert(NULL != pdir);
	getcwd(curExcDir,sizeof(curExcDir) - 1);
	
	strncpy(g_cocol_global.funcName,pdir,sizeof(g_cocol_global.funcName));
	strncpy(g_cocol_global.curWorkDir,pdir,sizeof(g_cocol_global.curWorkDir));
	
	chdir("/home/");
	return 0;
}

COCOL_TYPE(mm_pool_header) g_stringHeader;
COCOL_TYPE(mm_pool_header) g_intHeader;
COCOL_TYPE(mm_pool_header) g_charHeader;

int
cocol_type_init()
{
	mm_pool_init(&g_stringHeader);
    mm_pool_init(&g_intHeader);
    mm_pool_init(&g_charHeader);
}

int 
cocol_early_init()
{
    cocol_type_init();
	return 0;
}

int cocol_main()
{
	return 0;
}

int cocol_release()
{
	return 0;
}

#if 0
/*
 *  start.c/main
 *
 *  
 */
int main(int argc,char ** argv)
{
	int i = 0;
	struct cocol_type cocolType;
	
	memset(&cocolType,0,sizeof(struct cocol_type));
	get_current_workdir(argv[0]);
	parse_argc_param(argc,argv);
	cocol_early_init(&cocolType);
	cocol_main();
	cocol_release();
	return 0;
}
#endif
