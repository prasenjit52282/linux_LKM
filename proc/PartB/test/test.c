#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

char * cpyBuff(char *proc_buf, int32_t value){
	proc_buf[3]=(value>>24) & 0xFF;
        proc_buf[2]=(value>>16) & 0xFF;
        proc_buf[1]=(value>>8) & 0xFF;
        proc_buf[0]=value & 0xFF;
	return proc_buf;
}

char * cpyBuff8(char *proc_buf, int8_t value){
        proc_buf[0]=value & 0xFF;
        return proc_buf;
}

int main(){
	fork();
	fork();
	fork();
	fork();
	int32_t a,b=0;
	char proc_buf[4];
	int fp,fpfail;
	int pid=getpid();
	int passed=1; //initially passed is true

	//open once
	if ((fp=open("/proc/partb_1_1",O_RDWR))==-1){
		passed=0;
		printf("All testcases failed for pid %d- file is not opened!\n",pid);
		exit(1);
	}

	//open twice not allowed
	if ((fpfail=open("/proc/partb_1_1",O_RDWR))!=-1){
		passed=0;
        }


	a=201; //only 1 to 100 is allowed
	if(write(fp,cpyBuff8(proc_buf,a),1)>0){
	passed=0;}

	a=2; //invalid arg needs 1 Byte
        if(write(fp,cpyBuff(proc_buf,a),4)>0){
	passed=0;}

	//uninit queue for pid %d
	if(read(fp,proc_buf,4)>0){
	passed=0;
        memcpy(&b,proc_buf,4);}

	a=3; //initilizing prio queue with msize 3 for pid %d
        if(write(fp,cpyBuff8(proc_buf,a),1)<0){
	passed=0;}

	a=13; //write value 13 for pid %d
	if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=3; //write prio 3 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=14; //write value 14 for pid %d
	if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=2; //write prio 2 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=15; //write value 15 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=1; // write prio 1 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	a=16; //queue is full for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)>0){
	passed=0;}

	if(read(fp,proc_buf,4)>0){ // popped 15 for pid %d
	memcpy(&b,proc_buf,4);
	if(b!=15){passed=0;}}else{passed=0;}

	a=16; //write value 16 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

        a=1; // write prio 1 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	if(read(fp,proc_buf,4)>0){ //popped 16 for pid %d
        memcpy(&b,proc_buf,4);
	if(b!=16){passed=0;}}else{passed=0;}

	if(read(fp,proc_buf,4)>0){ //popped 14 for pid %d
        memcpy(&b,proc_buf,4);
	if(b!=14){passed=0;}}else{passed=0;}

	if(read(fp,proc_buf,4)>0){ //popped 13 for pid %d
        memcpy(&b,proc_buf,4);
	if(b!=13){passed=0;}}else{passed=0;}

	if(read(fp,proc_buf,4)>0){ //empty queue
	passed=0;
        memcpy(&b,proc_buf,4);}

	a=17; //write value 17 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

        a=5; // write prio 5 for pid %d
        if(write(fp,cpyBuff(proc_buf,a),4)<0){
	passed=0;}

	close(fp);

	if(passed==1){
		printf("Passed all testcases for pid %d\n",pid);
	}else{
		printf("Failed some testcases for pid %d\n",pid);
	}
	return 0;
}
