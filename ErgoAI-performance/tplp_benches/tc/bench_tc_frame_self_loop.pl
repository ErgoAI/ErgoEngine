% Plain XSB frame-shaped self-loop transitive-closure benchmark.
% The frame shape is represented as (Object,Method,Value), without Ergo semantics.

:- import self_loop/3 from edge.
:- table tc_frame_self_loop/3.

tc_frame_self_loop(objid,reachable(Limit,From),To):- self_loop(Limit,From,To).
tc_frame_self_loop(objid,reachable(Limit,From),To):-
    tc_frame_self_loop(objid,reachable(Limit,From),Mid),
    self_loop(Limit,Mid,To).

bench_frame_self_loop(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	(tc_frame_self_loop(objid,reachable(Limit,From),_),fail ; true),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_frame_self_loop(Limit,TimeC,TimeW)).
