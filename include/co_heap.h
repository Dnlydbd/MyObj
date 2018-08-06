/*
 *  cocol_func/heap.h
 *
 *  Copyright (C) 2018, 2020  cocol - Dnlydbd
 */
#ifndef _HEAP_H_
#define _HEAP_H_

struct cocol_heap
{
	char code;		// �洢����ֵ
	int refConut;	// ���ø���
	int heapIdx;	// �����ڶ��е����кš���0��ʼ
};

struct cocol_heap_header
{
	//pthread_mutex_t heapMutex;
	struct cocol_heap **proot;
	int heapSize;
	int totalSize;
};

#endif
