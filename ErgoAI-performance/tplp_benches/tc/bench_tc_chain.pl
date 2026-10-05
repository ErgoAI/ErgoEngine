% Plain XSB chain transitive-closure benchmark.

:- import chain/3 from edge.
:- table tc_chain/3.

tc_chain(Limit,From,To):- chain(Limit,From,To).
tc_chain(Limit,From,To):-
    tc_chain(Limit,From,Mid),
    chain(Limit,Mid,To).

bench_chain(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	once(tc_chain(Limit,From,_)),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_chain(Limit,TimeC,TimeW)).
