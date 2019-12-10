#ifndef MAIN_H_
#define MAIN_H_

/*-----------------------------------------------------------------------------------------
 *File Name 	: main.h
 *Author 	: Shubham Jaiswal
 *Description	:
 *Modified Date	:
 *Referneces	:
 *
------------------------------------------------------------------------------------------*/
/*------------------------------Preprocessor Statements----------------------------------*/
#include <getopt.h>
#include <ctype.h>
#include <unistd.h>
#include <vector>
#include "tree.h"

#define FULL_NAME "SHUBHAM JAISWAL"
#define MODULO (500)
/*---------------------------------------------------------------------------------------*/
/*-----------------------------------Global Varibals-------------------------------------*/
	
static int print_name_flag;

//Structure for thread arguments
typedef struct thread_arg
{
	int Id;
	int num_nodes;
	int ins_id;
}thread_arg;

thread_arg *thread_args;
pthread_barrier_t bar;

//Structure to store start and end time
struct timespec start, end;
/*---------------------------------------------------------------------------------------*/
/*----------------------------------Function Declarations---------------------------------*/
void usage(void);
void *thread_func(void *);
void *high_func(void *);
void *low_func(void *);
void cleanup(void);
void Postorder_del(bst_node* node);
#endif
