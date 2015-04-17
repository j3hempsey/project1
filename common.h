/*
 * common.h
 *
 *  Created on: Apr 11, 2015
 *      Author: haeseunl
 */

#ifndef COMMON_H_
#define COMMON_H_
#include <stdio.h>
#include <stdlib.h>

void ProcessInputArg(int argc, char** argv, int* matrix_size, int* thread_num);
int** Alloc2dMem(int size);
void GenRandomInput(int* target, int size);
void Print2DMatrix(int* target, int size);
void VerifyOutput(int* gold, int* output, int size);



#endif /* COMMON_H_ */
