%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   The N queens problem.

   Place N queens on an N x N chessboard so that none can attack another.
   The variable q[i] records in which row the queen in column i is placed.
   The alldifferent constraints ensure that no two queens are on the same row,
   or diagonal.
   The search ends either when the finite domain solver detects that all
   constraints are satisfied, and hence a solution has been found, or that
   the constraints are unsatisfiable.
*/

int: n;
array [1..n] of var 1..n: q; % queen is column i is in row q[i]

include "alldifferent.mzn";

constraint alldifferent(q);                       % distinct rows
constraint alldifferent([ q[i] + i | i in 1..n]); % distinct diagonals
constraint alldifferent([ q[i] - i | i in 1..n]); % upwards+downwards

% search
solve :: int_search(q, first_fail, indomain_min, complete)
      satisfy;
/*
output ["mzn_sol("]
        ++ [ if j==1 then "row(" else "" endif ++
             if fix(q[j]) == i then "Q" else "'.'" endif 
             ++ if j == n then ")" else "," endif
             ++ if i < n /\ j == n then ", " else "" endif
             | i,j in 1..n]
        ++ [").\n"];
*/
output ["mzn_sol("] ++ [show(q)] ++ [").\n"];


/*
minizinc --solver gecode --data queens.dzn queens.mzn
%% all solutions
minizinc -a --solver gecode --data queens.dzn queens.mzn
%% 5 solutions
minizinc -n 5 --solver gecode --data queens.dzn queens.mzn
*/


%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
