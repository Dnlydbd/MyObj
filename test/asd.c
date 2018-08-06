#include <stdio.h>


struct aaa{
	int a;
	int p;
};

int main()
{
	struct aaa **p;
	printf("%d  %d\n",sizeof(*p),sizeof(**p));
return 0; 
}
