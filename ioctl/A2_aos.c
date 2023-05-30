/*****************************************
* Group:1
* Name:Prasenjit Karmakar,   Roll: 22CS91P03
* Name:Harsh Borse,          Roll: 21CS72P02
******************************************/
#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/proc_fs.h>
#include <linux/uaccess.h>
#include "library/pqueue.h"
#include "library/map.h"
#include "library/ioctl_commands.h"

struct proc_dir_entry *myproc_entry;
struct Map *m;

static int myproc_open(struct inode *inode,struct file *file){
	struct Entry *e=getMapEntry(m,current->pid); int reset_flag=0;
	try_module_get(THIS_MODULE);
	if (e!=NULL){
		printk(KERN_ALERT "multiple open for %d is detected --reseting entry\n",current->pid);
		removeEntry(m,current->pid);reset_flag=1;
	}
	if(reset_flag==0){printk(KERN_ALERT "proc file is opened by %d --creating entry\n",current->pid);}
	createEntry(m,current->pid);
	return 0;
}

static int myproc_close(struct inode *inode,struct file *file){
	module_put(THIS_MODULE);
	printk(KERN_ALERT "proc file is closed by pid %d --removing entry\n",current->pid);
	removeEntry(m,current->pid);
	return 0;
}

long myproc_ioctl(struct file *f, unsigned int cmd, unsigned long arg){

	int32_t qsize;
	struct obj_info info;
	struct Entry *e=getMapEntry(m,current->pid);
	if(e->state==2){e->state=0;}

    switch(cmd){

    	case PB2_SET_CAPACITY:
    		if(copy_from_user(&qsize, (int32_t *)arg, sizeof(int32_t))){return -EACCES;}
    		if(qsize<1 || qsize>100){
				printk(KERN_ALERT "only 1 to 100 qsize is allowed for pid %d\n",current->pid);
				return -EINVAL;
			}
			if(e->queue!=NULL){
    			destroyQueue(e->queue);
    			printk(KERN_INFO "resetting priority queue with maxsize %d for pid %d\n",qsize,current->pid);
    		}else{printk(KERN_INFO "initilizing priority queue with maxsize %d for pid %d\n",qsize,current->pid);}
			e->queue=initQueue(qsize);
    		break;

		case PB2_INSERT_INT:
		case PB2_INSERT_PRIO:
			if(e->queue==NULL){
				printk(KERN_ALERT "uninit queue for pid %d\n",current->pid);
				return -EACCES;
			}else if(e->queue!=NULL && e->queue->size==e->queue->maxsize){
				printk(KERN_ALERT "queue is full for pid %d\n",current->pid);
				return -EACCES;
			}else if(e->state==0){
				e->state++;
				if(copy_from_user(&e->_val,(int32_t *)arg, sizeof(int32_t))){return -EACCES;}
				printk(KERN_INFO "read value %d from pid %d\n",e->_val,current->pid);
			}else if(e->state==1){
				e->state++;
				if(copy_from_user(&e->_prio,(int32_t *)arg, sizeof(int32_t))){return -EACCES;}
				printk(KERN_INFO "read priority %d from pid %d\n",e->_prio,current->pid);
				enQueue(e->queue, e->_prio, e->_val);
				printMap(m);
			}
			break;

		case PB2_GET_INFO:
			if(e->queue==NULL){
				printk(KERN_ALERT "uninit queue for pid %d\n",current->pid);
				return -EACCES;}
			info.prio_que_size=e->queue->size;
			info.capacity=e->queue->maxsize;
			if(copy_to_user((struct obj_info *)arg, &info, sizeof(struct obj_info))){return -EACCES;}
			break;

		case PB2_GET_MIN:
			if(e->queue==NULL){
				printk(KERN_ALERT "uninit queue for pid %d\n",current->pid);
				return -EACCES;
			}else if(e->queue!=NULL &&  e->queue->size==0){
				printk(KERN_ALERT "empty queue for pid %d\n",current->pid);
				return -EACCES;}
			e->_val=getMin(e->queue);
			printk(KERN_INFO "popped: %d for pid %d\n",e->_val,current->pid);
			printMap(m);
			if(copy_to_user((int32_t *)arg, &e->_val, sizeof(int32_t))){return -EACCES;}
			break;

		case PB2_GET_MAX:
			if(e->queue==NULL){
				printk(KERN_ALERT "uninit queue for pid %d\n",current->pid);
				return -EACCES;
			}else if(e->queue!=NULL &&  e->queue->size==0){
				printk(KERN_ALERT "empty queue for pid %d\n",current->pid);
				return -EACCES;}
			e->_val=getMax(e->queue);
			printk(KERN_INFO "popped: %d for pid %d\n",e->_val,current->pid);
			printMap(m);
			if(copy_to_user((int32_t *)arg, &e->_val, sizeof(int32_t))){return -EACCES;}
			break;
    }
    return 0;
}

static struct proc_ops myproc_fops = {
		 .proc_open=myproc_open,
		 .proc_release=myproc_close,
         .proc_ioctl=myproc_ioctl
};

static int __init QueueModule_init(void){
 	myproc_entry = proc_create( "cs60038_a2_1",0666,NULL,&myproc_fops);
 	if(myproc_entry==NULL){
 		printk(KERN_ALERT "error: Could not initialize %s\n", "cs60038_a2_1");
	}else{
 		printk(KERN_ALERT "cs60038_a2_1 is initilized\n");
 		m = initMap();
 	}
	return 0;
}

static void __exit QueueModule_exit(void){
	destroyMap(m);
	remove_proc_entry("cs60038_a2_1", NULL);
	printk(KERN_ALERT "cs60038_a2_1 is removed\n");
}

module_init(QueueModule_init);
module_exit(QueueModule_exit);

MODULE_DESCRIPTION("AoS-assignment-2");
MODULE_AUTHOR("Copyright(c) 2022 - Prasenjit-Harsh");
MODULE_LICENSE("GPL v2");