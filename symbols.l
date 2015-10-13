/* token scanner of Matrix Calculator */
/* Sen, 04/20/2013 */



%{
/* change YYSTYPE */

#include "ASTNode.h"
#define YYSTYPE ASTNode*
#include "expressions.tab.h"
#include "Global.h"
#include <stdlib.h>
#include <stdio.h>


%}


%option noyywrap

digit       [0-9]
/* floating number. remember, there should be space between +/- and operands */
number      ("+"|"-")?{digit}+("."{digit}+)?
letter      [a-zA-Z]
identifier  {letter}({letter}|{digit})*
newline     \n
whitespace  [ \t]


%%


"+"     {return PLUS;}
"-"     {return MINUS;}
"*"     {return TIMES;}
"/"     {return DIVIDE;}
"%"     {return MOD;}
"^"     {return POWER;}
"'"     {return TURN;}


"="     {return ASSIGN;}

"("     {return LPAREN;}
")"     {return RPAREN;}
"["     {return LBRACKET;}
"]"     {return RBRACKET;}
","     {return COMMA;}
":"     {
    yylval = createASTNode(Colon,DefaultSubType);
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode(yylval);
    #endif
    yylval->evaluated = 1;
    return COLON;
}
";"     {return ROWSEP;}


{number}        {
    yylval = createASTNode(Num,DefaultSubType);
    yylval->scalarValue = strtod(yytext,NULL);
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode(yylval);
    #endif
    
    yylval->evaluated = 1;
    
    return NUMBER;
}

{identifier}    {
    yylval = createASTNode(Identifier,DefaultSubType);
    strcpy(yylval->identifier, yytext);
    
    #ifdef DEBUGGING_ON
    displayInternalInfoOfASTNode(yylval);
    #endif
    
    yylval->evaluated = 1;
    
    return ID;
}

{newline}       {if (!inBracket) {
                    #ifdef DEBUGGING_ON
                    printf("EOL\n");  
                    #endif
                    return EOL; 
                } else {
                    #ifdef DEBUGGING_ON
                    printf("ROWSEP\n");  
                    #endif
                    return ROWSEP;
                }
                } /* end of an expression, needs refinement */
                
                
{whitespace}    {/*ignore white space*/}


.               {
                #ifdef DEBUGGING_ON
                printf("ERROR!!!\n");
                #endif
                return ERROR;}

%%
