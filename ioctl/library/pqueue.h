/*****************************************
* Group:1
* Name:Prasenjit Karmakar,   Roll: 22CS91P03
* Name:Harsh Borse,          Roll: 21CS72P02
******************************************/
#include <linux/slab.h>
#include <linux/kernel.h>

#ifndef PQUEUE_H_
#define PQUEUE_H_

struct Node {
    int prio;
    int value;
    struct Node *next;
};

struct pQueue {
    int size;
    int maxsize;
    struct Node *head;
};

struct pQueue* initQueue(int maxsize);
void destroyQueue(struct pQueue *q);
void enQueue(struct pQueue *q, int prio, int value);
int deQueue(struct pQueue *q);
void printQueue(struct pQueue *q);

struct pQueue* initQueue(int maxsize) {
    struct pQueue *q= (struct pQueue *) kmalloc(1*sizeof(struct pQueue),GFP_ATOMIC);
    q->size=0;
    q->maxsize=maxsize;
    q->head = NULL;
    return q;
}

void destroyQueue(struct pQueue *q) {
    kfree(q);
}

void enQueue(struct pQueue *q, int prio, int value){
    struct Node *item=(struct Node *) kmalloc(1*sizeof(struct Node),GFP_ATOMIC);
    item->prio=prio;
    item->value=value;
    item->next = NULL;

    if (q->head == NULL) {
        q->head = item;
    }else if (q->head->prio<item->prio){
	item->next=q->head;
	q->head=item;
    }else{
        struct Node *ptr = q->head;
        while (ptr->next!=NULL)
            {  if (ptr->next->prio>=item->prio){
                    ptr = ptr -> next;
                }else{
                    break;
                }
            }
        item->next=ptr->next;
        ptr->next=item;
    }
    q->size+=1;
}

int deQueue(struct pQueue *q){
    int value;
    struct Node *item;
    if (q->size>0){
        item=q->head;
        q->head=q->head->next;
        q->size-=1;
        value=item->value;
        kfree(item);
        return value;
    }else{
        return -1;
    }
}

int deQueueFromLast(struct pQueue *q){
    int value;
    struct Node *ptr,*item;
    if (q->size>0){
        ptr=q->head;
        while (ptr->next->next!=NULL){ptr=ptr->next;}
        item=ptr->next;
        ptr->next=NULL;
        q->size-=1;
        value=item->value;
        kfree(item);
        return value;
    }else{
        return -1;
    }
}

int getMax(struct pQueue *q){
    return deQueue(q);
}

int getMin(struct pQueue *q){
    return deQueueFromLast(q);
}

void printQueue(struct pQueue *q){
    struct Node *ptr;
    if(q==NULL){
	printk(KERN_INFO "uninit queue\n");
    }else if (q->size>0){
	printk(KERN_INFO "------[%d|%d]\n",q->size,q->maxsize);
        ptr = q->head;
        while (ptr!=NULL){
                printk(KERN_INFO "(p=%d,v=%d)\n",ptr->prio,ptr->value);
                ptr = ptr -> next;
        }
	printk(KERN_INFO "------\n");
    }else{
        printk(KERN_INFO "empty queue\n");
    }
}

#endif
