/*
 *  cocol_func/dict.c
 *
 *  Copyright (C) 2018, 2020  cocol - Dnlydbd
 */

#include "type.h"
#include "mem.h"
#include "dict.h"

#define COCOL_TYPE_TO_HEADER
struct cocol_dict_header g_dictHeader = {
	.pinherHeader = NULL
    .pcocolBaseType = NULL,
    .pmmBase = NULL,
    .idictSize = COCOL_DICT_SIZE,
    .idictUsed = 0,
};

struct cocol_type_header *
cocol_dict_new()
{
	struct cocol_dict_elem *pelem = NULL;
    struct cocol_dict_header *pheader = NULL;
    pheader = (struct cocol_dict_header *)cocol_malloc(sizeof(struct cocol_dict_header));
    if(!pheader) {
		return -1;
    }

    pheader->pcocolBaseType = &g_cocol_dict;
    pheader->idictSize = MM_POOL_DICT_SIZE;
    pheader->idictUsed = 0;
    return (struct cocol_type_header *)pheader;
    //cocol_dict_init(pheader);
}

int
cocol_dict_init(struct cocol_dict_header *pheader)
{
	struct cocol_dict_elem *pelem = NULL;
    pelem = (struct cocol_dict_elem *) cocol_malloc(pheader->idictSize * sizeof(struct cocol_dict_elem));
    if(!pelem) {
		return -1;
    }
    
    memset(pelem,0,pheader->idictSize * sizeof(struct cocol_dict_elem));
    pheader->pelem = pelem;
}

int 
cocol_dict_lookup(struct cocol_type_header *pelem)
{
    int i = 0;
	assert(pelem);
    for()
}

struct cocol_base_type g_cocol_dict = {
    COCOL_HEADER_OBJINIT(g_cocol_type)
	.name = "dict",
    .encocolType = COOCL_TYPE_dict,
    
}

