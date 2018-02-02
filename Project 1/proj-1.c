// student: Yu-Hsuan, Chuang 1211219305
// student: Yiqian Zhang 1211820737

#include <stdio.h>
#include <stdlib.h>
#include "sem.h"

int arr[3] = {0};
semaphore_t r_sem, w_sem, mutex;
int count = 0;

void adding(int* i)
{
	while(1)
	{
		P(&r_sem);
	    P(&mutex);
	    printf("Adding arr[%d]\n", *i);
	    arr[*i]++;
	    count++;
	    
	    sleep(1);

	    if(count == 3)
	    {
	    	V(&w_sem);
	    	count=0;
	    }
	    else
	    {
	    	V(&r_sem);
	    }
		V(&mutex);

	}
}   

void print()   
{
	while(1)
	{
		P(&w_sem);
	    P(&mutex);
	    
	    for(int i = 0; i < 3; i++)
            printf("arr[%d] = %d\n", i, arr[i]);
	    sleep(1);
	    V(&r_sem);
		V(&mutex);	
	}
}
int main()
{

	int id[3] = {0, 1, 2};

    init_sem(&mutex, 1);
    init_sem(&r_sem, 1);
    init_sem(&w_sem, 0);
    

    start_thread(adding, &id[0]);
    start_thread(adding, &id[1]);
    start_thread(adding, &id[2]);
    print();
    
    while(1) sleep(1);

    return 0;
}
