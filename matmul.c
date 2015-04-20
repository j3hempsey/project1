#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "common.h"

typedef struct _shd_mem {
	int* A;
	int* B;
	int ch_num;
	pid_t* children;
} shd_mem;


int main(int argc, char** argv) {

	// capture the input argument
	int matrix_size = 0;
	int thread_num = 0;

	int* C_parent = NULL;
	int sum,i,j,e;
	int order = 0;
	int start, end;

	// for fork
	pid_t pid;
	int shmid;
	int shm_2d;
	int* output;

	void *shared_memory = (void *)0;
	shd_mem* shd_matrix;
	int *partition = (int*)malloc(sizeof(int));
	*partition = 0;

	ProcessInputArg(argc, argv, &matrix_size, &thread_num);

	if (thread_num>0) {
		//allocating a shared memory segment
		shmid = shmget(IPC_PRIVATE, sizeof(shd_mem)*1, IPC_CREAT | 0666);

		//attach the shared memory segment to the partition variable
		shared_memory = (int*)shmat(shmid, NULL,0);
		shd_matrix = (shd_mem*)shared_memory;

		shd_matrix->A = (int*)malloc(matrix_size*matrix_size*sizeof(int)); //Size of array A
		shd_matrix->B = (int*)malloc(matrix_size*matrix_size*sizeof(int)); //Size of array B

		// allocate shared memory for result		
		shm_2d = shmget(IPC_PRIVATE, sizeof(shd_mem), IPC_CREAT | 0666);  //DEBUG: possibly wrong size of mem allocation
		output = (int*)shmat(shm_2d, NULL, 0);


		shd_matrix->ch_num = thread_num;        //Adds the number of children to shdmem
		shd_matrix->children = (pid_t*)malloc(sizeof(pid_t)*thread_num);    //creates the array of pids
		GenRandomInput(shd_matrix->A, matrix_size);
		GenRandomInput(shd_matrix->B, matrix_size);


		for(i = 0; i < thread_num; i++) {
			if (0 == 0) {	        //Only want the parent creating children		
				pid = fork();
				shd_matrix->children[i] = getpid();
			}
		}


		//error occurred
		if(pid < 0) {
			fprintf(stderr, "Fork Failed");
			return 1;
		}
		//child process
		else if(pid==0) {
			// Map input memory
			shared_memory = (int*)shmat(shmid, NULL,0);
			if (shared_memory == (char *)(-1))
			    perror("shmat");

			// Map output memory
			shd_matrix = (shd_mem*)shared_memory;
			output = (int*)shmat(shm_2d, NULL, 0);      //Result matrix 


			for (i=0; i<shd_matrix->ch_num; i++) {
				if (shd_matrix->children[i]==getpid()) {
					order = i;
					break;
				}
			}
			
			printf("Child process (%d)...\n", order);
			start = order*(matrix_size / (order + 1));
			end = start + (matrix_size / (order + 1));
			
			for (i = start; i < end; i++) {
				for (j=0;j<matrix_size;j++) {
					sum=0;
					for (e=0;e<matrix_size;e++) {
						sum+=shd_matrix->A[i*matrix_size + e]*shd_matrix->B[e*matrix_size + j];
					}
					output[i * matrix_size + j]=sum;
				}
			}
		}
		//parent process
		else {
			usleep(15000);

			// wait completion of children
			wait();
			C_parent = (int*)malloc(matrix_size*matrix_size*sizeof(int));
	
			for (i=0;i<matrix_size;i++) {
				for (j=0;j<matrix_size;j++) {
					sum=0;
					for (e=0;e<matrix_size;e++) {						
						sum+=shd_matrix->A[i*matrix_size + e]*shd_matrix->B[e*matrix_size + j];
					}	
					C_parent[i*matrix_size + j]=sum;
				}
			}

			// Print result from parent
			printf("\nInput matrix1: \n");
			Print2DMatrix(shd_matrix->A, matrix_size);

			printf("Input matrix2: \n");
			Print2DMatrix(shd_matrix->B, matrix_size);

			VerifyOutput(C_parent, output, matrix_size);

			printf("Output matrix: \n");
			Print2DMatrix(output, matrix_size);


			// The shared memory is detached and then deleted
		    if (shmdt(shared_memory) == -1) {
		        fprintf(stderr, "shmdt failed\n");
		        exit(EXIT_FAILURE);
		    }

		    if (shmctl(shmid, IPC_RMID, 0) == -1) {
		        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		        exit(EXIT_FAILURE);
		    }

		    if (shmdt(output) == -1) {
		        fprintf(stderr, "shmdt failed\n");
		        exit(EXIT_FAILURE);
		    }

		    if (shmctl(shm_2d, IPC_RMID, 0) == -1) {
		        fprintf(stderr, "shmctl(IPC_RMID) failed\n");
		        exit(EXIT_FAILURE);
		    }
		}
	}

	return 0;

}
