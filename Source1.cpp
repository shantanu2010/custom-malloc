#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "Header.h"


int main(void){

	
	char* array1 = (char*)bs_malloc(10 * sizeof(char));
	char* array2 = (char*)bs_malloc(40 * sizeof(char));
	
	for (int i = 0; i < 10; i++){
		array1[i] = 'A';
	}

	for (int i = 0; i < 40; i++){
		array2[i] = i;
	}


	for (int i = 0; i < 10; i++){
		printf("%c \t", array1[i]);
	}
	printf("\n");
	for (int i = 0; i < 40; i++){
		printf("%d\t", array2[i]);
	}

	bs_free(array1);
	for (int i = 0; i < 10; i++){
		printf("%c \t", array1[i]);
	}

	char* array3 = (char*)bs_malloc(10 * sizeof(char));

	if (array1 == array3){
		printf("\n\n freed !!");
	}

	// complete free
	getchar();
	return 0;

}