#include <stdio.h>
#include <stdlib.h>
#include "TCB.h"

typedef struct
{
    TCB_t *head;
} queue;

//head = NewQueue();
void NewQueue(queue *q)
{
    TCB_t *head;
    q->head = NULL;
    //    return head;
}

//item = NewItem();
TCB_t *NewItem()
{
    TCB_t *item;
    item = (TCB_t *)malloc(sizeof(TCB_t));
    //scanf("%d", &item->thread_id);
    //printf("%d\n", item->thread_id);
    item->next = NULL;
    item->prev = NULL;
    return item;
}

//AddQueue(head, item);
void AddQueue(queue *q, TCB_t *item)
{
    if (q->head == NULL)
    {
        q->head = item;
        q->head->next = item;
        q->head->prev = item;
        //printf("NULLNULLNULL\n");
    }
    else
    {
        item->prev = q->head->prev;
        item->next = q->head;
        q->head->prev->next = item;
        q->head->prev = item;
        //printf("NOTNULLNOTNULLNOTNULL\n");
    }
}

//item = DelQueue(head);
TCB_t *DelQueue(queue *q)
{

    if (q->head == NULL)
        return NULL;

    TCB_t *reserve = q->head;
    if (q->head->next == q->head)
    {
        q->head = NULL;
        return reserve;
    }

    q->head->next->prev = q->head->prev;
    q->head->prev->next = q->head->next;
    q->head = q->head->next;
    reserve->next = reserve->prev = NULL;
    return reserve;
}

void RotateQueue(queue *q)
{
    if (q->head != NULL)
        q->head = q->head->next;
}

void PrintQueue(queue *q)
{
    TCB_t *cur = q->head;
    printf("Data in queue:\n");
    do
    {
        printf(" -> %d", cur->thread_id);
        cur = cur->next;
    } while (cur != q->head);
    printf("\n");
}