
:- table tc/3.
tc(Limit,From,To):- edge(Limit,From,To).
tc(Limit,From,To):-
    tc(Limit,From,Mid),
    edge(Limit,Mid,To).

edge(Limit,From,To):-
    (Limit >= From ->
	 (To is From+1 ; To=From)
      ;  fail).


:- dynamic tc_app/6 as tabled.
tc_app(tc,Limit,From,To,_,_):- edge(Limit,From,To).
tc_app(tc,Limit,From,To,A,B):-
    tc_app(tc,Limit,From,Mid,A,B),
    edge(Limit,Mid,To).
