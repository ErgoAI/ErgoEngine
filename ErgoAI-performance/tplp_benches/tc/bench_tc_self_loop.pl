% Plain XSB self-loop transitive-closure benchmark.

:- import self_loop/3 from edge.
:- table tc_self_loop/3.

tc_self_loop(Limit,From,To):- self_loop(Limit,From,To).
tc_self_loop(Limit,From,To):-
    tc_self_loop(Limit,From,Mid),
    self_loop(Limit,Mid,To).

bench_self_loop(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	once(tc_self_loop(Limit,From,_)),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_self_loop(Limit,TimeC,TimeW)).
