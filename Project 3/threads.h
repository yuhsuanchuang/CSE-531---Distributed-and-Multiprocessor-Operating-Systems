#include "q.h"
queue *ReadyQ;
TCB_t *Curr_Thread;
int counter = 1;

void start_thread(void (*function)(void))
{ // begin pseudo code
    void *stack;
    stack = malloc(8192);

    TCB_t *item;
    item = (TCB_t *)malloc(sizeof(TCB_t));
    init_TCB(item, function, stack, 8192);
    item->thread_id = counter;
    printf("Thread id: %d\n", item->thread_id);
    counter++;
    AddQueue(ReadyQ, item);
    //end pseudo code
}

void run()
{ // real code
    //Curr_Thread = DelQueue(ReadyQ);
    Curr_Thread = ReadyQ->head;
    ucontext_t parent;   // get a place to store the main context, for faking
    getcontext(&parent); // magic sauce
    swapcontext(&parent, &(Curr_Thread->context)); // start the first thread
    //swapcontext(&parent, &(ReadyQ->head->context)); // start the first thread
}

void yield() // similar to run
{
	
	TCB_t *Prev_Thread;
	Prev_Thread = ReadyQ->head;
	RotateQueue(ReadyQ);
	swapcontext(&(Prev_Thread->context),&(ReadyQ->head->context)); 

    // TCB_t *Prev_Thread;
    // AddQueue(ReadyQ, Curr_Thread);
    // Prev_Thread = Curr_Thread;
    // Curr_Thread = DelQueue(ReadyQ);
    // swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));
}
