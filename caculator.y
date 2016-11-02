/* 我自己写的计算器 */

%{
#include <stdio.h>
#include <stdlib.h>
#include "caculator.h"
%}

%union {
	expression_t *e;
	expression_list_t *l;/*可以去掉么？*/
}

/* 声明用到的符号 */
%token<e> NUMBER
%token<e> SYMBOL
%token DEFINE IF END ELSE WHILE EQUAL NOT BIGGER_THAN SMALL_THAN NOT_BIGGER_THAN NOT_SMALL_THAN NOT_EQUAL PRINT

%type<e> EXP FACTOR TERM ASSIGNMENT_EXP EXP_WITHOUT_ASSIGN_AND_PRINT COMPARE_EXP PRINT_EXP EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE
%type<l> EXPLIST

%%
caculator: 
 | caculator DEFINITION_EXP
 | caculator IF_EXP
 | caculator WHILE_EXP
 | caculator EXP ';' {
	eval($2);
	printf("> ");
  }
 | caculator ';' {
	printf("> ");
  }
 ;
 
IF_EXP: IF '(' EXP ')' EXPLIST END{
	int _t = eval($3);
	if (_t)
	{
		eval_list($5);
	}
	free_expression($3);
	free_expression_list($5);
	printf("> ");
 }
 | IF '(' EXP ')' EXPLIST ELSE EXPLIST END{
	int _t = eval($3);
	if (_t)
	{
		eval_list($5);
	}
	else
	{
		eval_list($7);
	}
	free_expression($3);
	free_expression_list($5);
	free_expression_list($7);
	printf("> ");
 }
 ;
WHILE_EXP: WHILE '(' EXP ')' EXPLIST END{
	while (eval($3))
	{
		eval_list($5);
	}
	free_expression($3);
	free_expression_list($5);
	printf("> ");
 }
 ;
 
EXPLIST: EXP ';'{
	$$ = new_expression_list($1);
 }
 | EXPLIST EXP ';'{
	add_new_expression($1, $2);
	$$ = $1;
 }
 ;
 
DEFINITION_EXP: DEFINE SYMBOL '=' EXP ';'{
	add_symbol($2, eval($4));
	free_expression($4);
	printf("> ");
 }
 | DEFINE SYMBOL ';'{
	add_symbol($2, 0);
	printf("> ");
 }
 ;

ASSIGNMENT_EXP: SYMBOL '=' EXP{
	$$ = new_expression('=', $1, $3);
 }
 ;
 
COMPARE_EXP: EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE EQUAL EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('E', $1, $3);
 }
 | NOT EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('N', $2, NULL);
 }
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE BIGGER_THAN EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('B', $1, $3);
 }
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE SMALL_THAN EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('S', $1, $3);
 }
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE NOT_BIGGER_THAN EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('b', $1, $3);
 }
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE NOT_SMALL_THAN EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('s', $1, $3);
 }
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE NOT_EQUAL EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE{
	$$ = new_expression('e', $1, $3);
 }
 ;
 
PRINT_EXP: PRINT EXP_WITHOUT_ASSIGN_AND_PRINT{
	$$ = new_expression('p', $2, NULL);
 }
 ;
 
EXP_WITHOUT_ASSIGN :PRINT_EXP
 | EXP_WITHOUT_ASSIGN_AND_PRINT
 ;

EXP: ASSIGNMENT_EXP
 | EXP_WITHOUT_ASSIGN
 ;

EXP_WITHOUT_ASSIGN_AND_PRINT: COMPARE_EXP
 |EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE
 ;
 
EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE: FACTOR
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE '+' FACTOR {$$ = new_expression('+', $1, $3);}
 | EXP_WITHOUT_ASSIGN_AND_PRINT_AND_COMPARE '-' FACTOR {$$ = new_expression('-', $1, $3);}
 ;
 
FACTOR: TERM
 | FACTOR '*' TERM {$$ = new_expression('*', $1, $3);}
 | FACTOR '/' TERM {$$ = new_expression('/', $1, $3);}
 ;
 
TERM: NUMBER
 | SYMBOL
 | '|' EXP_WITHOUT_ASSIGN_AND_PRINT '|' {$$ = new_expression('|', $2, NULL);}
 | '(' EXP_WITHOUT_ASSIGN_AND_PRINT ')' {$$ = $2;}
 | '-' EXP_WITHOUT_ASSIGN_AND_PRINT {$$ = new_expression('-', $2, NULL);}
 ;

%%
