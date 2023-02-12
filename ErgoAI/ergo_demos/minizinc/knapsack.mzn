%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   The 0/1 knapsack problem

   This is a restricted form of the knapsack problem in which we can either
   choose to place the item in the knapsack or not. Each item has a weight
   and a profit and we want to find which choice of items leads to the
   maximum profit subject to the knapsack not being too heavy.
*/


enum ITEM;
int: capacity;

array[ITEM] of int: profits;
array[ITEM] of int: weights;

var set of ITEM: knapsack;

var int: total_weight = sum (i in knapsack) (weights[i]);
var int: total_profit = sum (i in knapsack) (profits[i]);

constraint total_weight <= capacity;

solve maximize total_profit;

%%output ["knapsack = \(knapsack)\n"];
