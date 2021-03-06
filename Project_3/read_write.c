// (c) Partha Dasgupta 2009
// permission to use and distribute granted.

/*
* Group members: 
* 1. Sanjay Arivazhagan; ASURITE ID: 1217643921; sarivazh@asu.edu 
* 2. Sethu Manickam; ASURITE ID: 1218452066; smanick4@asu.edu
* Used init_TCB() function as given by Dr. Dasgupta in Project 2 Instructions
*/
#define GNU_SOURCE
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sem.h"

semaphore_t *r_sem, *w_sem, *mutex;
int rwc = 0, wwc = 0, rc = 0, wc = 0, global_ID=0;

/*
* Function Name: reader_entry
* Parameters: int ID
* Return type: void
*/
void reader_entry(int ID)
{
	printf("[reader: #%d]\ttrying to read\n", ID);
	P(mutex);
	if (wwc > 0 || wc > 0) {
	    printf("[reader: #%d]\tblocking for writer\n", ID);
	    rwc++;		// increment waiting reader count.
	    V(mutex);		// let other processes use the mutex.
	    P(r_sem);		// sleep on r_sem
	    rwc--;		// program now has mutex and r_sem, go
			        // into CS.
	}
	rc++;
	if (rwc > 0)
	    V(r_sem);
	else
	    V(mutex);		// let other processes use the mutex.
}

/*
* Function Name: reader_exit
* Parameters: int ID
* Return type: void
*/
void reader_exit(int ID)
{
	P(mutex);
	rc--;			// i'm no longer a reader
	if (rc == 0 && wwc > 0) {
	    // if was the last reader, and there are waiting writers, open 
	    // the w_sem door for them.
	    V(w_sem);
	} else
	    V(mutex);

}

/*
* Function Name: writer_entry
* Parameters: int ID
* Return type: void
*/
void writer_entry(int ID)
{
	printf("[writer: #%d]\ttrying to write\n", ID);
	P(mutex);
	if (rc > 0 || wc > 0) {
	    printf("[writer: #%d] blocking for others\n", ID);
	    wwc++;		// increment waiting writers
	    V(mutex);		// let go of the mutex, since i'll be
	    // blocked
	    P(w_sem);		// wait in my line, when i wake up i DON'T 
		 	    	// need a P(mutex since i've been given it 
	    			// by the waking process.
	    wwc--;		// i'm no longer waiting
	}
	wc++;			// increment writers
	V(mutex);		// let go of the mutex
}

/*
* Function Name: writer_exit
* Parameters: int ID
* Return type: void
*/
void writer_exit(int ID)
{
	P(mutex);
	wc--;
	if (rwc > 0) {		// first, i let all the readers go.
	    V(r_sem);
	} else if (wwc > 0) {	// then i let another writer go
	    V(w_sem);
	} else {		// lastly, i give up the mutex
	    V(mutex);
	}
}

/*
* Function Name: reader
* Parameters: No paramteres
* Return type: void
*/
void reader(void)
{ 
  int ID;
  P(mutex); ID = global_ID++; V(mutex);
  while(1){
	reader_entry(ID);
	printf
	    ("\t\t\t\t[reader #%d]\t****READING****\n", ID);
	sleep(1);
	reader_exit(ID);
  };
}

/*
* Function Name: writer
* Parameters: No paramteres
* Return type: void
*/
void writer(void)
{
  int ID;
  P(mutex); ID = global_ID++; V(mutex);
  while(1){
 	writer_entry(ID);
	printf
	    ("\t\t\t\t[writer: #%d]\t&&&WRITING!&&&\n", ID);
	sleep(1);
	writer_exit(ID);
  };
}



//-------------------------------------------------------
// Original code
//int main()
//{
//    init_sem(&mutex, 1);
//    init_sem(&r_sem, 0);
//    init_sem(&w_sem, 0);
//    start_thread(reader, NULL);
//    start_thread(reader, NULL);
//    start_thread(reader, NULL);
//    start_thread(reader, NULL);
//    start_thread(writer, NULL);
//    start_thread(writer, NULL);
//    while (1) sleep(1);
//}   



//Updated code
int main() {
    mutex= CreateSem(1);
    r_sem= CreateSem(0);
    w_sem= CreateSem(0);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
    start_thread(reader);
    start_thread(writer);
    start_thread(writer);
    run();
    //while (1) {
	//	sleep(1);
	//}
}   
