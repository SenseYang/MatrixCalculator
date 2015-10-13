//
//  functions.h
//  Matrix Calculator
//
//  Created by Sen on 13-4-22.


#ifndef Matrix_Calculator_functions_h
#define Matrix_Calculator_functions_h
#include "Matrix.h"
#include "ASTNode.h"

Matrix callFunc(const char *funcName, ASTNode *argListNode);

#endif
