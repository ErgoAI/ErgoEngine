"""
   THIS FILE IS IN THE PUBLIC DOMAIN.

   IT IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
   INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
   IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
   OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
   ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
   OTHER DEALINGS IN THE SOFTWARE.
"""

# Change these according to your system.
# ERGOROOT is obtained by asking Ergo the query system{installdir=?Ins}.
# XSBARCHDIR is obtained via the query system{archdir=?Arch}.
import os

# **** Use of forward slashes is advised, even on Windows
# **** If one insists on backward slashes, they sometimes MUST be escaped with
# **** additional backward slashes to satisfy Python syntax.
HOMEDIR = os.environ['HOME'].replace('\\','/')
ERGOROOT = HOMEDIR + '/ERGOAI/ErgoAI'
XSBARCHDIR = HOMEDIR + '/XSB/XSB/config/x86_64-unknown-linux-gnu'

# Tell Python where the Py-Ergo interface is
# Backslash replacement here serves just as an example.
# Generally both ERGOROOT and XSBARCHDIR may need to undergo backslash
# replacement, depending on how these were set.
# In this example, we already replaced backslashes in HOMEDIR earlie.
import sys
sys.path.append(ERGOROOT.replace('\\','/') + '/python')

from pyergo import \
pyergo_start_session, pyergo_end_session, \
pyergo_command, pyergo_query, \
HILOGFunctor, PROLOGFunctor, \
ERGOVariable, ERGOString, ERGOIRI, ERGOSymbol, \
ERGOIRI, ERGOCharlist, ERGODatetime, ERGODuration, ERGOUserDatatype, \
pyxsb_query, pyxsb_command, \
XSBFunctor, XSBVariable, XSBAtom, XSBString, \
PYERGOException, PYXSBException

pyergo_start_session(XSBARCHDIR,ERGOROOT)

# ErgoAI/demos/family_obj.flr is a simple KB found in the ErgoAI installation.
# It is about all kinds of family relationships.
# One is a transitive relationship called 'ancestor', which we query below.
pyergo_command("['demos/family_obj'].")

# result gets a list of answers. Each element in the list (ans, below) is a
# 4-tuple: (variable-binding-list, compile-status, truth-value, exception-info)
# variable-binding-list is [(varname1,varbinding1), (varname2,varbinding2), ...]
# Truth-value can be True or Undefined. False answers are not returned,
# of course. For compile-status and exception-info, see the manual.
result = pyergo_query('?X[ancestor->?Y].')

for ans in result:
    [(XVarName,XVarVal),(YVarName,YVarVal)] = ans[0]
    print('ancestor of ' + XVarName + '=' + str(XVarVal) \
          + ' is ' + YVarName + '=' + str(YVarVal))

pyergo_end_session()
