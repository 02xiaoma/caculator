all:	caculator.l caculator.y caculator.h caculator.c
	bison -d caculator.y
	flex -ocaculator.lex.c caculator.l
	cc -o caculator caculator.tab.c caculator.lex.c caculator_funcs.c