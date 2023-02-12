%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   Color Australian states in colors 1, 2, 3 so that no two adjacent
   provinces are colored the same.
*/

int: nc = 3; %% number of colors

/*
    wa - W Australia
    nt - Northern Territory
    sa - S. Australia
    q  - Queensland
   nsw - New S. Wales
    v  - Victoria
    t  - Tasmania
*/

var 1..nc: wa;   var 1..nc: nt;  var 1..nc: sa;   var 1..nc: q;
var 1..nc: nsw;  var 1..nc: v;   var 1..nc: t;

constraint wa != nt;
constraint wa != sa;
constraint nt != sa;
constraint nt != q;
constraint sa != q;
constraint sa != nsw;
constraint sa != v;
constraint q != nsw;
constraint nsw != v;
solve satisfy;

/*
output ["p(\(wa), \(nt), \(sa), \(q), \(nsw), \(v), \(t)).", "\n"];
*/

%% minizinc --solver gecode australia.mzn
%% all solutions
%% minizinc -a --solver gecode australia.mzn
%% 5 solutions
%% minizinc -n 5 --solver gecode australia.mzn


%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
