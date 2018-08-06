
/*
 *  cocol_func/mem.h
 *
 *  Copyright (C) 2018, 2020  cocol - Dnlydbd
 */
#ifndef _MEM_H_
#define _MEM_H_
#include "queue.h"
#include "init.h"
#include "type.h"

/* dict */
#define MM_POOL_DICT_SIZE			(32)

#define MM_POOL_DEFAULT_LEN			(4096)

#define MM_POOL_DEFAULT_STRINGLEN	(32)	// default string len 32 bytes	
#define MM_POOL_DEFAULT_STRINGSIZE	(128)	// 4096 / 32 = 128 size
#define MM_POOL_DEFAULT_INTLEN		(sizeof(int))
#define MM_POOL_DEFAULT_INTSIZE		(1024)		

#define MM_POOL_DEFAULT_CHARLEN		(sizeof(char))
#define MM_POOL_DEFAULT_CHARSIZE	(128)

#define MM_POOL_BUFF				(4)
#define MM_POOL_EXPAND				(16)
#define MM_POOL_STRINGLEN			(16)

COCOL_TYPE(cocol_base_type) g_cocol_int;
COCOL_TYPE(cocol_base_type) g_cocol_char;
COCOL_TYPE(cocol_base_type) g_cocol_string;
COCOL_TYPE(cocol_base_type) g_cocol_array;
COCOL_TYPE(cocol_base_type) g_cocol_double;
COCOL_TYPE(cocol_base_type) g_cocol_float;
COCOL_TYPE(cocol_base_type) g_cocol_unknown;

struct asd
{
	int a;
};
struct mm_pool_header {
    struct cocol_base_type *pdataType;     // 表示该header 表示是什么类型的数据
	struct cocol_type_header *proot;
    char *pmemData;
	int imemSize;
	int imemFree;
	int imemUsed;
    int imemLen;
    int binitialized;
	unsigned int buffMask;
	struct cocol_type_header buff[MM_POOL_BUFF];
};

int  cocol_string_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_string_print(struct cocol_type_header *ptmp);
void cocol_string_free(struct cocol_type_header *ptmp);
int cocol_string_init(struct cocol_type_header *ptmp,void *);

int  cocol_char_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_char_free(struct cocol_type_header *ptmp);
void cocol_char_print(struct cocol_type_header *ptmp);
int cocol_char_init(struct cocol_type_header *ptmp,void *);

int cocol_int_dup(struct cocol_type_header *pdes,struct cocol_type_header *psrc);
void cocol_int_free(struct cocol_type_header *ptmp);
void cocol_int_print(struct cocol_type_header *ptmp);
int cocol_int_init(struct cocol_type_header *ptmp,void *);

struct cocol_type_header *cocol_string_malloc(struct cocol_type_header *ptmp);
struct cocol_type_header *cocol_int_malloc(struct cocol_type_header *ptmp);
struct cocol_type_header *cocol_char_malloc(struct cocol_type_header *ptmp);
void cocol_mm_pool_init();

void cocol_type_int_init(struct cocol_type_header *ptype);
void cocol_type_char_init(struct cocol_type_header *ptype);
void cocol_type_string_init(struct cocol_type_header *ptype);

#endif
