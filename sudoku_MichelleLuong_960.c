/*
 * sudoku_MichelleLuong_960.c
 *
 *  Created on: Sep 29, 2017
 *      Author: michelle
 */

#include <pthread.h>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define MAX_THREADS 27

/*
 * Function Prototypes
 */
void *check_sudoku(void* arg);
int chk_unique(int* in_arr);

typedef struct {
	int row;
	int col;
	int res_pos;	//Index position for result; Thread ID
} param;

/*
 * Hard-code of the 9x9 Grid
 */
int sudoku[9][9] = { {6,5,3,1,2,8,7,9,4}, {1,7,4,3,5,9,6,8,2}, {9,2,8,4,6,7,5,3,1},
		{2,8,6,5,1,4,3,7,9}, {3,9,1,7,8,2,4,5,6}, {5,4,7,6,9,3,2,1,8},
		{8,6,5,2,3,1,9,4,7}, {4,1,2,9,7,5,8,6,3}, {7,3,9,8,4,6,1,2,5} };

int result[27] = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

/*
 * chk_unqiue will go through each array of 9 elements and check to see if each
 * number is unique in that array from numbers 1 through 9.
 * Return 1 if array is unique -- Pass
 * Return 0 if array is not unique -- Fail
 */
int chk_unique(int* in_arr) {
	int work_array[9] = {0,0,0,0,0,0,0,0,0};	/* Initialized to zero, will be all 1s if array is unique */
	int i;

	for(i = 0; i < 9; i++) {
		if(work_array[in_arr[i] - 1] == 0)
			work_array[in_arr[i] - 1] = 1;
		else
			work_array[in_arr[i] - 1] = 0;
	}

	for(i = 0; i < 9; i++) {
		if(work_array[i] == 0)
			return 0;
	}
	return 1;
}

/*
 * The thread will begin control in this function
 */
void *check_sudoku(void* arg) {
	int i, j;
	int check_array[9];
	int row, col, res_pos;
	param *p = (param*) arg;

	row = p ->row;
	col = p ->col;
	res_pos = p ->res_pos;

	if((res_pos >= 0) && (res_pos < 9)) {

		/* 3x3 Check */
		for(i = 0; i < 3; i++)
			for(j = 0; j < 3; j++)
				check_array[3 * i + j] = sudoku[row + i][col + j];

	}

	if((res_pos >= 9) && (res_pos < 18)) {

		/* Row Check */
		for(i = 0; i < 9; i++) {
			check_array[i] = sudoku[row][col + i];
		}
	}

	if((res_pos >= 18) && (res_pos < 27)) {

		/* Col Check */
		for(i = 0; i < 9; i++) {
			check_array[i] = sudoku[row + i][col];
		}
	}

	result[res_pos] = chk_unique(check_array);

	return NULL;

}

/*
 * Main Function
 */
int main(void) {
	int i, j;

	pthread_t *threads;
	pthread_attr_t pthread_my_attr;
	param* arg;

	printf("CS 149 Sudoku from Michelle Luong\n");

	printf("Sudoku Puzzle: \n");

	for(i = 0; i < 9; i++) {
		for(j = 0; j < 9; j++)
			printf("%d ", sudoku[i][j]);
		printf("\n");
	}

	threads = (pthread_t)malloc(sizeof(pthread_t)*MAX_THREADS);
	pthread_attr_init(&pthread_my_attr);
	arg = (param*) malloc(sizeof(param)*MAX_THREADS);

	/* SPAWN THREADS */
	for(int i = 0; i < MAX_THREADS; i++) {

		/* Check 3x3 */
		if((i >= 0) && (i < 9)) {
			arg[i].row = (i / 3) * 3;
			arg[i].col = 3 * (i % 3);
			arg[i].res_pos = i;
		}
		else if((i >= 9) && (i < 18)) {
			arg[i].row = i - 9;
			arg[i].col = 0;
			arg[i].res_pos = i;
		}
		else {
			arg[i].row = 0;
			arg[i].col = i - 18;
			arg[i].res_pos = i;
		}

		pthread_create(&threads[i], &pthread_my_attr, check_sudoku, (void*)(arg + i));
	}

	/* Join all threads */
	for(i = 0; i < MAX_THREADS; i++) {
		pthread_join(threads[i], NULL);
	}

	/* Check result */
	j = 1;
	for(i = 0; i < MAX_THREADS; i++) {
		if(result[i] == 0)
			j = 0;
	}

	if(j)
		printf("Sudoku is valid\n");
	else
		printf("Sudoku is not valid\n");

	free(arg);

}



