/* File:      FloraSession.java
**
** Author(s): Aditi Pandit
**
** Contact:   see  ../CONTACTS.txt
** 
** Copyright (C)
**      The Research Foundation of SUNY, 2005-2018.
**      Coherent Knowledge Systems, LLC, 2017-2018.
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
import java.io.File;
import java.util.Arrays;
import java.time.LocalDateTime;

import net.sf.flora2.API.util.FlrException;

import com.declarativa.interprolog.PrologEngine;
import com.declarativa.interprolog.TermModel;

/** This class is a higher level wrapper to call the 
   lower level functions of the PrologFlora class
*/
public class FloraSession extends FloraConstants
{
    public PrologFlora flora;

    /* Constructor function. */
    public FloraSession()
    {
	flora =  makePrologFlora();	
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Ergo/Flora-2 session started");
    }
    
    protected PrologFlora makePrologFlora() {
    	return new PrologFlora();	
    }

    // this enables printing stdout
    public static void showOutput() {
        showFloraOutput = true;
        needsMoreFloraOutput = false;
        needsLessFloraOutput = true;
    }
    public static void hideOutput() {
        showFloraOutput = false;
        needsMoreFloraOutput = false;
        needsLessFloraOutput = true;
    }

    // this enables even more stdout output - see adjustFloraOutput()
    // which changes the verbosity of the output.
    public static void showMoreOutput() {
        showFloraOutput = true;
        needsMoreFloraOutput = true;
        needsLessFloraOutput = false;
    }
    public static void showLessOutput() {
        needsMoreFloraOutput = false;
        needsLessFloraOutput = true;
    }

    public static void enableDebugging() {
        debug = true;
    }
    public static void disableDebugging() {
        debug = false;
    }

    public static void enableLogging() {
        logging = true;
    }
    public static void disableLogging() {
        logging = false;
    }

    public boolean hasErrors() {
        return flora.hasErrors();
    }

    public boolean hasWarnings() {
        return flora.hasWarnings();
    }

    /* Execute a command at the Flora-2 session
    ** True, if the command succeeds
    **
    ** command : the command to be executed
    ** Returns: true/false, depending of whether "command" succeeded or failed.
    **
    ** @deprecated Use executeCommand() instead
    */
    @Deprecated
    public boolean ExecuteCommand(String command) {
        return executeCommand(command);
    }

    public boolean executeCommand(String command)
    {
	boolean result = false;
	try {
	    result = flora.simpleFloraCommand(adjustCmdString(command));
	    if (debug || logging)
		System.out.println(LocalDateTime.now()+
                                   "  executeCommand: "+command);
	}
	catch (FlrException e) {
	    throw e;
	}
	catch (Exception e) {
	    throw new FlrException("Executing Ergo/Flora-2 command " + command, e);
	}
	return result;
    }
	
    /* Execute a command at the Flora-2 session 
    ** The answer is a resultset that can be queried
    **
    ** query : Flora query to be executed 
    ** vars : Vector of variables to be bound
    */
    public Iterator<HashMap<String, FloraObject>> findAllMatches(String query,Vector<String> vars)
    {
	Vector<HashMap<String,FloraObject>> retBindings = new Vector<HashMap<String,FloraObject>>();
	Object[] bindings = null;
		
	try {
	    if (debug) {
		System.out.println("findAllMatches, before floraCommand. Query = " + query);
		System.out.println("findAllMatches, before floraCommand. Vars = " + vars);
	    }
	    bindings = flora.floraCommand(adjustCmdString(query),vars);
	    if (debug) {
		System.out.println("findAllMatches, after floraCommand. bindings =  " + Arrays.toString(bindings));
	    }
	 
	    for (int i=0; i<bindings.length; i++) {
				
		TermModel tm = (TermModel)bindings[i];
		HashMap<String,FloraObject> currBinding = new HashMap<String,FloraObject>();
		for (int j=0; j<vars.size(); j++) {
		    String varValue = vars.elementAt(j);

		    if (debug) {
			System.out.println("findAllMatches, term model: " + tm);
			System.out.println("findAllMatches, varValue="+varValue);
		    }

		    TermModel objName = PrologFlora.findValue(tm,varValue);
		    FloraObject obj = new FloraObject(objName,this);
		    currBinding.put(varValue,obj);		
		}		
		retBindings.add(currBinding);
	    }
	}
	catch (FlrException e) {
	    throw e;
	}
	catch (Exception e) {
	    throw new FlrException("Executing Ergo/Flora-2 query " + query, e);
	}

	Iterator<HashMap<String, FloraObject>> iter = retBindings.iterator();
	return iter;
    }

    
    public void close()
    {
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Ergo/Flora-2 session ended");
	flora.close();
    }

    
    /* Load a Flora-2 file into a module
    ** fileName : file to be loaded 
    ** moduleName : module name to load the file into
    */
    public boolean loadFile(String fileName,String moduleName)
    {
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Loading file " + fileName + 
                               " into module " + moduleName);
	return flora.loadFile(fileName,moduleName);
    }
    
    
    /* Compile a Flora-2 file for a module
    ** fileName : file to be compiled 
    ** moduleName : module name to compile the file for
    */
    public boolean compileFile(String fileName,String moduleName)
    {
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Compiling file " + fileName + 
                               " for module " + moduleName);
	return flora.compileFile(fileName,moduleName);
    }


    /* Add a Flora-2 file to an existing module
    ** fileName : file to be added 
    ** moduleName : module name to add the file to
    */
    public boolean addFile(String fileName,String moduleName)
    {
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Adding file " + fileName + 
                               " to module " + moduleName);
	return flora.addFile(fileName,moduleName);
    }


    /* Compile a Flora-2 file for addition to a module
    ** fileName : file to be compiled 
    ** moduleName : module name to compile the file for
    */
    public boolean compileaddFile(String fileName,String moduleName)
    {
	if (debug || logging)
	    System.out.println(LocalDateTime.now()+
                               "  Compiling file " + fileName + 
                               " for adding to module " + moduleName);
	return flora.compileaddFile(fileName,moduleName);
    }
    
    /** Delegates to same method in PrologFlora. 
    @see net.sf.flora2.API.PrologFlora#setLoadProgressHandler(String) */
    public void setLoadProgressHandler(String handler){
    	flora.setLoadProgressHandler(handler);
    }
    
    /** Delegates to same method in PrologFlora. 
    @see net.sf.flora2.API.PrologFlora#setLoadProgressHandler(String,double) */
    public void setLoadProgressHandler(String handler,double period){
    	flora.setLoadProgressHandler(handler,period);
    }
        
    /* Execute a query with variables
    **
    ** query : query to be executed
    ** vars : variables in the query
    **
    ** @deprecated Use executeQuery() instead
    */
    @Deprecated
    public Iterator<HashMap<String,FloraObject>> ExecuteQuery(String query,Vector<String> vars) {
        return executeQuery(query,vars);
    }

    public Iterator<HashMap<String,FloraObject>> executeQuery(String query,Vector<String> vars)
    {
	if (debug || logging)
	    System.out.println("\n"+LocalDateTime.now()+
                               "  executeQuery: " + query + 
                               " with parameters " +
                               Arrays.toString(vars.toArray(new String[vars.size()])));
	return findAllMatches(query,vars);
    }

    
    /*
    ** Like executeQuery/2 above, but is used only when a query has
    ** just one variable. This provides a simplified interface, since
    ** no variables need to be passed into executeQuery/1 and the
    ** output is just an iterator, which contains just a list of
    ** bindings for that single variable.
    **
    ** query : query to be executed
    **
    ** @deprecated Use executeQuery() instead
    */
    @Deprecated
    public Iterator<FloraObject> ExecuteQuery(String query) {
        return executeQuery(query);
    }

    public Iterator<FloraObject> executeQuery(String query)
    {
	Vector<FloraObject> retBindings = new Vector<FloraObject>();
	Object[] bindings = null;
		
	Vector<String> vars = new Vector<String>();
	vars.add("?");
	if (debug || logging)
	    System.out.println("\n"+LocalDateTime.now()+
                               "  executeQuery: " + query);

	try {
	    bindings = flora.floraCommand(adjustCmdString(query),vars);
			
	    for (int i=0; i<bindings.length; i++) {
		TermModel tm = (TermModel)bindings[i];
		TermModel objName = PrologFlora.findValue(tm,null);
		if (debug) {
		    System.out.println("executeQuery/1, term model: " + tm);
		    System.out.println("executeQuery/1, objName="+objName);
		}
		FloraObject obj = new FloraObject(objName,this);
		retBindings.add(obj);
	    }
	}
	catch(FlrException e) {
	    throw e;
	}
	catch(Exception e) {
	    throw new FlrException("Executing Ergo/Flora-2 query " + query, e);
	}
	Iterator<FloraObject> iter = retBindings.iterator();
	return iter;
    }	
    
    public PrologEngine getEngine()
    {
    	return flora.engine;
    }

    /* Utility to double each quote */
    private String doubleEachQuote(String str)
    {
	String outstr = "";

	for (int i=0; i<str.length(); i++) {
	    char ch = str.charAt(i);
	    if (ch == '\'')
		outstr += "''";
	    else
		outstr += String.valueOf(ch);
	}
	return outstr;
    }

    /* This is here in case we'll need to preprocess the command string.
       Previously we doubled the quotes, but this was wrong.
       For instance, in a query like
         p('a b').
       if we double the quotes, we'll have an incorrect query
         p(''a b'').
       Now we don't change the string, but will see if something needs
       to be done in the future.
    */
    private String adjustCmdString(String str)
    {
        return str;
    }
}
