//
//  main.c
//  Matrix Calculator
//
//  Created by Sen on 13-4-20.


#include <stdio.h>
#include <string.h>
#include "Global.h"
#include "Matrix.h"
#include "expressions.tab.h"

int yyparse(void);
MatList *matrixList = NULL;
MatList *currentMat = NULL;/* Point to current matrixlist*/
int error_sort;
int get_element;
int get_submatrix;
int main(int argc, const char * argv[])
{
	matrixList = createMatList();
	strcpy(matrixList->identifier,"ans");
	currentMat = matrixList;
	get_element = 0;
    get_submatrix = 0;
    //testMatrixFunctions();
    yyparse();
    printf("Hello, World!\n");
    return 0;
}

