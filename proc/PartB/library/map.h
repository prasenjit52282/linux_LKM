/*****************************************
* Group:1
* Name:Prasenjit Karmakar,   Roll: 22CS91P03
* Name:Harsh Borse,          Roll: 21CS72P02
******************************************/
#include <linux/slab.h>
#include <linux/kernel.h>
#include <linux/uaccess.h>
#include "pqueue.h"

#ifndef MAP_H_
#define MAP_H_

struct Entry {
    int pid;
    struct pQueue *queue;
    struct Entry *next;

    //temp vars
    int32_t _val,_prio;
    int state;
};

struct Map {
    int size;
    struct Entry *head;
};

struct Map* initMap(void);
void destroyMap(struct Map *m);
struct Entry* getMapEntry(struct Map *m, int pid);
struct Entry* createEntry(struct Map *m, int pid);
void removeEntry(struct Map *m, int pid);
void printMap(struct Map *m);

struct Map* initMap() {
    struct Map *m= (struct Map *) kmalloc(1*sizeof(struct Map),GFP_ATOMIC);
    m->size=0;
    m->head = NULL;
    return m;
}

void destroyMap(struct Map *m){
    kfree(m);
}

struct Entry* getMapEntry(struct Map *m, int pid){
    struct Entry *ptr = m->head;
    while (ptr!=NULL){
        if (ptr->pid==pid){
            return ptr;
        }
        ptr=ptr->next;
    }
    return ptr;
}

struct Entry* createEntry(struct Map *m, int pid){
    struct Entry *item=(struct Entry *) kmalloc(1*sizeof(struct Entry),GFP_ATOMIC);
    item->pid=pid;
    item->state=0;
    item->queue=NULL;
    item->next = m->head;
    m->head=item;
    m->size+=1;
    return m->head;
}

void removeEntry(struct Map *m, int pid){
    struct Entry *temp = m->head;
    struct Entry *prev = NULL;

    if (temp != NULL && temp->pid == pid){
        m->head = temp->next;
	destroyQueue(temp->queue);
        kfree(temp);
        m->size-=1;
        return;
    }else {
        while (temp != NULL && temp->pid != pid) {
            prev = temp;
            temp = temp->next;
        }
        if (temp == NULL)
            return;
        prev->next = temp->next;
	destroyQueue(temp->queue);
        kfree(temp);
        m->size-=1;
    }
}

void printMap(struct Map *m){
    if(m->size>0){
        struct Entry *ptr = m->head;
        while (ptr!=NULL)
            {
                printk(KERN_INFO "<pid=%d>\n",ptr->pid);
                printQueue(ptr->queue);
                ptr = ptr -> next;
            }
    }else{
        printk(KERN_INFO "empty map\n");
    }
}

#endif
