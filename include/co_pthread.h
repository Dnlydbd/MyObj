

#define  COMMON_PRIO		(50)
#define  HIGHER_PRIO		(90)


#define PTHREAD_ATGS_NUB	(10)

typedef struct pthread_param{
	void *(*start_func) (void param1,void param2,void param3,void param4,void param5,void param6,void param7,void param8,void param9,void param10);
	void param[10];
}pthread_param;