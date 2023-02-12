#!/usr/bin/env python
# -*- coding: utf-8 -*- 

#
# Copyright 2017, 2018 Guenter Bartsch
# Copyright 2018 Michael Kifer, Annie Liu, David Warren (XSB team at Stony Brook University of New York)
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

#
# demo using the high level XSB interface
#


import os
import platform
import sys

sys.path.append('..')

from pyxsb import pyxsb_start_session, pyxsb_end_session, pyxsb_command, \
                  pyxsb_query, XSBFunctor, XSBVariable, xsb_to_json, json_to_xsb

# Linux, Windows, Darwin
our_platform = platform.system()

if our_platform == 'Windows':
    # testing: mix of / and \
    XSB_ARCH_DIR_MK_CAND = 'H:/XSB\XSB\config/x64-pc-windows'
    # Annie's
    XSB_ARCH_DIR_ANNIE_CAND = 'c:/Program Files (x86)/XSB/config/x64-pc-windows'
    if os.path.isdir(XSB_ARCH_DIR_MK_CAND.replace('\\','/')):
        XSB_ARCH_DIR = XSB_ARCH_DIR_MK_CAND
    elif os.path.isdir(XSB_ARCH_DIR_ANNIE_CAND.replace('\\','/')):
        XSB_ARCH_DIR = XSB_ARCH_DIR_ANNIE_CAND
    else:
        raise Exception ("XSB_ARCH_DIR is not set")
elif os.path.isdir('/opt/xsb-3.8.0/config/x86_64-redhat-linux-gnu'):
    XSB_ARCH_DIR = '/opt/xsb-3.8.0/config/x86_64-redhat-linux-gnu'
elif our_platform == 'Linux':
    XSB_ARCH_DIR = os.environ["HOME"] + '/XSB/XSB/config/x86_64-unknown-linux-gnu'
else:
    XSB_ARCH_DIR = os.environ["HOME"] + '/XSB/XSB/config/i386-apple-darwin17.3.0'

pyxsb_start_session()
pyxsb_command('set_prolog_flag(character_set,utf_8).')

# simple string-based interface

pyxsb_command('[curl].')
pyxsb_command("load_page(url('http://www.google.com'),[],_Prop,R,W).")
# pyxsb_command('fail.') 
pyxsb_query('catch(abort,Exception,true).')

pyxsb_command('consult(ft).')

for row in pyxsb_query('label(X, L).'):
    print(u"label of %s is %s" % (row[0], row[1]))

# structured XSB* interface

for row in pyxsb_query(XSBFunctor('descend', [XSBVariable('X'), XSBVariable('Y')])):
    print(u"decendant of %s is %s" % (row[0], row[1]))

for row in pyxsb_query(u'A = 1, B = 0.5, C = "hello", D = yes, E = foo(bar), F = [1.1,2.2], G = \'günter\'.'):

    for i, r in enumerate(row):
        print(u"#%d: %-10s (type: %-20s, class: %-20s)" % (i, r, type(r), r.__class__))

    js = xsb_to_json(row)
    print("json: %s" % js)

    row2 = json_to_xsb(js)
    print("restored: %s" % str(row2))

pyxsb_query('catch(abort,Exception,true).')

try:
    # throw has no effect
    pyxsb_query("writeln('Trying to throw up:'), throw('test throw').")
except:
    print('Test throw -- caught')


# testing abort
try:
    pyxsb_query("abort('Test abort').")
except:
    print('Test abort -- caught')

print('Testing complex explicit module names')
pyxsb_command('assert(ppp(simplemod:abcde(p,q,r))).')
pyxsb_command('assert(ppp(mod(arg1,f(arg2)):abc(p,q,r))).')
pyxsb_command('assert(ppp(mod2(arg,g(arg)):cde(p,q,r))).')
for row in pyxsb_query("ppp(X)."):
    print('answer: ', row[0])

# Close connection 
pyxsb_end_session()
