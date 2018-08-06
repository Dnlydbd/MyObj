

#define  MS_NEW_CONNECT		1
#define  MS_PROCESS			2
#define  MS_READ_MORE		3
#define  MS_TIMEOUT			4


#define SAFE_FREE(p)		if(p) { free(p); p = NULL; } else { p = NULL } 
#define SAFE_CLOSE(fd)		if(fd > 0) { close(fd); fd = -1; } else { fd = -1 } 

typedef struct ms_quene_item {
	//SIMPLEQ_ENTRY(ms_quene_item) next;
	MS_QUENS_ITEM *pnext;
	struct event soket_evt;
	struct event time_evt;
	struct timeval timeval;
	int isockfd;
	int istatus;
	char *pdatabuf;
	int ibufflen;
	int iwriteIdx;
}MS_QUENS_ITEM;

typedef struct ms_quene {
	MS_QUENS_ITEM *phead;
	MS_QUENS_ITEM *ptail;
	pthread_mutex_t msquene_lock;
}MS_QUENS;


typedef struct ms_thread{
	pthread_t ithread_id;
	struct event_base *pbase;
	struct event notify_event;
	int ms_notify_recfd;
	int ms_notify_sendfd;
	//pthread_mutex_t pthread_msquene_lock;
	MS_QUENS ms_quene;
	//SIMPLEQ_HEAD(ms_disp_quene,ms_quene_item) disp_header;
	//SIMPLEQ_HEAD(ms_free_quene,ms_quene_item) free_header;
}MS_THREAD;


typedef struct message_header {
	int mslen;
	int mstype;
}MS_H;

typedef struct message_t {
		
}MS_T;


#define MS_ERR()	NULL
#define MS_INFO()	NULL