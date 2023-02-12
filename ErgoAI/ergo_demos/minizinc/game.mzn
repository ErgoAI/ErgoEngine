% 
% Game theory problem in MiniZinc.
% 
% From http://www.ms.uky.edu/~mathecon/ma416
% Vanderbei "Linear Programming - Foundations - and Extensions", 
% page 174ff
% 
% This MiniZinc model was created by Hakan Kjellerstrand, hakank@bonetmail.com
% See also his MiniZinc page: http://www.hakank.org/minizinc/
%
/*
    A[1..n,1..n] - matrix of payoffs.
    If row player picks row i (arg1) and column player picks column j (arg2)
    then the row player pays A[i,j] value to column player. (If A[i,j] < 0
    then the column player pays.)
    X[1..n] - vector of probabilities. X[j] is the probability for the
    column player to choose column j.
    For any i, pay(i)=sum_j(A[i,j]:X[j]) is the expected payment by row
    player to the column player.
    We want to find a strategy for the column player (probabilities in
    vector X) that will maximize min_i(pay(i)), i.e., the minimal guaranteed
    payment to the column player (or minimize his loss, if he loses).

    The input parameters are A, the matrix, and its size, n.
    They are set in Ergo before calling MiniZinc.
*/

int: n;
array[1..n, 1..n] of float: A;    % payoff matrix
array[1..n] of var 0.0..1.0: X;   % probability of strategy j for column player
var -1.0..1.0: w;                 % auxiliary value to maximize and thus also
                                  % maximize min_i(pay(i)

%% n is assigned in Ergo
%% n=3;
%% This input variable is assigned in Ergo
%%A = [|  0.0,  1.0, -2.0 | -3.0,  0.0,  4.0 |  5.0, -6.0,  0.0 |];


solve maximize w;

%% Other requests to solve
% solve satisfy;
% solve::float_search(X,0.00001,input_order,indomain_split,complete) maximize w;


constraint 
        forall(i in 1..n) (
           sum(j in 1..n) (A[i,j]*X[j]-w) >=0.0
        );

% subject to probability:
constraint
    sum(j in 1..n) (X[j]) = 1.0;

