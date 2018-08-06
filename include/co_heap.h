/*
 *  cocol_func/heap.h
 *
 *  Copyright (C) 2018, 2020  cocol - Dnlydbd
 */
#ifndef _HEAP_H_
#define _HEAP_H_

struct cocol_heap
{
	char code;		// 存储的数值
	int refConut;	// 引用个数
	int heapIdx;	// 数据在堆中的序列号。从0开始
};

struct cocol_heap_header
{
	//pthread_mutex_t heapMutex;
	struct cocol_heap **proot;
	int heapSize;
	int totalSize;
};

#endif
