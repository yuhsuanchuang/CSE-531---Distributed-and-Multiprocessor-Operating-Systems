#include "threads.h"

typedef struct semaphore_t {
    int count;
    TCB_t *Q;
} semaphore_t;

// mallocs a semaphore structure, initializes it to the InitValue and returns the pointer to the semaphore.
semaphore_t *CreateSem(int InputValue)
{
	semaphore_t *s = (semaphore_t *)malloc(sizeof(semaphore_t));

	if (s == NULL)
	    exit(0);
	s->count = InputValue;
	s->Q = NULL;
	return s;
}

// takes a pointer to a semaphore and performs P, 
//i.e. decrements the semaphore, 
//and if the value is less than zero then blocks the thread in the queue associated with the semaphore.
void P(semaphore_t *s)
{   
    TCB_t *wait = NULL;
    TCB_t *temp = NULL;
    s->count--;
    
    if (s->count < 0){
	wait = DelQueue(&ReadyQ);
    	AddQueue(&s->Q, Curr_Thread);
	temp = Curr_Thread;
	Curr_Thread = wait;
	swapcontext(&(temp->context),&(wait->context));	
    }
}

// increments the semaphore, and if the value is 0 or negative, 
//then takes a PCB out of the semaphore queue and puts it into the ReadyQ. 
void V(semaphore_t * s)
{   
    TCB_t *curr;
    s->count++;

    if(s->count <= 0)
    {
	curr = DelQueue(&(s->Q));
	AddQueue(&ReadyQ, curr);
    }
    yield();
}
