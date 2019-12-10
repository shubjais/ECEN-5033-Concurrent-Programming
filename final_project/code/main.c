/*-----------------------------------------------------------------------------------------
 *File Name 	: main.c
 *Author 	: Shubham Jaiswal
 *Description	: 
 *Modified Date	:
 *Referneces	: https://www.gnu.org/software/libc/manual/html_node/Getopt.html
 *
 ------------------------------------------------------------------------------------------*/
/*------------------------------Preprocessor Statements----------------------------------*/
#include "main.h"

/*---------------------------------------------------------------------------------------*/
/*-----------------------------------Global Varibals-------------------------------------*/			

int LOCK_NUM = 0;
int NUM_THREADS = 10;
int NUM_NODES = 30;
int TEST_NUM = 0;

std::vector<std::vector<int>> keys;
std::vector<std::vector<int>> values;
/*---------------------------------------------------------------------------------------*/
/*----------------------------------Function Definitions---------------------------------*/

int main(int argc,  char **argv)
{
	int opt, l_flag = 0, t_flag = 0;	
	const char *lock[] = {"pthread","r_w"};
	const char *tests[] = {"", "high-contention", "low-contention"};
#ifdef DEBUG
	printf("Number of cli arguments: %d.\n", argc);
#endif
	while(1)
	{
		//Supported long Options 
		static struct option long_options[] =
		{
			{"name", no_argument, &print_name_flag, 1},
			{"lock", optional_argument, 0, 'l'},
			{"threads", optional_argument, 0, 't'},
			{"nodes", optional_argument, 0, 'n'},
			{"help", optional_argument, 0, 'h'},
			{"test", optional_argument, 0, 'a'},
			{0, 0, 0, 0}
		};

		int option_index = 0;

		opt = getopt_long(argc, argv, "hl:", long_options, &option_index);  //Check the command line arguments passed
		if(opt == -1)
		{
			break;
		}

		switch(opt)
		{
			case 'h': //help
				usage();
				exit(0);
				break;

			case 'l': //Type of lock to use
				for(int i = 0; i < 2; i++)
				{
					if(strcmp(lock[i], optarg) == 0)
					{
						l_flag = 1;
						LOCK_NUM = i;
						printf("LOCK_NUM: %d, lock:%s\n", i, lock[i]);
						break;
					}
				}
				if(l_flag == 0)
				{
					printf("Invalid argument given to option.Exiting!\n");
					usage();
					exit(-1);
				}
				printf("Option --lock specified. %s lock will be used for synchronization.\n", optarg);
				break;

			case 't': //Number of threads to spawn
				printf("Option --threads specified. %s number of threads will be created.\n", optarg);
				NUM_THREADS = atoi(optarg);
				break;

			case 'n': // Number of nodes to insert in Binary Search Tree
				printf("Option --nodes specified. %s number of internal nodes will be created for binary tree.\n", optarg);
				NUM_NODES = atoi(optarg);
				break;

			case 'a': //Which Test to perform
				for(int i = 0; i < 3; i++)
				{
					if(strcmp(tests[i], optarg) == 0)
					{
						t_flag = 1;
						TEST_NUM = i;
						printf("TEST_NUM: %d, Test:%s\n", i, tests[i]);
						break;
					}
				}
				if(t_flag == 0)
				{
					printf("Invalid argument given to option.Exiting!\n");
					usage();
					exit(-1);
				}
				printf("Option --test specified. %s test will be performed.\n", optarg);
				break;

			case 0: //Long option with flag
				if(long_options[option_index].flag != 0)
				{
					*long_options[option_index].flag = long_options[option_index].val; 
				}
				break;

			case '?': //Unknown option or argument
				printf("Either an unknown option provided or a required argument missed!!\n");
				usage();
				exit(-1);

			default:
				usage();
				exit(-1);
		}

	}

	if(print_name_flag) //--name option specified
	{
		printf(FULL_NAME);
		putchar('\n');
		printf("Name option provided...Exiting!\n");
		return(0);	
	}

	if(argc > optind) //Non-option argument provided
	{
		printf("Non-option arguments provided. Exiting!\n");	
		usage();
		exit(-1);
	}
	else // All the required arguments provided
	{
		int ins_ind = 0, ins_thread = 0, ret = 0; 
		if(TEST_NUM == 0)
			ins_thread = (NUM_THREADS/3) + 1;
		else
			ins_thread = NUM_THREADS;

		//resize vectors to number of inserting threads.
		keys.resize(ins_thread); 
		values.resize(ins_thread);
#ifdef DEBUG
		printf("Number of insertion threads: %d.\n", ins_thread);
#endif
		srand(time(0)); //seed for random number generation
		pthread_mutex_init(&bst_lock, NULL); //Initialize mutex for tree
		pthread_rwlock_init(&bst_rwlock, NULL); //Initialize reader/writer lock for tree
		pthread_barrier_init(&bar, NULL, NUM_THREADS); //Initialize barrier

		pthread_t thread_id[NUM_THREADS]; //Array to store thread Ids
		thread_args = (thread_arg *)malloc(NUM_THREADS * sizeof(thread_arg)); //Alloc memory for thread argument structure

		for(int i = 0; i < NUM_THREADS; i++) //Loop to spwan the required number of threads
		{
			thread_args[i].Id = i; 

			if(TEST_NUM == 0) //Normal Test
			{
				if((thread_args[i].Id % 3) == 0)
				{
					if((ins_ind + 1) == ins_thread)
					{
						thread_args[i].num_nodes = NUM_NODES - (NUM_NODES/ins_thread) * ins_ind;//Number of nodes thread should insert
					}
					else
					{
						thread_args[i].num_nodes = NUM_NODES/ins_thread; //Number of nodes thread should insert
					}
					thread_args[i].ins_id = ins_ind; //Index of inserting thread
					ins_ind++;
				}
				else
				{
					thread_args[i].num_nodes = NUM_NODES/ins_thread;
					thread_args[i].ins_id = -1;
				}
			}
			else //High-contention/low-contention test
			{
				thread_args[i].ins_id = i; //Index of inserting thread
				if(thread_args[i].Id == (NUM_THREADS - 1))
					thread_args[i].num_nodes = NUM_NODES - (NUM_NODES/NUM_THREADS) * (NUM_THREADS - 1); //Number of nodes thread should Insert
				else
					thread_args[i].num_nodes = NUM_NODES/NUM_THREADS; //Number of nodes thread should insert
			}

			//Spawn the thread
			if(TEST_NUM == 0) //Normal test
				ret = pthread_create(&thread_id[i], NULL, thread_func, (void*) &thread_args[i]);
			else if(TEST_NUM == 1) //High-contention		
				ret = pthread_create(&thread_id[i], NULL, high_func, (void*) &thread_args[i]);
			else//Low-contention
				ret = pthread_create(&thread_id[i], NULL, low_func, (void*) &thread_args[i]);

			if(ret) //Check for error
			{
				printf("ERROR; pthread_create: %d\n", ret);
				exit(-1);
			}
		}

		for(int i = 0; i < NUM_THREADS; i++) //Loop to join the spwaned threads
		{
			ret = pthread_join(thread_id[i], NULL); //Join the thread
			if(ret) //Error Check
			{
				printf("ERROR; pthread_join: %d\n", ret);
				exit(-1);
			}
		}

		cleanup(); //Function to clean function

		//Calculate Elapsed Time
		unsigned long long elapsed_ns;
		elapsed_ns = (end.tv_sec-start.tv_sec)*1000000000 + (end.tv_nsec-start.tv_nsec);
		printf("Elapsed (ns): %llu\n",elapsed_ns);
		double elapsed_s = ((double)elapsed_ns)/1000000000.0;
		printf("Elapsed (s): %lf\n",elapsed_s);

	}

	return(0);
}

//Function executed by spwaned threads in Normal Test
void *thread_func(void* arg)
{
	thread_arg *t_arg = (thread_arg *)arg; //Cast argument
#ifdef DEBUG
	printf("thread id: %d | nodes: %d | insertion_ind: %d.\n", t_arg->Id, t_arg->num_nodes, t_arg->ins_id);
#endif
        pthread_barrier_wait(&bar); //Wait till all threads reach here
        if(t_arg->Id == 0) //First thread will record start time
        {
                clock_gettime(CLOCK_MONOTONIC,&start);
        }

	if((t_arg->Id % 3) == 0) //threads will perform insertion
	{
		for(int i = 0; i < t_arg->num_nodes; i++)
		{
			int key = rand() % MODULO; //random key
			keys[t_arg->ins_id].push_back(key); //Insert key in vector of thread
			int value = rand() % MODULO; //random value
			values[t_arg->ins_id].push_back(value); //Inser value in vector of thread
			put(NULL, key, value); //Insert
		}
	}
	else if((t_arg->Id % 3) == 1) //threads will perform search
	{
		for(int i = 0; i < t_arg->num_nodes; i++)
		{
			int key = rand() % MODULO; //random key
			int val = 0; 
			if((val = get_value(NULL, key)) == -1) //Search
			{
				printf("Cannot find key %d in the BST.\n", key); 
			}
			else
			{
				printf(" Got Value: %d for key: %d.\n", val, key);
			}
		}	
	}
	else //threads will perform range querry
	{
		//int s_key = rand()%100;
		//int e_key = rand()%100;
		//range(s_key, e_key);
	}
        
	pthread_barrier_wait(&bar); //Wait till all threads reach here
        if(t_arg->Id == 0) //First thread will record End time
        {
                clock_gettime(CLOCK_MONOTONIC,&end);
        }

	return arg;	
}

//Function executed by spawned threads in High Contention Test
void *high_func(void *arg)
{
	thread_arg *t_arg = (thread_arg *)arg;

	pthread_barrier_wait(&bar); //Wait till all threads reach here
	if(t_arg->Id == 0) //First thread will record start time
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}

	for(int i = 0; i < t_arg->num_nodes; i++)
	{
		int key = rand() % MODULO; //random key
		int val = rand() % MODULO; //random value
		keys[t_arg->ins_id].push_back(key);//Add key to vector
		values[t_arg->ins_id].push_back(val); //Add value to vector

		put(NULL, key, val);
	}

	if(t_arg->Id == 0)
		put(NULL, 50, 100); 

	pthread_barrier_wait(&bar); //Wait till all threads reach here

	//int key = keys[0].at(0);
	for(int i = 0; i < t_arg->num_nodes; i++)
	{
		get_value(NULL, 50); //All threads search the same key
	}

	pthread_barrier_wait(&bar); //Wait till all threads reach here
	if(t_arg->Id == 0) //First thread will record end time
	{
		clock_gettime(CLOCK_MONOTONIC,&end);
	}

	return 0;
}

//Function Executed by Spawned threads in Low Contention test
void *low_func(void *arg)
{
	thread_arg *t_arg = (thread_arg *)arg;

	pthread_barrier_wait(&bar); //Wait till all threads reach here
	if(t_arg->Id == 0) //First thread will record start time
	{
		clock_gettime(CLOCK_MONOTONIC,&start);
	}

	for(int i = 0; i < t_arg->num_nodes; i++)
	{
		int key = rand() % MODULO; //random key
		int val = rand() % MODULO; //random value
		put(NULL, key, val); //Insert
	}

	pthread_barrier_wait(&bar); //wait till all threads reach here

	int itr = keys[t_arg->ins_id].size(); //Number of keys each thread should search
	for(int i = 0; i < itr; i++)
	{
		int key = keys[t_arg->ins_id].at(i); //get the key from vector
		get_value(NULL, key); //Search
	}

	pthread_barrier_wait(&bar); //wait till all threads reach here
	if(t_arg->Id == 0){ //First thread will record end time
		clock_gettime(CLOCK_MONOTONIC,&end);
	}

	return 0;
}

//Function to show user usage syntax of utlity/Program
void usage(void)
{
	printf("USAGE: ./concurrent_tree [--name] [--lock=<pthread,r_w>] [--threads=<>] [--nodes=<>] [--test=<high-contention/low-contention>] [--help].\n");
	printf("	--name: 	Will just print the name.\n");
	printf("	--lock: 	Will select the type of lock to be used for synchronization.\n");
	printf("	--threads:	Specifies the number of threads to be created.\n");
	printf("	--nodes:	Specifies the number of nodes to be created for BST.\n");
	printf("	--test:		Specifies whether you want to perform high-contention/low-contention test.\n");
	printf("	--help:		Prints this Application Usage Information.\n");
}

//Function to free all the allocated memory and all destroy synchronization mechanisims
void cleanup(void)
{
	free((void *)thread_args);
	pthread_mutex_destroy(&bst_lock);
	pthread_barrier_destroy(&bar);
	pthread_rwlock_destroy(&bst_rwlock);
	Postorder_del(root);
	free(root);
}

//Post-order traversal to delete nodes 
void Postorder_del(bst_node* node)
{
    if (node == NULL)
        return;

    Postorder_del(node->left); //Recurse left sub-tree
    Postorder_del(node->right); //Recurse right sub-tree

    pthread_mutex_destroy(&node->lock); //destroy node mutex
    pthread_rwlock_destroy(&node->rwlock); //destroy node reader/writer lock
    if(node->left != NULL)
	    free(node->left); //free left child
    if(node->right != NULL)
	    free(node->right); //free right child
}
