//
//  ASTNode.h
//  Matrix Calculator
//
//  Created by Sen on 13-4-21.


#ifndef Matrix_Calculator_ASTNode_h
#define Matrix_Calculator_ASTNode_h

#include "Global.h"
#include "Matrix.h"

enum NodeType {
    DefaultType, /* maybe useful for initialization */
    Expression, /* the type of one complete expression */
    
    SimpleExp,
    
    Term,
    Factor,
    
    SubMatrixList,
    SubMatrixListRow,
    SubMatrix,
    
    ArgList,
    Arg,
    
    FuncExp,
    
    Identifier,
    Num,
    VectorExp,
    Colon, /* a special type of vector exp */
    BracketedMatrix,
};

enum NodeSubType {
    /* used for those without type */
    DefaultSubType,
	
    /* 3 derivations of exp */
    ExpSimpleAssign, /* ID = xxx */
    ExpPartAssign, /* ID(X,Y) = xxx */
    ExpSimpleExp,
    /* 4 derivations of simpleExp */
    SimpleExpTerm,
    SimpleExpPlus,
    SimpleExpMinus,
	SimpleExpPower,
    /* 4 derivations of term */
    TermFactor,
    TermTimes,
    TermDivide,
    TermMod,
	TermTurn,
    /* 6 derivations of factor */
    FactorParen,
    FactorID,
    FactorFuncExp,
    FactorNumber,
    FactorVectorExp,
    FactorBracketedMatrix,
    
    /* 2 derivations of subMatrixList */
    SubMatrixListSingle,
    SubMatrixListMultiple,
    
    /* 2 derivations of subMatrixListRow (COMMA and space versions are combined) */
    SubMatrixListRowSingle,
    SubMatrixListRowMultiple,
    
    /* 4 derivations of subMatrix */
    SubMatrixFuncExp,
    SubMatrixID,
    SubMatrixVectorExp,
    SubMatrixNumber,
    
    /* 2 derivations of argList */
    ArgListSingle,
    ArgListMultiple,
    
    /* 4 derivations of arg */
    ArgNumber,
    ArgVectorExp,
    ArgID,
    ArgColon,
};

typedef enum NodeType NodeType;
typedef enum NodeSubType NodeSubType;




struct ASTNode {
    NodeType type; /* save type */
    NodeSubType subType;
    Matrix mat; /* for matrix */
    Number scalarValue; /* for number type */
    char identifier[IDMAXLENGTH+1];  /* save ID */
    /* for vector expression */
    double start;
    double end;
    double step;
    unsigned char evaluated; /* flag showing if this node has been evaluated */
    
    /* children */
    struct ASTNode *l;
    struct ASTNode *r;
    
    /* additional child*/
    struct ASTNode *a;
    
    /* special for ArgList*/
    //struct ASTNode *nextArg;
};

typedef struct ASTNode ASTNode;

/* functions for ASTNode*/

ASTNode *createASTNode(NodeType type, NodeSubType subType);
void deleteASTNode(ASTNode *node);
void evaluateASTNode(ASTNode *node);
void displayInternalInfoOfASTNode(ASTNode *node);
void traverseASTNode(ASTNode *node);
int getNumberOfArgs(ASTNode *node);
ASTNode *getNthArgFromArgList(ASTNode *node, int N);

#endif
