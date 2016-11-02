#ifndef _CACULATOR_H
#define _CACULATOR_H

enum expression_types_enum
{
	number_e,	//只有一个数字
	add_e,		//加
	sub_e,		//减
	mul_e,		//乘
	div_e,		//除
	abs_e,		//绝对值
	minus_e,	//负值
	print_e,	//打印
	exp_list_e,	//expression list
	sym_e,		//变量
	E_exp_e,	// ==
	N_exp_e,	// !
	B_exp_e,	// >
	S_exp_e,	// <
	nb_exp_e,	// <=
	ns_exp_e,	// >=
	ne_exp_e,	// !=
	assign_exp_e//assign
};

// 定义用到的数据结构
typedef struct _num
{
	enum expression_types_enum type;	//数字
	double d;
} num_t;

typedef struct _var
{
	enum expression_types_enum type;	//变量
	char *name;
} var_t;

typedef struct _expression
{
	enum expression_types_enum type;	//expression
	void *left;
	void *right;
} expression_t;

typedef struct _exp_list_node
{
	expression_t *expression;
	struct _exp_list_node *next;
} expression_list_node_t;

typedef struct _exp_list
{
	enum expression_types_enum type;	//expression list
	expression_list_node_t *begin;	//插入的时候直接插到头部，求值的时候逆着求值
} expression_list_t;

typedef struct _symbol		//符号，用链表的形式连起来
{
	const char *name;
	double val;
	struct _symbol *next;
} symbol_t;

symbol_t *find_symbol(const char *name);	//根据一个名字查找符号，如果不存在则返回NULL
void add_symbol(var_t *p_var, double val);	//对应定义的情况，符号表中加入一项
extern symbol_t *symbol_table;	//全局变量，符号表

void free_expression(const expression_t *p_expression);
void free_expression_list(expression_list_t *p_expression_list);
double eval(const expression_t *p_expression);
void eval_list(expression_list_t *p_expression_list);

expression_t *new_expression(const char ch, expression_t *l, expression_t *r);
expression_list_t *new_expression_list(expression_t *p_expression);
void add_new_expression(expression_list_t *p_expression_list, expression_t *p_expression);
num_t *new_num(const double d);
var_t *new_var(const char*);
void yyerror(char *s, ...);

#endif	// _CACULATOR_H