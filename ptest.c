#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>
#include "pool.h"

/*data*/

char help_message[]=
" ptest: test bed pool memory manager allocator\n"
"--------------------------options-------------------------------\n"
"  -p [BYTES] poolsize in bytes. default 32MB\n"
"  -s [BYTES] size of allocations default 5MB\n"
"  -n [NUM] number of allocations default\n"
"  -r [MAX_NUM] random size not exceeding max overiding -s\n"
"  -u {TODO} unoredered allocation deallocation according to plan or scenario\n"
"  -c {TODO} chaos unordered allocation and deallocation the programm\n"
"     in that case shoiuld be run for a pacific time and record\n"
"     statstics\n"
"  -h help print this message\n"
"---------------------------------------------------------------\n"
"     Alaa Zakariya 2023-08-23\n"
"----------------------------------------------------------------\n";


/*---------------  DATA -----------------*/
int poolsize=32*1024;
int asize=5*1024; 	/*allocation size in bytes*/
int anumber=10; 	/*allocations numbr  */
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

/*--------------- FUNCTIONS ----------------*/
double time_palloc(void ** p,int bytes); /*return calculated time of allocation and
					   pointe in args as pointer pointer*/
double time_pfree(void *p); 		/*return time of freeing p pointer*/	


/*-------------------- MAIN FUNCTION --------------*/
int main(int argc,char* argv[]){
/*
if(argc ==1){
printf("%s\n",help_message);
return 0;
}
*/

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
}

/* free them again */
r=records;
for(int i=0;i<anumber;i++,r++){
if(r->pointer)r->dtime=time_pfree(r->pointer);
else
	r->dtime=0;
}

/*print and calculate statics */
r=records;
unsigned memmax=0,memmin=0,memavrg=0,memtotal=0;
double alltavrg=0,detavrg=0,allttotal=0,dettotal=0;
int failcount=0;
for(int i=0;i<anumber;i++,r++){
	allttotal += r->atime;
	dettotal += r->dtime;
	memtotal += r->size;
	memmax= r->size > memmax ? r->size : memmax;
	memmin= r->size < memmin ? r->size : memmin;
	r->pointer? failcount : failcount++;
	printf("%2d size:%6d pointer:%14p t_alloc:%f t_free:%f\n",i+1,r->size,r->pointer,r->atime,r->dtime);
}
memavrg=memtotal/anumber;
alltavrg=allttotal/anumber;
detavrg=dettotal/anumber;
printf("\tavrg_alloc_time %f sec\n\
	avrg_dealloc_time %f sec\n\
	max_alloc %d bytes\n\
	min_alloc %d bytes\n\
	avr_alloc %d bytes\n\
	total number %d\n\
	fails %d\n\
	please run with -h option for help\n",
	alltavrg,detavrg,memmax,memmin,memavrg,anumber,failcount);


return 0;
}	

/* abstracting allocating time
 * in function returning time and passed
 * pointer to pointer to set it to allocated
 * void pointer
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


/*abstracting time to deallocate*/
double time_pfree(void *p){
double time;
clock_t start, end;
start = clock();
pfree(p);
end = clock();
time = ((double)(end - start))/CLOCKS_PER_SEC;
return time;
}
