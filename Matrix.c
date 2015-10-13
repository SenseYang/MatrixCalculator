//
//  Matrix.c
//  Matrix Calculator
//
//  Created by Sen on 13-4-21.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <malloc.h>
#include "Matrix.h"

extern MatList *matrixList;
extern MatList *currentMat;/* To record the number of used matlist in matrixList*/
extern int error_sort;
extern int get_element;
extern int get_submatrix;

/* note that error-checking is not offered by these functions */

Matrix createMatrix(int row, int col) {
    int i;
    /* allocate space for a matrix */
    Matrix tmp;
    tmp.row = row;
    tmp.col = col;
    /* allocate space for acutal data */
    tmp.data = (Number *)malloc(sizeof(Number)*row*col);
    
    for (i = 0; i < row*col; i++) {
        tmp.data[i] = 0;
    }
    
    return tmp;
}

Matrix createEmptyMatrix(void){
    Matrix tmp;
    tmp.row = 0;
    tmp.col = 0;
    tmp.data = NULL;
    return tmp;
}

void deleteMatrix(Matrix *mat){
    free(mat->data);
    mat->data = NULL;
    mat->row = 0;
    mat->col = 0;
    
}

Matrix cloneMatrix(Matrix oldMat){
    int i;
    Matrix newMat;
    newMat.row = oldMat.row;
    newMat.col = oldMat.col;
    newMat.data = (Number *)malloc(sizeof(Number)*(newMat.row)*(newMat.col));
    for (i = 0; i < (newMat.row)*(newMat.col); i++) {
        newMat.data[i] = oldMat.data[i];
    }
    return newMat;
}


void changeOneElementOfMatrix(Matrix mat, int row, int col, Number val){
	int rowLength = mat.row;
	if ((row<=mat.row)&&(col<=mat.col)){ 
		mat.data[rowLength*(col-1)+row-1] = val; /* values are stored in column major form*/
	}
	else{
		error_sort = ERROR_EXCEED;
		errorReport();
	}
   
}


Number readOneElementOfMatrix(Matrix mat, int row, int col){
    int rowLength = mat.row;
    return mat.data[rowLength*(col-1)+row-1];
}


Matrix createZeroMatrix(int row, int col) {
    return createMatrix(row, col);
}

Matrix createSingletonMatrix(Number val){
    Matrix tmp = createMatrix(1, 1);
    changeOneElementOfMatrix(tmp, 1, 1, val);
    return tmp;
}

// To create a unit matrix
Matrix createEyeMatrix(int N) {
    Matrix tmp = createZeroMatrix(N, N);
    int i;
    for (i = 1; i <= N; i++) {
        changeOneElementOfMatrix(tmp, i, i, 1);
    }
    return tmp;
}



// To create a rand matrix
Matrix createRandMatrix(int N, int M){
	Matrix tmp = createZeroMatrix(N,M);
	int i;
	int j;
	Number figure;
	srand((unsigned)time(NULL));
	for(i = 1; i<=N; i++){
		for(j = 1; j<=M; j++){
			figure = rand()/(RAND_MAX+0.1);
			changeOneElementOfMatrix(tmp,i,j,figure);		
		}	
	}
	return tmp;
}

void displayMatrix(Matrix mat) {
    
    if ((mat.row==0) || (mat.col==0)) {
        printf("<empty>\n");
        return;
    }
    
    int i,j;
    for (i = 1; i <= mat.row; i++) {
        for (j = 1; j <= mat.col; j++) {
            /* this line may have to be changed, if not using floating number for Number */
            printf("%6.4f\t", readOneElementOfMatrix(mat,i,j));
        }
        printf("\n");
    }
}


// To add two matrixes
Matrix addMatrix(Matrix mat1, Matrix mat2){
	int	i,j;
	Number add1, add2;
	Number result;
	int label;
	Matrix tmp;

	if (((mat1.col==1)&&(mat1.row==1))||((mat2.col==1)&&(mat2.row==1))){
		if (mat1.col==1)
			label = 1;
		else
			label = 2;
		if (label==1)
		{
			tmp = createMatrix(mat2.row, mat2.col);
			for (i = 1; i <= mat2.row; i++) {
				for (j = 1; j <= mat2.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,1,1);
					add2 =  readOneElementOfMatrix(mat2,i,j);
					result = add1 + add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}
		else 
		{
			tmp = createMatrix(mat1.row, mat1.col);
			for (i = 1; i <= mat1.row; i++) {
				for (j = 1; j <= mat1.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,i,j);
					add2 =  readOneElementOfMatrix(mat2,1,1);
					result = add1 + add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}	
	}

	else if ((mat1.col==mat2.col)&&(mat1.row==mat2.row)){	
		tmp = createMatrix(mat1.row, mat1.col);
		for (i = 1; i <= mat1.row; i++) {
			for (j = 1; j <= mat1.col; j++) {
				add1 =  readOneElementOfMatrix(mat1,i,j);
				add2 =  readOneElementOfMatrix(mat2,i,j);
				result = add1 + add2;
				changeOneElementOfMatrix(tmp,i,j,result);           
			}
		}
	}
	else{
		error_sort = ERROR_MATRIX_DISMATCH;
		errorReport();
		
	}
	return tmp;	
}

// To minus two matrix
Matrix minusMatrix(Matrix mat1, Matrix mat2){
	int	i,j;
	Number add1, add2;
	Number result;
	int label;
	Matrix tmp;

	if (((mat1.col==1)&&(mat1.row==1))||((mat2.col==1)&&(mat2.row==1))){
		if (mat1.col==1)
			label = 1;
		else
			label = 2;
		if (label==1)
		{
			tmp = createMatrix(mat2.row, mat2.col);
			for (i = 1; i <= mat2.row; i++) {
				for (j = 1; j <= mat2.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,1,1);
					add2 =  readOneElementOfMatrix(mat2,i,j);
					result = add1 - add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}
		else 
		{
			tmp = createMatrix(mat1.row, mat1.col);
			for (i = 1; i <= mat1.row; i++) {
				for (j = 1; j <= mat1.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,i,j);
					add2 =  readOneElementOfMatrix(mat2,1,1);
					result = add1 - add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}	
	}

	else if ((mat1.col==mat2.col)&&(mat1.row==mat2.row)){	
		tmp = createMatrix(mat1.row, mat1.col);
		for (i = 1; i <= mat1.row; i++) {
			for (j = 1; j <= mat1.col; j++) {
				add1 =  readOneElementOfMatrix(mat1,i,j);
				add2 =  readOneElementOfMatrix(mat2,i,j);
				result = add1 - add2;
				changeOneElementOfMatrix(tmp,i,j,result);           
			}
		}
	}
	else{
		error_sort = ERROR_MATRIX_DISMATCH;
		errorReport();
		
	}
	return tmp;	
}

// To multiply a matrix and a number
Matrix multiplyMatrix(Matrix mat1, Matrix mat2){
	int	i,j;
	int a,b,c,d;
	Number add1, add2;
	Number result;
	int label;
	Matrix tmp;

	if (((mat1.col==1)&&(mat1.row==1))||((mat2.col==1)&&(mat2.row==1))){
		if (mat1.col==1)
			label = 1;
		else
			label = 2;
		if (label==1)
		{
			tmp = createMatrix(mat2.row, mat2.col);
			for (i = 1; i <= mat2.row; i++) {
				for (j = 1; j <= mat2.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,1,1);
					add2 =  readOneElementOfMatrix(mat2,i,j);
					result = add1 * add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}
		else 
		{
			tmp = createMatrix(mat1.row, mat1.col);
			for (i = 1; i <= mat1.row; i++) {
				for (j = 1; j <= mat1.col; j++) {
					add1 =  readOneElementOfMatrix(mat1,i,j);
					add2 =  readOneElementOfMatrix(mat2,1,1);
					result = add1 * add2;
					changeOneElementOfMatrix(tmp,i,j,result);           
				}
			}
		}	
	}

	else if (mat1.col==mat2.row){
		tmp = createMatrix(mat1.row, mat2.col);
		for (a = 1;a <= mat1.row; a++){
			for (b = 1; b <= mat2.col; b++){
				result = 0;
				for (c = 1;c <=mat1.col; c++){
					add1 = readOneElementOfMatrix(mat1,a,c);
					add2 = readOneElementOfMatrix(mat2,c,b);
					result = result + add1*add2;
				}
				changeOneElementOfMatrix(tmp,a,b,result);			
			}
		
		}
		
	}
	else{
		error_sort = ERROR_MATRIX_DISMATCH;
		errorReport();
		
	}
	return tmp;	
}

// To divide a number
Matrix divideMatrix(Matrix mat1, Matrix mat2){
	Matrix tmp;
	tmp = createMatrix(mat1.row, mat1.col);
	int i,j;
	Number result;
	Number figure;
	figure = readOneElementOfMatrix(mat2,1,1);
	for (i = 1; i <=mat1.row; i++){
		for (j = 1; j <=mat1.col; j++){
			result = readOneElementOfMatrix(mat1,i,j)/figure;
			changeOneElementOfMatrix(tmp,i,j,result);
		}
	}
	return tmp;
}


// To get mod of Matrix
Matrix modMatrix(Matrix mat1, Matrix mat2){
	Matrix tmp;
	int i,j;
	int result;
	int figure;
	int mod;
	mod = (int)readOneElementOfMatrix(mat2,1,1);
	tmp = createMatrix(mat1.row, mat1.col);
	for (i = 1; i <= mat1.row; i++){
		for (j = 1; j<=mat1.col; j++){
			figure = (int)readOneElementOfMatrix(mat1,i,j);
		    result = figure%mod;
			changeOneElementOfMatrix(tmp,i,j,result);
		}
	
	}
	return tmp;
}

// To get sum of Matrix
Number sumMatrix(Matrix mat){
	int i,j;
	Number sum, tmp;
	sum = 0;
	for (i = 1; i <= mat.row; i++){
		for (j = 1; j <= mat.col; j++)	{
			tmp = readOneElementOfMatrix(mat,i,j);
			sum = sum + tmp;
		}
	}
	return sum;
}

// To get maximum of matrix
Number maxMatrix(Matrix mat){
	int i,j;
	Number max, tmp;
	max = readOneElementOfMatrix(mat,1,1);
	for (i = 1; i <= mat.row; i++){
		for (j = 1; j <= mat.col; j++)	{
			tmp = readOneElementOfMatrix(mat,i,j);
			if (max<=tmp)
			{max = tmp;}

		}
	}
	return max;
}

// To get minimum of matrix
Number minMatrix(Matrix mat){
	int i,j;
	Number min, tmp;
	min = readOneElementOfMatrix(mat,1,1);
	for (i = 1; i <= mat.row; i++){
		for (j = 1; j <= mat.col; j++)	{
			tmp = readOneElementOfMatrix(mat,i,j);
			if (min>=tmp)
			{min = tmp;}

		}
	}
	return min;
}

// generate matrix using form like Matname = [N:x:M]
Matrix createBracketedMatrix(Number N, Number x, Number M){
	Matrix tmp;
	int length;
	int i;
	Number result;
	length = (M-N)/x + 1;	
	tmp = createMatrix(1,length);
	for (i = 1; i<=length; i++){
		result = N + (i-1) * x;
		changeOneElementOfMatrix(tmp,1,i,result);
	}
	return tmp;
}

Matrix roundMatrix(Matrix mat){
	Matrix tmp;
	Number figure1;
	int figure2;
	Number figure3;
	int i,j;
	tmp = createMatrix(mat.row,mat.col);
	for (i = 1; i <= mat.row; i++){
		for (j = 1;j <= mat.col;j++){
			figure1 = readOneElementOfMatrix(mat,i,j);
			figure2 = (int)figure1;
			if (figure1<0)
				figure2 = figure2 - 1;
			figure3 = (Number)figure2;
			figure1 = figure1 - figure3;
			if (figure1>=0.5){
				figure1 = ((Number)figure2+1);}
			else{
				figure1 = (Number)figure2;
			}
			changeOneElementOfMatrix(tmp,i,j,figure1);	
			
		}
	}
	return tmp;
}

Matrix upperMatrix(Matrix mat){
	Matrix tmp;
	Number figure1;
	int figure2;
	
	int i,j;
	tmp = createMatrix(mat.row,mat.col);
	for (i = 1; i <= mat.row; i++){
		for (j = 1;j <= mat.col;j++){
			figure1 = readOneElementOfMatrix(mat,i,j);
			figure2 = (int)figure1;
			if (figure1<0)
				figure2 = figure2 - 1;
			
			figure1 = ((Number)figure2+1);
			changeOneElementOfMatrix(tmp,i,j,figure1);	
			
		}
	}
	return tmp;
}

Matrix lowerMatrix(Matrix mat){
	Matrix tmp;
	Number figure1;
	int figure2;
	int i,j;
	tmp = createMatrix(mat.row,mat.col);
	for (i = 1; i <= mat.row; i++){
		for (j = 1;j <= mat.col;j++){
			figure1 = readOneElementOfMatrix(mat,i,j);
			figure2 = (int)figure1-1;
			if (figure1<0)
				figure2 = figure2 - 1;
			figure1 = ((Number)figure2+1);
			changeOneElementOfMatrix(tmp,i,j,figure1);	
			
		}
	}
	return tmp;
}

MatList *createMatList(void){
    MatList *tmp = (MatList *)malloc(sizeof(MatList));
    tmp->next = NULL;
    strcpy(tmp->identifier, "[undefined]");
    tmp->mat = createEmptyMatrix();
    return tmp;
}

MatList *checkMatList(char matName[]){/* to check if the matName was defined before, yes then return the pointer of this element, no then return new pointer of matrixList*/
	int label;
	MatList *p;
	p = matrixList;
	while ((strcmp(p->identifier,matName)!=0)&&(p->next!=NULL)){
		p = p->next;
	}
	if (strcmp(p->identifier,matName)==0){
		return p;
	}
	else{
		currentMat = createMatList();
		p->next = currentMat;
		p = p->next;		
		return p;
	}
	
}

MatList *checkMatName(const char *matName){/* to check if the matName was defined before, yes then return the pointer of this element, no then RETURN TO MATRIXLIST(different to checkMatList())*/
	int label;
	MatList *p;
	p = matrixList;
	while ((strcmp(p->identifier,matName)!=0)&&(p->next!=NULL)){
		p = p->next;
	}
	if (strcmp(p->identifier,matName)==0){
		return p;
	}
	else{
		p = matrixList;		
		return p;
	}
	
}

MatList *intoMatList(char matName[], Matrix newmat){
	MatList *p;
	p = checkMatList(matName);
	strcpy(p->identifier, matName);
	p->mat = cloneMatrix(newmat);
	return p;
}

MatList *outMatList(char matName[]){/* Get out of previously defined matrixlist*/
	MatList *p;
	p = matrixList;
	while ((strcmp(p->identifier,matName)!=0)&&(p->next!=NULL)){
		p = p->next;
	}
	if (strcmp(p->identifier,matName)==0){
		return p;
	}
	else{
		error_sort = ERROR_UNDEFINED_VARIABLE;
		errorReport();// Error report will be written altogether
	}
}

void errorReport(void){
	switch (error_sort){
	case ERROR_UNDEFINED_VARIABLE:
		printf("Undefined function or variable.\n");
		break;
	case ERROR_MATRIX_DISMATCH:
		printf("Matrix dimensions must agree.\n");
		break;
	case ERROR_SYNTAX:
		printf("Syntax error.\n");
		break;
	case ERROR_EXCEED:
		printf("Index exceeds matrix dimensions.\n");
		break;
	case ERROR_NOTSCALAR:
		printf("You should input two scalars.\n");
		break;
	default:
		printf("There are errors in your command.\n");
		break;

	}
}

void displayInternalInfoOfMatlist(MatList *node){
    
    if (node == NULL) {
        return;
    } else {
        /* show type and subtype */
        printf("%s = \n",node->identifier);
        displayMatrix(node->mat);
        /* show more detail */
       
	}
}

Number DeterminantCalc(int N, Matrix mat){
	Number result;
	Number tmp;
	Number element;
	Matrix tmpMat;
	int i;
	int j,k;
	int sign = -1;
	result = 0;
	if (N>1){	
		for (i = 1; i<=N; i++){
			sign = sign * -1;
			tmp = (Number)sign * readOneElementOfMatrix(mat,1,i);
			/*To calc remnant determinant - tmpMat*/
			tmpMat = createEyeMatrix(N-1);
			for (j = 1; j<=N-1;j++){
				for (k = 1; k <=i-1; k++){
					element = readOneElementOfMatrix(mat,j+1,k);
					changeOneElementOfMatrix(tmpMat,j,k,element);
				}
				for (k = i;k<=N-1;k++){
					element = readOneElementOfMatrix(mat,j+1,k+1);
					changeOneElementOfMatrix(tmpMat,j,k,element);
				}
			}
			result = result + tmp * DeterminantCalc(N-1,tmpMat);

		}
	}
	else {
		result = readOneElementOfMatrix(mat,1,1);
	}
	return result;
}

Matrix turnMatrix(Matrix mat){
	Matrix tmp;
	Number element;
	int M;
	int N;
	int i,j;
	M = mat.row;
	N = mat.col;
	tmp = createMatrix(N,M);
	for (i = 1; i<=N; i++){
		for (j = 1; j<=M; j++){
			element = readOneElementOfMatrix(mat,j,i);
			changeOneElementOfMatrix(tmp,i,j,element);
		}
	}
	return tmp;
}

void testMatrixFunctions(void){
    
}

Matrix powerMatrix(Matrix mat, int N){
	Matrix tmp;
	if (N == 0){
		tmp = createEyeMatrix(mat.row);
		return tmp;
	}
	if (N%2==0){
		tmp = powerMatrix(mat, N/2);
		tmp = multiplyMatrix(tmp,tmp);
	}
	else if (N%2 == 1){
		tmp = powerMatrix(mat,(N-1)/2);
		tmp = multiplyMatrix(multiplyMatrix(tmp, tmp), mat);
	}
	return tmp;

}

Matrix dotMatrix(Matrix mat1, Matrix mat2){
	Number sum = 0;
	Number figure;
	Matrix tmp;
	int label1;
	int label2;
	int i;
	int length1, length2;
	if (mat1.row == 1){
		length1 = mat1.col;
		label1 = 1;
	}
	else if (mat1.col == 1){
		length1 = mat1.row;
		label1 = 0;
	}
	if (mat2.row == 1){
		length2 = mat1.col;
		label2 = 1;
	}
	else if (mat2.col == 1){
		length2 = mat1.row;
		label2 = 0;
	}

	if (length1 != length2){
		error_sort = ERROR_MATRIX_DISMATCH;
		errorReport();
	}
	if (((label1!=1)&&(label1!=0))||((label2!=1)&&(label2!=0)))
	{
		error_sort = ERROR_NOTSCALAR;
		errorReport();
	}
	for (i = 1; i<=length1; i++ ){
		if ((label1 == 1)&&(label2 == 1)){
			figure = readOneElementOfMatrix(mat1,1,i)*readOneElementOfMatrix(mat2,1,i);
			sum = sum + figure;
		}
		else if ((label1 == 0)&&(label2 == 0)){
			figure = readOneElementOfMatrix(mat1,i,1)*readOneElementOfMatrix(mat2,i,1);
			sum = sum + figure;
		}
		else if ((label1 == 1)&&(label2 == 0)){
			figure = readOneElementOfMatrix(mat1,1,i)*readOneElementOfMatrix(mat2,i,1);
			sum = sum + figure;
		}
		else if ((label1 == 0)&&(label2 == 1)){
			figure = readOneElementOfMatrix(mat1,i,1)*readOneElementOfMatrix(mat2,1,i);
			sum = sum + figure;
		}
	}
	tmp = createEyeMatrix(1);
	changeOneElementOfMatrix(tmp, 1, 1, sum);
	return tmp;
}

Matrix normMatrix(Matrix mat1){
	Number sum = 0;
	Number figure;
	Matrix tmp;
	int label1;
	int i;
	int length1;
	if (mat1.row == 1){
		length1 = mat1.col;
		label1 = 1;
	}
	else if (mat1.col == 1){
		length1 = mat1.row;
		label1 = 0;
	}
		
	if ((label1!=1)&&(label1!=0))
	{
		error_sort = ERROR_NOTSCALAR;
		errorReport();
	}
	for (i = 1; i<=length1; i++ ){
		if (label1 == 1){
			figure = readOneElementOfMatrix(mat1,1,i)*readOneElementOfMatrix(mat1,1,i);
			sum = sum + figure;
		}
		else if (label1 == 0){
			figure = readOneElementOfMatrix(mat1,i,1)*readOneElementOfMatrix(mat1,i,1);
			sum = sum + figure;
		}
		
	}
	sum = sqrt(sum);
	tmp = createEyeMatrix(1);
	changeOneElementOfMatrix(tmp, 1, 1, sum);
	return tmp;
}

Matrix angleMatrix(Matrix mat1, Matrix mat2){
	Number sum = 0;
	Number figure;
	Number norm1;
	Number norm2;
	Matrix tmp;
	int label1;
	int label2;
	int i;
	int length1, length2;
	if (mat1.row == 1){
		length1 = mat1.col;
		label1 = 1;
	}
	else if (mat1.col == 1){
		length1 = mat1.row;
		label1 = 0;
	}
	if (mat2.row == 1){
		length2 = mat1.col;
		label2 = 1;
	}
	else if (mat2.col == 1){
		length2 = mat1.row;
		label2 = 0;
	}

	if (length1 != length2){
		error_sort = ERROR_MATRIX_DISMATCH;
		errorReport();
	}
	if (((label1!=1)&&(label1!=0))||((label2!=1)&&(label2!=0)))
	{
		error_sort = ERROR_NOTSCALAR;
		errorReport();
	}
	for (i = 1; i<=length1; i++ ){
		if ((label1 == 1)&&(label2 == 1)){
			figure = readOneElementOfMatrix(mat1,1,i)*readOneElementOfMatrix(mat2,1,i);
			sum = sum + figure;
		}
		else if ((label1 == 0)&&(label2 == 0)){
			figure = readOneElementOfMatrix(mat1,i,1)*readOneElementOfMatrix(mat2,i,1);
			sum = sum + figure;
		}
		else if ((label1 == 1)&&(label2 == 0)){
			figure = readOneElementOfMatrix(mat1,1,i)*readOneElementOfMatrix(mat2,i,1);
			sum = sum + figure;
		}
		else if ((label1 == 0)&&(label2 == 1)){
			figure = readOneElementOfMatrix(mat1,i,1)*readOneElementOfMatrix(mat2,1,i);
			sum = sum + figure;
		}
	}
	norm1 = readOneElementOfMatrix(normMatrix(mat1),1,1);
	norm2 = readOneElementOfMatrix(normMatrix(mat2),1,1);
	sum = sum/(norm1*norm2);
	sum = acos(sum);
	tmp = createEyeMatrix(1);
	changeOneElementOfMatrix(tmp, 1, 1, sum);
	return tmp;
}

Matrix createSubMatrix(Matrix mat, Matrix mat1, Matrix mat2){
	int i,j;
	Matrix tmp;
	if ((mat1.row==0)&&(mat1.col==0)){    /* Corresponding to form like Matname(:,1:10)*/
		tmp = createMatrix(mat.row,mat2.col);
		for (i = 1;i <=mat.row; i++){
			for (j = 1;j <=mat2.col; j++){
				changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat,i,(int)readOneElementOfMatrix(mat2,1,j)));
			}
		}
	}
	else if ((mat2.row==0)&&(mat2.col==0)){/* Corresponding to form like Matname(1:10,:)*/
		tmp = createMatrix(mat1.col,mat.col);
		for (i = 1;i <=mat1.col; i++){
			for (j = 1;j <=mat.col; j++){
				changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat,(int)readOneElementOfMatrix(mat1,1,i),j));
			}
		}
	}
	else if ((mat1.row==0)&&(mat2.col==0)){
		tmp = createMatrix(mat.col,mat.col);
		for (i = 1;i <=mat.row; i++){
			for (j = 1;j <=mat.col; j++){
				changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat,i,j));
			}
		}
	}
	else{                                  /* Corresponding to form like Matname(1:10,1:10)*/
		tmp = createMatrix(mat1.col,mat2.col);
		for (i = 1;i <=mat1.col; i++){
			for (j = 1;j <=mat2.col; j++){
				changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat,(int)readOneElementOfMatrix(mat1,1,i),(int)readOneElementOfMatrix(mat2,1,j)));
			}
		}
	}
	return tmp;
}

Matrix cmbMatrixList(Matrix mat1, Matrix mat2){
	Matrix tmp;
	int i,j;
	tmp = createMatrix(mat1.row+mat2.row, mat1.col);
	for (i=1;i<=mat1.row;i++){
		for (j = 1;j<=mat1.col;j++){
			changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat1,i,j));
		}
	}
	for (i = mat1.row+1;i<=mat1.row+mat2.row;i++){
		for (j = 1;j <=mat1.col;j++){
			changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat2,i-mat1.row,j));
		}
	}
	return tmp;
}

Matrix cmbMatrixListRow(Matrix mat1, Matrix mat2){
	Matrix tmp;
	int i,j;
	tmp = createMatrix(mat1.row, mat1.col + mat2.col);
	for (i = 1; i <=mat1.row;i++){
		for (j = 1; j<=mat1.col;j++){
			changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat1,i,j));
		}
		for (j = mat1.col+1; j<=mat1.col+mat2.col;j++){
			changeOneElementOfMatrix(tmp,i,j,readOneElementOfMatrix(mat2,i,j-mat1.col));
		}
	}
	return tmp;
}