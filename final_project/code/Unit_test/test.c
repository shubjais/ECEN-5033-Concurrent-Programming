/*-----------------------------------------------------------------------------------------
 *File Name 	: test.c
 *Author 	: Shubham Jaiswal
 *Description	: 
 *Modified Date	:
 *Referneces	: http://cunit.sourceforge.net/doc/introduction.html
 *
 ------------------------------------------------------------------------------------------*/
/*------------------------------Preprocessor Statements----------------------------------*/
#include "test.h"

/*---------------------------------------------------------------------------------------*/
/*-----------------------------------Global Varibals-------------------------------------*/			
//bst_node *root;
//pthread_mutex_t bst_lock;

int duplicates;
/*---------------------------------------------------------------------------------------*/
/*----------------------------------Function Definitions---------------------------------*/
/*First Initialization Function*/

int bst_init()
{
	root = NULL;
	pthread_mutex_init(&bst_lock, NULL);
	srand(time(NULL));
	return 0;
}

int clean_suite(void)
{
      return 0;
}

void test_put()
{
	bst_node *temp = NULL;
	for(int i = 0; i < 10; i++)
	{
		int key = rand()%100;
		int value = rand()%100;
		temp = put(NULL, key, value);
		if(temp != NULL)
		{
			key_arr[i] = key;
			val_arr[i] = value;
			CU_ASSERT_EQUAL(key, temp->key);
			CU_ASSERT_EQUAL(value, temp->value);
			duplicates++;
		}
	}
}

void test_get()
{
	bst_node *temp = NULL;
	for(int i = 0; i < 10 ; i++)
	{
		printf("index: %d, key:%d \n", i, key_arr[i]);
		if(key_arr[i] != 0)
		{
			temp = get(NULL, key_arr[i]);
			CU_ASSERT_PTR_NOT_EQUAL(NULL, temp);
			CU_ASSERT_EQUAL(key_arr[i], temp->key);
	
		}
	}
}

void test_get_value()
{
	int val = 0;
	for(int i = 0; i < 10; i++)
	{
		if(key_arr[i] != 0)
		{
			val = get_value(root, key_arr[i]);
			CU_ASSERT_EQUAL(val_arr[i], val);
		}
	}
}

//void test_range()
//{
//
//}

int main(void)
{
	if (CUE_SUCCESS != CU_initialize_registry())
		return CU_get_error();

	CU_pSuite pSuite=NULL;
	pSuite = CU_add_suite( "CONCURRENT TREE TEST", bst_init, clean_suite);
	if ( NULL == pSuite )
	{
		CU_cleanup_registry();
		return -1;
	}

	if ((NULL == CU_add_test(pSuite, "Insert/put Node test", test_put))||
			(NULL == CU_add_test(pSuite, "Search/Get Node test", test_get))||
			(NULL == CU_add_test(pSuite, "Get Value Test", test_get_value)) /*||
			(NULL == CU_add_test(pSuite, "Range Test", test_range))*/)      
	{
		CU_cleanup_registry();
		return CU_get_error();
	}

	CU_basic_set_mode(CU_BRM_VERBOSE);
	CU_basic_run_tests();

	CU_set_output_filename("test");

	CU_automated_run_tests();
	CU_cleanup_registry();
	return CU_get_error();
}

