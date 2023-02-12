%% From www.minizinc.org/doc-2.2.3/en/modelling.html#real-number-solving
/*
    Taking a loan. Illustrates linear constraints with floats.

    Problem: taking out a short loan for one year to be repaid in 4
             quarterly installments.

    NOTE: this requires the solver osicbc. Gecode is not very good with floats,
          and it'll return "unsatisfiable".
*/

% variables
var float: R;        % quarterly repayment
var float: P;        % principal initially borrowed
var 0.0 .. 10.0: I;  % interest rate

% intermediate variables
var float: B1; % balance after one quarter
var float: B2; % balance after two quarters
var float: B3; % balance after three quarters
var float: B4; % balance owing at end

constraint B1 = P * (1.0 + I) - R;
constraint B2 = B1 * (1.0 + I) - R;
constraint B3 = B2 * (1.0 + I) - R; 
constraint B4 = B3 * (1.0 + I) - R;

solve satisfy;
