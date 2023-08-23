#ifndef POOL
#define POOL
void * palloc(unsigned nbytes);
void pfree(void *mp);
typedef double Align;
union header {
	struct  {
		union header *next;     
		unsigned size;          
	}s;
	Align x;        
};                                                                

typedef union header Header;
Header * init_pool(unsigned size);
#endif
