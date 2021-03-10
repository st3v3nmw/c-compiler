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
    : '\n'? 'for' ' '? '(' expression? ';' expression? ';' expression? ')' ' '? '{' '\n'? (statement | loop_flow_control)* '\n'? '}' '\n'?;

while_loop
    : '\n'? 'while' ' '? '(' expression ')' ' '? '{' '\n'? (statement | loop_flow_control)* '\n'? '}' '\n'?;

loop_flow_control
    : 'break;' '\n'?
    | 'continue;' '\n'?;

declaration
    : type_specifier ' '? IDENTIFIER ';' '\n'?;

assign
    : type_specifier? ' '? IDENTIFIER '=' ' '? expression ';' '\n'?;

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
    : type_specifier ' '? IDENTIFIER '(' parameter_list ')';
parameter
    : type_specifier IDENTIFIER;
parameter_list
    : parameter
    | parameter_list ',' ' '? parameter;
function_body
    : '{' '\n'? statement* ('return' ' '? expression)? ';' '\n'? '}' '\n'?;

if_statement
    : '\n'? 'if' ' '? '(' expression ')' ' '? '{' '\n'? statement* '}' '\n'? else_if* else_bit?;
else_if
    : 'else' ' '? 'if' ' '? '(' expression ')' ' '? '{' '\n'? statement* '\n'? '}' '\n'?;
else_bit
    : 'else' ' '? '{' '\n'? statement* '\n'? '}' '\n'?;

IDENTIFIER
    : [A-Za-z_][0-9A-Za-z_]*;

INTEGER
    : [+-]?[0-9]+;

FLOAT
    : [+-]?[0-9]+'.'[0-9]+;

STRING
    : '"' [ 0-z]* '"';

WHITESPACE
    : ' ' -> skip;