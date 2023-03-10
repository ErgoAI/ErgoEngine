/* File:      FloraObject.java
**
** Author(s): Aditi Pandit, Michael Kifer
**
** Contact:   see  ../CONTACTS.txt
** 
** Copyright (C) The Research Foundation of SUNY, 2005-2018.
** 
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**      http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
**
** 
*/

package net.sf.flora2.API;
 
import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

import net.sf.flora2.API.util.FlrException;

import com.declarativa.interprolog.TermModel;

/** This class is the glue between the high-level generated Java
   classes and the low-level interface. It contains the logic to
   translate calls from the high-level Java classes to F-logic queries
   to be called using the lower-level interface
*/

public class FloraObject extends FloraConstants
{
    FloraSession session;
    // floraOID is set by proxy object constructors within
    // the files automatically generated by %write(?,?,?)
    public TermModel floraOID;

    public FloraObject(TermModel name,FloraSession sess)
    {
	floraOID = name;
	session = sess;
    }

    public FloraObject(String name, FloraSession sess)
    {
	String oid = name.trim();
	session = sess;

	if (oid.endsWith(")")) {
	    // oid was a complex term - convert it into a TermModel object
	    String query = "buildTermModel(("+oid+"),Model)";
	    floraOID =
		(TermModel)sess.flora.engine.deterministicGoal(query,"[Model]")[0];
	} else
	    // oid was not a complex term
	    floraOID = new TermModel(oid);
    }


    public String toString ()
    {
	if (floraOID.isAtom() || floraOID.isVar()
	    /*|| floraOID.isList()*/ || floraOID.isNumber())
	    return floraOID.toString();
	/*
	else {
	    // ensure that HiLog terms are printed without flapply
	    String query =
		"flora_plg2hlg(PTerm, (" + this.floraOID + "), "
		+ WRAP_HILOG + ", 0), buildTermModel(PTerm,Model)";
	    return
		((TermModel)session.flora.engine.deterministicGoal(query,"[Model]")[0]).toString();
	}
	*/

	/*
	String query =
	    "flora_decode_oid_as_atom((" + toQuotedString(this.floraOID) + "),Printable), "
	    + "buildTermModel(Printable,Model)";
	return ((TermModel)session.flora.engine.deterministicGoal(query,"[Model]")[0]).toString();
	*/
	
	Object[] objectsP = {this.floraOID};
	// recoverTermModel takes TermModel, converts into Prolog term, 
	// then we passit to prolog call flora_decode_oid_as_atom/2
	String query =
	    "recoverTermModel(OID,OIDterm), flora_decode_oid_as_atom((OIDterm),Printable), buildTermModel(Printable,Model)";
	return ((TermModel)session.flora.engine.deterministicGoal(query,"[OID]",objectsP,"[Model]")[0]).toString();
    }

    /* This is a modified method from Interprolog's TermModel.java */
    private static String toQuotedString (TermModel modObj) {
	String nodeString =
	    (modObj.node instanceof String ?
	     "'"+modObj.node.toString()+"'" : modObj.node.toString());
	if (modObj.getChildCount()==0) 
	    return  nodeString;
	else if (modObj.isList()) {
	    return listToQuotedString(modObj);
	} else {
	    StringBuffer s=
		new StringBuffer(nodeString+"("
				 +toQuotedString(modObj.children[0]));
	    for (int i=1;i<modObj.children.length;i++){
		s.append(","+toQuotedString(modObj.children[i]));
	    }
	    return s+")";
	}
    }

    /* This is a modified method from Interprolog's TermModel.java */
    private static final int listMaxLength=1000;
    private static String listToQuotedString(TermModel term) {
	int i;
	StringBuffer s = new StringBuffer("[");
	TermModel temp = term;
	for( i = 0 ; i < listMaxLength ; i++ ){
	    s.append(toQuotedString(temp.children[0])); // head
	    temp = temp.children[1];
	    if (temp.isListEnd()) break;
	    if( ! temp.isList() ) break ; // tail is not a list
	    s.append(',') ;
	}
	if( i == listMaxLength )
	    s.append("...");
        else if ( ! temp.isListEnd() ) {
	    s.append('|') ;
	    s.append(toQuotedString(temp)); 
	}
	return s + "]";
    }


    /* Get a boolean property of the object
    **
    ** methodName is the name of the boolean method
    ** inherit: whether the method is inheritable or not
    ** parameters: parameters passed to the method
    */
    public boolean getboolean(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
	   
	String floraQuery =
	    floraOID + open_bracket
	    + arrow
	    + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Command failed, " + floraQuery + "\n");
	}
    }


    /* Get all values for a Boolean method where the arguments might be unbound.
    ** For instance, obj[m(aaa,?Y)] \or obj[|m(aaa,?Y)|]
    **
    ** methodName  : name of method whose value is to be got
    ** inherit     : whether the method is inheritable or not 
    ** parameters  : the parameters for the method
    */
    public Iterator<HashMap<String,FloraObject>> getbooleanAll(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String floraQuery = "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";
	
	// add variable arguments of method to get their bindings out
	Vector<String> argList = new Vector<String>();
	int noOfPars = parameters.size();
	for (int i=0; i<noOfPars; i++) {
	    String param = parameters.elementAt(i).toString();
	    if (param.startsWith("?")) argList.add(param);
	}
		
	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
	   
	floraQuery =
	    floraOID + open_bracket + arrow 
	    + methodName.toString() + paramList 
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.findAllMatches(floraQuery,argList);
	}
	catch(Exception e) {
	    e.printStackTrace();	
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getbooleanAll(). Query was "
				   + floraQuery + "\n");
	}
    }



    /* Set a boolean property of the object
    **
    ** methodName : name of the boolean method
    ** inherit: whether the method is inheritable or not
    ** parameters: parameters passed to the method
    */
    public boolean setboolean(String moduleName,Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	String floraQuery=
	    "insert{" + floraOID + open_bracket
	    + arrow
	    + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + "}.";

	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Command failed, " + floraQuery + "\n");
	}
    }

    
    /* Delete a boolean property of the object
    **
    ** methodName : name of the boolean method
    ** inherit: whether the method is inheritable or not
    ** parameters: parameters passed to the method
    */
    public boolean deleteboolean(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	String floraQuery =
	    "deleteall{" + floraOID + open_bracket
	    + arrow
	    + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + "}.";

	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Command "
				   + floraQuery + " failed\n");
	}
    }

    
    /* Get a procedural property of the object
    **
    ** methodName is the name of the boolean method
    ** inherit: ignored - for future use
    ** parameters: parameters passed to the method
    */
    public boolean getprocedural(String moduleName,Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	String floraQuery =
	    floraOID + open_bracket
	    + arrow + " "
	    + PROCEDURAL_METHOD_SYMBOL + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Command "
				   + floraQuery + " failed");
	}
    }


    /* Get all values for a procedural method. The arguments might be unbound.
    ** For instance, obj[m(aaa,?Y)] \or obj[|m(aaa,?Y)|]
    **
    ** methodName  : name of method whose value is to be got
    ** inherit     : whether the method is inheritable or not 
    ** parameters  : the parameters for the method
    */
    public Iterator<HashMap<String,FloraObject>> getproceduralAll(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String floraQuery = "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";
	
	// add variable arguments of method to get their bindings out
	Vector<String> argList = new Vector<String>();
	int noOfPars = parameters.size();
	for (int i=0; i<noOfPars; i++) {
	    String param = parameters.elementAt(i).toString();
	    if (param.startsWith("?")) argList.add(param);
	}
		
	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
	   
	floraQuery =
	    floraOID + open_bracket + arrow + " "
	    + PROCEDURAL_METHOD_SYMBOL
	    + methodName.toString() + paramList 
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.findAllMatches(floraQuery,argList);
	}
	catch(Exception e) {
	    e.printStackTrace();	
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getproceduralAll(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Set a procedural property of the object
    **
    ** methodName : boolean method
    ** inherit: ignored - for future use
    ** parameters: parameters passed to the method
    */
    public boolean setprocedural(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	String floraQuery=
	    "insert{" + floraOID + open_bracket + arrow + " "
	    + PROCEDURAL_METHOD_SYMBOL
	    + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + "}.";
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Command "
				   + floraQuery + " failed\n");
	}
    }

    
    /* Delete a procedural property of the object
    **
    ** methodName : name of the boolean method
    ** inherit: ignored - for future use
    ** parameters: parameters passed to the method
    */
    public boolean deleteprocedural(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = "";
	else
	    arrow = DATA_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	String floraQuery=
	    "deleteall{" + floraOID + open_bracket + arrow + " "
	    + PROCEDURAL_METHOD_SYMBOL
	    + methodName.toString()
	    + paramList
	    + close_bracket + AT_MODULE_SYMBOL + moduleName + "}.";
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in deleteprocedural(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Delete a single value from method's result
    ** methodName : name of method 
    ** inherit    : whether the method is inheritable or not 
    ** parameters : the parameters to the method
    ** value      : the value of the method to be deleted
    */
    public boolean deletevalue(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters,Object value)
    {
	String floraQuery= "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";

	String arrow = "";
	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
		
	floraQuery =
	    "deleteall{"+floraOID + open_bracket
	    + methodName.toString() + paramList
	    + arrow + value + close_bracket
	    + AT_MODULE_SYMBOL + moduleName + "}.";
		
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in deletevalue(). Query was "
				   + floraQuery + "\n");
	}
    }

	
    /* Get all values for a method where the arguments might be unbound.
    ** For instance, obj[m(aaa,?Y) -> ?Z] or obj[|m(aaa,?Y) -> ?Z|]
    **
    ** methodName  : name of method whose value is to be got
    ** inherit     : whether the method is inheritable or not 
    ** parameters  : the parameters for the method
    */
    public Iterator<HashMap<String,FloraObject>> getvalueAll(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String floraQuery = "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";
	
	// add variable arguments of method to get their bindings out
	Vector<String> argList = new Vector<String>();
	int noOfPars = parameters.size();
	for (int i=0; i<noOfPars; i++) {
	    String param = parameters.elementAt(i).toString();
	    if (param.startsWith("?")) argList.add(param);
	}
		
	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
	   
	// Use weird name Value___395792 to avoid clashes with argList vars
	floraQuery =
	    floraOID + open_bracket + methodName.toString()
	    + paramList + arrow
	    + "?Value___395792" + close_bracket
	    + AT_MODULE_SYMBOL + moduleName + ".";
	argList.add("?Value___395792");
		
	try {
	    return session.findAllMatches(floraQuery,argList);
	}
	catch(Exception e) {
	    e.printStackTrace();	
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getvalueAll(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all values for a method, where all arguments are bound.
    **
    ** methodName  : name of method whose value is to be got
    ** inherit     : whether the method is inheritable or not 
    ** parameters  : the parameters for the method
    */
    public Iterator<FloraObject> getvalue(String moduleName,Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String floraQuery = "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	
	String arrow = "";
	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
	      
	floraQuery =
	    floraOID + open_bracket + methodName.toString()
	    + paramList + arrow
	    + "?Value" + close_bracket + AT_MODULE_SYMBOL + moduleName + ".";

	if (debug)
	    System.out.println("In get/3, floraQuery: " + floraQuery);
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Method "
				   + methodName
				   + " failed on object " + floraOID
				   + " in module " + moduleName + "\n");
	}
    }
	

    /* Delete a set of values from a method's result
    ** methodName : method 
    ** inherit    : whether the method is inheritable or not
    ** parameters : the parameters to the method 
    ** value      : the values of the method to be deleted
    */
    public boolean deletevalue(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters,Vector<Object> value)
    {
	String floraQuery= "";
	String paramList = makeParameterListString(parameters);
	String valuelist = makeValueListString(value);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
		
	floraQuery =
	    "deleteall{" + floraOID + open_bracket + methodName.toString()
	    + paramList + arrow+valuelist + close_bracket
	    + AT_MODULE_SYMBOL + moduleName + "}.";
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in deletevalue(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Delete all values for a method
    **
    ** methodName : name of method
    ** inherit    : whether the method is inheritable or not
    ** parameters : the parameters to the method
    */
    public boolean deletevalue(String moduleName, Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters)
    {
	String floraQuery= "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";
	
	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
		
	floraQuery =
	    "deleteall{" + floraOID + open_bracket + methodName.toString()
	    + paramList + arrow + "?Value___395792" + close_bracket
	    + AT_MODULE_SYMBOL + moduleName + "}.";
		
	try {
	    return session.executeCommand(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in deletevalue(). Query was "
				   + floraQuery + "\n");
	}
    }


     /* Add a single value to method
     **
     ** methodName : method whose value is to be set
     ** inherit    : whether the method is inheritable
     ** parameters : parameters to the method
     ** value      : value to be set to 
     */
    public boolean setvalue(String moduleName,Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters,Object value)
    {
	String floraQuery= "";
	String paramList = makeParameterListString(parameters);
	String open_bracket = "", close_bracket = "";
	String arrow = "";
	
	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
		
	floraQuery =
	    "insert{"+floraOID + open_bracket + methodName + paramList
	    + arrow + value.toString() + close_bracket
	    + AT_MODULE_SYMBOL + moduleName + "}.";
		
	try {
	    return session.executeCommand(floraQuery);
	} catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in setvalue(). Query was "
				   + floraQuery + "\n");
	}
    }

	
    /* Add multiple values
    **
    ** methodName   : method to be set 
    ** inherit      : whether the method is inheritable or not 
    ** parameters   : parameters to the method 
    ** value        : vector of values to be set to
    */
    public boolean setvalue(String moduleName,Object methodName,boolean inherit,boolean isDataAtom,Vector<Object> parameters,Vector<Object> value)
    {
	String floraQuery= "";
	String paramList = makeParameterListString(parameters);
	String valuelist = makeValueListString(value);
	String open_bracket = "", close_bracket = "";
	String arrow = "";

	if (isDataAtom)
	    arrow = DATA_ARROW;
	else
	    arrow = SIGNATURE_ARROW;
	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}
		
	floraQuery =
	    "insert{"+floraOID + open_bracket
	    + methodName.toString() + paramList
	    + arrow + valuelist + close_bracket
	    + AT_MODULE_SYMBOL + moduleName +"}.";
	try {
	    return session.executeCommand(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in setvalue(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all instances of of this object */
    public Iterator<FloraObject> getInstances(String moduleName)
    {
	String floraQuery =
	    "?Object" + ISA_SYMBOL + this.toString()
	    + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getInstances(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all instances of the object that are it's 
    ** direct instances and not instances of any subclasses
    */
    public Iterator<FloraObject> getDirectInstances(String moduleName)
    {
	// Construct a query of the form ?X:C, not ((?_S::C, ?X:?_S)).
	String floraQuery =
	    "( ?Object" + ISA_SYMBOL + this.toString()
	    + ", not ((?__Subcl" + SUBCLASS_SYMBOL
	    + this.toString() + ", ?Object"
	    + ISA_SYMBOL + "?__Subcl)) )"
	    + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch(Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getDirectInstances(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all SubClasses of this object */
    public Iterator<FloraObject> getSubClasses(String moduleName)
    {
	String floraQuery =
	    "?Class" + SUBCLASS_SYMBOL + this.toString()
	    + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getSubClasses(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all direct SubClasses of this object */
    public Iterator<FloraObject> getDirectSubClasses(String moduleName)
    {
	// construct query of the form ?S :: SC, not ((?S::?_Mid, ?_Mid::SC)).
	String floraQuery =
	    "( ?Class " + SUBCLASS_SYMBOL + this.toString()
	    + ", not (( ?Class" + SUBCLASS_SYMBOL
	    + "?__Mid, ?__Mid" + SUBCLASS_SYMBOL + this.toString() + " )))"
	    + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in geDirecttSubClasses(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all SuperClasses of this object */
    public Iterator<FloraObject> getSuperClasses(String moduleName)
    {
	String floraQuery =
	    this.toString() + SUBCLASS_SYMBOL
	    + "?Class" + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getSuperClasses(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all direct SuperClasses of this object */
    public Iterator<FloraObject> getDirectSuperClasses(String moduleName)
    {
	// construct query of the form SC::?S, not ((SC::?_Mid, ?_Mid::?S)).
	String floraQuery =
	    "( " + this.toString() + SUBCLASS_SYMBOL + "?Class, "
	    + "not ((" + this.toString() + SUBCLASS_SYMBOL
	    + "?__Mid, ?__Mid" + SUBCLASS_SYMBOL + "?Class)))"
	    + AT_MODULE_SYMBOL + moduleName + ".";
		
	try {
	    return session.executeQuery(floraQuery);
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getDirectSuperClasses(). Query was "
				   + floraQuery + "\n");
	}
    }


    /* Get all the declared methods of the class to which this object belongs
    **
    ** moduleName - module in which the methods are defined
    */
    public Iterator<FloraMethod> getMethods(String moduleName)
    {
	Vector<FloraMethod> allMethods = new Vector<FloraMethod>();
		
	Iterator<FloraMethod> methodIter = getMethods(moduleName,NONINHERITABLE,VALUE);
	while(methodIter.hasNext())
	    allMethods.add((methodIter.next()));
			
	methodIter = getMethods(moduleName,INHERITABLE,VALUE);
	while(methodIter.hasNext())
	    allMethods.add((methodIter.next()));
			
	methodIter = getMethods(moduleName,INHERITABLE,BOOLEAN);
	while(methodIter.hasNext())
	    allMethods.add((methodIter.next()));
			
	methodIter = getMethods(moduleName,NONINHERITABLE,BOOLEAN);
	while(methodIter.hasNext())
	    allMethods.add((methodIter.next()));
			
	methodIter = getMethods(moduleName,NONINHERITABLE,PROCEDURAL);
	while(methodIter.hasNext())
	    allMethods.add((methodIter.next()));
			
	return allMethods.iterator();
    }
		

    /* Get methods of a particular type (inheritable or
    ** noninheritable, boolean, ptocedural, etc.).
    **
    ** inherit : indicates if the method is inheritable
    ** methType:  VALUE, BOOLEAN, PROCEDURAL
    */
    private Iterator<FloraMethod> getMethods(String moduleName, boolean inherit, int methType)
    {
	String callModifier = ""; // *, %, or nothing
	String valuePart = "";    // => ?Val for data symbols
	String open_bracket = "", close_bracket = "";

	checkMethodType(methType);

	if (inherit) {
	    open_bracket = "[|"; close_bracket = "|]";
	} else {
	    open_bracket = "["; close_bracket = "]";
	}

	if (methType == VALUE) {
	    valuePart = SIGNATURE_ARROW + "?Value";
	} else if (methType == BOOLEAN) {
	    callModifier = SIGNATURE_ARROW;
	} else if (methType == PROCEDURAL) {
	    if (inherit)
		throw new FlrException("\n*** Java-Flora-2 interface: Invalid method type -- procedural+inheritable methods are not supported\n");
	    else
		callModifier =
		    SIGNATURE_ARROW + PROCEDURAL_METHOD_SYMBOL;
	}
			
	Vector<String> vars = new Vector<String>();
	vars.add("?Method");
	vars.add("?Value"); // shouldn't matter if ?Value is not used
	vars.add("?Arguments");
		
	String floraQueryString =
	    "(" + this.toString()
	    + open_bracket + callModifier + "?__Call" + valuePart + close_bracket
	    + ", (?__Call =.. [hilog(?Method) | ?Arguments] "
               + "\\or (?__Call =.. [?Method | ?Arguments], atom(?Method)))"
	    + ")" + AT_MODULE_SYMBOL + moduleName + ".";
		
	Iterator<HashMap<String, FloraObject>> methodMatches;
	Vector<FloraMethod> returnMethodVec = new Vector<FloraMethod>();
	try {
	    methodMatches = session.findAllMatches(floraQueryString,vars);
	    HashMap<String,FloraObject> firstmatch;
	    while (methodMatches.hasNext()) {	
		firstmatch = methodMatches.next();
		FloraObject methodObj = firstmatch.get("?Method");

		// unbound, if ?Value isn't used for procedural/bool
		FloraObject returnTypeObj = firstmatch.get("?Value");
		if (returnTypeObj.floraOID.isVar()) returnTypeObj = null;

		TermModel methodArgs = firstmatch.get("?Arguments").floraOID;
				
		Vector<FloraObject> methodPars = new Vector<FloraObject>();
		while (!methodArgs.isLeaf()) {
		    methodPars.add(new FloraObject((TermModel)methodArgs.getChild(0),session));
		    methodArgs = (TermModel) methodArgs.getChild(1);
		}
		returnMethodVec.add(new FloraMethod(methodObj,methodPars,
						    returnTypeObj,inherit,methType));
	    }		
	}
	catch (Exception e) {
	    e.printStackTrace();
	    throw new FlrException("\n*** Java-Flora-2 interface: Error in getMethods(). Query was "
				   + floraQueryString + "\n");
	}
	return returnMethodVec.iterator();
    }

	
    private String makeParameterListString(Vector<Object> parameters)
    {
	String paramList = "(";
	int noOfPars = parameters.size();
		
	for (int i=0; i<noOfPars; i++)
	    paramList +=
		parameters.elementAt(i).toString()+(i < noOfPars-1 ? "," : ")");

	if (noOfPars==0) paramList = "";

	return paramList;
    }


    private String makeValueListString(Vector<Object> value)
    {
	String valuelist = "{";
	int noOfVals = value.size();

	for (int i=0; i<noOfVals; i++)
	    valuelist +=
		value.elementAt(i).toString()+(i < noOfVals-1 ? "," : "");
	valuelist += "}";

	return valuelist;
    }
}
