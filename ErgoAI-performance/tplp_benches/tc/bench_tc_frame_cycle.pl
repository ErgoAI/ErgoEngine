% Plain XSB frame-shaped cycle transitive-closure benchmark.
% The frame shape is represented as (Object,Method,Value), without Ergo semantics.

:- import cycle/3 from edge.
:- table tc_frame_cycle/3.

tc_frame_cycle(objid,reachable(Limit,From),To):- cycle(Limit,From,To).
tc_frame_cycle(objid,reachable(Limit,From),To):-
    tc_frame_cycle(objid,reachable(Limit,From),Mid),
    cycle(Limit,Mid,To).

bench_frame_cycle(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	(tc_frame_cycle(objid,reachable(Limit,From),_),fail ; true),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_frame_cycle(Limit,TimeC,TimeW)).
