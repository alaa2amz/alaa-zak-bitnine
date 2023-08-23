/*
 * ALAA A. M. ZAKARIYA
 * 2023-08-22
 * pool.c
 * bitnine's task
 */
#include <stdlib.h>
#include <stdio.h>

#define POOLSIZE (32*1024) 


typedef double Align; /* it is better to use value from types sizes header
			like int_64 to align block size */

union header {
	struct  {
		union header *next; 	/*next free header*/
		unsigned size; 		/* size of this free chu*n*ck ;) */
	}s;
	Align x; 	/*controlling size of block by aligning in union */
};

typedef union header Header;

static Header *iheader; 	/* initialised header pointer*/
static Header *freeheader; 	/* first free header pointer*/
static unsigned isize = 0; 	/* intialised bytes size */

	/**function init_pool
	 * initilise pool of memory manager
	 * @param unsigned size memory size of the main pool
	 * @return on sucess pointer to first header or NULL in failing
	 */ 
Header * init_pool(unsigned size){
	/*for safty it is restricted to single pool
	 *but could beextended to multiple pools
	 *so the the init_pool should be follwed by
	 *free_pool to reintilise
	 * */
	if(iheader != NULL){
		fprintf(stderr,"pool already initialized");
		return iheader;
	}

	iheader=(Header *)malloc(size);
	/* if malloc failed iheader would 
	 * be NULL 
	 */
	
	if(iheader != NULL){/* in case of sucess */
	
	/* recording next free Entry which points to itself*/
	iheader->s.next=iheader; 
	
	/* the size of free space by blocks not by bytes */
	iheader->s.size=(size+sizeof(Header)-1)/sizeof(Header) + 1;
	
	/* the first or current free header available */
	freeheader=iheader;
	
	/*isize=size-sizeof(Header);*/
	
	/* initial size in bytes to make early checks */
	isize=size;
	
	/* freeheader=iheader;*/
	}
	return iheader;
}


/* pool alloc same as malloc*/
void * palloc(unsigned nbytes){
	
	/*check if error or uintialised*/
	if(iheader == NULL){
		fprintf(stderr,"error or uninitialised");
		return NULL;
	}
	
	/* check if size is greater than the pool size itself  */
	else if(nbytes > isize){
	       	fprintf(stderr,"size is too big");
       		return NULL;
	 	}		
	
	/* currnent and prevous header pinters and nuber of blocks  */
	Header * point;
	Header * prevpoint;
	unsigned nunits;
	
	/* converting nuber of giving bytes to nober of blockes same
	 * size as header
	 */
	nunits=(nbytes+sizeof(Header)-1)/sizeof(Header) + 1;
	
	/* setting current free entry to previous
	 * as preparation to enter the loop */
	prevpoint=freeheader;

	/* intially setting pointer next in previous  
	 * every round point to previous and next to point
	 * till finding fitting space
	 * */
	for(point=prevpoint->s.next;;prevpoint=point,point=point->s.next){
		if(point->s.size >= nunits){ /*big enough*/
			if(point->s.size == nunits)  /*exactly*/
				prevpoint->s.next = point->s.next;/*ulink point*/
			else {       /*allocate tail end*/
				point->s.size -=nunits; /*getting out wanted units*/
				point += point->s.size; /*shifting point location
							forward same units*/
				point->s.size = nunits; /*re adjusting size*/
				/******/
			}
			/* returning */
			freeheader = prevpoint;
			/* shifting the header to return pure space  */
			return (void*)(point+1);
		}
		if(point == freeheader) /*wrapped around free list*/
			return NULL;
	
	}
}


/* just as stdlib free()  */
void pfree(void *mp){
	Header *backpoint; /*back to pool pointer*/
	Header *point; /*current header pointer*/

	backpoint=(Header *)mp - 1;/*shift back one block to include header*/
	
	/*starting from free header trying to find the order where the chunk
	 * belongs and help it back to the pool safely by reuniun it by old
	 * empty friends back and forth if they are there */
	/*the toback point cant be simultanusly greater than current point
	 * and less than next point of the current i.e. thats where the wrap
	 * begins so quick and break the loop*/
	/*every round advance by setting current point to its next*/
	for(point = freeheader; !(backpoint > point && backpoint < point->s.next);point = point->s.next){
		/*if point greater than orcoincident with its next
		 * then it is on boundary for always next should be greater
		 * and in the same time to back is greater than point
		 * or less than its next then the to be freed chunck is on
		 * start or end*/
		if(point >= point->s.next && (backpoint > point || backpoint < point->s.next))break;
	}
	/*!!!!!the previous loop side effect of moving
	 * to freeheader*/
	
	/*reuniun the upper free*/
	/*if back+its size eq next's next ie freeheader which came by
	 * loops side effect so it is free space*/
	if(backpoint + backpoint->s.size == point->s.next) {
		/*I am back to pool add to me next'next size
		 * since it is empty too*/
		backpoint->s.size += point->s.next->s.size;
		/*and set my next to my next's next so we will
		 * become one empty container*/
		backpoint->s.next = point->s.next->s.next;
	
	} else {
		/*if not just find the next empty and set
		 * the pointer to it*/
		backpoint->s.next = point->s.next;
	}
	
	/*the reverse from point's standplace where it sat by for side effect
	 * to freeheader if it and its size equal the backpoint so
	 * they are backward adjecent and we can reunion them
	 * by the same methodology but in reverse*/
	if (point + point->s.size== backpoint ) {
		point->s.size += backpoint->s.size;
		point->s.next = backpoint->s.next;
	} else
		point->s.next = backpoint;
	/*set free header to the point after collesing if there is*/
	freeheader=point;
	
	/*and then we are done and freed the happy memmory to the
	 * the pool */
	/*note:since the a void function function no return value*/

}


#if 0

int main(int arc,char *argv[]){
	printf("hello\n");
	printf("iheader:%p,freeheader:%p,isize%d\n",iheader,freeheader,isize);
	Header *r=init_pool(POOLSIZE);
	printf("iheader:%p,freeheader:%p,isize%d\n",iheader,freeheader,isize);
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

/* allocate * cast * increse * test */
#endif
