% Plain XSB frame-shaped chain transitive-closure benchmark.
% The frame shape is represented as (Object,Method,Value), without Ergo semantics.

:- import chain/3 from edge.
:- table tc_frame_chain/3.

tc_frame_chain(objid,reachable(Limit,From),To):- chain(Limit,From,To).
tc_frame_chain(objid,reachable(Limit,From),To):-
    tc_frame_chain(objid,reachable(Limit,From),Mid),
    chain(Limit,Mid,To).

bench_frame_chain(Limit,From):-
	abolish_all_tables,
	cputime(BeforeC),
	walltime(BeforeW),
	(tc_frame_chain(objid,reachable(Limit,From),_),fail ; true),
	cputime(AfterC),
	walltime(AfterW),
	TimeC is AfterC-BeforeC,
	TimeW is AfterW-BeforeW,
	writeln(time_prolog_tc_frame_chain(Limit,TimeC,TimeW)).
