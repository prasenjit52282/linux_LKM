/*****************************************
* Group:1
* Name:Prasenjit Karmakar,   Roll: 22CS91P03
* Name:Harsh Borse,          Roll: 21CS72P02
******************************************/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include<sys/ioctl.h>
#include "../library/ioctl_commands.h"

void printPass(char from[], int *pass){
	printf("pass =%d at %s\n",*pass,from);
}

struct obj_info get_info(int fd,int * pass, int init){
	struct obj_info info;
	if (ioctl(fd, PB2_GET_INFO, &info)<0){
		info.prio_que_size=-1;
		info.capacity=-1;
		if(init==1){
			*pass=0;
		}
	}
	//printPass("getinfo",pass);
	//printf("info=%d,%d\n\n",info.prio_que_size,info.capacity);
	return info;
}

void set_capacity(int fd, int32_t cap, int * pass, int valid){
	if (ioctl(fd, PB2_SET_CAPACITY, &cap)<0){
		if(valid==1){*pass=0;}
	}else{
		struct obj_info info=get_info(fd,pass,1);
		if(info.prio_que_size!=0 && info.capacity!=cap){
			*pass=0;
		}
	}
	//printPass("setcap",pass);
}


void insert_int(int fd, int32_t val, int * pass){
	struct obj_info info=get_info(fd,pass,1);
	if (ioctl(fd, PB2_INSERT_INT, &val)<0){
		if(info.prio_que_size<info.capacity){*pass=0;}
	}
	//printPass("insint",pass);
}

void insert_prio(int fd, int32_t prio, int * pass){
	struct obj_info info=get_info(fd,pass,1);
	if (ioctl(fd, PB2_INSERT_PRIO, &prio)<0){
		if(info.prio_que_size<info.capacity){*pass=0;}
	}
	//printPass("insprio",pass);
}


void assert_min(int fd, int32_t should_be, int * pass){
	int32_t val;
	struct obj_info info=get_info(fd,pass,1);
	if (ioctl(fd, PB2_GET_MIN, &val)<0){
		if(info.prio_que_size>0){*pass=0;}
	}else{
		if(val!=should_be){
			*pass=0;
		}
	}
	//printPass("assmin",pass);
}

void assert_max(int fd, int32_t should_be, int * pass){
	int32_t val;
	struct obj_info info=get_info(fd,pass,1);
	if (ioctl(fd, PB2_GET_MAX, &val)<0){
		if(info.prio_que_size>0){*pass=0;}
	}else{
		if(val!=should_be){
			*pass=0;
		}
	}
	//printPass("assmax",pass);
}


int main(){
	fork();
	fork();
	fork();
	fork();
	int fd,pass=1,pid=getpid();;

	//open once
	if((fd=open("/proc/cs60038_a2_1",O_RDWR))==-1){
		printf("All testcases failed: file is not opened!\n");
		exit(1);
	}

	struct obj_info info= get_info(fd,&pass,0);
	if (info.prio_que_size!=-1 && info.capacity!=-1){pass=0;}

	set_capacity(fd,-20,&pass,0); //invalid capacity

	info= get_info(fd,&pass,0);
	if (info.prio_que_size!=-1 && info.capacity!=-1){pass=0;}

	set_capacity(fd,110,&pass,0); //invalid capacity
	
	info= get_info(fd,&pass,0);
	if (info.prio_que_size!=-1 && info.capacity!=-1){pass=0;}

	set_capacity(fd,4,&pass,1);
	assert_min(fd,-1,&pass); //no data
	assert_max(fd,-1,&pass); //no data

	insert_int(fd,pid-12,&pass);
	insert_prio(fd,1,&pass);

	insert_int(fd,pid-13,&pass);
	insert_prio(fd,8,&pass);

	insert_int(fd,pid-14,&pass);
	insert_prio(fd,3,&pass);

	insert_int(fd,pid-15,&pass);
	insert_prio(fd,4,&pass);

	insert_int(fd,pid-16,&pass); //full queue
	insert_prio(fd,5,&pass); //full queue


	assert_max(fd,pid-13,&pass);
	assert_min(fd,pid-12,&pass);

	assert_max(fd,pid-15,&pass);
	assert_max(fd,pid-14,&pass);


	assert_min(fd,-1,&pass); //no data
	assert_max(fd,-1,&pass); //no data

	/// setting capacity again

	set_capacity(fd,5,&pass,1);

	insert_int(fd,pid-10,&pass);
	insert_prio(fd,1,&pass);

	insert_int(fd,pid-13,&pass);
	insert_prio(fd,2,&pass);

	insert_int(fd,pid-14,&pass);
	insert_prio(fd,3,&pass);

	insert_int(fd,pid-15,&pass);
	insert_prio(fd,4,&pass);

	insert_int(fd,pid-16,&pass);
	insert_prio(fd,5,&pass);

	assert_max(fd,pid-16,&pass);
	assert_min(fd,pid-10,&pass);


	//open once
	if((fd=open("/proc/cs60038_a2_1",O_RDWR))==-1){
		printf("All testcases failed: file is not opened!\n");
		exit(1);
	}

	info= get_info(fd,&pass,0);
	if (info.prio_que_size!=-1 && info.capacity!=-1){pass=0;}

	set_capacity(fd,1,&pass,1);

	insert_int(fd,pid-5,&pass);
	insert_prio(fd,pid-5,&pass);

	insert_int(fd,pid-16,&pass); //full queue
	insert_prio(fd,pid-5,&pass); //full queue

	assert_max(fd,pid-5,&pass);

	assert_min(fd,-1,&pass); //no data
	assert_max(fd,-1,&pass); //no data

	close(fd);

	if(pass==1){
		printf("Passed all testcases for pid %d\n",pid);
	}else{
		printf("Failed some testcases for pid %d\n",pid);
	}
	return 0;
}
