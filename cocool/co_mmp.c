/*
 * mem.c 
 * Copyright (C) 2018 -Dnlydbd
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License (version 2) as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 */
#include "mem.h"

//#define 	NULL   		0
#if 1
#define TAILQ_FIRST_ELEM(head)  do { \
	return head->tqh_first;		\
}
#endif

#define MM_POOL_ROUNDUP(a,b) ( (a + b - 1) & ~(b - 1) )

struct mm_pool_header g_stringHeader = {
	.pdataType = &g_cocol_string,
    .proot = NULL,
    .pmemData = NULL,
    .imemSize = MM_POOL_DEFAULT_STRINGSIZE,
    .imemLen = MM_POOL_DEFAULT_STRINGLEN,
    .binitialized = 0,
};

struct mm_pool_header g_intHeader = {
	.pdataType = &g_cocol_int,
    .proot = NULL,
    .pmemData = NULL,
    .imemSize = MM_POOL_DEFAULT_INTSIZE,
    .imemLen = MM_POOL_DEFAULT_INTLEN,
    .binitialized = 0,
};

struct mm_pool_header g_charHeader = {
	.pdataType = &g_cocol_char,
    .proot = NULL,
    .pmemData = NULL,
    .imemSize = MM_POOL_DEFAULT_CHARSIZE,
    .imemLen = MM_POOL_DEFAULT_CHARLEN,
    .binitialized = 0,
};

/**
 * @brief  replace the heapIdx one with pelem.
 *
 * @param t  pool header
 * @param t  the hool need replace
 * @param t  the elem need to be replaced
 *
 * @return NO_ERROR on success.
 */
 
static int 
mm_pool_shit_up(struct mm_pool_header *pheader,unsigned int heapIdx,struct cocol_type_header *pelem)
{
	assert(pheader && pelem);
    int idataheapIdx = 0;
    //char *ptmp = NULL;
    //ptmp = pheader->proot[heapIdx]->pmem;
    //pheader->proot[heapIdx]->pmem = pelem->pmem;
    //pelem->pmem = ptmp;
    struct cocol_type_header tmpType;
    
    memcpy(&tmpType,&pheader->proot[heapIdx],sizeof(struct cocol_type_header));
    memcpy(&pheader->proot[heapIdx],pelem,sizeof(struct cocol_type_header));
    memcpy(pelem,&tmpType,sizeof(struct cocol_type_header));
    return 0;
}

/**
 * @brief  replace the heapIdx one with pelem.
 *
 * @param t  pool header
 * @param t  the hool need replace
 * @param t  the elem need to be replaced
 *
 * @return NO_ERROR on success.
 */

static int 
mm_pool_shit_down(struct mm_pool_header *pheader,unsigned int heapIdx,struct cocol_type_header *pelem)
{
	assert(pheader && pelem);
    int idataheapIdx = 0;
    //char *ptmp = NULL;
    //ptmp = pheader->proot[heapIdx]->pmem;
    //pheader->proot[heapIdx]->pmem = pelem->pmem;
    //pelem->pmem = ptmp;
    
    idataheapIdx = pheader->proot[heapIdx].iheapIdx;
    pheader->proot[heapIdx].iheapIdx = pelem->iheapIdx;
    pelem->iheapIdx = idataheapIdx;
    return 0;
}

//#define mm_pool_free(pelem)		(pelem->iused = FALSE)
static int 
mm_pool_free_elem(struct mm_pool_header *pheader,struct cocol_type_header *pelem)
{
	int  i = 0;
	assert(pelem && pheader);
    mm_pool_shit_down(pheader,pheader->imemFree++,pelem);
    pheader->imemUsed--;
    #if 0
	TAILQ_INSERT_TAIL(&pheader->tailqFreeHead,pelem,tailqElem);
	for(i = 0; i < MM_POOL_BUFF; i++) {
		TAILQ_INSERT_TAIL(&pheader->tailqFreeHead,pheader->buff[i],tailqElem);
	}
	#endif
	return 0;
}

static int
mm_pool_expand(struct mm_pool_header *pheader)
{
    int i = 0;
    assert(pheader);
    struct cocol_type_header *pelem = NULL;
    char *pmemData = NULL;
	pelem = (struct cocol_type_header *) cocol_realloc((char *)(pheader->proot),(pheader->imemSize + MM_POOL_EXPAND)*sizeof(struct cocol_type_header));
	if(!pelem) {
		return -1;
	}

    pheader->proot = pelem;
    pmemData = (char *) cocol_realloc(pheader->pmemData,(pheader->imemSize + MM_POOL_EXPAND) * pheader->imemLen);
	if(!pmemData) {
		return -1;
	}
    
    pheader->pmemData = pmemData;
    pheader->imemUsed += MM_POOL_EXPAND;
    for(i = 0;i < MM_POOL_EXPAND;i++) {
		pheader->proot[pheader->imemSize].iheapIdx = pheader->imemSize;
        mm_pool_free_elem(pheader,&pheader->proot[pheader->imemSize++]);
    }
    
    return 0;
}

static int 
mm_pool_get_free(struct mm_pool_header *pheader)
{
	int i = 0;
	assert(pheader && pheader->proot);
	struct cocol_type_header *pelem = NULL;

    #if 0
	/* put it into the free list */
	for(i = 0; i < MM_POOL_BUFF; i++) {
		TAILQ_INSERT_TAIL(&pheader->tailqFreeHead,pheader->buff[i],tailqElem);
		//pelem = (struct mm_pool_elem *)pheader->tailqHead.tqh_first;
		pelem = (struct mm_pool_elem *)TAILQ_FIRST_ELEM(pheader->tailqHead);
		TAILQ_REMOVE(&pheader->tailqHead,pelem,tailqElem);
		pheader->buff[i] = (struct mm_pool_elem *)pelem;
	}
    #endif
    if(pheader->imemFree < MM_POOL_BUFF) {
		if(mm_pool_expand(pheader) == -1) {
			return -1;
        }
    }

    for(i = 0; i < MM_POOL_BUFF; i++) {
        memcpy(&pheader->buff[i],&pheader->proot[--pheader->imemFree],sizeof(struct cocol_type_header));
		pheader->buff[i].privData = &pheader->pmemData[pheader->imemLen * pheader->proot[pheader->imemFree].iheapIdx];
        //mm_pool_shit_up(pheader,0,&pheader->proot[--pheader->imemFree]);
        pheader->imemUsed++;
    }
    
    #if 0
    for(i = 0; i < MM_POOL_BUFF; i++) {
        memcpy(&pheader->buff[i],&pheader->proot[0],sizeof(struct cocol_type));
		pheader->buff[i].privData = &pheader->pmemData[pheader->imemLen * pheader->proot[0].iheapIdx];
        mm_pool_shit_up(pheader,0,&pheader->proot[--pheader->imemFree]);
        pheader->imemUsed++;
    }
	#endif
	return 0;
}

/**
 * @brief  get a free elem.
 *
 * @param t  mempool header
 *
 * @return NULL on failure.
 */

static void 
mm_pool_init_free_elem(struct mm_pool_header *pheader)
{
	assert(pheader && pheader->proot);
	if(pheader->buffMask == MM_POOL_BUFF) {
		if(mm_pool_get_free(pheader) == -1) {
			return;
        }
		pheader->buffMask = 0;
        return ; 
	}
}

/**
 * @brief  get a free elem.
 *
 * @param t  mempool header
 *
 * @return NULL on failure.
 */

static struct cocol_type_header * 
mm_pool_get_free_elem(struct mm_pool_header *pheader)
{
	assert(pheader && pheader->proot);
    //printf("pheader->buffMask : %d\n",pheader->buffMask);
	if(pheader->buffMask == MM_POOL_BUFF) {
		if(mm_pool_get_free(pheader) == -1) {
			return NULL;
        }
		pheader->buffMask = 0;
		//pheader->buff[0]->iused = TRUE;
		return &pheader->buff[pheader->buffMask++];
	}
	else {
		//pheader->buff[pheader->buffMask]->iused = TRUE;
		//printf("pheader->buff[] : %p\n",&pheader->buff[pheader->buffMask]);
		return &pheader->buff[pheader->buffMask++];
	}
}


static struct cocol_type_header * 
mm_pool_check_elem(struct mm_pool_header *pheader,struct cocol_type_header *pdata)
{
    int i = 0;
	assert(pheader && pdata);
    for(i = pheader->imemFree;i < pheader->imemSize;i++) {
        //printf("pheader->proot[i].iheapIdx : %d\n",pheader->proot[i].iheapIdx);
		if(0 == memcmp(&pheader->pmemData[pheader->imemLen * pheader->proot[i].iheapIdx],pdata->privData,pheader->imemLen)) {
			pheader->proot[i].irefcount++;
            //pheader->proot[i]->pmem = &pheader->pmemData[pheader->proot[i]->iheapIdx];
            return &pheader->proot[i];
        }
    }
    
    return NULL;
}

static struct cocol_type_header *
mm_pool_get_elem(struct mm_pool_header *pheader,struct cocol_type_header *pdata)
{
    struct cocol_type_header *pelem = NULL;
    if(pheader->binitialized == 0) {
		return NULL;
    }
    
    pelem = mm_pool_check_elem(pheader,pdata);
	if(pelem == NULL) {
		pelem = mm_pool_get_free_elem(pheader);
        //printf("mm_pool_get_free_elem ,,, pelem->pcocolBaseType : %p\n",pelem->pcocolBaseType);
        return pelem;
    }
    
	return pelem;
    
}

/*
 * - dnly_func/mm_pool_manage
 * - manage the used mem blocks & put the unused blocks mem to free list
 */

int 
mm_pool_manage(struct mm_pool_header *pheader)
{
	assert(pheader && pheader->proot);
	
}

int mm_pool_data_print()
{
	int i = 0;
    struct mm_pool_header *pheader = NULL;
    pheader = &g_intHeader;
    printf("int type free \nhole :				data:\n");
    for(i = 0; i < pheader->imemFree;i++) {
		printf("%d				%d\n",pheader->proot[i].iheapIdx,(int)pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }
    
    printf("\n\nused \nhole :				data:\n");
    for(i = pheader->imemFree; i < pheader->imemSize;i++) {
		printf("%d				%d\n",pheader->proot[i].iheapIdx,(int)pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }

    pheader = &g_charHeader;
    printf("char type free \nhole :				data:\n");
    for(i = 0; i < pheader->imemFree;i++) {
		printf("%d				%c\n",pheader->proot[i].iheapIdx,(char)pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }
    
    printf("\n\nused \nhole :				data:\n");
    for(i = pheader->imemFree; i < pheader->imemSize;i++) {
		printf("%d				%c\n",pheader->proot[i].iheapIdx,(char)pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }

    pheader = &g_stringHeader;
    printf("string type free \nhole :				data:\n");
    for(i = 0; i < pheader->imemFree;i++) {
		printf("%d				%s\n",pheader->proot[i].iheapIdx,&pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }
    
    printf("\n\nused \nhole :				data:\n");
    for(i = pheader->imemFree; i < pheader->imemSize;i++) {
		printf("%d				%s\n",pheader->proot[i].iheapIdx,&pheader->pmemData[pheader->imemLen*pheader->proot[i].iheapIdx]);
    }
    
}

void 
mm_pool_status_print()
{
	struct mm_pool_header *pheader = NULL;
    pheader = &g_intHeader;
    printf("int header......................\n");
    printf("pheader->proot : %p\n",pheader->proot);
    printf("pheader->pmemData : %p\n",pheader->pmemData);
    printf("pheader->imemSize : %d\n",pheader->imemSize);
    printf("pheader->imemFree : %d\n",pheader->imemFree);
    printf("pheader->imemUsed : %d\n",pheader->imemUsed);
    printf("pheader->imemLen : %d\n",pheader->imemLen);
    printf("pheader->buffMask : %d\n",pheader->buffMask);
    printf("pheader->binitialized : %d\n",pheader->binitialized);

    pheader = &g_charHeader;
    printf("char header......................\n");
    printf("pheader->proot : %p\n",pheader->proot);
    printf("pheader->pmemData : %p\n",pheader->pmemData);
    printf("pheader->imemSize : %d\n",pheader->imemSize);
    printf("pheader->imemFree : %d\n",pheader->imemFree);
    printf("pheader->imemUsed : %d\n",pheader->imemUsed);
    printf("pheader->imemLen : %d\n",pheader->imemLen);
    printf("pheader->buffMask : %d\n",pheader->buffMask);
    printf("pheader->binitialized : %d\n",pheader->binitialized);

    pheader = &g_stringHeader;
    printf("string header......................\n");
    printf("pheader->proot : %p\n",pheader->proot);
    printf("pheader->pmemData : %p\n",pheader->pmemData);
    printf("pheader->imemSize : %d\n",pheader->imemSize);
    printf("pheader->imemFree : %d\n",pheader->imemFree);
    printf("pheader->imemUsed : %d\n",pheader->imemUsed);
    printf("pheader->imemLen : %d\n",pheader->imemLen);
    printf("pheader->buffMask : %d\n",pheader->buffMask);
    printf("pheader->binitialized : %d\n",pheader->binitialized);
}

#define COCOL_INT_TYPECHECK(ptype) (COCOL_BASE_TYPE(ptype) == &g_cocol_int)
#define COCOL_BASE_TYPE(ptype)		ptype->pcocolBaseType
#define COCOL_INT_INITIALIZED		(g_intHeader.binitialized)
/**
 * @brief  pool init.
 *
 * @param t  one data len
 *
 * @return NULL on failred.
 */
struct mm_pool_header *
mm_pool_init(struct mm_pool_header *pheader)
{
	int i = 0;
    char *pdata = NULL;
    struct cocol_type *pelem = NULL;
    if(1 == pheader->binitialized) {
		return NULL;
    }
	
	pheader->imemLen = MM_POOL_ROUNDUP(pheader->imemLen,4);
    pheader->imemSize = MM_POOL_ROUNDUP(pheader->imemSize,4);
	pelem = (struct cocol_type *) cocol_malloc(pheader->imemSize * sizeof(struct cocol_type));
	if(!pelem) {
		return NULL;
	}
    
	pdata = (char *)cocol_malloc(pheader->imemSize*pheader->imemLen);
    if(!pdata) {
        cocol_free((char *)pheader);
		return NULL;
    }
    
	//TAILQ_INIT(&pheader->tailqHead);
	//TAILQ_INIT(&pheader->tailqFreeHead);
	memset(pelem, 0,pheader->imemSize*sizeof(struct cocol_type_header));
    memset(pdata,0,pheader->imemSize*pheader->imemLen);
    pheader->pmemData = pdata;
	pheader->proot = (struct cocol_type_header *)pelem;
	pheader->imemFree = pheader->imemSize;
	pheader->imemSize = pheader->imemSize;
	pheader->imemUsed = 0;
	pheader->buffMask = MM_POOL_BUFF;
    pheader->imemLen = pheader->imemLen;
    
	for(i = 0; i < pheader->imemSize; i++) {
		pheader->proot[i].privData = ((char *)pheader->pmemData) + pheader->imemLen * i;//&pheader->proot[i]->idata;
		pheader->proot[i].iheapIdx = i;
		pheader->proot[i].pcocolBaseType = (struct cocol_base_type *)pheader->pdataType;
        //printf("pheader->proot[i].pcocolBaseType : %p\n",pheader->proot[i].pcocolBaseType);
        //TAILQ_INSERT_TAIL(&pheader->tailqHead,&pheader->proot[i],tailqElem);
	}
    
    mm_pool_init_free_elem(pheader);
    pheader->binitialized = 1;
	return pheader;
}
#if 0
void
cocol_string_print(struct cocol_type *ptmp)
{
	return ;
}

struct cocol_type * 
cocol_string_malloc(struct cocol_type *ptmp)
{
    if(g_stringHeader.binitialized == 0) {
		mm_pool_init(&g_stringHeader);
    }
    
	return mm_pool_get_elem(&g_stringHeader,ptmp);
}

void 
cocol_string_free(struct cocol_type *ptmp)
{
	mm_pool_free_elem(&g_stringHeader,ptmp);
    return ;
}

struct cocol_type * 
cocol_int_malloc(struct cocol_type *ptmp)
{
    if(g_intHeader.binitialized == 0) {
		mm_pool_init(&g_intHeader);
    }
	return mm_pool_get_elem(&g_intHeader,ptmp);
}

void 
cocol_int_free(struct cocol_type *ptmp)
{
	mm_pool_free_elem(&g_intHeader,ptmp);
    return ;
}

struct cocol_type * 
cocol_char_malloc(struct cocol_type *ptmp)
{
    if(g_charHeader.binitialized == 0) {
		mm_pool_init(&g_charHeader);
    }
	return mm_pool_get_elem(&g_charHeader,ptmp);
}

void 
cocol_char_free(struct cocol_type *ptmp)
{
	mm_pool_free_elem(&g_charHeader,ptmp);
    return;
}
#endif

void 
cocol_mm_pool_init()
{
	mm_pool_init(&g_stringHeader);
    mm_pool_init(&g_intHeader);
    mm_pool_init(&g_charHeader);
}

/* cocol inheritance funcs */
#define COCOL_FUNC_FREE(type)	\
void cocol_##type##_free(struct cocol_type *ptmp) \
{	\
	mm_pool_free_elem(&g_##type##Header,ptmp);	\
}

#define COCOL_FUNC_MALLOC(type)	\
struct cocol_type * cocol_##type##_malloc(struct cocol_type *ptmp) \
{	\
	if(g_##type##Header.binitialized == 0) {	\
		mm_pool_init(&g_##type##Header);	\
    }	\
	return mm_pool_get_elem(&g_##type##Header,ptmp);	\
}

#define COCOL_FUNC_PRINT(type)	\
void cocol_##type##_print(struct cocol_type *ptmp) \
{	\
    \
}

#define COCOL_FUNC_DUP(type)	\
int cocol_##type##_dup(struct cocol_type *pdes,struct cocol_type *psrc) \
{	\
    memcpy(pdes,psrc,sizeof(struct cocol_type));	\
}

#define COCOL_FUNC_INIT(type)	\
int cocol_##type##_init(struct cocol_type *ptmp,void *pdata) \
{	\
    if(g_##type##Header.binitialized == 0) {	\
		mm_pool_init(&g_##type##Header);	\
    }	\
    memcpy(ptmp->privData,pdata,g_##type##Header.imemLen);	\
}


COCOL_FUNC_FREE(int)
COCOL_FUNC_MALLOC(int)
COCOL_FUNC_PRINT(int)
COCOL_FUNC_DUP(int)
COCOL_FUNC_INIT(int)

COCOL_FUNC_FREE(char)
COCOL_FUNC_MALLOC(char)
COCOL_FUNC_PRINT(char)
COCOL_FUNC_DUP(char)
COCOL_FUNC_INIT(char)

COCOL_FUNC_FREE(string)
COCOL_FUNC_MALLOC(string)
COCOL_FUNC_PRINT(string)
COCOL_FUNC_DUP(string)
COCOL_FUNC_INIT(string)


/* cocol obj type init funcs */
#define COCOL_FUNC_OBJ_TYPE_INIT(name)	\
void cocol_type_##name##_init(struct cocol_type *ptype)	\
{	\
	ptype->pcocolBaseType = &g_cocol_##name;	\
}

COCOL_FUNC_OBJ_TYPE_INIT(int)
COCOL_FUNC_OBJ_TYPE_INIT(char)
COCOL_FUNC_OBJ_TYPE_INIT(string)

#if 0
/* cocol obj data init funcs */
#define COCOL_FUNC_OBJ_DATA_INIT(name)	\
void cocol_data_##name##_init(struct cocol_type *ptype)	\
{	\
	ptype->pcocolBaseType = &g_cocol_##name;	\
}

COCOL_FUNC_OBJ_TYPE_INIT(int)
COCOL_FUNC_OBJ_TYPE_INIT(char)
COCOL_FUNC_OBJ_TYPE_INIT(string)
#endif
