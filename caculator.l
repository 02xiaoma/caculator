
%option noyywrap nodefault yylineno

%{
#include "caculator.h"
#include "caculator.tab.h"
%}

EXP	([Ee][-+]?[0-9]+)

%%
"="	|
"+" |
"-" |
"*" |
"/" |
"|" |
"(" |
")"     {return yytext[0]; }

[0-9]+"."[0-9]*{EXP}? |
"."?[0-9]+{EXP}? {yylval.a = (ast_t *)new_num(atof(yytext)); return NUMBER;}

"define"	{ return DEFINE; }

([a-z]|[A-Z])([a-z]|[A-Z]|[0-9]|_)*	{yylval.s = strdup(yytext); return SYMBOL;}

\n    { return EOL; }

"//".*\n  
[ \t]   { /* ignore white space */ }

.	{ yyerror("Mystery character %c\n", *yytext); }

%%
