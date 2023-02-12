%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   Color Australian states in colors of Color so that no two adjacent
   provinces are colored the same.
*/

enum Color;
var Color: wa; 
var Color: nt; 
var Color: sa;
var Color: q;
var Color: nsw;
var Color: v;
var Color: t;
constraint wa != nt /\ wa != sa /\ nt != sa /\ nt != q /\ sa != q;
constraint sa != nsw /\ sa != v /\ q != nsw /\ nsw != v; 
solve satisfy;
