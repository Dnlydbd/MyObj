#include <stdarg.h>
#include "pthread.h"

void *
start_routine(void *param)
{
	pthread_param *pthreadparam = NULL;
	assert(param);
	pthreadparam = (pthread_param *)param;
	return pthreadparam->start_func(pthreadparam->param[0],pthreadparam->param[1],pthreadparam->param[2],pthreadparam->param[3],pthreadparam->param[4],
		pthreadparam->param[5],pthreadparam->param[6],pthreadparam->param[7],pthreadparam->param[8],pthreadparam->param[9]);
	
}


int 
cpthread_creat(pthread_t *pthreadMs,int stacksize,int ipriority,void *(*start_func) (void *),char argc, ...)
{
	int i = 0;
	int ipolicy = -1;
	int iscope = -1;
	int istacksize = -1;
	int idetachstatus = -1;
	int iinheritsched = -1;
	pthread_attr_t pthreadattr;
	struct sched_param scparam;
	pthread_param threadparam;
	
	bzero(&scparam,sizeof(scparam));
	bzero(&threadparam,sizeof(threadparam));
	bzero(&pthreadattr,sizeof(pthreadattr));

	if(argc > PTHREAD_ATGS_NUB) {
		argc = PTHREAD_ATGS_NUB;
	}
	va_list ap;
	va_start(ap,argc);
	for(i = 0; i < argc; i++) {
		threadparam.param[i] = va_arg(ap, void);
	}
	va_end(ap);
	
	pthread_attr_init(&pthreadattr);
	if(stacksize < 256) {
		istacksize = 256;
	}
	else if(stacksize > 8*1024*1024) {
		istacksize = 8*1024*1024;
	}
	else {
		istacksize = stacksize;
	}
	//pthread_attr_getsatcksize(&pthreadattr,&istacksice);
	//istacksice = 1024*1024;
	pthread_attr_setsatcksize(&pthreadattr,istacksize);

	#if 0
	pthread_attr_getdetachstatus(&pthreadattr,&idetachstatus);
	idetachstatus = PTHREAD_CREAT_DETACHED; 	/* thread detached */
	idetachstatus = PTHREAD_CREAT_JOINABLE; 	/* thread normal */
	pthread_attr_getdetachstatus(&pthreadattr,idetachstatus);
	
	pthread_attr_getinheritsched(&pthreadattr,&iinheritsched);
	iinheritsched = PTHREAD_INHERIT_SCHED;		/* thread inherit */
	iinheritsched = PTHREAD_EXPLICITE_SCHED;	/* thread self attr */
	pthread_attr_setinheritsched(&pthreadattr,iinheritsched);

	pthread_attr_getschedpolicy(&pthreadattr,&ipolicy);
	ipolicy = SCHED_FIFO;
	ipolicy = SCHED_RR;
	ipolicy = SCHED_OTHER;
	pthread_attr_setschedpolicy(&pthreadattr,ipolicy);

	pthread_attr_setscope(&pthreadattr,&iscope);
	iscope = PTHREAD_SCOPE_SYSTEM;
	iscope = PTHREAD_SCOPE_PROCESS;
	pthread_attr_setscope(&pthreadattr,iscope);
	#endif
	
	//pthread_attr_getschedparam(&pthreadattr,&scparam);
	if(ipriority > 99 || ipriority < 1) {
		scparam.sched_priority = 50;
	}
	else {
		scparam.sched_priority = ipriority;
	}
	pthread_attr_setschedparam(&pthreadattr,&scparam);
	
	threadparam.start_func = start_func;
	pthread_creat(pthreadMs,&pthreadattr,start_routine,(void *)&threadparam);
	pthread_attr_destroy(&pthreadattr);
	return 0;
}
		

