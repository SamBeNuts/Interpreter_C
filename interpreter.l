%{
  #include "y.tab.h"
%}

%option noyywrap

spaces [ \t\n]+

%%

{spaces} ;

"COP" return t_cop;
"AFC" return t_afc;
"PRI" return t_pri;
"ADD" return t_add;
"SOU" return t_sou;
"MUL" return t_mul;
"DIV" return t_div;
[R][0-9]{1,2} {
  strcpy(yylval.Registre, yytext);
  return t_reg;
}
[0-9]+ {
  yylval.Integer=atoi(yytext);
  return t_num;
}

%%
