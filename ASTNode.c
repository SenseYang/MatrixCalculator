//
//  ASTNode.c
//  Matrix Calculator
//
//  Created by Sen on 13-4-21.


#include <stdio.h>
#include "ASTNode.h"
#include <string.h>
#include <stdlib.h>
#include "functions.h"
#include <assert.h>

/* some internal functions for evaluation */
static void evaluateASTNodeExpression(ASTNode *node);
static void evaluateASTNodeSimpleExp(ASTNode *node);
static void evaluateASTNodeTerm(ASTNode *node);
static void evaluateASTNodeFactor(ASTNode *node);
static void evaluateASTNodeSubMatrixList(ASTNode *node);
static void evaluateASTNodeSubMatrixListRow(ASTNode *node);
static void evaluateASTNodeSubMatrix(ASTNode *node);
//static void evaluateASTNodeVectorExp(ASTNode *node);
static void evaluateASTNodeArgList(ASTNode *node);
static void evaluateASTNodeArg(ASTNode *node);
static void evaluateASTNodeFuncExp(ASTNode *node);
static void evaluateASTNodeBracketedMatrix(ASTNode *node);

extern MatList *matrixList;
extern MatList *currentMat;/* To record the number of used matlist in matrixList*/
extern int error_sort;
extern int get_element;
extern int get_submatrix;

const char typeList[][IDMAXLENGTH+1] = {
    "DefaultType", /* maybe useful for initialization */
    "Expression", /* the type of one complete expression */
    
    "SimpleExp",
    
    "Term",
    "Factor",
    
    "SubMatrixList",
    "SubMatrixListRow",
    "SubMatrix",
    
    "ArgList",
    "Arg",
    
    "FuncExp",
    
    "Identifier",
    "Num",
    "VectorExp",
    "Colon", /* a special type of vector exp */
    "BracketedMatrix",
};

const char subTypeList[][IDMAXLENGTH+1] = {
    /* used for those without type */
    "DefaultSubType",
	
    /* 3 derivations of exp */
    "ExpSimpleAssign", /* ID = xxx */
    "ExpPartAssign", /* ID(X,Y) = xxx */
    "ExpSimpleExp",
    /* 3 derivations of simpleExp */
    "SimpleExpTerm",
    "SimpleExpPlus",
    "SimpleExpMinus",
	"SimpleExpPower",
    /* 4 derivations of term */
    "TermFactor",
    "TermTimes",
    "TermDivide",
    "TermMod",
	"TermTurn",
    /* 6 derivations of factor */
    "FactorParen",
    "FactorID",
    "FactorFuncExp",
    "FactorNumber",
    "FactorVectorExp",
    "FactorBracketedMatrix",
    
    /* 2 derivations of subMatrixList */
    "SubMatrixListSingle",
    "SubMatrixListMultiple",
    
    /* 2 derivations of subMatrixListRow (COMMA and space versions are combined) */
    "SubMatrixListRowSingle",
    "SubMatrixListRowMultiple",
    
    /* 4 derivations of subMatrix */
    "SubMatrixFuncExp",
    "SubMatrixID",
    "SubMatrixVectorExp",
    "SubMatrixNumber",
    
    /* 2 derivations of argList */
    "ArgListSingle",
    "ArgListMultiple",
    
    /* 4 derivations of arg */
    "ArgNumber",
    "ArgVectorExp",
    "ArgID",
    "ArgColon",
};

ASTNode *createASTNode(NodeType type, NodeSubType subType){
    ASTNode *tmp = (ASTNode *)malloc(sizeof(ASTNode));
    tmp->type = type;
    tmp->subType = subType;
    tmp->l = NULL;
    tmp->r = NULL;
    tmp->a = NULL;
    strcpy(tmp->identifier, "[undefined]");
    tmp->mat = createEmptyMatrix();
    tmp->start = 0;
    tmp->end = 0;
    tmp->step = 0;
    tmp->scalarValue = 0;
    
    tmp->evaluated = 0;
    
    //tmp->nextArg = NULL;
    
#ifdef DEBUGGING_ON
    printf("creating node %p\n",tmp);
#endif
    
    return tmp;
}

void deleteASTNode(ASTNode *node){
    
    if (node==NULL){
        return;
    } else {
#ifdef DEBUGGING_ON
        printf("deleting node %p\n", node);
#endif
        deleteASTNode(node->l);
        deleteASTNode(node->r);
        deleteASTNode(node->a);
        deleteMatrix(&(node->mat));
        free(node);
        return;
    }
    
}
void evaluateASTNode(ASTNode *node){
    if (node == NULL || node->evaluated) { /* evaluation unnecessary */
        return;
    } else {
        
        evaluateASTNode(node->l);
        evaluateASTNode(node->r);
        evaluateASTNode(node->a);
        
        switch (node->type) {
            case Expression:
                evaluateASTNodeExpression(node);
                break;
            case SimpleExp:
                evaluateASTNodeSimpleExp(node);
                break;
            case Term:
                evaluateASTNodeTerm(node);
                break;
            case Factor:
                evaluateASTNodeFactor(node);
                break;
            case SubMatrixList:
                evaluateASTNodeSubMatrixList(node);
                break;
            case SubMatrixListRow:
                evaluateASTNodeSubMatrixListRow(node);
                break;
            case SubMatrix:
                evaluateASTNodeSubMatrix(node);
                break;
			case ArgList:
                evaluateASTNodeArgList(node);
                break;
            case Arg:
                evaluateASTNodeArg(node);
                break;
            case FuncExp:
                evaluateASTNodeFuncExp(node);
                break;
            case BracketedMatrix:
                evaluateASTNodeBracketedMatrix(node);
                break;
            default:
                printf("ERROR!!! the node must have been evaluated!!!\n");
                break;
        }
        node->evaluated = 1; /* finished evaluation */
        return;
    }
}

void traverseASTNode(ASTNode *node){
    if (node == NULL) {
        return;
    } else {
        displayInternalInfoOfASTNode(node);
        traverseASTNode(node->l);
        traverseASTNode(node->r);
        traverseASTNode(node->a);
    }
}

void displayInternalInfoOfASTNode(ASTNode *node){
    
    if (node == NULL) {
        return;
    } else {
        /* show type and subtype */
        printf("type: %s - %s\n",typeList[node->type],subTypeList[node->subType]);
        displayMatrix(node->mat);
        /* show more detail */
        switch (node->type) {
            case Num:
                printf("scalarValue: %f\n", node->scalarValue);
                break;
			case VectorExp:
				printf("node->start:%f, node->step:%f, node->end:%f\n",node->start,node->step,node->end);
			default:				
                break;
        }
    }
}

void evaluateASTNodeExpression(ASTNode *node){
	MatList *p;
	Matrix result;
	ASTNode *argNode[10];
	Number figure;
    switch (node->subType) {
        case ExpSimpleAssign:
		    p = intoMatList(node->l->identifier,node->r->mat);
			matrixList->mat = cloneMatrix(p->mat);
			displayInternalInfoOfMatlist(p);
			node->mat = cloneMatrix(node->r->mat);
            break;
        case ExpPartAssign:                                 /* Still not written*/
			p = checkMatList(node->l->identifier);
			argNode[0] = getNthArgFromArgList(node->r,1);
		    argNode[1] = getNthArgFromArgList(node->r,2);
			figure = readOneElementOfMatrix(node->a->mat,1,1);
			matrixList->mat = createEyeMatrix(1);
			changeOneElementOfMatrix(matrixList->mat,1,1,figure);
			changeOneElementOfMatrix(p->mat,(int)readOneElementOfMatrix(argNode[0]->mat,1,1),(int)readOneElementOfMatrix(argNode[1]->mat,1,1),figure);
			printf("%s(%d,%d) = \n",p->identifier,(int)readOneElementOfMatrix(argNode[0]->mat,1,1),(int)readOneElementOfMatrix(argNode[1]->mat,1,1));
			printf("%f\n",figure);
            break;
        case ExpSimpleExp:
            /*just clone the result*/
            node->mat = cloneMatrix(node->l->mat);
			matrixList->mat = cloneMatrix(node->mat);
			displayMatrix(node->mat);
			 
            break;
        default:
            break;
    }
    return;
}



void evaluateASTNodeSimpleExp(ASTNode *node){
    switch (node->subType) {
        case SimpleExpTerm:
            /*just clone the result*/
            node->mat = cloneMatrix(node->l->mat);
            break;
		case SimpleExpPlus:
			node->mat = addMatrix(node->l->mat, node->r->mat);
			break;
		case SimpleExpMinus:
			node->mat = minusMatrix(node->l->mat, node->r->mat);
			break;
		case SimpleExpPower:
			node->mat = powerMatrix(node->l->mat, (int)readOneElementOfMatrix(node->r->mat,1,1));
			break;
		default:
            break;
    }
    return;
}
void evaluateASTNodeTerm(ASTNode *node){
    switch (node->subType) {
        case TermFactor:
            /*just clone the result*/
            node->mat = cloneMatrix(node->l->mat);
            break;
		case TermTimes:
			node->mat = multiplyMatrix(node->l->mat, node->r->mat);
			break;
		case TermDivide:
			node->mat = divideMatrix(node->l->mat, node->r->mat);
			break;
		case TermMod:
			node->mat = modMatrix(node->l->mat, node->r->mat);
			break;
		case TermTurn:  
			node->mat = turnMatrix(node->l->mat);
			break;
        default:
            break;
    }
    return;
}
void evaluateASTNodeFactor(ASTNode *node){
	MatList *p;
    switch (node->subType) {
        case FactorParen:
        case FactorFuncExp:
            /*just clone the result*/
            node->mat = cloneMatrix(node->l->mat);
            break;
        case FactorNumber:
            node->mat = createSingletonMatrix(node->l->scalarValue);
			break;
		case FactorID:
			strcpy(node->identifier, node->l->identifier);
			p = outMatList(node->identifier);
			if (strcmp(p->identifier,node->identifier)==0){
				node->mat = cloneMatrix(p->mat);}
			break;	
		case FactorBracketedMatrix:
			node->mat = cloneMatrix(node->l->mat);
			break;
		/* some work left undone */
        default:
            break;
    }
    return;
}

void evaluateASTNodeBracketedMatrix(ASTNode *node){
	switch (node->subType) {
        case DefaultSubType:
			node->mat = cloneMatrix(node->l->mat);
			break;        
        default:
            break;
    }
    return;
}
void evaluateASTNodeSubMatrixList(ASTNode *node){
	switch (node->subType) {
        case SubMatrixListSingle:
			node->mat = cloneMatrix(node->l->mat);
			break;   
		case SubMatrixListMultiple:
			node->mat = cmbMatrixList(node->l->mat,node->r->mat);
			break;
        default:
            break;
    }
    return;
    
}
void evaluateASTNodeSubMatrixListRow(ASTNode *node){
	switch (node->subType) {
        case SubMatrixListRowSingle:
			node->mat = cloneMatrix(node->l->mat);
			break;        
		case SubMatrixListRowMultiple:
			node->mat = cmbMatrixListRow(node->l->mat,node->r->mat);
			break;
        default:
            break;
    }
    return;
}
void evaluateASTNodeSubMatrix(ASTNode *node){
	switch (node->subType) {
        case SubMatrixVectorExp:
			#ifdef DEBUGGING_ON
			printf("evaluate SubMatrixVectorExp!\n");
            #endif			
			node->l->mat = createBracketedMatrix(node->l->start,node->l->step,node->l->end);
			node->mat = cloneMatrix(node->l->mat);
			break;        
		case SubMatrixID:
			node->mat = callFunc(node->l->identifier, node->r);
			break;
		case SubMatrixNumber:
			node->mat = createSingletonMatrix(node->l->scalarValue);
			break;
        default:
            break;
    }
    return;
}



void evaluateASTNodeArgList(ASTNode *node){
    return;
}
void evaluateASTNodeArg(ASTNode *node){
	MatList *p;
    switch (node->subType) {
        case ArgNumber:
            node->mat = createSingletonMatrix(node->l->scalarValue);
			break;
		case ArgID:
			strcpy(node->identifier, node->l->identifier);
			p = outMatList(node->identifier);
			if (strcmp(p->identifier,node->identifier)==0){
				node->mat = cloneMatrix(p->mat);}
			get_submatrix = 1;
			break;	
		case ArgVectorExp:
			node->l->mat = createBracketedMatrix(node->l->start,node->l->step,node->l->end);
			node->mat = cloneMatrix(node->l->mat);
			get_submatrix = 1;
			break;
		case ArgColon:
			node->mat = createEyeMatrix(0);
			//changeOneElementOfMatrix(node->mat,1,1,COLONLABEL);/*COLONLABEL = 0 */
			break;
        default:
            break;
    }
    return;
}
void evaluateASTNodeFuncExp(ASTNode *node){
    node->mat = callFunc(node->l->identifier, node->r);
    return;
}


int getNumberOfArgs(ASTNode *node){
    assert(node->type == ArgList);
    ASTNode *tmp;
    int argc;
    
    tmp = node;
    argc = 0;
    while ((tmp != NULL) && (tmp->subType==ArgListMultiple) ) {
        argc = argc + 1;
        tmp = tmp->l;
    }
    argc = argc + 1;
    return argc;
}


ASTNode *getNthArgFromArgList(ASTNode *node, int N){
    int argc; /* number of args */
    int leftDepth;
    int i;
    ASTNode *tmp;
    assert(node->type == ArgList);

    argc = getNumberOfArgs(node);
    
    #ifdef DEBUGGING_ON
    printf("number of args: %d\n.", argc);
    #endif
    
    assert(N<=argc);
    
    leftDepth = argc-N;
    
    
    for (tmp = node, i = 0; i < leftDepth; i++) {
        tmp = tmp->l;
    }
    
    assert( (tmp->subType == ArgListSingle) ||
           (tmp->subType == ArgListMultiple));
    
    if (tmp->subType == ArgListSingle) {
        return tmp->l;
    } else {
        return tmp->r;
    }
    
}