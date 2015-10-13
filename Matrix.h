//
//  Matrix.h
//  Matrix Calculator
//
//  Created by Sen on 13-4-21.


#ifndef Matrix_Calculator_Matrix_h
#define Matrix_Calculator_Matrix_h
#include "Global.h"



struct Matrix {
    int row;
    int col;
    Number *data;
};
typedef struct Matrix Matrix;

struct MatList {/* To record defined matrix by user*/
	char identifier[IDMAXLENGTH+1];
	Matrix mat;
	struct MatList *next;/* Next defined matrix to form a chain*/
};
typedef struct MatList MatList;
/* some related functions for Matrix */

Matrix createMatrix(int row, int col);
void deleteMatrix(Matrix *mat);
void changeOneElementOfMatrix(Matrix mat, int row, int col, Number val);
Number readOneElementOfMatrix(Matrix mat, int row, int col);
Matrix createZeroMatrix(int row, int col);
Matrix createEyeMatrix(int N);
Matrix createRandMatrix(int N, int M);
Matrix createBracketedMatrix(Number N, Number x, Number M);
Matrix addMatrix(Matrix mat1, Matrix mat2);
Matrix minusMatrix(Matrix mat1, Matrix mat2);
Matrix multiplyMatrix(Matrix mat1, Matrix mat2);
Matrix divideMatrix(Matrix mat1, Matrix mat2);
Matrix modMatrix(Matrix mat1, Matrix mat2);
Matrix createSingletonMatrix(Number val);
Matrix roundMatrix(Matrix mat);
Matrix upperMatrix(Matrix mat);
Matrix lowerMatrix(Matrix mat);
Matrix turnMatrix(Matrix mat);
Matrix powerMatrix(Matrix mat, int N);
Matrix dotMatrix(Matrix mat1, Matrix mat2);
Matrix normMatrix(Matrix mat1);
Matrix angleMatrix(Matrix mat1, Matrix mat2);
Number sumMatrix(Matrix mat);
void displayMatrix(Matrix mat);

Matrix cloneMatrix(Matrix mat);
Number DeterminantCalc(int N, Matrix mat);

Matrix createEmptyMatrix(void);
MatList *createMatList();
MatList *checkMatList(char matName[]);
MatList *checkMatName(const char *matName);
MatList *intoMatList(char matName[], Matrix mat);
MatList *outMatList(char matName[]);
Matrix createSubMatrix(Matrix mat, Matrix mat1, Matrix mat2);
Matrix cmbMatrixList(Matrix mat1, Matrix mat2);
Matrix cmbMatrixListRow(Matrix mat1, Matrix mat2);
void displayInternalInfoOfMatlist(MatList *node);
void errorReport(void);

void testMatrixFunctions(void);


#endif
