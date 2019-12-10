/*-----------------------------------------------------------------------------------------
 *File Name 	: tree.c
 *Author 	: Shubham Jaiswal
 *Description	: 
 *Modified Date	:
 *Referneces	: https://www.geeksforgeeks.org/binary-search-tree-set-1-search-and-insertion/
 *		: https://danrl.com/blog/2018/basic-data-structures-binary-tree/
 *		: http://swapnil-pimpale.github.io/lock-free-BST/
 ------------------------------------------------------------------------------------------*/
/*------------------------------Preprocessor Statements----------------------------------*/
#include "tree.h"

/*---------------------------------------------------------------------------------------*/
/*-----------------------------------Global Variable------------------------------------*/			
bst_node *root = NULL;
pthread_mutex_t bst_lock;
pthread_rwlock_t bst_rwlock;
/*---------------------------------------------------------------------------------------*/
/*----------------------------------Function Definitions---------------------------------*/

//Function to get value associated with provided key
int get_value(bst_node *r_node, int key)
{
	bst_node *temp = NULL;
	temp = get(r_node, key); //Call get function
	if(temp == NULL)
	{
		return (-1);
	}
	else
	{
		return (temp->value); //Return value

	}
}

//Function to get node associated with provided key
bst_node *get(bst_node *r_node, int key)
{ 
	if(r_node == NULL) //if provided node is NULL start from root
	{
		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_lock(&bst_lock);
		else
			pthread_rwlock_rdlock(&bst_rwlock);

		if(root == NULL) //Binary tree is empty
		{
			printf("Empty Binary Search Tree.\n");

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&bst_lock);
			else
				pthread_rwlock_unlock(&bst_rwlock);

			return root;
		}

		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_lock(&root->lock);
		else
			pthread_rwlock_rdlock(&root->rwlock);

		r_node = root; //As starting from root assign root to r_node

		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_unlock(&bst_lock);
		else
			pthread_rwlock_unlock(&bst_rwlock);
	}

	if(key < r_node->key) //key is less than node key, recurse left subtree
	{
		if (r_node->left == NULL) //Key is not there in BST
		{
			//printf("Can't find node with key %d in BST.\n", key);
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&r_node->lock);
			else
				pthread_rwlock_unlock(&r_node->rwlock);

			return r_node->left;
		} 
		else // Recurse left sub-tree
		{
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_lock(&r_node->left->lock);
			else
				pthread_rwlock_rdlock(&r_node->left->rwlock);

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&r_node->lock);
			else
				pthread_rwlock_unlock(&r_node->rwlock);

			return get(r_node->left, key);
		}
	}
	else if (key > r_node->key) //Key is greater than node key, recurse right sub-tree
	{
		if (r_node->right == NULL) //Key is not present in BST
		{
			//printf("Can't find node with key %d in BST.\n", key);
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&r_node->lock);
			else
				pthread_rwlock_unlock(&r_node->rwlock);

			return r_node->right;
		} 
		else //Recurse right sub-tree
		{
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_lock(&r_node->right->lock);
			else
				pthread_rwlock_rdlock(&r_node->right->rwlock);
			
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&r_node->lock);
			else
				pthread_rwlock_unlock(&r_node->rwlock);

			return get(r_node->right, key);
		}
	} 
	else //key is equal to node key
	{
		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_unlock(&r_node->lock);
		else
			pthread_rwlock_unlock(&r_node->rwlock);

		return r_node;
	}
	
}

bst_node *put(bst_node *node, int key, int value)
{
	if(node == NULL) //if node is NULL, start from root
	{
		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_lock(&bst_lock);
		else
			pthread_rwlock_wrlock(&bst_rwlock);
			
		if(root == NULL) //BST is empty
		{
			root = new_node(key, value); //Insert ne node and assign it to root

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&bst_lock);
			else
				pthread_rwlock_unlock(&bst_rwlock);

			return root;
		}

		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_lock(&root->lock);
		else
			pthread_rwlock_wrlock(&root->rwlock);

		node = root; //As starting from root, assign root to node
		
		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_unlock(&bst_lock);
		else
			pthread_rwlock_unlock(&bst_rwlock);
	}

	if (key < node->key) //Key is less than node key, recurse left sub tree
	{	
		if(node->left == NULL) //key is not present in BST
		{
			node->left = new_node(key, value); //Insert node with provided key and value
			
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&node->lock);
			else
				pthread_rwlock_unlock(&node->rwlock);

			return node->left;
		}
		else //Recurse left sub tree
		{
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock	
				pthread_mutex_lock(&node->left->lock);
			else
				pthread_rwlock_wrlock(&node->left->rwlock);

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&node->lock);
			else
				pthread_rwlock_unlock(&node->rwlock);

			put(node->left, key, value); 
		}
	}
	else if (key > node->key) //key is greater than node key, recurse right sub tee
	{	
		if(node->right == NULL) //key is not present in BST
		{
			node->right = new_node(key, value); //Insert new node

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&node->lock);
			else
				pthread_rwlock_unlock(&node->rwlock);

			return node->right;
		}
		else //Recurse right sub tree
		{
			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_lock(&node->right->lock);
			else
				pthread_rwlock_wrlock(&node->right->rwlock);

			if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
				pthread_mutex_unlock(&node->lock);
			else
				pthread_rwlock_unlock(&node->rwlock);

			put(node->right, key, value); 
		}    
	}
	else if (key == node->key) //key is equal to node key
	{
		printf("Duplicate key %d! Key must be unique.\n", key);

		if(LOCK_NUM == 0) //Check if mutex or reader/writer lock
			pthread_mutex_unlock(&node->lock);
		else
			pthread_rwlock_unlock(&node->rwlock);
	}

	return NULL; 
}

bst_node *new_node(int key, int value)
{
	bst_node *temp = (bst_node *)malloc(sizeof(bst_node)); //Allocate memory for node 
	if(temp == NULL) 
	{
		printf("Failed to allocate memory for new node:%s.\n", strerror(errno));
		return temp;
	}

	temp->key = key; //Assign key to node
	temp->value = value; //Assign value to node
	temp->left = NULL; //Empty leaf nodes
	temp->right = NULL; //Empty leaf nodes
	pthread_mutex_init(&temp->lock, NULL); //Initialize node mutex	
	pthread_rwlock_init(&temp->rwlock, NULL); //Initialize node reader/writer lock

#ifdef DEBUG
	printf("key: %d | value: %d.\n", key, value);
#endif
	return temp; //return node pointer
}

void inorder(bst_node * r_node, int start_key, int end_key)
{
	if (r_node != NULL) 
	{ 
		inorder(r_node->left, start_key, end_key);
		if((start_key <= r_node->key) && (end_key >= r_node->key) && (start_key != -1) && (end_key != -1))
		{
			printf("key:%d Value:%d.\n", r_node->key, r_node->value); 
		}
		
		if((start_key == -1) && (end_key == -1))
		{
			printf("key:%d Value:%d.\n", r_node->key, r_node->value); 
		}
		inorder(r_node->right, start_key, end_key); 
	} 
}

int range(int start_key, int end_key)
{
	if(start_key > end_key)
	{
		int temp = start_key;
		start_key = end_key;
		end_key = temp;
	}
#ifdef DEBUG
	printf("Inside range. start_key: %d|end_key: %d.\n", start_key, end_key);
#endif
	

	return 0;	
}
