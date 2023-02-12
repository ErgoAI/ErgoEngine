%% From the Minizinc tutorial www.minizinc.org/doc-2.2.3/en/part_2_tutorial.html
/*
   We know how to make two sorts of cakes
   A banana cake takes 250g of self-raising flour, 2 mashed bananas,
   75g sugar and 100g of butter.
   A chocolate cake takes 200g of self-raising flour, 75g of cocoa,
   150g sugar and 150g of butter. 
   A chocolate cake sells for $4.50 and a banana cake for $4.00.

   The input parameters are found in the data file bake2.dzn
   They state that we have:
        8kg of self-raising flour
        11 bananas
        3kg of sugar
        1.5kg of butter
        800g of cocoa.
        
   How many of each sort of cake should we bake for the school fete to
   maximize the profit.
*/

int: flour;  %no. grams of flour available
int: banana; %no. of bananas available
int: sugar;  %no. grams of sugar available
int: butter; %no. grams of butter available
int: cocoa;  %no. grams of cocoa available
var int: price;

constraint assert(flour >= 0,"Invalid datafile: " ++
                  "Amount of flour should be non-negative");
constraint assert(banana >= 0,"Invalid datafile: " ++
                  "Amount of banana should be non-negative");
constraint assert(sugar >= 0,"Invalid datafile: " ++
                  "Amount of sugar should be non-negative");
constraint assert(butter >= 0,"Invalid datafile: " ++
                  "Amount of butter should be non-negative");
constraint assert(cocoa >= 0,"Invalid datafile: " ++
                  "Amount of cocoa should be non-negative");

var 0..100: b; % no. of banana cakes
var 0..100: c; % no. of chocolate cakes

% flour
constraint 250*b + 200*c <= flour;
% bananas
constraint 2*b  <= banana;
% sugar
constraint 75*b + 150*c <= sugar;
% butter
constraint 100*b + 150*c <= butter;
% cocoa
constraint 75*c <= cocoa;

price = 400*b + 450*c;

% maximize our profit
solve maximize price;

/*
output ["p(\(b), \(c)).", "\n"];
*/

/*
minizinc --solver gecode --data bake2.dzn bake2.mzn
*/

%%% Local Variables: 
%%% eval: (auto-fill-mode 0)
%%% End:
