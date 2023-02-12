% prolog

child(anne,bridget). 
child(bridget,caroline). 
child(caroline,donna). 
child(donna,emily).

descend(X,Y) :- child(X,Y). 
descend(X,Y) :- child(X,Z), descend(Z,Y).

label('g�nter', "G�nter").
label(anne, "Anne").
label(bridget, "Bridget").
label(caroline, "Caroline").
label(donna, "Donna").
