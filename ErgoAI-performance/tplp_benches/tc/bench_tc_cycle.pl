% Plain XSB cycle transitive-closure benchmark.

:- import cycle/3 from edge.
:- table tc_cycle/3.

tc_cycle(Limit,From,To):- cycle(Limit,From,To).
tc_cycle(Limit,From,To):-
    tc_cycle(Limit,From,Mid),
    cycle(Limit,Mid,To).

bench_cycle(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	once(tc_cycle(Limit,From,_)),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_cycle(Limit,TimeC,TimeW)).
