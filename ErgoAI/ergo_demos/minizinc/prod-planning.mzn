%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   Simple production planning. Like baking problem, but generic.

   We wish to determine how much of each kind of product to make to maximise
   the profit where manufacturing a product consumes varying amounts of some
   fixed resources. Generalizes bake.mzn to handle this kind of problem with
   a model that is generic in the kinds of resources and products. 

   Data files are prod-planning.dzn and prod-planning2.dzn.
*/

% Products to be produced
enum Products;  
% profit per unit for each product
array[Products] of int: profit;
% Resources to be used
enum Resources;
% amount of each resource available
array[Resources] of int: capacity; 

% units of each resource required to produce 1 unit of product
array[Products, Resources] of int: consumption; 
constraint assert(forall (r in Resources, p in Products) 
           (consumption[p,r] >= 0), "Error: negative consumption");

% bound on number of Products
int: mproducts = max (p in Products) 
                     (min (r in Resources where consumption[p,r] > 0) 
                          (capacity[r] div consumption[p,r]));

% Variables: how much should we make of each product
array[Products] of var 0..mproducts: produce;
array[Resources] of var 0..max(capacity): used;

var int: maxprofit = sum (p in Products) (profit[p]*produce[p]);

% Production cannot use more than the available Resources:
constraint forall (r in Resources) (     
      used[r] = sum (p in Products)(consumption[p, r] * produce[p]) 
);
constraint forall (r in Resources) (     
      used[r] <= capacity[r]
);    

% Maximize profit
solve maximize maxprofit;


/*
output ["mzn_sol(produce("]
            ++ ["\(p) = \(produce[p])"
                ++ if p == max(Products) then "" else ", " endif
                | p in Products ]
        ++ ["), "] ++ ["rsrc("]
                           ++ [ "\(r) = \(used[r])"
                                ++  if r == max(Resources) then "" else ", " endif
                                | r in Resources ]
       ++ [")).", "\n"];
*/

%%output ["mzn_sol("] ++ [show(Products)] ++ [" = "] ++ [show(produce)] ++ [", "] ++ [show(Resources)] ++ [" = "] ++ [show(used)] ++ [").\n"];


/*
minizinc --solver gecode --data prod-planning.dzn prod-planning.mzn
*/



%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
