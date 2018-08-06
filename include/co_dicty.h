#ifndef _DICT_H_
#define _DICT_H_

struct cocol_dict_elem
{
    //struct cocol_type cocoType;
	struct mm_pool_header *pinherHeader;		// ��ʾ������ָ����������͵�����ͷ ,--- �̳�����
	unsigned int iheapIdx;
    struct cocol_dict_header *pinherDict;		// �̳����� 
}

struct cocol_dict_header
{	
    struct cocol_type_header objHeader;
    struct cocol_dict_elem *pmmBase;			// ���ݿ�
    unsigned int idictSize;						// ���ݴ�С
    unsigned int idictUsed;						// �Ѿ�ʹ�õ����ݴ�С
    int (*cocol_dict_look_up)(struct cocol_dict_header *pdictHeader,struct cocol_type_header *hashKey,unsigned int hash);
};

#endif
