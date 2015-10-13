//
//  functions.c
//  Matrix Calculator
//
//  Created by Sen on 13-4-22.


#include <stdio.h>
#include <string.h>
#include "functions.h"

extern MatList *matrixList;
extern MatList *currentMat;/* To record the number of used matlist in matrixList*/
extern int error_sort;
extern int get_element;
extern int get_submatrix;


Matrix callFunc(const char *funcName, ASTNode *argListNode){
    /* sort out args */
    ASTNode *argNode[10];
    Matrix result;
	Number calcresult;
	MatList *p;
    if (strcmp(funcName, "eye")==0) { /* generate eye matrix */
        argNode[0] = getNthArgFromArgList(argListNode,1);
        result =  createEyeMatrix((int)readOneElementOfMatrix(argNode[0]->mat,1,1));
    }
	else if(strcmp(funcName, "rand")==0){/*generate rand matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		argNode[1] = getNthArgFromArgList(argListNode,2);
		result = createRandMatrix((int)readOneElementOfMatrix(argNode[0]->mat,1,1),readOneElementOfMatrix(argNode[1]->mat,1,1));
	}
	else if(strcmp(funcName, "zeros")==0){/*generate rand matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		argNode[1] = getNthArgFromArgList(argListNode,2);
		result = createZeroMatrix((int)readOneElementOfMatrix(argNode[0]->mat,1,1),readOneElementOfMatrix(argNode[1]->mat,1,1));
	}
	else if(strcmp(funcName, "max")==0){/*calculate maximum of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		calcresult = maxMatrix(argNode[0]->mat);
		result = createEyeMatrix(1);
		changeOneElementOfMatrix(result, 1, 1, calcresult);
	}
	else if(strcmp(funcName, "min")==0){/*calculate minimum of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		calcresult = minMatrix(argNode[0]->mat);
		result = createEyeMatrix(1);
		changeOneElementOfMatrix(result, 1, 1, calcresult);
	}
	else if(strcmp(funcName, "sum")==0){/*calculate sum of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		calcresult = sumMatrix(argNode[0]->mat);
		result = createEyeMatrix(1);
		changeOneElementOfMatrix(result, 1, 1, calcresult);
	}
	else if(strcmp(funcName, "round")==0){/*calculate round of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		result = roundMatrix(argNode[0]->mat);
	}
	else if(strcmp(funcName, "upper")==0){/*calculate upper of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		result = upperMatrix(argNode[0]->mat);
	}
	else if(strcmp(funcName, "lower")==0){/*calculate upper of matrix*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		result = lowerMatrix(argNode[0]->mat);
	}
	else if(strcmp(funcName, "det")==0){/*calculate DeterminantCalc*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		calcresult = DeterminantCalc(argNode[0]->mat.row, argNode[0]->mat);
		result = createEyeMatrix(1);
		changeOneElementOfMatrix(result, 1, 1, calcresult);
	}

	else if(strcmp(funcName, "turn")==0){/*calculate turn*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		result = turnMatrix(argNode[0]->mat);		
	}
	else if(strcmp(funcName, "power")==0){/*calculate power*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		argNode[1] = getNthArgFromArgList(argListNode,2);
		result = powerMatrix(argNode[0]->mat,(int)readOneElementOfMatrix(argNode[1]->mat,1,1));		
	}
	else if(strcmp(funcName, "dot")==0){/*calculate dot*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		argNode[1] = getNthArgFromArgList(argListNode,2);
		result = dotMatrix(argNode[0]->mat,argNode[1]->mat);		
	}
	else if(strcmp(funcName, "norm")==0){/*calculate norm*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		result = normMatrix(argNode[0]->mat);		
	}
	else if(strcmp(funcName, "angle")==0){/*calculate norm*/
		argNode[0] = getNthArgFromArgList(argListNode,1);
		argNode[1] = getNthArgFromArgList(argListNode,2);
		result = angleMatrix(argNode[0]->mat,argNode[1]->mat);	
	}

	else{
		if (get_submatrix == 1){
			p = checkMatName(funcName);
			argNode[0] = getNthArgFromArgList(argListNode,1);
			argNode[1] = getNthArgFromArgList(argListNode,2);
			result = createSubMatrix(p->mat,argNode[0]->mat,argNode[1]->mat);
			get_submatrix = 0;
		}
		else{
			p = checkMatName(funcName);
			argNode[0] = getNthArgFromArgList(argListNode,1);
			argNode[1] = getNthArgFromArgList(argListNode,2);
			calcresult = readOneElementOfMatrix(p->mat,(int)readOneElementOfMatrix(argNode[0]->mat,1,1),readOneElementOfMatrix(argNode[1]->mat,1,1));
			result = createEyeMatrix(1);
			changeOneElementOfMatrix(result, 1, 1, calcresult);
		}
	}
	
	

    return result;
}