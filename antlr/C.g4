grammar C;

program:
    statement+;

statement
    : assign
    | declaration
    | function
    | loop
    | if_statement;

factor
    : IDENTIFIER
    | INTEGER
    | FLOAT
    | STRING;

type_specifier 
    : 'int'
    | 'float'
    | 'char'
    | 'void';

loop
    : for_loop
    | while_loop;

for_loop
    :  'for' '(' expression? ';' expression? ';' expression? ')' '{'  (statement | loop_flow_control)* '}' ;

while_loop
    :  'while' '(' expression ')' '{'  (statement | loop_flow_control)* '}' ;

loop_flow_control
    : 'break;' 
    | 'continue;' ;

declaration
    : type_specifier ' ' IDENTIFIER ';' ;

assign
    : type_specifier? ' ' IDENTIFIER  '='  expression ';' ;

expression
    : factor
    | arithmetic_expression
    | expression '<' expression
    | expression '<=' expression
    | expression '>' expression
    | expression '>=' expression
    | expression '!=' expression
    | expression '==' expression
    | expression '&&' expression
    | expression '||' expression
    | '!' expression;
arithmetic_expression
    : additive_expression;
additive_expression
    : additive_expression '+' multiplicative_expression
	| additive_expression '-' multiplicative_expression
    | multiplicative_expression;
multiplicative_expression
    : multiplicative_expression '*' factor
	| multiplicative_expression '/' factor
	| factor;

function
    : function_definition function_body;
function_definition
    : type_specifier ' ' IDENTIFIER '(' parameter_list ')';
parameter
    : type_specifier ' ' IDENTIFIER;
parameter_list
    : parameter
    | parameter_list ','  parameter;
function_body
    : '{' statement* ('return'  expression)? ';' '}' ;

if_statement
    :  'if'  '(' expression ')'  '{'  statement* '}'  else_if* else_bit?;
else_if
    : 'else'  'if'  '(' expression ')'  '{'  statement*  '}' ;
else_bit
    : 'else'  '{'  statement*  '}' ;

IDENTIFIER
    : [A-Za-z_][0-9A-Za-z_]*;

INTEGER
    : [+-]?[0-9]+;

FLOAT
    : [+-]?[0-9]+'.'[0-9]+;

STRING
    : '"' [0-9A_Za-z !#$%&'()+-,./:;<=>?\\@^{|}]* '"';

WHITESPACE
    : ' ' -> skip;