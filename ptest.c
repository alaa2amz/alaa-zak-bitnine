#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "pool.h"

/*data*/

char help_message[]=
" ptest\n"
" test bed for pool allocator\n"
"  -p poolsize in bytes\n"
"  -s size of allocations\n"
"  -n number of allocations if not given it will run till it is filled\n"
"  -r random size not exceeding max\n"
"  -u unoredered deallocation\n"
"------------------deffred-------------------------------\n"
"  -c chaos unordered allocation and deallocation the programm\n"
"     in that case shoiuld be run for a pacific time andrecord\n"
"------------------deffred--------------------------------\n"
"     statstics\n"
"  -h help print this message\n"
"     Alaa Zakariya 2023-08-23\n";

int poolsize=128*1024;
int asize=1024; 	/*allocation size in bytes*/
int anumber=130; 	/*allocations numbr  */
int rand_max=0; 	/*maximum random size*/
int unordered=0; 	/*ordered allocation and deallocation*/
int chaos=0; 		/*unordere */
int c;


struct data_record {
	int size;
	void * pointer;
	double atime;
	double dtime;
};

typedef struct data_record Record;
Record * records;

char optstring[]="p:s:n:r:uc";

/* functions */
double time_palloc(void ** p,int bytes); /*return calculated time of allocation and
					   pointe in args as pointer pointer*/
double time_pfree(void *p); 		/*return time of freeing p pointer*/	



int main(int argc,char* argv[]){
if(argc ==1){/******/
printf("%s\n",help_message);
return 0;
}
	/*process args*/
	while ((c = getopt (argc, argv, optstring)) != -1)
		switch (c){
			case 'p':
				poolsize = atoi(optarg);
				break;
			case 's':
				asize = atoi(optarg);
				break;
			case 'n':
				anumber = atoi(optarg);
				break;
			case 'r':
				rand_max= atoi(optarg);
				break;
			case 'u':
				unordered= 1;
				break;
			case 'h':
				printf("%s\n",help_message);
				return 0;
				break;	
			case 'c':
				chaos= 1;
				break;
			case '?':
				if (optopt == 'c')
					fprintf (stderr, "Option -%c requires an argument.\n", optopt);
				else if (isprint (optopt))
					fprintf (stderr, "Unknown option `-%c'.\n", optopt);
				else
					fprintf (stderr,"Unknown option character `\\x%x'.\n",optopt);
					return 1;
				default:
				abort ();
			}

if(!init_pool(poolsize)){
fprintf(stderr,"eror initialising pool");
}else{
puts("pool initialised successfully");
}



/*allocating data array*/
records=(Record *)malloc(sizeof(Record)*anumber);
if(records == NULL){
	fprintf(stderr,"eror allocating data");
	exit(1);
}


/* allocating */
void *v;
srand(time(NULL));
Record* r=records;
for(int i=0;i<anumber;i++,r++){
	if(rand_max){
	asize=rand()%rand_max;
	}
	r->size=asize;
	r->atime=time_palloc(&v,asize);
	r->pointer=v;
	/*r->dtime=time_pfree(v);*/
	printf("%d size:%d time2alloc: %f pointer: %p\n",i+1,r->size,r->atime,r->pointer);
}
/* free them again */

r=records;
for(int i=0;i<anumber;i++,r++){
if(r-pointer)r->dtime=time_pfree(r->pointer);
}

/*print and calculate statics */



return 0;
}	
/*
void * n;
double test=time_palloc(&n,asize);
printf("time: %f , pointer: %p",test,n);

}
*/

double time_palloc(void ** p,int bytes){
double time;
clock_t start, end;
start = clock();
*p=palloc(bytes);
end = clock();
time = ((double)(end - start))/CLOCKS_PER_SEC;
return time;
}


double time_pfree(void *p){
double time;
clock_t start, end;
start = clock();
pfree(p);
end = clock();
time = ((double)(end - start))/CLOCKS_PER_SEC;
return time;
}
