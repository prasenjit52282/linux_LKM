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

struct proc_dir_entry *myproc_entry;
struct Map *m;

static ssize_t myproc_write(struct file *fp, const char *buf, size_t len, loff_t *off){
	int8_t qsize;
	char proc_buf[4];
        struct Entry *e=getMapEntry(m,current->pid);
	if(e->state==2){e->state=0;}
	if((e->queue==NULL && len!=1) || (e->queue!=NULL && len!=4)){
		printk(KERN_ALERT "invalid argument error for pid %d\n",current->pid);
		return -EINVAL;
	}
	if(copy_from_user(proc_buf, buf, len)){return -EFAULT;}
	if(e->queue==NULL){
		memcpy(&qsize,proc_buf,sizeof(int8_t));
		if(qsize<1 || qsize>100){
			printk(KERN_ALERT "only 1 to 100 qsize is allowed for pid %d\n",current->pid);
			return -EINVAL;
		}
		printk(KERN_INFO "initilizing priority queue with maxsize %d for pid %d\n",qsize,current->pid);
		e->queue=initQueue(qsize);
	}else{
		if(e->queue->size==e->queue->maxsize){
			printk(KERN_ALERT "queue is full for pid %d\n",current->pid);
			return -EACCES;
		}else if(e->state==0){
			e->state++;
			memcpy(&e->_val,proc_buf,sizeof(int32_t));
			printk(KERN_INFO "read value %d from pid %d\n",e->_val,current->pid);
		}else if(e->state==1){
			e->state++;
			memcpy(&e->_prio,proc_buf,sizeof(int32_t));
			printk(KERN_INFO "read priority %d from pid %d\n",e->_prio,current->pid);
			enQueue(e->queue, e->_prio, e->_val);
			printMap(m);
		}
	}
	return len;
}

static ssize_t myproc_read(struct file *fp, char *buf, size_t len, loff_t *off){
	char proc_buf[4];
	struct Entry *e=getMapEntry(m,current->pid);
	if(e->queue==NULL){
		printk(KERN_ALERT "uninit queue for pid %d\n",current->pid);
		return -EACCES;
	}else if(e->queue!=NULL &&  e->queue->size==0){
		printk(KERN_ALERT "empty queue for pid %d\n",current->pid);
		return -EACCES;
	}
	e->_val=deQueue(e->queue);
	printk(KERN_INFO "popped: %d for pid %d\n",e->_val,current->pid);
	printMap(m);
	len=sizeof(int32_t);
	proc_buf[3]=(e->_val>>24) & 0xFF;
	proc_buf[2]=(e->_val>>16) & 0xFF;
	proc_buf[1]=(e->_val>>8) & 0xFF;
	proc_buf[0]=e->_val & 0xFF;
	if(copy_to_user(buf,proc_buf,len)){return -EFAULT;}
	return len;
}

static int myproc_open(struct inode *inode,struct file *file){
	struct Entry *e=getMapEntry(m,current->pid);
	if (e!=NULL){
		printk(KERN_ALERT "multiple open for %d is not permitted\n",current->pid);
		return -EAGAIN;
	}
	try_module_get(THIS_MODULE);
	printk(KERN_ALERT "proc file is opened by %d --creating entry\n",current->pid);
	createEntry(m,current->pid);
	return 0;
}

static int myproc_close(struct inode *inode,struct file *file){
	module_put(THIS_MODULE);
	printk(KERN_ALERT "proc file is closed by pid %d --removing entry\n",current->pid);
	removeEntry(m,current->pid);
	return 0;
}

static struct proc_ops myproc_fops = {
		 .proc_read=myproc_read,
		 .proc_write=myproc_write,
		 .proc_open=myproc_open,
		 .proc_release=myproc_close
};

static int __init QueueModule_init(void){
 	myproc_entry = proc_create( "partb_1_1",0666,NULL,&myproc_fops);
 	if(myproc_entry==NULL){
 		printk(KERN_ALERT "error: Could not initialize %s\n", "partb_1_1");
	}else{
 		printk(KERN_ALERT "partb_1_1 is initilized\n");
 		m = initMap();
 	}
	return 0;
}

static void __exit QueueModule_exit(void){
	destroyMap(m);
	remove_proc_entry("partb_1_1", NULL);
	printk(KERN_ALERT "partb_1_1 is removed\n");
}

module_init(QueueModule_init);
module_exit(QueueModule_exit);

MODULE_DESCRIPTION("AoS-assignment-1-PartB");
MODULE_AUTHOR("Copyright(c) 2022 - Prasenjit-Harsh");
MODULE_LICENSE("GPL v2");
