#include "common.h"


void Print2DMatrix(int* target, int size)
{
	int i,j;

	if (size>=1) {
		//Showing C
		for (i = 0; i < size; i++) {
			for (j = 0; j < size; j++)
				printf("%d\t", target[i*size+j]);

			printf("\n");
		}
		printf("\n");
	}
	else {
		printf("[FATAL] matrix size should be larger than 0 (size: %d)\n", size);
	}
}

void VerifyOutput(int* gold, int* output, int size)
{
	int i=0;
	int fail = 0;
	for (i=0; i<(size*size); i++) {
		if ((gold[i]-output[i])>0.01 && fail==0) {
			fail = 1;
		}
	}

	if (fail==1) {
		printf("[FAIL] Data mismatch!!\n");
	}
	else {
		printf("[PASS] Data correct!!\n");
	}
}



void GenRandomInput(int* target, int size)
{
	int i,j;
	for (i=0;i<size;i++) {
		for (j=0;j<size;j++) {
			target[i*size+j] = rand()%100;
		}
	}
}


void ProcessInputArg(int argc, char** argv, int* matrix_size, int* thread_num)
{
	if (argc==3){
		*matrix_size = atoi(argv[1]);
		if (*matrix_size<2) {
			printf("[FATAL] matrix size should be larger than 1 (input: %d)\n", *matrix_size);
			exit(1);
		}
		*thread_num = atoi(argv[2]);
		if ((*matrix_size%*thread_num)>0) {
			printf("[FATAL] Matrix_size should be multiple of child process number (matrix_size: %d | thread_num: %d)\n", *matrix_size, *thread_num);
			printf("[Usage] %s <matrix size> <child process number>\n", argv[0]);

			exit(1);
		}

	}
	else if (argc==1) {
		printf("[Usage] %s <matrix size> <child process number>\n", argv[0]);
		printf("[DEMO] matrix size: 4x4 | Use 1 child\n");
		*matrix_size = 4;
		*thread_num = 1;
	}
	else {
		printf("[Usage] %s <matrix size> <child process number>\n", argv[0]);
	}
}
