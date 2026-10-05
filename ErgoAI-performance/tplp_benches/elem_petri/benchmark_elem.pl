:- export bench_process_1/2, bench_private_process_2/1, gen_proc_2/2.

:- import reachable/2 from elemNet.
:- import concat_atom/2 from string.
:- index(rule/3,trie).
:- import between/3 from basics.

/* 
  generation and benchmarks for elementary Petri nets.

  All these routines are based on the process-view of a run of a Petri
  net for a mutex, unfolded many times (see below). 

      a1  b1  c1
       |    | /
      s1   t1/
       |   | \
       |   |  \
      a2  b2   c2
        \  |   |
         \t2   s2  
         /|    | 
        / |    |
      a3  b3   c3

     (then repeats) 

  Thus, we have "braids" of a, b and c places that form chain-like
  elements that we can traverse in parallel, join, etc.  For odd N a
  braid has 2*N transitions, 3*N places, and 4(N+2) - 1 reachable
  states.
*/

%-------------------------------------

/* benchmark_1 can be used to bench private tables.

   On my Mac on 12/07 it bench_process_1(400000,T) takes about 45 seconds of
   walltime (and 20-30 secs for generation).
*/

/* Top level private tables */     
bench_process_1(N,Time):-
	abolish_all_tables,
	generate_process_1(N),
	walltime(Before),
        reachable([a1,b1,c1],_),
	walltime(After),
	Time is After - Before,
	statistics.

generate_process_1(N):- 
	between(1,N,M),
	IsOdd is M mod 2,
	IsOdd == 1,
	generate_process_odd_1(M),
	M1 is M + 1,
	generate_process_even_1(M1),
	fail.
generate_process_1(_).

generate_process_odd_1(Nin):-
	concat_atom([a,Nin],A),
	concat_atom([b,Nin],B),
	concat_atom([c,Nin],C),
	concat_atom([s,Nin],S),
	concat_atom([t,Nin],T),
	Next is Nin + 1,
	concat_atom([a,Next],Anext),
	concat_atom([b,Next],Bnext),
	concat_atom([c,Next],Cnext),
	assert(rule([A],[Anext],S)),
	assert(rule([B,C],[Bnext,Cnext],T)).

generate_process_even_1(Nin):-
	concat_atom([a,Nin],A),
	concat_atom([b,Nin],B),
	concat_atom([c,Nin],C),
	concat_atom([s,Nin],S),
	concat_atom([t,Nin],T),
	Next is Nin + 1,
	concat_atom([a,Next],Anext),
	concat_atom([b,Next],Bnext),
	concat_atom([c,Next],Cnext),
	assert(rule([A,B],[Anext,Bnext],S)),
	assert(rule([C],[Cnext],T)).

%------------------------------------------------------------------------------------

/* This is a benchmark routine that can be used for shared tables.  It
   consists of Numthread process-chains like in the first benchmark,
   with a joining transition at the end to a state 'finish'.  Given
   the two initial states that are tested, the state 'finish' is never
   reached.  However, each query generates 4(N+2) - 1 independent
   states, for Numthread*(4(N+2) - 1) overall.  */

/* Toplevel (for shared?) */     
gen_proc_2(N,Numbraids):-
      retractall(rule(_,_,_)),
      generate_process_2(N,Numbraids).

%bench_2( NThreads, NBraids ) :-
%	bench_2_priv( NThreads, NBraids ),
%	bench_2_shared( NThreads, NBraids ).
%
%bench_2_priv( NThreads, NBraids ) :-
%	bench_braids_2( NBraids, NThreads, reachable ).

%bench_braids_2( NBraids, NThreads, Pred ) :-
%	abolish_all_tables,
%	generate_goallist_thr(NBraids,NThreads,List,Pred,0),
%	walltime(Before),
%%	all_par_execute(List),
%	walltime(After),
%	Time is After - Before,
%	(Pred = reachable -> Bench = priv ; Bench = shared ),
%	write(datum([elem,Bench],NThreads,Time)),
%	writeln('.'),
%	flush_output.

bench_private_process_2(Numthreads):-
	abolish_all_tables,
	generate_goallist(Numthreads,List,reachable),
        List = [Goal],
	walltime(Before),
	elemNet:Goal,
	walltime(After),
	Time is After - Before,
	write(datum([elem_net],Time)),
	writeln('.'),
      flush_output.

%bench_private_process_cs(Numthreads):- 
%	abolish_all_tables,
%	generate_goallist(Numthreads,List,right_reachable),
%	walltime(Before),
%	par_execute(List),
%	walltime(After),
%	Time is After - Before,
%	write(datum([pr_cs_elem],Numthreads,Time)),
%	writeln('.'),
%%	flush_output.

generate_process_2(N,Numthreads):- 
	between(1,N,M),
	IsOdd is M mod 2,
	IsOdd == 1,
	generate_odd_processes(Numthreads,M),
	M1 is M + 1,
	generate_even_processes(Numthreads,M1),
	fail.
generate_process_2(N,Numthreads):- 
	generate_join(N,Numthreads).

generate_odd_processes(0,_M):- !.
generate_odd_processes(N,M):- !,
	generate_process_odd_2(M,N),
	N1 is N - 1,
	generate_odd_processes(N1,M).

generate_process_odd_2(Nin,Index):-
	concat_atom([a,Index,'_',Nin],A),
	concat_atom([b,Index,'_',Nin],B),
	concat_atom([c,Index,'_',Nin],C),
	concat_atom([s,Index,'_',Nin],S),
	concat_atom([t,Index,'_',Nin],T),
	Next is Nin + 1,
	concat_atom([a,Index,'_',Next],Anext),
	concat_atom([b,Index,'_',Next],Bnext),
	concat_atom([c,Index,'_',Next],Cnext),
	assert(rule([A],[Anext],S)),
	assert(rule([B,C],[Bnext,Cnext],T)).

generate_even_processes(0,_M):- !.
generate_even_processes(N,M):- !,
	generate_process_even_2(M,N),
	N1 is N - 1,
	generate_even_processes(N1,M).

generate_process_even_2(Nin,Index):-
	concat_atom([a,Index,'_',Nin],A),
	concat_atom([b,Index,'_',Nin],B),
	concat_atom([c,Index,'_',Nin],C),
	concat_atom([s,Index,'_',Nin],S),
	concat_atom([t,Index,'_',Nin],T),
	Next is Nin + 1,
	concat_atom([a,Index,'_',Next],Anext),
	concat_atom([b,Index,'_',Next],Bnext),
	concat_atom([c,Index,'_',Next],Cnext),
	assert(rule([A,B],[Anext,Bnext],S)),
	assert(rule([C],[Cnext],T)).

generate_join(Nin,Numthreads):- 
	N is Nin + 2,  % join final states.
	generate_joinlist(Numthreads,N,List),
	sort(List,Slist),
	Slist = [A1|Rest],
	assert(rule([A1|Rest],[finish],u)).

generate_joinlist(0,_N,[]):- !.
generate_joinlist(Index,N,[A,C|Rest]):- 
	concat_atom([a,Index,'_',N],A),
	concat_atom([c,Index,'_',N],C),
	I1 is Index - 1,
	generate_joinlist(I1,N,Rest).

generate_goallist(0,[],_P):- !.
generate_goallist(Index,[G|Rest],P):- 
	concat_atom([a,Index,'_1'],A),
	concat_atom([b,Index,'_1'],B),
	concat_atom([c,Index,'_1'],C),
	G =.. [P, [A, B, C], _ ], I1 is Index - 1, 
	generate_goallist(I1,Rest,P).

%generate_goallist2(0,[],_P,_I0):- !.
%generate_goallist2(I,[G|Rest],P,I0):- 
%	Index is I + I0,
%	concat_atom([a,Index,'_1'],A),
%	concat_atom([b,Index,'_1'],B),
%	concat_atom([c,Index,'_1'],C),
%	G =.. [P, [A, B, C], _ ],
%	I1 is I - 1,
%	generate_goallist2(I1,Rest,P,I0).

%generate_goallist_thr(0,_NT,[],_P,_I):-!.
%generate_goallist_thr(NB,NT,[H|T],P,I) :-
%	NB1 is NB - NT,
%	I1 is I + NT,
%	generate_goallist2(NT,H,P,I),
%	generate_goallist_thr(NB1,NT,T,P,I1).

%-----------------------------------------------------------------------------1

/* This benchmark routine uses the same process structure in
   bench_process_2, but has an initial state that reaches finish.  Be
   careful, however, as it generates (4(N+2))^2 reachable states.

   I can make this N-ary if needed.
*/

%bench_process_3(N,NumThreads,Time):- 
%	abolish_all_tables,
%	generate_process_2(N,2),
%	walltime(Before),
%	n_par_execute( reachable([a1_1,a2_1,b1_1,b2_1,c1_1,c2_1],_), NumThreads ),
%	walltime(After),
%	Time is After - Before,
%	statistics.


