#include <stdio.h>
#include <stdlib.h>
#include "TCB.h"

//AddQueue(head, item);
void AddQueue(TCB_t **q, TCB_t *item)
{
    if (*q == NULL)
    {
        *q = item;
        item->next = *q;
        item->prev = *q;
    }
    else
    {
	item->next = *q;        
	item->prev = (*q)->prev;
        (*q)->prev->next = item;
	(*q)->prev = item;
    }
}

//item = DelQueue(head);
TCB_t* DelQueue(TCB_t **q)
{
    TCB_t* item = NULL;
    if (*q == NULL)
    {
	item = NULL;
    }
    else
    {
	if ((*q)->next == (*q))
	{
	    item = *q;
	    *q = NULL;
	}
	else
	{
	    item = *q;
	    (*q) = (*q)->next;
	    (*q)->prev = item->prev;
	    item->prev->next = *q;
	}
    }
    return item;
}
