:- export chain/3, cycle/3, self_loop/3.


chain(Limit,From,To):-
    (Limit >= From ->
	 To is From+1
     ;  fail).


cycle(Limit,From,To):-
    (Limit > From ->
	 To is From+1
    ;   (Limit = From ->
	     To = 1
	;    fail) ).

self_loop(Limit,From,To):-
    (Limit >= From ->
	 (To is  From+1 ; To = From)
      ;  fail).

