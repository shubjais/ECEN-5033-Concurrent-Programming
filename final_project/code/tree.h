#ifndef TREE_H_
#define TREE_H_

/*-----------------------------------------------------------------------------------------
 *File Name 	: main.h
 *Author 	: Shubham Jaiswal
 *Description	:
 *Modified Date	:
 *Referneces	:
 *
------------------------------------------------------------------------------------------*/
/*------------------------------Preprocessor Statements----------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#define DEBUG
/*---------------------------------------------------------------------------------------*/
/*-----------------------------------Global Varibals-------------------------------------*/

typedef struct bst_node
{
	int key;
	int value;
	struct bst_node *left, *right;
	pthread_mutex_t lock;
	pthread_rwlock_t rwlock;
}bst_node;

extern int LOCK_NUM;
extern bst_node *root;
extern pthread_mutex_t bst_lock;
extern pthread_rwlock_t bst_rwlock;
/*---------------------------------------------------------------------------------------*/
/*----------------------------------Function Declarations---------------------------------*/

int get_value(bst_node *, int key);
bst_node *get(bst_node *, int key);
bst_node *put(bst_node *, int key, int value);
bst_node *new_node(int key, int value);
void inorder(bst_node *, int start_key, int end_key);
int range(int, int);


#endif
