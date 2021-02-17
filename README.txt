C subset specification
======================

<statement> ::= <assign>
		| <declaration>
		| <function>
		| <loop>
		| <loop-flow-control>

<factor> ::= <identifier> | <integer> | <float>
<identifier> ::= [A-Za-z_][0-9A-Za-z_]*
<integer> ::= [+-]?[0-9]+
<float> ::= [+-]?[0-9]+.[0-9]+
<type-specifier> ::=	int | float | char | void

<loop> ::= <for-loop> | <while-loop>
<for-loop> ::= for (<expression>?; <expression>?; <expression>?) { <statement> }
<while-loop> ::= while (<expression>) { <statement> }
<loop-flow-control> ::= break ;
			| continue ;

<declaration> ::= <type-specifier> <identifier>;
<assign> ::= <type-specifier>? <identifier> = <expression>

<expression> ::= <factor>
		| <arithmetic-expression>
		| <relational-expression>

<arithmetic-expression> ::= <additive-expression>
<additive-expression> ::= <additive-expression> + <multiplicative-expression>
			    | <additive-expression> - <multiplicative-expression>
			    | <multiplicative-expression>
<multiplicative-expression> ::= <multiplicative-expression> * <factor>
				| <multiplicative-expression> / <factor>
				| <factor>

<relational-expression> ::=	 <expression> < <expression>
 				| <expression> <= <expression>
				| <expression> > <expression>
				| <expression> >= <expression>
				| <expression> != <expression>
				| <expression> == <expression>
				| <logical-expression>
				
<logical-expression> ::=	  <relational-expression> && <relational-expression>
				| <relational-expression> || <relational-expression>
				| !<relational-expression>

<function> ::= <function-definition> <function-body>
<function-definition> ::= <type-specifier> <identifier> (<parameter-list>)
<parameter-list> ::= <parameter-list>
       | <type-specifier> <identifier> ,?
<function-body> ::= { <statement>* (return <expression>)? }

<if-statement>::= if (<relational-expression>) { <statement>* } <else-if>* <else>?
<else-if> ::= else if (<relational-expression>) { <statement>* }
<else> ::= else { <statement>* }
