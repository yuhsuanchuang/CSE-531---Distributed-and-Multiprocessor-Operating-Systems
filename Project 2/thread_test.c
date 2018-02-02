//Name: Yiqian Zhang, ASUID: 1211820737
//Name: Yu-Hsuan Chuang, ASUID: 1211219305

#include "threads.h"
int global = 0;

void function1()
{
    int local1 = 1;

    while (1)
    {
        printf("Thread %d:\tLocal: %d\tGlobal: %d\n", Curr_Thread->thread_id, local1, global);
        global++;
        local1 = local1 * 2;
        sleep(2);
        yield();
    }
}

void function2()
{
    int local2 = 1;

    while (1)
    {
        printf("Thread %d:\tLocal: %d\tGlobal: %d\n", Curr_Thread->thread_id, local2, global);
        global++;
        local2 = local2 * 4;
        sleep(2);
        yield();
    }
}

void function3()
{
    int local3 = 1;

    while (1)
    {
        printf("Thread %d:\tLocal: %d\tGlobal: %d\n", Curr_Thread->thread_id, local3, global);
        global++;
        local3 = local3 * 8;
        sleep(2);
        yield();
    }
}

void function4()
{
    int local4 = 1;
    while (1)
    {
        printf("Thread %d:\tLocal: %d\tGlobal: %d\n", Curr_Thread->thread_id, local4, global);
        global++;
        local4 = local4 * 16;
        sleep(2);
        yield();
    }
}

int main()
{
    ReadyQ = (queue *)malloc(sizeof(queue));
    NewQueue(ReadyQ);

    start_thread(function1);
    start_thread(function2);
    start_thread(function3);
    start_thread(function4);
    run();
    return 0;
}
