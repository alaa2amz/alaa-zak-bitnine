#include <stdio.h>
#include "pool.h"
#define POOLSIZE 32 * 1024
int main(int arc,char *argv[]){
	printf("hello\n");
	Header *r=init_pool(POOLSIZE);
	printf("%ld\n",(long int)r);
	void * ppp = palloc(1000);
	printf("att-1st15\n");
	void  * oppp = palloc(15000);
	printf("att-2nd15\n");
	void * ooppp = palloc(15000);
	pfree(oppp);
	if(!palloc(14000))puts("not done the done");
	/*void *gggg=palloc(64000);*/
	return 0;
}

