#ifndef _CACULATOR_H
#define _CACULATOR_H

enum types_enum
{
	number_e,	//只有一个数字
	add_e,	//加
	sub_e,	//减
	mul_e,	//乘
	div_e,	//除
	abs_e,	//绝对值
	minus_e	//负值
};

// 定义用到的数据结构
typedef struct _ast
{
	enum types_enum type;	//指明是什么类型
	struct _ast *left;
	struct _ast *right;
} ast_t;

typedef struct _num
{
	enum types_enum type;	//指明是什么类型
	double d;
} num_t;

typedef struct _symbol		//符号，用链表的形式连起来
{
	const char *name;
	double val;
	struct _symbol *next;
} symbol_t;

symbol_t *find_symbol(const char *name);	//根据一个名字查找符号，如果不存在则返回NULL
void add_symbol(char *name, double val);	//对应定义的情况，符号表中加入一项
extern symbol_t *symbol_table;	//全局变量，符号表

void free_ast(const ast_t *ast);
double eval(const ast_t *ast);

ast_t *new_ast(const char ch, ast_t *l, ast_t *r);
num_t *new_num(const double d);
void yyerror(char *s, ...);
#endif	// _CACULATOR_H