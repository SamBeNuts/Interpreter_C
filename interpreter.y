%code requires {
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
}

%{

  #include <stdio.h>
  #include <stdlib.h>
  #include <string.h>

  int yylex(void);
  void yyerror (char const *s) {
      fprintf (stderr, "%s\n", s);
  }

  extern FILE *yyin;

%}

%union {
  int Integer;
  char Registre[3];
};

%token t_cop t_afc t_pri t_add t_sou t_mul t_div
%token <Integer> t_num
%token <Registre> t_reg

%start File

%%

File:
    /* Vide */
  | Instructions
  ;

Instructions:
    /* Vide */
  | Instruction Instructions
  ;

Instruction:
    Operateur t_reg t_reg t_reg
  | t_cop t_reg t_reg
  | t_afc t_reg t_num
  | t_pri t_reg
  ;

Operateur:
    t_add 
  | t_sou
  | t_mul
  | t_div
  ;



%%

int main(int argc, char *argv[]) {
  yyin = fopen(argv[1], "r");
  yyparse();
  return 0;
}
