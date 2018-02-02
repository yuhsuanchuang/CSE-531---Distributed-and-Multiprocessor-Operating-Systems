#include "threads.h"

typedef struct semaphore_t {
    int count;
    queue *Q;
} semaphore_t;

// mallocs a semaphore structure, initializes it to the InitValue and returns the pointer to the semaphore.
semaphore_t *CreateSem(int InputValue)
{
	semaphore_t *s = (semaphore_t *)malloc(sizeof(semaphore_t));
	s->count = InputValue;
	s->Q = (queue *)malloc(sizeof(queue));
	return s;
}

// takes a pointer to a semaphore and performs P, 
//i.e. decrements the semaphore, 
//and if the value is less than zero then blocks the thread in the queue associated with the semaphore.
void P(semaphore_t *s)
{   
    s->count--;
    
    if (s->count < 0){
    	TCB_t *temp;
    	temp = DelQueue(ReadyQ);
    	AddQueue(s->Q,temp);
		swapcontext(&(s->Q->head->prev->context),&(ReadyQ->head->context));	
    }
}

// increments the semaphore, and if the value is 0 or negative, 
//then takes a PCB out of the semaphore queue and puts it into the ReadyQ. 
void V(semaphore_t * s)
{   
    s->count++;

	if(s->count <= 0)
		AddQueue(ReadyQ,DelQueue(s->Q));

	yield();
}
