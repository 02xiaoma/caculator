/* 我自己写的计算器 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "caculator.h"
%}

%union {
	ast_t *a;
}

/* 声明用到的符号 */
%token<a> NUMBER
%token EOL

%type<a> EXP FACTOR TERM

%%
caculator: 
 | caculator EXP EOL {
	printf("= %4.4g\n", eval($2));
	free_ast($2); 
	printf("> ");
  }
 | caculator EOL {
	printf("\n> ");
  }
 ;

EXP: FACTOR
 | EXP '+' FACTOR {$$ = new_ast('+', $1, $3);}
 | EXP '-' FACTOR {$$ = new_ast('-', $1, $3);}
 ;
 
FACTOR: TERM
 | FACTOR '*' TERM {$$ = new_ast('*', $1, $3);}
 | FACTOR '/' TERM {$$ = new_ast('/', $1, $3);}
 ;
 
TERM: NUMBER
 | '|' EXP '|' {$$ = new_ast('|', $2, NULL);}
 | '(' EXP ')' {$$ = $2;}
 | '-' EXP {$$ = new_ast('-', $2, NULL);}
 ;

%%
