%prolog
% this is just to test that C calling XSB works with the preprocessor
:- compiler_options([xpp_on]).
#include "char_defs.h"

p(100,aa,bb).
p(300,first,second).
p(400,wrong,one).
p(300,third,fourth).
p(X,Y,Z) :- repeat, 
	writeln('Enter a term of the form p(300,atom1,atom2). (or end_of_file.)'),
	write(': '),
	read(T), 
	(T=end_of_file -> 
		!,fail
	;
		true
	),
	T=p(X,Y,Z).
