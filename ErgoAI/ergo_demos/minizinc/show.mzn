
array[1..2,1..3] of int: x;
set of int: y;
string: pp;
enum OOO;

OOO = { 'BananaCake', 'ChocolateCake #', Foobar };

pp = "aaaaa";

x = [|1,2,3|4,5,6|];
y = {3,4,5,33,6};

output [show2d(x), "\n"];
output [show(x), "\n"];
output [showJSON(x), "  ", showJSON(123), "  ", showJSON("ppp"), "  ", showJSON({3,4,5,33,6}), "\n"];
output [show(y), "  ", show(pp), "  ", show(OOO), "\(OOO)", "\n"];
