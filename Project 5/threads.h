#include "q.h"
TCB_t *ReadyQ;
TCB_t *Curr_Thread;
int counter = 1;

void start_thread(void (*function)(void), char *filename)
{ // begin pseudo code
    int size = 8192 * 2;
    void *stack;
    stack = (void *)malloc(size);

    TCB_t *item;
    item = (TCB_t *)malloc(sizeof(TCB_t));
    init_TCB(item, function, stack, size, filename);
    item->thread_id = counter;
    //printf("Thread id: %d\n", item->thread_id);
    counter++;
    AddQueue(&ReadyQ, item);
    //end pseudo code
}

void run()
{ // real code

    Curr_Thread = DelQueue(&ReadyQ);
    //Curr_Thread = ReadyQ->head;
    ucontext_t parent;
    getcontext(&parent);
    swapcontext(&parent, &(Curr_Thread->context)); // start the first thread
    //swapcontext(&parent, &(ReadyQ->head->context)); // start the first thread
}

void yield() // similar to run
{
     TCB_t *Prev_Thread;
     AddQueue(&ReadyQ, Curr_Thread);
     Prev_Thread = Curr_Thread;
     Curr_Thread = DelQueue(&ReadyQ);
     swapcontext(&(Prev_Thread->context), &(Curr_Thread->context));
}
