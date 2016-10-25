#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include "caculator.h"

extern int yylineno; /* from lexer */
extern FILE * yyin;
symbol_t *symbol_table = NULL;

symbol_t *find_symbol(const char *name)
{
	// 从前往后扫描
	symbol_t *p_s = symbol_table;
	while(p_s)
	{
		if (strcmp(name, p_s->name)==0)
		{
			return p_s;
		}
		p_s = p_s->next;
	}
	return NULL;
}

void add_symbol(char *name, double val)
{
	// 往头部插入
	symbol_t *p_s = (symbol_t *)malloc(sizeof(symbol_t));
	if (!p_s)
	{
		yyerror("out of space");
		exit(0);
	}
	p_s->name = name;
	p_s->val = val;
	p_s->next = symbol_table;
	symbol_table = p_s;
}

void free_ast(const ast_t *p_ast)
{
	if (!p_ast)
	{
		return;
	}
	if (p_ast->type == number_e)
	{
		free((num_t*)p_ast);
	}
	else
	{
		if (p_ast->left)
		{
			free_ast(p_ast->left);
		}
		if (p_ast->right)
		{
			free_ast(p_ast->right);
		}
		free(p_ast);
	}
}

double eval(const ast_t *p_ast)
{
	if (p_ast == NULL)
	{
		return 0;
	}
	else if (p_ast->type == number_e)
	{
		return ( (num_t*)(p_ast) ) ->d;
	}
	else
	{
		double l = eval(p_ast->left);
		double r = eval(p_ast->right);
		switch (p_ast->type)
		{
			case add_e:
				l += r;
				break;
			case sub_e:
				l -= r;
				break;
			case mul_e:
				l *= r;
				break;
			case div_e:
				l /= r;
				break;
			case abs_e:
				l = l>0 ? l : -l;
				break;
			case minus_e:
				l = -l;
				break;
		}
		return l;
	}
}

ast_t *new_ast(const char ch, ast_t *l, ast_t *r)
{
	ast_t *p_ast = NULL;
	switch (ch)
	{
		case '+':
			p_ast = (ast_t *)malloc(sizeof(ast_t));
			if (!p_ast)
			{
				yyerror("out of space");
				exit(0);
			}
			p_ast->type = add_e;
			p_ast->left = l;
			p_ast->right = r;
			break;
		case '-':
			if (r)
			{
				p_ast = (ast_t *)malloc(sizeof(ast_t));
				if (!p_ast)
				{
					yyerror("out of space");
					exit(0);
				}
				p_ast->type = sub_e;
				p_ast->left = l;
				p_ast->right = r;
				break;
			}
			else
			{
				p_ast = (ast_t *)malloc(sizeof(ast_t));
				if (!p_ast)
				{
					yyerror("out of space");
					exit(0);
				}
				p_ast->type = minus_e;
				p_ast->left = l;
				p_ast->right = r;
				break;
			}
			
		case '*':
			p_ast = (ast_t *)malloc(sizeof(ast_t));
			if (!p_ast)
			{
				yyerror("out of space");
				exit(0);
			}
			p_ast->type = mul_e;
			p_ast->left = l;
			p_ast->right = r;
			break;
		case '/':
			p_ast = (ast_t *)malloc(sizeof(ast_t));
			if (!p_ast)
			{
				yyerror("out of space");
				exit(0);
			}
			p_ast->type = div_e;
			p_ast->left = l;
			p_ast->right = r;
			break;
		case '|':
			p_ast = (ast_t *)malloc(sizeof(ast_t));
			if (!p_ast)
			{
				yyerror("out of space");
				exit(0);
			}
			p_ast->type = abs_e;
			p_ast->left = l;
			p_ast->right = r;
			break;
	}
	return p_ast;
}

num_t *new_num(const double _d)
{
	num_t *p_num = (num_t *)malloc(sizeof(num_t));
	p_num->type = number_e;
	p_num->d = _d;
	
	return p_num;
}

void yyerror(char *s, ...)
{
  va_list ap;
  va_start(ap, s);

  fprintf(stderr, "%d: error: ", yylineno);
  vfprintf(stderr, s, ap);
  fprintf(stderr, "\n");
}

int main(int argc, char **argv)
{
	printf("> ");
	if (argc == 2)
	{
		FILE *fp = fopen(argv[1], "r");
		yyin = fp;
	}
	return yyparse();
}