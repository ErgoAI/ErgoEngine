#!/usr/bin/python

import sys

if len(sys.argv) < 3:
    raise Exception ("\n+++ Not enough arguments given to start this script.\n    Needs XSB root directory and Ergo root directory.\n")

XSBARCHDIR =  sys.argv[1]
ERGOROOT = sys.argv[2]

# Add path to pyergo in case this example program is elsewhere
sys.path.append(ERGOROOT.replace('\\','/') + '/python')

from pyergo import \
    pyergo_start_session, pyergo_end_session,       \
    pyergo_command, pyergo_query,                   \
    HILOGFunctor, PROLOGFunctor,                    \
    ERGOVariable, ERGOString, ERGOIRI, ERGOSymbol,  \
    ERGOIRI, ERGOCharlist, ERGODatetime,            \
    ERGODuration, ERGOUserDatatype,                 \
    pyxsb_query, pyxsb_command,                     \
    XSBFunctor, XSBVariable, XSBAtom, XSBString,    \
    PYERGOException, PYXSBException

# Tests directly querying XSB
def run_xsb_tests():
    pyxsb_command('[curl].') # test curl
    pyxsb_command("load_page(url('http://www.google.com'),[],_Prop,R,W).")
    pyxsb_query('fail.')
    pyxsb_query('catch(abort,Exception,true).')
    for row in pyxsb_query('assert(ppp(Y)), ppp(X).'): # test unbound var
        print("unbound var: ", row[0], row[1])
    print("")

    try:
        print("Testing an XSB throw:")
        pyxsb_query("throw('Testing throw').")
    except Exception as ThrownException:
        print('Throw test caught: ',repr(ThrownException))
    print("")

    pyxsb_command("flora_call_string_command('write(''Test '')@\plg.',[],Status,XWamState,Exception), writeln(status=Status).")
    pyxsb_command("flora_call_string_command('insert{p({1,2},3)}.',[],Status,XWamState,Exception), writeln('insert status'=Status).")
    pyxsb_query("flora_call_string_command('p(?X,?Y).',['?X'=X,'?Y'=Y],Status,XWamState,Exception), writeln('p(?X,?Y) status'=X+Y+Status), fail.")

    print("")
    
    for row in \
        pyxsb_query("flora_call_string_command('p(?X,?Y).',['?X'=X,'?Y'=Y],Status,XWamState,Exception).") :
        print(row[0],row[1],row[2],row[3],repr(row[4]))
    print("")

##################################### tests ##################

pyergo_start_session(XSBARCHDIR,ERGOROOT)

print("\n*** XSB tests (including low-level calls to Ergo):")
run_xsb_tests()

print("*** Ergo COMMAND tests:")
pyergo_command("writeln(E = 'mc^2')@\\io.")
pyergo_command('insert{qq(fff(444),33)}.')
pyergo_command('insert{qq({11,fff(abc)(22)},33)}.')
pyergo_command('insert{qq(${ggg(cde)@\\plg},44)}.')
pyergo_command('insert{qq(55,${hhh(cde)@\\plg(somemod)})}.')
pyergo_command('insert{qq("this is string"^^\\string,"http://example.com/foo"^^\iri)}.')
pyergo_command('insert{qq("P0022Y02M10DT01H02M03S"^^\\duration,"-2018-6-27T10:30:55-00:20"^^\datetime)}.')
pyergo_command('insert{qq("-P22Y02M10DT01H02M03S"^^\\duration,"2008-6-27T10:30:55.23456-00:20"^^\datetime)}.')
pyergo_command('insert{qq("2008-6-27"^^\\date,"10:30:55.23456-00:20"^^\\time)}.')
pyergo_command('insert{ttt(?X)}.')
pyergo_command("writeln('Just a Test Message')@\\plg, \\true.")
print("")
try:
    print("Testing a compile error in pyergo_command():")
    pyergo_command('insert{qq(ggg(cde)@\\plg,44)}.')
except Exception as CommandSyntaxErr:
    print('Ergo exception raised in command -- caught: ',repr(CommandSyntaxErr))
print("")
try:
    pyergo_command("writeln('Testing a failed command:')@\\plg, \\false.")
except Exception as FalseCmd:
    print('Command failed -- caught: ',repr(FalseCmd))
print("")

print("*** Ergo QUERY tests:")
# row[0] - list of answers
# row[1] - status
# row[2] - XWam state: 0 - true; != 0 - undefined
# row[3] - exception or 'normal'
for row in pyergo_query('qq(?X,?Y).  '):
    print("row", row[0])
    [(XVarname,XVarVal),(YVarname,YVarVal)] = row[0]
    if isinstance(XVarVal,HILOGFunctor):
        #Xresult= XVarname+'='+str(XVarVal.name)+'('+str(XVarVal.args[0])+')'
        Xresult= XVarname+'='+str(XVarVal)+" (HiLog case)"
    elif isinstance(XVarVal,PROLOGFunctor):
        #Xresult= XVarname+'='+str(XVarVal.name)+'('+str(XVarVal.args[0])+')@\\plg'
        Xresult= XVarname+'='+str(XVarVal)+" (Prolog case)"
    else:
        Xresult= XVarname+'='+str(XVarVal)+" (Catchall case)"
    if isinstance(YVarVal,HILOGFunctor):
        #Yresult= YVarname+'='+str(YVarVal.name)+'('+str(YVarVal.args[0])+')'
        Yresult= YVarname+'='+str(YVarVal)+" (HiLog case)"
    elif isinstance(YVarVal,PROLOGFunctor):
        #Yresult= YVarname+'='+str(YVarVal.name)+'('+str(YVarVal.args[0])+')@\\plg'
        Yresult= YVarname+'='+str(YVarVal)+" (Prolog case)"
    else:
        Yresult= YVarname+'='+str(YVarVal)+" (Catchall case)"
    print("result: ",Xresult+" and "+Yresult,row[1],row[2],row[3].value)
print("")

for row in pyergo_query('ttt(?X).'):
    [(XVarname,XvarVal)] = row[0]
    print("row", row[0])
    result= XVarname+'='+str(XvarVal)+' (repr: '+repr(XvarVal)+')'
    print("openvar: ",result,row[1],row[2],row[3].value)
print("")

for row in pyergo_query("qq(?X,?_Y), abort('Test abort -- as expected.')@\\sys."):
    print("abort: ",row[0],row[1],row[2],row[3])
print("")

for row in pyergo_query('qq(?X,?_Y), \\undefined.'):
    print("undefined: ",row[0],row[1],row[2],row[3].value)
print("")


for row in pyergo_query("writeln('Error in arithmetic expr (expected): ')@\\plg, ?X \\is 1+abc."):
    print("error in arith (expected): ", row[0],row[1],row[2],row[3].name)
print("")

for row in pyergo_query("throw{'Test throw -- expected'}."):
    print("throw test: ", row[0],row[1],row[2],row[3].name)
print("")

for row in pyergo_query("writeln('\nNo-answer query test')@\\plg, \\true."):
    print("no-answer test: ", row[0],row[1],row[2],repr(row[3]), '\n')
print("")

#test for terms that cannot be returned to Python (eg, reification)
pyergo_command("insert{pp123(${pqr})}.")
try:
    print("Testing an unreturnable answer exception (answers in unsupported form):")
    pyergo_query("pp123(?X).")
except Exception as UnreturnableAnswer:
    print('UnreturnableAnswer test caught: ',repr(UnreturnableAnswer))
print("")

# needed only to save resources if Ergo is no longer needed
# for the rest of the computation
print("\nClosing connection to Ergo")
pyergo_end_session()
