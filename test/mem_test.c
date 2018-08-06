

#include <stdio.h>
#include <string.h>
#include "mem.h"

#define MM_MALLOC_LEN 		12

int main()
{
    int i = 0;
    char tmp[32] = {0};
    struct cocol_type stringType;
    struct cocol_type *pobjType = NULL;
	cocol_mm_pool_init();
    mm_pool_status_print();
    cocol_type_string_init(&stringType);
	for(i = 0;i < 8;i++) {
        sprintf(tmp,"hello %d",i);
        stringType.privData = &tmp;
        printf("in ...\n");
        pobjType = stringType.pcocolBaseType->cocol_malloc(&stringType);
        printf("malloc success pobjType->pcocolBaseType: %p\n",pobjType->pcocolBaseType);
        printf("pobjType->pcocolBaseType->cocol_init : %p\n",pobjType->pcocolBaseType->cocol_init);
        pobjType->pcocolBaseType->cocol_init(pobjType,tmp);
        printf("out  \n");
    }
    
	mm_pool_data_print();
    //mm_pool_status_print();
    return ;
}
