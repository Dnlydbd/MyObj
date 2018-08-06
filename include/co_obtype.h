/*
 * type.h 
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

#ifndef TYPE_H
#define TYPE_H
#include <string.h>

enum _type
{
    COOCL_TYPE_TYPE = 0,
	COCOL_TYPE_INT,
	COCOL_TYPE_CHAR,
	COCOL_TYPE_STRING,
	COCOL_TYPE_ARRAY,
	COOCL_TYPE_DICT,
	COCOL_TYPE_DOUBLE,
	COCOL_TYPE_FLOAT,
	COCOL_TYPE_UNKNOWN
};

typedef union _typedata{
    char *privData;                             /* string对象私有数据 */
    char data;									/* 字符对象私有数据 */
    unsigned int idata;							/* 整形对象私有数据 */
}typedata;

/* cocol_type_header 已经不区分是什么类型以及类型的构造函数了，代码中所有的入参类型都应该是cocol_type_header类型 */
struct cocol_type_header
{
	struct cocol_base_type *pcocolBaseType;		/* 基础类型: 属于哪个类型 */
	struct cocol_type_header *pcocolType;		/* 继承类型: 暂时不用 */ 
    typedata typeData;							/* 对象数据内容: 内容字段 */
    char *privData;                             /* 对象私有数据: 每个对象的私有数据 */
	unsigned int irefcount;						/* 对象引用计数:  */
    unsigned int iheapIdx;						/* 内存中的序列号 */
};

#define CO_HEADER_OBJINIT(type,pinhertype,privdata)	\
	.pcocolBaseType = type,\
	.pcocolType = pinhertype,	\
	.typeData = 0,	\
	.privData = privdata,	\
	.irefcount = 0,		\
	.iheapIdx = -1,
	
#define CO_OBJ_HEADER 	struct cocol_type_header objHeader

struct cocol_base_type
{
    struct cocol_type_header objHeader;			/* obj header: include the private data */
	char name[12];								/* obj name */
	enum _type encocolType;						/* it meams the objtype */

    /* obj private data malloc & free ,if you wang store any private data bonding with the obj,usb coocl_malloc */
	char * (*cocol_malloc)(int);
	void (*cocol_free)(char *);
    
    /* obj operation */
	int (*cocol_objinit)(struct cocol_type_header *,void *);
	int (*cocol_objdup)(struct cocol_type_header *,struct cocol_type_header *);
    int (*cocol_objcmp)(struct cocol_type_header *,struct cocol_type_header *);
    int (*cocol_objdestroy)(struct cocol_type_header *);
    int (*cocol_objhash)(struct cocol_type_header *);
	void (*cocol_objprint)(struct cocol_type_header *);
};

#endif
