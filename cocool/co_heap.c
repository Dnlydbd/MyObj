#if 0
/*
 *  cocol_func/heap.c
 *
 *  Copyright (C) 2018, 2020  cocol - Dnlydbd
 */

//struct cocol_heap_header *pg_letter_heap = NULL;
#define COCOL_HEAP_SIZE		128
#define cocol_heap_data_cmp
#define cocol_heap_elem_greater(a, b) ((a->refConut > b->refConut)?TRUE:FALSE)

struct cocol_heap_header * cocol_heap_header_init(int totalSize)
{
	struct cocol_heap_header *pheader = NULL;
	pheader = (struct cocol_heap_header *) cocol_malloc(sizeof(struct cocol_heap_header));
	if(pheader) {
		memset(pheader,0,sizeof(struct cocol_heap_header));
		pheader->heapSize = 0;
		pheader->proot = NULL;
		pheader->totalSize = totalSize;
	}
	
	return pheader;
}

int cocol_heap_elt_is_top_(const struct cocol_heap *pcont)
{
	return pcont->heapIdx == 0;
}

int cocol_heap_adjust(struct cocol_heap_header *pheader)
{
	assert(pheader);
	int isize = pheader->heapSize;
	struct cocol_heap **proot = NULL;
	
	if(NULL == pheader->proot || pheader->heapSize <= 1) {
		return 0;
	}
	
	pheader->heapSize = 1;
	proot = pheader->proot;
	while(pheader->heapSize < isize) {
		cocol_heap_shift_up(pheader,pheader->heapSize,proot[pheader->heapSize]);
		pheader->heapSize++;
	}
	
	return;
}

int cocol_heap_elem_free(struct cocol_heap_header *pheader)
{
	assert(pheader);
	int isize = pheader->heapSize;
	while(isize) {
		if(pheader->proot[isize-1]->refConut == -1) {
			pheader->heapSize--;
			pheader->proot[isize-1]->heapIdx = -1;
			isize --;
		}
		else {
			break;
		}
	}
}

int cocol_heap_size(struct cocol_heap_header *pheader)
{
	assert(pheader);
	return pheader->heapSize;
}

int cocol_heap_empty(struct cocol_heap_header *pheader)
{
	assert(pheader);
	return (pheader->heapSize == 0);
}

int cocol_heap_init(struct cocol_heap_header *pheader)
{
	struct cocol_heap **proot = NULL;
	assert(pheader && pheader->totalSize >= pheader->heapSize);
	if(pheader->heapSize == 0) {
		proot = (struct cocol_heap **) cocol_malloc(COCOL_HEAP_SIZE * sizeof(struct cocol_heap));
		if(proot == NULL) {
			pheader->proot = NULL;
			return -1;
		}
		memset(proot,0,COCOL_HEAP_SIZE * sizeof(struct cocol_heap));
		proot->refConut = 1;
		proot->pfront = NULL;
		proot->pnext = NULL;
		pheader->proot = proot;
		pheader->heapSize = 1;
		pheader->totalSize = COCOL_HEAP_SIZE;
	}
	return 0;
}

void cocol_heap_shift_up_froce(struct cocol_heap_header *pheader, unsigned int heapIndex, struct cocol_heap *pcont)
{
	assert(pheader && pcont);
	unsigned int parent = (heapIndex - 1) / 2;
	do{
		pheader->proot[heapIndex] = pheader->proot[parent];
		pheader->proot[heapIndex]->heapIdx = heapIndex;
		heapIndex = parent;
		parent = (heapIndex - 1) / 2;
	}while(heapIndex && cocol_heap_elem_greater(pcont,pheader->proot[parent]));
	pcont->heapIdx = heapIndex;
	pheader->proot[heapIndex] = pcont;
	return ;
}

void cocol_heap_shift_up(struct cocol_heap_header *pheader, unsigned int heapIndex, struct cocol_heap *pcont)
{
	assert(pheader && pcont && heapIndex > 0);
    unsigned int parent = (heapIndex - 1) / 2;
    while (heapIndex && cocol_heap_elem_greater(pcont,pheader->proot[parent])) {
		pheader->proot[heapIndex] = pheader->proot[parent];
		pheader->proot[heapIndex]->heapIdx = heapIndex;
		heapIndex = parent;
		parent = (heapIndex - 1) / 2;
    }
	
	pcont->heapIdx = heapIndex;
    pheader->proot[heapIndex] = pcont;
	return ;
}

void cocol_heap_shift_down(struct cocol_heap_header* pheader, int heapIndex, struct cocol_heap *pcont)
{
    int childIdx = 2 * (heapIndex + 1);
    while (childIdx < pheader->heapSize) {
		if(cocol_heap_elem_greater(pheader->proot[childIdx-1],pheader->proot[childIdx])) {
			childIdx -= 1; /* get the greater idx of Its child */
		}
		
		/* compare the child with pcont */	
		if(cocol_heap_elem_greater(pcont,pheader->proot[childIdx])) {
			break; /* find out the pcont is the greater */
		}

		pheader->proot[heapIndex] = pheader->proot[childIdx];
		pheader->proot[heapIndex]->heapIdx = heapIndex;
		heapIndex = childIdx;
		childIdx = 2 * (heapIndex + 1);
	}
	
	if(childIdx == pheader->heapSize) {
		if(cocol_heap_elem_greater(pheader->proot[childIdx - 1],pcont)) {
			pheader->proot[heapIndex] = pheader->proot[[childIdx - 1];
			pheader->proot[heapIndex]->heapIdx = heapIndex;
			pcont->heapIdx = childIdx - 1;
			pheader->proot[childIdx - 1] = pcont;
		}
		else {
			pcont->heapIdx = heapIndex;
			pheader->proot[heapIndex] = pcont;
		}
	}else {
		pcont->heapIdx = heapIndex;
		pheader->proot[heapIndex] = pcont;
	}
	
	return ;
}

int cocol_heap_root_delete(struct cocol_heap_header *pheader)
{
	if(pheader->heapSize) {
		cocol_heap_shift_down(pheader,0,pheader->proot[--pheader->heapSize]);
	}
}

int cocol_heap_elem_delete(struct cocol_heap_header *pheader,struct cocol_heap *pcont)
{
	assert(pheader && pcont);
	unsigned int parent = (pcont->heapIdx - 1) / 2;
	if(pcont->heapIdx == -1 && pheader->heapSize < 1) {
		return -1; // this node is not contented in root
	}
	
	if(pcont->heapIdx > 0 && cocol_heap_elem_greater(pheader->proot[pheader->heapSize-1], pheader->proot[parent])) {
		cocol_heap_shift_up_froce(pheader, pcont->heapIdx, pheader->proot[--pheader->heapSize]);
	}
	else {
		cocol_heap_shift_down(pheader, pcont->heapIdx, pheader->proot[--pheader->heapSize]);
	}
	
	return 0;
}

int cocol_heap_insert(struct cocol_heap_header *pheader,struct cocol_heap *pcont)
{
	assert(pheader && pcont);
	struct cocol_heap **proot = NULL;
	if(pheader->totalSize - pheader->heapSize > 1) {
		if(!(proot = (struct cocol_heap **)cocol_calloc(pheader->proot,COCOL_HEAP_SIZE * sizeof(struct cocol_heap)))) {
			return -1;
		}
		memset(proot[pheader->totalSize-1],0,sizeof(COCOL_HEAP_SIZE * sizeof(struct cocol_heap)));
		pheader->proot = proot;
		pheader->totalSize += COCOL_HEAP_SIZE;
	}

	cocol_heap_shift_up(pheader, pheader->heapSize++, pcont);
	return 0;
}
#endif
