
%option noyywrap nodefault yylineno

%{
#include "caculator.h"
#include "caculator.tab.h"
%}

EXP	([Ee][-+]?[0-9]+)

%%
";" |
"="	|
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")"     {return yytext[0]; }

"define"	{ return DEFINE; }
"if" 		{ return IF; }
"else"		{ return ELSE; }
"end"		{ return END; }
"while"		{ return WHILE; }
"=="		{ return EQUAL; }
"!="		{ return NOT_EQUAL; }
"!"			{ return NOT; }
">"			{ return BIGGER_THAN; }
"<"			{ return SMALL_THAN; }
"<="		{ return NOT_BIGGER_THAN; }
">="		{ return NOT_SMALL_THAN; }
"print"		{ return PRINT; }


[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {yylval.e = (expression_t *)new_num(atof(yytext)); return NUMBER;}



([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|_)*	{yylval.e = new_var(yytext); return SYMBOL;}

\n    { /*return EOL;*/ }

"//".*\n  
[ \t]   { /* ignore white space */ }

.	{ yyerror("Mystery character %c\n", *yytext); }

%%
