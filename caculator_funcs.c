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

void add_symbol(var_t *p_var, double val)
{
	// 往头部插入
	symbol_t *p_s = (symbol_t *)malloc(sizeof(symbol_t));
	if (!p_s)
	{
		yyerror("out of space");
		exit(0);
	}
    p_s->name = p_var->name;
	p_s->val = val;
	p_s->next = symbol_table;
	symbol_table = p_s;
}

void free_expression(const expression_t *p_expression)
{
	if (!p_expression)
	{
		return;
	}
	if (p_expression->type == number_e)
	{
		free((num_t*)p_expression);
	}
    else if (p_expression->type == sym_e)
    {
		// never free a symbol(varient)
        return;
    }
	else
	{
		if (p_expression->left)
		{
			free_expression(p_expression->left);
		}
		if (p_expression->right)
		{
			free_expression(p_expression->right);
		}
		free(p_expression);
	}
}

void free_expression_list(expression_list_t *p_expression_list)
{
	if (p_expression_list->type != exp_list_e)
	{
		yyerror("can't free a expression list");
		exit(0);
	}
	expression_list_node_t *p_node = p_expression_list->begin;
	while (p_node)
	{
		expression_list_node_t *p_t = p_node->next;
		free_expression(p_node->expression);
		free(p_node);
		p_node = p_t;
	}
	free(p_expression_list);
}

double eval(const expression_t *p_expression)
{
	if (p_expression == NULL)
	{
		return 0;
	}
    else if (p_expression->type == number_e)
	{
		return ( (num_t*)(p_expression) ) ->d;
	}
	else if (p_expression->type == sym_e)
	{
		symbol_t *p_symbol = find_symbol(((var_t*)p_expression)->name);
		if (!p_symbol)
		{
			yyerror("can't find symbol %s\n", ((var_t*)p_expression)->name);
			exit(0);
		}
		return p_symbol->val;
	}
	else if (p_expression->type == assign_exp_e)
	{
		symbol_t *p_symbol = find_symbol(((var_t*)(p_expression->left))->name);
		p_symbol->val = eval((const expression_t *)(p_expression->right));
		return 0;
	}
	else
	{
		double l = eval(p_expression->left);
		double r = eval(p_expression->right);
		switch (p_expression->type)
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
			case print_e:
				printf(" = %4.4g\n", l);
				break;
			case E_exp_e:
				l = ((l==r) ? 1 : 0);
				break;
			case N_exp_e:
				l = !l;
				break;
			case B_exp_e:
				l = l>r;
				break;
			case S_exp_e:
				l = l<r;
				break;
			case nb_exp_e:
				l = l<=r;
				break;
			case ns_exp_e:
				l = l>=r;
				break;
			case ne_exp_e:
				l = l!=r;
				break;
			default:
				break;
		}
		return l;
	}
}

void _do_eval_list(expression_list_node_t *p_expression_list_node)
{
	if (!p_expression_list_node)
	{
		return;
	}
	if (p_expression_list_node->next)
	{
		_do_eval_list(p_expression_list_node->next);
	}
    eval(p_expression_list_node->expression);
}
void eval_list(expression_list_t *p_expression_list)
{
	if (p_expression_list && (p_expression_list->type == exp_list_e))
	{
		_do_eval_list(p_expression_list->begin);
	}
}

expression_t *new_expression(const char ch, expression_t *l, expression_t *r)
{
	expression_t *p_expression = NULL;
	switch (ch)
	{
		case '+':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = add_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case '-':
			if (r)
			{
				p_expression = (expression_t *)malloc(sizeof(expression_t));
				if (!p_expression)
				{
					yyerror("out of space");
					exit(0);
				}
				p_expression->type = sub_e;
				p_expression->left = l;
				p_expression->right = r;
				break;
			}
			else
			{
				p_expression = (expression_t *)malloc(sizeof(expression_t));
				if (!p_expression)
				{
					yyerror("out of space");
					exit(0);
				}
				p_expression->type = minus_e;
				p_expression->left = l;
				p_expression->right = r;
				break;
			}
			
		case '*':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = mul_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case '/':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = div_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case '|':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = abs_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case '=':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = assign_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'E':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = E_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'N':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = N_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'B':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = B_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'S':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = S_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'b':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = nb_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 's':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = ns_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'e':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = ne_exp_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		case 'p':
			p_expression = (expression_t *)malloc(sizeof(expression_t));
			if (!p_expression)
			{
				yyerror("out of space");
				exit(0);
			}
			p_expression->type = print_e;
			p_expression->left = l;
			p_expression->right = r;
			break;
		default:
			break;
	}
	return p_expression;
}

num_t *new_num(const double _d)
{
	num_t *p_num = (num_t *)malloc(sizeof(num_t));
	p_num->type = number_e;
	p_num->d = _d;
	
	return p_num;
}

expression_list_t *new_expression_list(expression_t *p_expression)
{
	expression_list_t *p_expression_list = (expression_list_t *)malloc(sizeof(expression_list_t));
	if (!p_expression_list)
	{
		yyerror("out of space");
		exit(0);
	}
	p_expression_list->type = exp_list_e;
	p_expression_list->begin = (expression_list_node_t*)malloc(sizeof(expression_list_node_t));
	if (!p_expression_list->begin)
	{
		yyerror("out of space");
		exit(0);
	}
	p_expression_list->begin->next = NULL;
	p_expression_list->begin->expression = p_expression;
	
	return p_expression_list;
}

void add_new_expression(expression_list_t *p_expression_list, expression_t *p_expression)
{
	expression_list_node_t *p_new_node = (expression_list_node_t*)malloc(sizeof(expression_list_node_t));
	if (!p_new_node)
	{
		yyerror("out of space");
		exit(0);
	}
	p_new_node->next = p_expression_list->begin;
	p_new_node->expression = p_expression;
	p_expression_list->begin = p_new_node;
}

var_t *new_var(const char* s)
{
	var_t *p_var = (var_t *)malloc(sizeof(var_t));
	if (!p_var)
	{
		yyerror("out of space");
		exit(0);
	}
	p_var->type = sym_e;
	p_var->name = strdup(s);
	if ( !(p_var->name) )
	{
		yyerror("out of space");
		exit(0);
	}
	
	return p_var;
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
