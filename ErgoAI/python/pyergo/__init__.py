#!/usr/bin/python

## File:   pyergo/__init__.py
##
## Author: Michael Kifer
##
## Contact:   see  ../CONTACTS.txt
##
## Copyright (C) 2017, 2018 Guenter Bartsch
## Copyright (C) Coherent Knowledge Systems, LLC, 2018-2023.
##
## Licensed under the Apache License, Version 2.0 (the "License");
## you may not use this file except in compliance with the License.
## You may obtain a copy of the License at
##
##      http://www.apache.org/licenses/LICENSE-2.0
##
## Unless required by applicable law or agreed to in writing, software
## distributed under the License is distributed on an "AS IS" BASIS,
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
## See the License for the specific language governing permissions and
## limitations under the License.
##
## PROPRIETARY, DO NOT DISTRIBUTE.

"""
Much of the code is adapted from the pyxsb package by Guenter Bartsch
"""

from six    import python_2_unicode_compatible, text_type, string_types
from ctypes import cdll, c_long, c_longlong, c_int64, c_int32, c_int, \
    c_double, c_char_p, POINTER, c_size_t, create_string_buffer, byref

from pyxsb import pyxsb_start_session, pyxsb_end_session, \
    pyxsb_command, pyxsb_query, \
    xsb_to_json, json_to_xsb, \
    XSB_SUCCESS, XSB_FAILURE, XSB_ERROR, XSB_OVERFLOW, XSB_SPECIAL_RETURN, \
    XSBAtom, XSBFunctor, XSBVariable, XSBString, \
    PYXSBException, \
    check_session_parameter, \
    is_python3, dbgprint

HILOG_WRAP='flapply'
XSBMODULE_FUNCTOR = ":"
ERGO_PREFIX='_$_$_ergo'

IRI_PREFIX='i\b'
STRING_PREFIX='s\b'

ERGODATATYPE='\\datatype'
ERGODATETIME='\\datetime'
ERGODATE    ='\\date'
ERGOTIME    ='\\time'
ERGODURATION='\\duration'

ergo_session_active=False

def pyergo_start_session(xsb_archdir,ergo_root):
    '''xsb_archdir must be a Python string, a path like, "/home/me/XSB",
       pointing to XSB root directory.
       ergo_root must be a Python string, pointing to the Ergo root directory,
       like "/home/me/Ergo"
    '''

    global ergo_session_active

    ergo_root_forw = ergo_root.replace("'","").replace('\\','/')

    check_session_parameter(ergo_root_forw,'/flora2.xwam','Ergo root')
    check_session_parameter(ergo_root_forw,'/syslib/flranswer.xwam','Ergo root')
    check_session_parameter(ergo_root_forw,'/lib/flrio.flr','Ergo root')

    #dbgprint("xsb_root=",xsb_root, "xsb_archdir_forw=",xsb_archdir_forw, "ergo_root_forw=",ergo_root_forw)

    pyxsb_start_session(xsb_archdir, \
                        ["--noprompt","--quietload","--nofeedback","--nobanner"])

    # Now execute these
    #    "bootstrap_flora",
    #    "consult(flrimportedcalls)",
    #    "import flora_get_DT_var_datatype/2 from usermod(flrunify_handlers)",
    #    "import flora_call_string_command/5 from flrcallflora",
    #    "import '\\\\load'/1, '\\\\add'/1 from flora2"
    pyxsb_command("add_lib_dir(a('"+ergo_root_forw+"')).")
    pyxsb_command('[flora2].')
    pyxsb_command('bootstrap_flora, consult(flrimportedcalls).')
    pyxsb_command('import flora_get_DT_var_datatype/2 from usermod(flrunify_handlers).')
    pyxsb_command('import flora_call_string_command/5 from flrcallflora.')
    pyxsb_command('import flora_call_string_command2prolog/5 from flrcallflora.')

    ergo_session_active=True


@python_2_unicode_compatible
class PYERGOException(Exception):
    def __init__(self,code=None,query=None,command=None,message=None):
        self.code=code
        self.command=command
        self.query=query
        self.message=message

    def __str__(self):
        if not self.command == None:
            if self.code==XSB_FAILURE:
                return "Ergo Command %s failure." % (self.command)
            elif self.code == XSB_ERROR:
                return "Ergo Command %s error: %s" % (self.command, self.message)
            elif self.code == XSB_OVERFLOW:
                return "Ergo Command %s overflow." % (self.command)

        elif not self.query == None:
            if self.code == XSB_ERROR:
                return "Ergo Query %s error: %s %s" % (self.query, self.message)
            elif self.code == XSB_OVERFLOW:
                return "Ergo Query %s overflow." % (self.query)
        else:  # not a query or command
            return "PYERGO Error: %s" % (self.message)

    def __repr__(self):
        if not self.command == None:
            if self.code==XSB_FAILURE:
                return 'PYERGOException(command=%s,message="Ergo Command failed")' % (self.command)
            elif self.code == XSB_ERROR:
                return 'PYERGOException(command=%s,message=%s)' % (self.command, self.message)
            elif self.code == XSB_OVERFLOW:
                return "PYERGOException(command=%s,message='Overflow in XSB command')" % (self.command)

        elif not self.query == None:
            if self.code == XSB_ERROR:
                return "PYERGOException(query=%s,message=%s)" % (self.query, self.message)
            elif self.code == XSB_OVERFLOW:
                return "PYERGOException(query=%s,message='Overflow in XSB query')" % (self.query)
        else:  # not a query or command
            return "PYERGOException(message=%s)" % (self.message)


@python_2_unicode_compatible
class HILOGFunctor:

    def __init__ (self, name=None, args=None):
        self.name = name
        self.args = args

    def __str__(self):
        if not self.args:
            return str(self.name)
        #dbgprint(self.name,self.args)
        if is_python3:
            return str(self.name) + '(' + ','.join(str(a) for a in self.args) + ')'
        else:
            return str(self.name) + u'(' + u','.join(map(lambda a: unicode(a), self.args)) + u')'

    def __repr__(self):
        return 'HILOGFunctor(name=%s, args=%s)' % (repr(self.name), repr(self.args))

    def __getitem__(self, sliced):
        return self.args[sliced]

@python_2_unicode_compatible
class PROLOGFunctor:

    def __init__ (self, name=None, module=None, args=None):

        self.name = str(name)
        self.args = args
        # XSB module
        if module == None or module == 'usermod':
            self.module = module
            self.plg_suffix = "@\\plg"
        else:
            self.module = str(module)
            self.plg_suffix = "@\\plg("+self.module+")"

    def __str__(self):
        if not self.args:
            return self.name+"()"+self.plg_suffix
        #dbgprint(self.name,self.args)
        if is_python3:
            return self.name + '(' + ','.join(str(a) for a in self.args) + ')' + self.plg_suffix
        else:
            return self.name + u'(' + u','.join(map(lambda a: unicode(a), self.args)) + u')' + self.plg_suffix

    def __repr__(self):
        return 'PROLOGFunctor(name=%s, module=%s, args=%s)' % (self.name, self.module, repr(self.args))

    def __getitem__(self, sliced):
        return self.args[sliced]


# similarly to pyxsb_command
def pyergo_command(cmd):

    from pyxsb import xsb_query_string, xsb_close_query, \
        reg_term, p2c_string, \
        xsb_get_error_type, xsb_get_error_message

    check_active_ergo_session()

    real_cmd = "flora_call_string_command('" + \
               cmd.replace('\\','\\\\').replace("'","''") + \
               "',[],Status,XWamState,Exception)."

    rcode = xsb_query_string(real_cmd.encode('utf-8'))
    ## Only XSB_FAILURE can happen, as flora_call_string_command handles
    ## the rest. XSB_ERROR/XSB_OVERFLOW cases are kept just in case.
    if rcode == XSB_FAILURE:
        raise PYERGOException(code=rcode,command=cmd,message="Ergo Command %s failure." % (cmd))
    elif rcode == XSB_ERROR:
        raise PYERGOException(code=rcode,command=cmd,message="Ergo Command %s error: %s %s" % (cmd, xsb_get_error_type(), xsb_get_error_message()))
    elif rcode == XSB_OVERFLOW:
        raise PYERGOException(code=rcode,command=cmd,message="Ergo Command %s overflow." % (cmd))

    cmd_answer = hilog_term2py(reg_term(2))
    xsb_close_query()
    cmd_exception = (cmd_answer[-1]).value
    if cmd_exception != "normal":
        raise PYERGOException(code=XSB_ERROR,command=cmd,message="Exception in Ergo Command %s: %s" % (cmd,cmd_exception))


def hilog_term2py(term, auto_string=True):

    ## These must be imported here, after pyxsb_start_session
    from pyxsb import \
        is_float, is_int, is_var, is_atom, is_functor, is_list, \
        is_string, is_charlist, is_nil, \
        p2c_string, p2c_int, p2c_float, p2c_functor, p2c_chars, \
        p2c_varnum, p2c_arity, \
        p2p_arg, p2p_car, p2p_cdr, \
        print_pterm_fun


    if is_float(term):
        return p2c_float(term)

    elif is_int(term):
        return p2c_int(term)

    elif is_var(term):
        ## FIXED: we just print _VarNNN for output variables.
        ##        Using p2c_string was producing gibberish!
        varnum = p2c_varnum(term)
        varnum = 2*varnum if varnum >= 0 else 2*abs(varnum)+1
        varname = '_Var' + str(varnum)
        return ERGOVariable(name = varname)

    elif is_atom(term):
        atomvalue=p2c_string(term).decode('utf8',errors='ignore')
        if atomvalue.startswith(IRI_PREFIX):
            return ERGOIRI(value=atomvalue[2:])
        elif atomvalue.startswith(STRING_PREFIX):
            return ERGOString(value=atomvalue[2:])
        else:
            return ERGOSymbol(value=atomvalue)

    elif is_functor(term) and not is_list(term):

        name_maybe = p2c_functor(term).decode('utf8', errors='ignore')

        # term module:fun(args)  -- XSB term with explicit module
        xsb_module = p2p_arg(term,1)
        if name_maybe == XSBMODULE_FUNCTOR and p2c_arity(term) == 2 and is_atom(xsb_module):
            term = p2p_arg(term,2)
            name = p2c_functor(term).decode('utf8', errors='ignore')
            xsb_module = p2c_string(xsb_module).decode('utf8', errors='ignore')
        else:
            name = name_maybe
            xsb_module = None

        args_maybe = []
        for i in range(p2c_arity(term)):
            arg = p2p_arg(term, i+1)
            args_maybe.append(hilog_term2py(arg, auto_string=auto_string))

        # Now check if this is a HiLog term
        if name_maybe == HILOG_WRAP:
            name = args_maybe[0]
            args = args_maybe[1:]
            return HILOGFunctor(name=name, args=args)
        elif name_maybe == ERGODATATYPE:
            # \datatype(DT_OBJ, DTNAME)
            dtype_name = args_maybe[1].value.lower()
            if dtype_name == ERGODATETIME:
                # eg, \dateTime(-1,2018,6,27,10,30,55,-1,0,20,...)
                #     for "-2018-6-27T10:30:55-00:20"^^\datetime
                dtype_args = args_maybe[0].args
                return ERGODatetime(date=dtype_args[0:4],time=dtype_args[4:10])
            elif dtype_name == ERGODATE:
                # eg, \date(-1,2018,6,27,...)
                #     for "-2018-6-27"^^\date
                dtype_args = args_maybe[0].args
                return ERGODatetime(date=dtype_args[0:4])
            elif dtype_name == ERGOTIME:
                # eg, \time(10,30,55,-1,0,20,...)
                #     for "10:30:55-00:20"^^\time
                dtype_args = args_maybe[0].args
                return ERGODatetime(time=dtype_args[0:6])
            elif dtype_name == ERGODURATION:
                # eg, \duration(1,22,2,10,1,2,3,....)
                #       for "P0022Y02M10DT01H02M03S"^^\duration
                dtype_args = args_maybe[0].args
                return ERGODuration(value=dtype_args[0:7])
            else:
                # some unrecognized data type:
                #    \datatype(\datatype(DTNAME,content),DTNAME)
                if isinstance(args_maybe[0],ERGOUserDatatype):
                    return args_maybe[0]
                else:
                    return ERGOUserDatatype(name=args_maybe[0],value=args_maybe[1])
        elif name.startswith(ERGO_PREFIX):
            raise PYERGOException(message='Cannot return Ergo terms of this form to Python: %s. Only Prolog or HiLog terms can be returned.' % print_pterm_fun(term).decode("utf8"))
        else:
            args = args_maybe
            return PROLOGFunctor(name=name, module=xsb_module, args=args)

    else:

        int_only = True

        res = []
        while is_list(term):
            r = hilog_term2py(p2p_car(term), auto_string=auto_string)
            res.append(r)
            # FIXED: now checks if r is a Unicode number, not just an int
            if not isinstance(r,int) or r < 0 or r > 0x10FFFF:
                int_only=False
            term = p2p_cdr(term)

        # if not is_nil(term):
        #     res.append(hilog_term2py(term, auto_string=auto_string))
        
        # import pdb; pdb.set_trace()
        if auto_string and int_only and res:
            if is_python3:
                s = u''.join(map(lambda b: chr(b), res))
            else:
                s = u''.join(map(lambda b: unichr(b), res))

            #dbgprint("s=",s)
            res = ERGOCharlist(value=s)
        
        #dbgprint("res=",str(res))
        return res

    raise PYERGOException(message='failed to detect datatype of %s' % print_pterm_fun(term).decode("utf8"))



def pyergo_query(query, auto_string=True):

    from pyxsb import xsb_query_string, reg_term, xsb_close_query, \
        xsb_next, \
        xsb_get_error_type, xsb_get_error_message

    check_active_ergo_session()

    real_query = "flora_call_string_command2prolog('" + \
                 query.replace('\\','\\\\').replace("'","''") + \
                 "',Results,Status,XWamState,Exception)."

    rcode = xsb_query_string(real_query.encode('utf8'))
    res = []

    while not rcode:

        # xsb_query_string returns answers in register 2
        term = reg_term(2)
        # import pdb; pdb.set_trace()

        raw_answer = hilog_term2py(term, auto_string=auto_string)
        #print(str(raw_answer))

        fixedup_answer = simplify_answer(raw_answer[0])
        answer_status  = strip_XSBAtom_ERGOSymbol_from_list(raw_answer[1])
        xwam_state     = True if raw_answer[2] == 0 else None
        ans_exception  = raw_answer[3]

        fixedup_result = (fixedup_answer,answer_status,xwam_state,ans_exception)

        try:
            res.append(fixedup_result)
        except Exception as e:
            xsb_close_query()
            raise e

        rcode = xsb_next()

    # These exceptions never arise: flora_call_string_command2prolog
    # returns Ergo exceptions. Keep just in case.
    if rcode == XSB_ERROR:
        raise PYERGOException(code=rcode,query=query,message="Ergo query: %s error: %s %s" % (query, xsb_get_error_type(), xsb_get_error_message()))
    elif rcode == XSB_OVERFLOW:
        raise PYERGOException(code=rcode,query=query,message="Ergo query: %s overflow." % (query))

    return res

def pyergo_end_session():
    from pyxsb import xsb_close
    xsb_close()
    ergo_session_active=False

def strip_XSBAtom_ERGOSymbol_from_list(list):
    result = []

    if isinstance(list,XSBAtom):
        return tuple(result)
    if isinstance(list,ERGOSymbol):
        return tuple(result)

    for elt in list:
        if isinstance(elt,XSBAtom):
            result.append(elt.name)
        elif isinstance(elt,ERGOSymbol):
            result.append(elt.value)
        else:
            result.append(elt)

    return tuple(result)

def simplify_answer(var_val_pairlist):
    result = []

    if isinstance(var_val_pairlist,XSBAtom):
        return result
    if isinstance(var_val_pairlist,ERGOSymbol):
        return result

    for pair in var_val_pairlist:
        if isinstance(pair,XSBFunctor):
            # pair is Varname=Value
            result.append((pair.args[0].name,pair.args[1]))
        elif isinstance(pair,PROLOGFunctor):
            # pair is Varname=Value
            result.append((pair.args[0].value,pair.args[1]))
        else:
            result.append(pair)
        
    return result


def check_active_ergo_session():
    global ergo_session_active
    if not ergo_session_active:
        raise PYERGOException(message="No active Ergo session -- call pyergo_start_session() first.")


##### Ergo primitive data types
@python_2_unicode_compatible
class ERGOSymbol:
    def __init__ (self, value=None):
        self.value = value

    def __str__(self):
        return "'" + self.value + "'"

    def __repr__(self):
        return 'ERGOSymbol(value=%s)' % self.value

@python_2_unicode_compatible
class ERGOString:
    def __init__ (self, value=None):
        self.value = value

    def __str__(self):
        return '"' + self.value + '"^^\\string'

    def __repr__(self):
        return 'ERGOString(value=%s)' % self.value

@python_2_unicode_compatible
class ERGOCharlist:
    def __init__ (self, value=None):
        self.value = value

    def __str__(self):
        return '"' + self.value + '"^^\\charlist'

    def __repr__(self):
        return 'ERGOCharlist(value=%s)' % self.value

@python_2_unicode_compatible
class ERGOIRI:
    def __init__ (self, value=None):
        self.value = value

    def __str__(self):
        return '"' + self.value + '"^^\\iri'

    def __repr__(self):
        return 'ERGOIRI(value=%s)' % self.value

@python_2_unicode_compatible
class ERGODatetime:
    # date: [Sign,Yr,Mon,Day]
    # time: [Hr,Min,Sec,TzSign,TzHr,TzMin]. Sec is decimal
    # Assume Ergo never sends gibberish like date=time=None
    def __init__ (self, date=None, time=None):
        self.date = date
        self.time = time
        # convert symbol-secs to float
        if time != None:
            self.time[2] = float(time[2].value)

    def __str__(self):
        if self.date!=None:
            sign = '-' if self.date[0]<0 else ''
            daterepr = sign + str(self.date[1]) + '-' \
                       + str(self.date[2]) + '-' + str(self.date[3])
        if self.time!=None:
            sign = '-' if self.time[3]<0 else '+'
            timerepr = str(self.time[0]) + ':' + str(self.time[1]) \
                       + ':' + str(self.time[2]) \
                       + sign \
                       + str(self.time[4]) + ':' + str(self.time[5])
        if self.date==None and self.time!=None:
            return '"' + timerepr + '"^^\\time'
        elif self.date!=None and self.time==None:
            return '"' + daterepr + '"^^\\date'
        else:
            return '"' + daterepr + 'T' + timerepr + '"^^\\datetime'

    def __repr__(self):
        if self.date==None and self.time!=None:
            return 'ERGODatetime(time=%s)' % str(self.time)
        elif self.date!=None and self.time==None:
            return 'ERGODatetime(date=%s)' % str(self.date)
        else:
            return 'ERGODatetime(date=%s,time=%s)' % (str(self.date),str(self.time))


@python_2_unicode_compatible
class ERGODuration:
    # duration: [Sign,Yr,Mon,Day,Hr,Min,Sec]
    # Assume Ergo never sends gibberish like date=time=None
    def __init__ (self, value=None):
        self.value = value
        if value != None:
            self.value[6] = float(value[6].value)

    def __str__(self):
        sign = '-' if self.value[0]<0 else ''
        return '"' + sign + 'P' \
            + str(self.value[1]) + "Y" \
            + str(self.value[2]) + 'M' + str(self.value[3]) + 'DT' \
            + str(self.value[4]) + 'H' + str(self.value[5]) + 'M' \
            + str(self.value[6]) + 'S' + '"^^\\duration'

    def __repr__(self):
        return 'ERGODuration(value=%s)' % str(self.value)


@python_2_unicode_compatible
class ERGOVariable:

    def __init__ (self, name=None):
        self.name = name

    def __str__(self):
        return '?'+self.name

    def __repr__(self):
        return "ERGOVariable(name=?%s)" % self.name


@python_2_unicode_compatible
class ERGOUserDatatype:

    def __init__ (self, name=None, value=None):
        self.name = name
        self.value = value

    def __str__(self):
        return '"' + self.value.value + '"^^' + self.name.value

    def __repr__(self):
        return "ERGOUserDatatype(name=%s,value=%s)" % (self.name,self.value)
