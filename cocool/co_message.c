#include <pthread.h>
#include<sys/types.h>  
#include<sys/socket.h>  
#include<netinet/in.h>  
#include<arpa/inet.h>  
#include<errno.h>  
#include<unistd.h>  
  
#include<stdio.h>  
#include<string.h>  
#include<stdlib.h>  
  
#include"event.h"  
#include"event2/util.h"

#define  MS_DOMAIN_PATH			"/tmp/message_bus"
#define  MS_LISTEN_NUM			(256)
#define  MS_PROCESS_NUM			(4)
#define  EV_TIMEOUT		        (60)	
#define  MS_QUENS_NUM			(60)
#define  MS_BUFF_LEN			(1024)
#define  MS_ITEM_LEN			(sizeof(MS_QUENS_ITEM) + MS_BUFF_LEN)
#define  MS_TIMEOUT_TIME		(5)

static MS_THREAD *g_pms_thread = NULL;
static MS_QUENS_ITEM *g_pms_quenes = NULL;
static pthread_mutex_t g_msquene_lock = PTHREAD_MUTEX_INITIALIZER;

MS_QUENS_ITEM *
ms_item_alloc()
{
	MS_QUENS_ITEM *pms_item = NULL;
	//assert(g_pms_quenes);
	pthread_mutex_lock(&g_msquene_lock);
	if(g_pms_quenes) {
		pms_item = g_pms_quenes;
		g_pms_quenes = pms_item->pnext;
	}
	pthread_mutex_unlock(&g_msquene_lock);
	
	return pms_item;	
}

int 
ms_item_free(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);
	pthread_mutex_lock(&g_msquene_lock);
	pms_item->pnext = g_pms_quenes;
	g_pms_quenes = pms_item;
	pthread_mutex_unlock(&g_msquene_lock);
	
	return 0;	
}

int 
ms_quene_push(MS_QUENS *pms_quene,MS_QUENS_ITEM *pms_item)
{
	assert(pms_quene && pms_item);
	pms_item->pnext = NULL;
	pthread_mutex_lock(&pms_quene->msquene_lock);
	if(NULL == pms_quene->ptail) {
		pms_quene->phead = pms_item;
	}
	else {
		pms_quene->ptail->pnext = pms_item;
	}
	pms_quene->ptail = pms_item;
	pthread_mutex_unlock(&pms_quene->msquene_lock);
	
	return 0;	
}

MS_QUENS_ITEM * 
ms_quene_pop(MS_QUENS *pms_quene)
{
	MS_QUENS_ITEM *pms_item = NULL;
	assert(pms_quene);
	pthread_mutex_lock(&pms_quene->msquene_lock);
	pms_item = pms_quene->phead;
	if(NULL != pms_item) {
		pms_quene->phead = pms_item->pnext;
	}
	else {
		pms_item = NULL;
	}
	
	if(NULL == pms_quene->phead) {
		pms_quene->ptail = NULL;
	}
	pthread_mutex_unlock(&pms_quene->msquene_lock);
	
	return pms_item;	
}


void 
ms_handler(int isockfd,short ievent,void *pargs)
{
	assert(isockfd > 0);
	MS_QUENS_ITEM *pms_item = NULL;
	struct event* evaccept = NULL;
	
	pms_item = (MS_QUENS_ITEM *)pargs;	
	if(EV_READ == ievent) {
		pms_item->istatus = MS_COMMOND;
	}
	else if(EV_TIMEOUT == EV_READ) {
		pms_item->istatus = MS_TIMEOUT;
	}
	else
	{
		MS_ERR("ievent[%d] error\n",ievent);
		return ;
	}
	
	message_dispatch(pms_item);
	
	return ;
}

void 
ev_accept_cb(int isockfd,short eventtpye,void *pevbase)
{
	char notifyMs = 0;
	int ilen = 0;
	int iacceptfd = -1;
	static int ithread_id = 0;
	struct event_base* pbase = NULL;
	struct sockaddr_un cliaddr;
	MS_QUENS_ITEM *pms_item = NULL;
	//pbase = (struct event_base*)pevbase;
	iacceptfd = accept(isockfd,(struct sockaddr*)&cliaddr,&ilen);   
    if(iacceptfd < 0)   
    {   
        perror("cannot accept client connect request unix MS_PATH[%s]\n",MS_DOMAIN_PATH);   
        //close(isockfd);   
        //unlink(MS_DOMAIN_PATH);   
        //return 1;   
    }
	
	notifyMs = 'w';
	pms_item = ms_item_alloc();
	pms_item->isockfd = iacceptfd;
	memset(pms_item->soket_evt,0,sizeof(pms_item->soket_evt));
	memset(pms_item->time_evt,0,sizeof(pms_item->time_evt));
	ms_quene_push(&g_pms_thread[ithread_id].ms_quene,pms_item);
	write(g_pms_thread[ithread_id++].ms_notify_sendfd,&notifyMs,1);
	
	return ;
}

void
ms_item_init(MS_QUENS_ITEM *pms_item)
{
	memset(&pms_item->time_evt,0,sizeof(pms_item->time_evt));
	memset(&pms_item->soket_evt,0,sizeof(pms_item->soket_evt));
	pms_item->istatus = 0;
	pms_item->iwriteIdx = 0;
	pms_item->isockfd = -1;
	pms_item->timeval.tv_sec = MS_TIMEOUT_TIME;
	pms_item->timeval.tv_usec = 0;
}
void 
ms_timeout_process(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);

	SAFE_CLOSE(pms_item->isockfd);
	event_del(&pms_item->soket_evt);
	event_del(&pms_item->time_evt);
	ms_item_init(pms_item);
	return ;
}

int 
ms_new_connect(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);
	int iretval = 0;
	struct event *pevent = NULL;
	struct event_base *pbase = NULL;
	struct timeval timeval;
	
	memset(&timeval,0,sizeof(timeval));
	pevent = &pms_item->soket_evt;
	pbase = pevent->ev_base;
	iretval = event_assign(pevent, pbase, pms_item->isockfd, EV_READ|EV_PERSIST, ms_handler, pms_item);
	if(iretval < 0) {
		MS_ERR("event assign failed \n");
		SAFE_CLOSE(pms_item->isockfd);
		return -1;
	}
	
	iretval = event_add(pevent,NULL);
	if(iretval < 0) {
		MS_ERR("event add failed \n");
		SAFE_CLOSE(pms_item->isockfd);
		return -1;
	}

	pevent = &pms_item->time_evt;
	iretval = event_assign(pevent, pbase, NULL, EV_TIMEOUT, ms_handler, pms_item);
	if(iretval < 0) {
		MS_ERR("event assign failed \n");
		SAFE_CLOSE(pms_item->isockfd);
		event_del(&pms_item->time_evt);
		return -1;
	}

	timeval.tv_sec = MS_TIMEOUT_TIME;
	timeval.tv_usec = 0;
	pms_item.timeval = timeval;
	iretval = event_add(pevent,&timeval);
	if(iretval < 0) {
		SAFE_CLOSE(pms_item->isockfd);
		event_del(&pms_item->time_evt);
		MS_ERR("event add failed \n");
		return -1;
	}

	return 0;
}

int 
ms_process(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);
	
	int ilen = 0;
	int iwriteidx = 0;
	int itotolbuflen = 0;
	char *pdata = NULL;
	MS_H ms_header;
	pdata = pms_item->pdatabuf;
	iwriteidx = pms_item->iwriteIdx;
	itotolbuflen = pms_item->ibufflen;
	memset(&ms_header,0,sizeof(ms_header));
	
	ilen = read(pms_item->isockfd,pdata+iwriteidx,itotolbuflen-iwriteidx);
	if(ilen <= 0) {
		// it means that client closed the socket
		SAFE_CLOSE(pms_item->isockfd);
		event_del(&pms_item->soket_evt);
		event_del(&pms_item->time_evt);
		return ;
	}
	
	if(ilen != sizeof(ms_header)) {
		MS_ERR("ilen[%d] headrlen[%d] error \n",ilen,sizeof(ms_header));
		
	}

	
}

void 
ms_read_more(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);
	
	int ilen = 0;
	int iwriteidx = 0;
	int itotolbuflen = 0;
	char *pdata = NULL;
	MS_H ms_header;
	pdata = pms_item->pdatabuf;
	iwriteidx = pms_item->iwriteIdx;
	itotolbuflen = pms_item->ibufflen;
	memset(&ms_header,0,sizeof(ms_header));
	
	ilen = read(pms_item->isockfd,&ms_header,sizeof(ms_header));
	if(ilen <= 0) {
		// it means that client closed the socket
		SAFE_CLOSE(pms_item->isockfd);
		event_del(&pms_item->soket_evt);
		event_del(&pms_item->time_evt);
		return -1;
	}
	
	if(ilen != sizeof(ms_header)) {
		MS_ERR("ilen[%d] headrlen[%d] error \n",ilen,sizeof(ms_header));
	}
	//todo: 
	
}


void 
message_dispatch(MS_QUENS_ITEM *pms_item)
{
	assert(pms_item);
	int iretval = -1;
	
	switch (pms_item->istatus) {
		case MS_NEW_CONNECT:
			iretval = ms_new_connect(pms_item);
			if(-1 == iretval) {
				ms_item_free(pms_item);
			}
			break;
		case MS_PROCESS:
			iretval = ms_process(pms_item);
			if(-1 == iretval) {
				ms_item_free(pms_item);
			}
			break;
		case MS_READ_MORE:
			iretval = ms_read_more(pms_item);
			if(-1 == iretval) {
				ms_item_free(pms_item);
			}
			break;
		case MS_TIMEOUT:
			ms_timeout_process(pms_item);
			ms_item_free(pms_item);
			break;
		default:
			break;
	}
	
	return;
}

void 
message_notify(int inotify_fd,short ievent,void *pargs)
{
	char cnothfy_code = 0;
	MS_THREAD *pms_thread = NULL;
	MS_QUENS_ITEM *pms_item = NULL;
	struct event *pnothfy_event = NULL;
	if(EV_READ != (EV_READ & ievent)) {
		perror("accept error \n")
		return ;
	}
	
	pms_thread = (MS_THREAD *) pargs;
	pnothfy_event = pms_thread->notify_event;
	if(1 != read(inotify_fd,&cnothfy_code,1) {
		perror("notify accept error \n");
		return ;
	}
	
	pms_item = ms_quene_pop(pms_thread);
	if(NULL == pms_item) {
		MS_ERR("pms_item is nil \n");
		return ;
	}
	switch(cnothfy_code) {
		case 'w':
			message_dispatch(pms_thread);
			break;
			
	}
	
	
}

void 
message_thread_setup(MS_THREAD *pms_thread)
{
	int iretval = -1;
	struct event_base *pbase = NULL;
	
	pbase = event_base_new();
	if(NULL == pbase) {
		perror("event base new failed \n");
		return;
	}
	
	pms_thread->pbase = pbase;
	iretval = event_assign(&pms_thread->notify_event, pbase, pms_thread->ms_notify_recfd, EV_READ|EV_PERSIST, message_notify, &pms_thread)
	if(iretval < 0) {
		mm_free(pms_thread->pbase);
		pms_thread->pbase = NULL;
		perror("event assign failed \n");
		return ;
	}
	
	iretval = event_add(&pms_thread->notify_event,NULL);
	if(iretval < 0) {
		perror("event add failed \n");
		mm_free(pms_thread->pbase);
		pms_thread->pbase = NULL;
		return ;
	}

	SIMPLEQ_HEAD_INITIALIZER(&pms_thread->header);
}

void
message_thread_proc(MS_THREAD *pms_thread)
{
	assert(pms_thread);
	
	event_base_dispatch(pms_thread->pbase);
	event_base_free(pms_thread->pbase);
	return;
}

void 
message_thread_start(MS_THREAD *pms_thread)
{
	int iretval = -1;
	iretval = cpthread_creat(&pms_thread->ithread_id,250*1024,COMMON_PRIO,message_thread_proc,pms_thread);
	if(iretval < 0) {
		perror("cpthread_creat error \n");
		return ;
	}
	return ;
}

void 
ms_thread_process_init()
{
	int i = 0;
	char *pdata = NULL;
	int ifds[2] = {0};
	MS_QUENS_ITEM *pms_quenes = NULL;
	g_pms_thread = (MS_THREAD*)malloc(MS_PROCESS_NUM * sizeof(MS_THREAD));
	if(NULL == g_pms_thread){
		perror("pms_thread malloc error \n");
		abort();
	}
	
	memset(g_pms_thread,0,MS_PROCESS_NUM*sizeof(MS_THREAD))
	for(i = 0;i < MS_PROCESS_NUM; i++) {
		if (pipe(ifds)) {
            perror("Can't create notify pipe");
            abort();
        }
		g_pms_thread[i].ms_quene.phead = NULL;
		g_pms_thread[i].ms_quene.phead = NULL;
		pthread_mutex_init(&g_pms_thread[i].ms_quene.msquene_lock,NULL);
		g_pms_thread[i].ms_notify_recfd = ifds[0];
		g_pms_thread[i].ms_notify_sendfd = ifds[1];
		message_thread_setup(&g_pms_thread[i]);
		message_thread_start(&g_pms_thread[i]);
	}

	pthread_mutex_init(&g_msquene_lock,NULL);
	pdata = (char *)malloc(MS_QUENS_NUM * MS_ITEM_LEN);
	pms_quenes = (MS_QUENS_ITEM *)pdata;
	for(i = 0; i < MS_QUENS_NUM - 1; i++) {
		pms_quenes[i].pnext = &pms_quenes[i+1];
		pms_quenes[i].pdatabuf = (MS_QUENS_NUM*sizeof(MS_QUENS_ITEM) + i*MS_BUFF_LEN);
		memset(pms_quenes[i].pdatabuf,0,MS_BUFF_LEN);
	}
	
	pms_quenes[MS_QUENS_NUM - 1] = NULL;
	g_pms_quenes = &pms_quenes[0];
	
}

int 
message_base_server()
{
	int iret = -1;
	int ilen = 0;
	int iacceptfd = -1;
	int isockfd = -1;
	pthread_t pthreadM;
	struct sockaddr_un seraddr;
	struct sockaddr_un cliaddr;
	struct event_base* base = NULL;
	struct event* evlisten = NULL;
	bzero(&seraddr,sizeof(struct sockaddr_un));
	bzero(&cliaddr,sizeof(struct sockaddr_un)); 
	isockfd = socket(PF_UNIX,SOCK_STREAM,0);
	if(isockfd < 0){
		perror("cannot creat unix socket MS_PATH[%s] \n",MS_DOMAIN_PATH);
		abort();	
	}
	
	socket_set_noneblock(isockfd);
	seraddr.sun_family = AF_UNIX;
	strncpy(seraddr.sun_path,MS_DOMAIN_PATH,sizeof(seraddr.sun_path);
	unlink(MS_DOMAIN_PATH); 
	iret = bind(isockfd,(struct sockaddr*)&seraddr,sizeof(seraddr));
	if(iret < 0) {
		perror("cannot bind unix socket MS_PATH[%s] \n",MS_DOMAIN_PATH);
		close(isockfd);
		abort();
	}
	
	iret = listen(isockfd,MS_LISTEN_NUM);
	if(iret < 0) {
		perror("cannot listen unix socket MS_PATH[%s] \n",MS_DOMAIN_PATH);
		close(isockfd);
		abort();
	}

	base = event_base_new();
	evlisten = event_new(base, isockfd, EV_READ | EV_PERSIST, ev_accept_cb, base); 
	event_add(evlisten, NULL);
	event_base_dispatch(base);
	event_base_free(base);
	#if 0
	while(1) {
		iacceptfd = accept(isockfd,(struct sockaddr*)&cliaddr,&ilen);   
	    if(iacceptfd < 0)   
	    {   
	        perror("cannot accept client connect request unix MS_PATH[%s]\n",MS_DOMAIN_PATH);   
	        close(isockfd);   
	        unlink(MS_DOMAIN_PATH);   
	        return 1;   
	    }
		
		cpthread_creat(&pthreadM,256*1024,50,message_proc,1,iacceptfd);
	}
	#endif
	
}


int 
message_register()
{
	ms_thread_process_init();
	message_base_server();
}
















