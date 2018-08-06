/*
 * init.h 
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
#ifndef _INIT_H_
#define _INIT_H_
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#ifndef COCOL_TYPE
#define COCOL_TYPE(TYPE)	extern struct TYPE  
#endif

//#ifndef COCOL_MEM_EXPORT
//#define COCOL_MEM_EXPORT		extern struct mm_pool_header
//#endif

struct cocol_global
{
	char funcName[32];
	char curWorkDir[64];
	
};


#endif
