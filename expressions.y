/* expressions used in Matrix Calculator */
/* Sen, 04/20/2013 */

%{
    #include <stdio.h>
    #include "Global.h"
    #include "ASTNode.h"
    /* change YYSTYPE */
    #define YYSTYPE ASTNode *
    
    int yyerror(char *s);
    int yylex(void);
    int inBracket; /* detect if now we're inputing a bracketed matrix */
    %}




/* tokens */

%token PLUS MINUS TIMES DIVIDE MOD POWER TURN
%token LPAREN RPAREN COMMA COLON LBRACKET RBRACKET
%token NUMBER ID

%token ASSIGN

%token ROWSEP

%token EOL

%token ERROR

%%

/* the language of Matrix Calculator */

calclist: /* empty */
| calclist exp EOL {
    #ifdef DEBUGGING_ON
    printf("an expression has just been stored.\n");
    displayInternalInfoOfASTNode($2);
    #endif
    evaluateASTNode($2);
//    printf("travsersing begins...\n");
//
//    traverseASTNode($2);
//    printf("travsersing done...\n");
    #ifdef DEBUGGING_ON
    printf("result of the expression:\n");
    displayMatrix($2->mat);
    #endif
    deleteASTNode($2);

	
} /* needing to evaluate exp */
| calclist EOL { printf("doneempty> ");} /* one more newline, another prompt */
;



exp : ID ASSIGN simpleExp {
    $$ = createASTNode(Expression,ExpSimpleAssign);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| ID LPAREN argList RPAREN ASSIGN simpleExp {
    $$ = createASTNode(Expression,ExpPartAssign);
    $$->l = $1;
    $$->r = $3;
    $$->a = $6;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| simpleExp {
    $$ = createASTNode(Expression,ExpSimpleExp);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};


simpleExp: term {
    $$ = createASTNode(SimpleExp,SimpleExpTerm);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| simpleExp PLUS term {
    $$ = createASTNode(SimpleExp,SimpleExpPlus);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| simpleExp MINUS term{
    $$ = createASTNode(SimpleExp,SimpleExpMinus);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| simpleExp POWER term {
    $$ = createASTNode(SimpleExp,SimpleExpPower);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};


term: factor {
    $$ = createASTNode(Term,TermFactor);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}

| term TIMES factor {
    $$ = createASTNode(Term,TermTimes);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| term DIVIDE factor {
    $$ = createASTNode(Term,TermDivide);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| term TURN{
    $$ = createASTNode(Term,TermTurn);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}

| term MOD factor {
    $$ = createASTNode(Term,TermMod);
    $$->l = $1;
    $$->r = $3;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};


factor: LPAREN simpleExp RPAREN {
    $$ = createASTNode(Factor,FactorParen);
    $$->l = $2;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| ID {
    $$ = createASTNode(Factor,FactorID);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| funcExp {
    $$ = createASTNode(Factor,FactorFuncExp);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| NUMBER {
    $$ = createASTNode(Factor,FactorNumber);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| vectorExp {
    $$ = createASTNode(Factor,FactorVectorExp);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| bracketedMatrix {
    $$ = createASTNode(Factor,FactorBracketedMatrix);
    $$->l = $1;
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};


bracketedMatrix: LBRACKET {inBracket = 1; /* embedded action counts as a $x! */} subMatrixList RBRACKET {
    inBracket = 0;
    $$ = createASTNode(BracketedMatrix,DefaultSubType);
    $$->l = $3;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};

subMatrixList: subMatrixListRow {
    $$ = createASTNode(SubMatrixList,SubMatrixListSingle);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| subMatrixList ROWSEP subMatrixListRow {
    $$ = createASTNode(SubMatrixList,SubMatrixListMultiple);
    $$->l = $1;
    $$->l = $3;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};

subMatrixListRow: subMatrix {
    $$ = createASTNode(SubMatrixListRow,SubMatrixListRowSingle);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| subMatrixListRow COMMA subMatrix {
    $$ = createASTNode(SubMatrixListRow,SubMatrixListRowMultiple);
    $$->l = $1;
    $$->r = $3;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};
| subMatrixListRow subMatrix {
    $$ = createASTNode(SubMatrixListRow,SubMatrixListRowMultiple);
    $$->l = $1;
    $$->r = $2;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};


subMatrix: funcExp {
    $$ = createASTNode(SubMatrix,SubMatrixID);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| ID {
    $$ = createASTNode(SubMatrix,SubMatrixID);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| vectorExp {
    $$ = createASTNode(SubMatrix,SubMatrixVectorExp);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}

| NUMBER    {
    $$ = createASTNode(SubMatrix,SubMatrixNumber);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};

vectorExp: NUMBER COLON NUMBER {
    $$ = createASTNode(VectorExp,DefaultSubType);
    $$->start = $1->scalarValue;
    $$->end = $3->scalarValue;
    $$->step = 1;
    
    deleteASTNode($1);
    deleteASTNode($2);
    deleteASTNode($3);
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
    
    $$->evaluated = 1;
    
};
|
NUMBER COLON NUMBER COLON NUMBER {
    $$ = createASTNode(VectorExp,DefaultSubType);
    $$->start = $1->scalarValue;
    $$->end = $5->scalarValue;
    $$->step = $3->scalarValue;
    
    deleteASTNode($1);
    deleteASTNode($2);
    deleteASTNode($3);
    deleteASTNode($4);
    deleteASTNode($5);
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
    
    $$->evaluated = 1;
};

funcExp: ID LPAREN argList RPAREN {
    $$ = createASTNode(FuncExp,DefaultSubType);
    $$->l = $1;
    $$->r = $3;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};

argList: arg {
    $$ = createASTNode(ArgList,ArgListSingle);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}
| argList COMMA arg {
    $$ = createASTNode(ArgList,ArgListMultiple);
    $$->l = $1;
    $$->r = $3;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};



arg: NUMBER {
    $$ = createASTNode(Arg,ArgNumber);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};
| vectorExp {
    $$ = createASTNode(Arg,ArgVectorExp);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}

| ID {
    $$ = createASTNode(Arg,ArgID);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
}

| COLON {
    $$ = createASTNode(Arg,ArgColon);
    $$->l = $1;
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode($$);
    #endif
};

%%

int yyerror(char *s)
{
    fprintf(stderr, "error: %s\n", s);
    return 0;
}