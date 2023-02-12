%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
    Stable marriage problem.

    We have n women and n men.
    Each man has a ranked list of women and vice versa.
    We want to find a husband/wife for each women/man so that all marriages
    are stable in the sense that:
      - whenever a man, m, prefers another women, o, to his wife w,
        o prefers her husband to m, and
      - whenever a woman, w, prefers another man, o, to her husband m,
        o prefers his wife to w.

    The data file is marriage.dzn.
*/

int: n;

enum Man = anon_enum(n);
enum Woman = anon_enum(n);

array[Woman, Man] of int: rankWomen;
array[Man, Woman] of int: rankMen;

array[Man] of var Woman: wife;
array[Woman] of var Man: husband;

% assignment
constraint forall (m in Man) (husband[wife[m]]=m);
constraint forall (w in Woman) (wife[husband[w]]=w);
% ranking
constraint forall (m in Man, o in Woman) (
     rankMen[m,o] < rankMen[m,wife[m]] -> 
         rankWomen[o,husband[o]] < rankWomen[o,m] );

constraint forall (w in Woman, o in Man) (
     rankWomen[w,o] < rankWomen[w,husband[w]] -> 
         rankMen[o,wife[o]] < rankMen[o,w] );
solve satisfy;

%%output ["mzn_sol(", "wives= \(wife), husbands= \(husband)).\n"];

/*
minizinc --solver gecode --data marriage.dzn marriage.mzn
%% all solutions
minizinc -a --solver gecode --data marriage.dzn marriage.mzn
%% 5 solutions
minizinc -n 5 --solver gecode --data marriage.dzn marriage.mzn
*/



%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
