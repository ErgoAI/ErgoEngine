%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
    The social golfers problem.
    
    Schedule a golf tournament over weeks using groups x size golfers.
    Each week we schedule different groups `groups', each of size `size'.
    No two pairs of golfers should ever play in two groups.

    The variables in the model are sets of golfers Sched[i,j] for the i-th
    week and j-th group.

    The constraints first enforce an ordering on the first set in each week
    to remove symmetry in swapping weeks. Next they enforce an ordering
    on the sets in each week, and make each set have a cardinality of size.
    They then ensure that each week is a partition of the set of golfers using
    the global constraint partition_set. Finally the last constraint ensures
    that no two players play in two groups together (since the cardinality
    of the intersection of any two groups is at most 1).
*/

include "partition_set.mzn";
int: weeks;    set of int: WEEK = 1..weeks; 
int: groups;   set of int: GROUP = 1..groups;
int: size;     set of int: SIZE = 1..size;
int: ngolfers = groups*size; 
set of int: GOLFER = 1..ngolfers;

array[WEEK,GROUP] of var set of GOLFER: Sched;

% constraints
constraint 
    forall (i in WEEK, j in GROUP) (
           card(Sched[i,j]) = size
        /\ forall (k in j+1..groups) (
                Sched[i,j] intersect Sched[i,k] = {}
           )
    ) /\
    forall (i in WEEK) (
          partition_set([Sched[i,j] | j in GROUP], GOLFER)
    ) /\
    forall (i in 1..weeks-1, j in i+1..weeks) (
        forall (x,y in GROUP) (
            card(Sched[i,x] intersect Sched[j,y]) <= 1
        )
    );
% symmetry
  constraint 
    % Fix the first week %
    forall (i in GROUP, j in SIZE) (
        ((i-1)*size + j) in Sched[1,i]
    ) /\
    % Fix first group of second week %
    forall (i in SIZE) (
        ((i-1)*size + 1) in Sched[2,1]
    ) /\
    % Fix first 'size' players
    forall (w in 2..weeks, p in SIZE) (
        p in Sched[w,p]
    );
  
solve satisfy;

/*
output [ show(Sched[i,j]) ++ " " ++
         if j == groups then "\n" else "" endif |
         i in WEEK, j in GROUP ];
*/
