#ifndef _DICT_H_
#define _DICT_H_

struct cocol_dict_elem
{
    //struct cocol_type cocoType;
	struct mm_pool_header *pinherHeader;		// 表示该数据指向的其他类型的数据头 ,--- 继承类型
	unsigned int iheapIdx;
    struct cocol_dict_header *pinherDict;		// 继承类型 
}

struct cocol_dict_header
{	
    struct cocol_type_header objHeader;
    struct cocol_dict_elem *pmmBase;			// 数据块
    unsigned int idictSize;						// 数据大小
    unsigned int idictUsed;						// 已经使用的数据大小
    int (*cocol_dict_look_up)(struct cocol_dict_header *pdictHeader,struct cocol_type_header *hashKey,unsigned int hash);
};

#endif
