/* 我自己写的计算器 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "caculator.h"
%}

%union {
	ast_t *a;
	char *s;
}

/* 声明用到的符号 */
%token<a> NUMBER
%token<s> SYMBOL
%token EOL DEFINE

%type<a> EXP FACTOR TERM

%%
caculator: 
 | caculator DEFINE SYMBOL '=' EXP EOL{
	add_symbol($3, eval($5));
	free_ast($5);
	printf("> ");
  }
 | caculator SYMBOL '=' EXP EOL{
	symbol_t *p_s = find_symbol($2);
	if (!p_s)
	{
		yyerror("the symbol %s is not found\n", $2);
		exit(0);
	}
	p_s->val = eval($4);
	free_ast($4);
	printf("> ");
  }
 | caculator EXP EOL {
	printf("= %4.4g\n", eval($2));
	free_ast($2); 
	printf("> ");
  }
 | caculator EOL {
	printf("> ");
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
 | SYMBOL {$$ = (ast_t*)new_num(find_symbol($1)->val);}
 | '|' EXP '|' {$$ = new_ast('|', $2, NULL);}
 | '(' EXP ')' {$$ = $2;}
 | '-' EXP {$$ = new_ast('-', $2, NULL);}
 ;

%%
