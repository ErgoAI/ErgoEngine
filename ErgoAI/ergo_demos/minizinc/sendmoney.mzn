%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
    Send More Money.

    The SEND+MORE=MONEY problem requires assigning a different digit to each
    letter so that the arithmetic constraint holds.
*/

include "alldifferent.mzn";

var 1..9: S;
var 0..9: E;
var 0..9: N;
var 0..9: D;
var 1..9: M;
var 0..9: O;
var 0..9: R;
var 0..9: Y;

constraint           1000 * S + 100 * E + 10 * N + D
                   + 1000 * M + 100 * O + 10 * R + E
       = 10000 * M + 1000 * O + 100 * N + 10 * E + Y;

constraint alldifferent([S,E,N,D,M,O,R,Y]);

solve satisfy;

%%output ["mnzsol(send(\(S),\(E),\(N),\(D)), more(\(M),\(O),\(R),\(E)), money(\(M),\(O),\(N),\(E),\(Y))).\n"];

/*
minizinc --solver gecode sendmoney.mzn
%% all solutions
minizinc -a --solver gecode sendmoney.mzn
%% 5 solutions
minizinc -n 5 --solver gecode sendmoney.mzn
*/


%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
