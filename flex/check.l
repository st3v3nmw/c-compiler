%{
int total =0 ;
%}

%option noyywrap

%%

#.* {total++; fprintf(yyout,"Directive %s\n",yytext);}
[''|,|;(|)|{|}|.|_] {total++; fprintf(yyout,"Delimiter %s\n",yytext);}
[[]] {total++; fprintf(yyout,"Delimiter %s\n",yytext);}
"#"|"@"|"$"|"^"|"%"|"^"|"&" {total++; fprintf(yyout,"S_char %s\n",yytext);}
"["|"]" {total++; fprintf(yyout,"Delimiter %s\n",yytext);}
"="  {total++; fprintf(yyout,"Assign_op %s\n",yytext);}
"+"|"-"|"*"|"/"  {total++; fprintf(yyout,"Arith_op %s\n",yytext);}
"&&"|"||"|"!"  {total++; fprintf(yyout,"Logic_op %s\n",yytext);}
"<="|">="|"++"|"!="|"=="|"<"|">"  {total++; fprintf(yyout,"Relation_op %s\n",yytext);}
("int")|("if")|("else")|("while")|("do")|("break")|("continue")|("double")|("float")|("return")|("EOF") {total++; fprintf(yyout,"Keyword %s\n",yytext);}
("return")|("char")|("case")|("sizeof")|("long")|("short")|("typedef")|("switch")|("unsigned")|("void")|("static")|("struct")|("goto") {total++;fprintf(yyout,"Keyword %s\n",yytext);}
[a-zA-Z_][a-zA-Z0-9_]* {total++; fprintf(yyout,"Identifier %s\n",yytext);}
[0-9]*"."[0-9]+ {total++;fprintf(yyout,"Float %s\n", yytext);}
[-][0-9]*"."[0-9]+ {total++;fprintf(yyout,"Neg_Float %s\n", yytext);}
[0-9]+ {total++; fprintf(yyout,"Integer %s\n",yytext);}
"-"[0-9]+ {total++; fprintf(yyout,"Neg_Integer %s\n",yytext);}
["]([^"\\\n]|\\.|\\\n)*["] {total++; fprintf(yyout,"String %s\n",yytext);}
"//".*" "*    {total++;fprintf(yyout,"Comments  %s\n",yytext);}
\/\*(.*\n)*.*\*\/    {total++;fprintf(yyout,"Comment %s\n",yytext);}
. {fprintf(yyout,"",yytext);}
[\t\n]+

%%

int main()
{
extern FILE *yyin, *yyout; 
    yyin = fopen("../flex/input.txt", "r"); 
    yyout = fopen("Output.txt", "w"); 
    yylex();

    return 0; 
}