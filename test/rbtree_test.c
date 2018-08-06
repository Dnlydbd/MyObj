

#include <stdio.h>
#include <string.h>
#include "rbtree.h"


typedef struct{
    rb_node_t rbNode;
    char szData[64];
    int inum;
}STRING_STOR;
 

STRING_STOR *prbRoot = NULL;
STRING_STOR * 
cocol_rbregister_string(STRING_STOR *pstring)
{
    rb_node_t **p = (rb_node_t **) &prbRoot;
    STRING_STOR *pnode = NULL;
    STRING_STOR *entry = NULL;
    //printf("*p : %p",*p);
    while (*p)
    {
        //printf("aaaaaaaaaaaaa\n");
      entry = (STRING_STOR *) *p;
      if (pstring->inum < entry->inum)
			p = &(*p)->rb_left;
      else if (pstring->inum > entry->inum)
			p = &(*p)->rb_right;
      else
	  {
		  return (pstring);
	  }
    }

  pnode = (STRING_STOR *) malloc(sizeof(STRING_STOR));
  pnode->rbNode.rb_parent = (rb_node_t *) entry;
  pnode->rbNode.rb_left = pnode->rbNode.rb_right = NULL;
  strncpy(pnode->szData,pstring->szData,sizeof(pstring->szData));
  pnode->inum = pstring->inum;
  *p = &(pnode->rbNode);

  rb_rebalance(&pnode->rbNode, (rb_node_t **) &prbRoot);
  return (pstring);
}
#define STRING_NUM 		25

int
main()
{
    int i = 0;
    STRING_STOR stringtmp;
    STRING_STOR *ptmpstring = NULL;
    rb_node_t *ptmpNode = NULL;
    rb_traverse_t rbtravers;
    for(i = 32;i>0;i--) {
        memset(&stringtmp,0,sizeof(stringtmp));
        stringtmp.inum = i;
        sprintf(stringtmp.szData,"hello world hahahahaa %d",i);
		cocol_rbregister_string(&stringtmp);
    }
    
    for(ptmpNode = rb_first((rb_node_t *)prbRoot, &rbtravers);ptmpNode!= NULL;ptmpNode = rb_next(&rbtravers)) {
        ptmpstring = (STRING_STOR *)ptmpNode;
		printf("inum : %d ptmpNode->ptmpstring : %s \n",ptmpstring->inum,ptmpstring->szData);
    }
    return 0;
}
