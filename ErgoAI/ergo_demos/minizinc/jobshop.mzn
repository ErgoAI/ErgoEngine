%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   The Job Shop Scheduling Problem.

   An example of the use of disjunctive modelling capability. In job shop
   scheduling we have a set of jobs, each consisting of a sequence of tasks
   on separate machines: so i,j denotes the task in the i-th job performed
   on the j-th machine. Each sequence of tasks must be completed in order,
   and no two tasks on the same machine can overlap in time. Even for small
   instances of this problem it can be challenging to find optimal solutions.

   Input data: jobshop.dzn + inline params in Ergo call.
*/

%% JOB and TASK are inputs provided in the Ergo call
enum JOB;
enum TASK;
TASK: last = max(TASK);
array [JOB,TASK] of int: d;                    % task durations
int: total = sum(i in JOB, j in TASK)(d[i,j]); % total duration
array [JOB,TASK] of var 0..total: s;           % start times
var 0..total: end;                             % total end time


constraint %% ensure the tasks occur in sequence
    forall(i in JOB) (
        forall(j in TASK where j < last) 
            (s[i,j] + d[i,j] <= s[i,enum_next(TASK,j)]) /\
        s[i,last] + d[i,last] <= end
    );

constraint %% ensure no overlap of tasks
    forall(j in TASK) (
        forall(i,k in JOB where i < k) (
            s[i,j] + d[i,j] <= s[k,j] \/
            s[k,j] + d[k,j] <= s[i,j]
        )
    );

solve minimize end;

/*
%% This should not be present, if invoked from Ergo
int: digs = ceil(log(10.0,int2float(total)));  % digits for output
output ["end = \(end)\n"] ++
       [ show_int(digs,s[i,j]) ++ " " ++ 
         if j == last then "\n" else "" endif |
         i in JOB, j in TASK ];
*/
