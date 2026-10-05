:- export reachable/2.

:- import ord_subset/2, ord_disjoint/2,ord_subtract/3 from ordsets.
:- import flatten/2,member/2,length/2 from basics.
%:- writeln(loading).

% Elementary Petri Net Evaluator.
% Also, could find useful transitions, live transitions, (sequential)
% configuration graph. 
% Based on this, could extend to check whether two nets are bisimular, 
% to find all processes in a net, contact-freeness, etc.

% Finds all configurations reachable from instate
% First arg is a state: i.e. a sorted list of places each of which
% have a token in S.

%:- conset(findall_cnt,0).

:- table reachable/2.
reachable(InState,NewState):-
	reachable(InState,State),
%	writeln(reachable(InState,State)),
	hasTransition(State,NewState).
%        bound_search.
reachable(InState,NewState):-
	hasTransition(InState,NewState).

hasTransition(State,NewState):-
	get_rules_for_state(State,AllRules),
	member(Rule,AllRules),
	apply_rule_to_state(Rule,State,NewState).
%        writeln(state(State,Rule,NewState)).

%----
% Constructs sets of rules with concession in State.  Also indicates which
% rules have input or output conflicts
get_rules_for_state(State,Flatrules):-
	get_rules_for_state_1(State,State,Rules),
	flatten(Rules,Flatrules),
	!.

get_rules_for_state_1([],_State,[]).
get_rules_for_state_1([H|T],State,[Rules1|RT]):-
    findall(rule([H|Places],Output,Tran),
	    gen_elem:rule([H|Places],Output,Tran),Rules),
%	coninc(findall_cnt),
	check_concession(Rules,State,Rules1),
	get_rules_for_state_1(T,State,RT).

% Check concession checks that a token is in all places in *t, and no
% token is in t*. 
check_concession([],_,[]).
check_concession([rule([Inp|Ilist],Outlist,Name)|T],Input,
                	[rule([Inp|Ilist],Outlist,Name)|T1]):-
	ord_subset(Ilist,Input),
	ord_disjoint(Outlist,Input),!,
	check_concession(T,Input,T1).
check_concession([_Rule|T],Input,T1):-
	check_concession(T,Input,T1).

%-------
apply_rule_to_state(rule(Input,Out,_Name),State,NewState):-
	ord_subtract(State,Input,Diff),
	flatsort([Out|Diff],NewState).

flatsort(In,Out):- 
	flatten(In,In1),
	sort(In1,Out).

%count_states(L):- 
%	findall(1,get_residual(reachable(_X,_Y),_F),List),
%	length(List,L).

%show_states:- 
%	get_residual(reachable(X,Y),_F),writeln((X,Y)),fail.
%show_states.

%count_rules(L):- 
%	findall(1,gen_elem:rule(_,_,_),List),
%	length(List,L).

end_of_file.

