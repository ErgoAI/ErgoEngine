/* File:      PrologFlora.java
**
** Author(s): Aditi Pandit
**
** Contact:   see  ../CONTACTS.txt
** 
** Copyright (C)
**      The Research Foundation of SUNY, 2005-2017.
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

import java.io.File;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Set;
import java.util.Vector;
import java.util.Arrays;

//import org.apache.log4j.Level;
//import org.apache.log4j.Logger;

import net.sf.flora2.API.util.FlrException;

import com.declarativa.interprolog.PrologEngine;
import com.declarativa.interprolog.PrologOutputListener;
import com.declarativa.interprolog.TermModel;
import com.declarativa.interprolog.XSBSubprocessEngine;
import com.declarativa.interprolog.util.IPException;
import com.declarativa.interprolog.util.OutputListener;
import com.xsb.interprolog.NativeEngine;

/** This class is used to call Flora-2 commands 
    at a low level from JAVA using Interprolog libraries */
public class PrologFlora extends FloraConstants
{
    public static String sFloraRootDir = null;
    XSBSubprocessEngine engine = null;
    protected String commands[];
    boolean engineInitialized = false;


    //private static Logger logger = Logger.getLogger(PrologFlora.class);

    
    HashMap<Integer,Exception> exceptionStore = new HashMap<Integer,Exception>();
    int numExceptions = 0;
    
    private String loadProgressHandlerPredicate = null;
    private long loadProgressPeriod = 0; // mS

    class PrologMessageListener implements PrologOutputListener {
        // collects lines that contain errors and warnings
        private StringBuffer sb = new StringBuffer();
        
        public void clear(){
            sb = new StringBuffer();
        }
        
        @Override
        public void printStderr(final String str) {
            if (str.contains("++Warning[") || str.contains("++Error[") || str.contains("++Abort["))
                sb.append(str);
            if (showFloraOutput)
                System.out.println(str);
        }
        
        @Override
        public void printStdout(final String str) {
            if (str.contains("++Warning[") || str.contains("++Error[") || str.contains("++Abort["))
                sb.append(str);
            if (showFloraOutput)
                System.out.println(str);
        }
        
        @Deprecated
        public void print(String str) {
            // ignored
        }
        
        // returns lines that contain errors and warnings
        public String getString() {
            return sb.toString();
        }
    }

    PrologMessageListener messageListener = null;

    
    /* Function for setting Initialization commands */
    protected void initCommandStrings(String FloraRootDir)
    {
	commands = (new String[] {
		"(import bootstrap_flora/0 from flora2)",
		String.valueOf(String.valueOf((new StringBuffer("asserta(library_directory('")).append(FloraRootDir).append("'))"))),
		"consult(flora2)",
		"bootstrap_flora",
		"consult(flrimportedcalls)",
		//"consult(flrunify_handlers)", // this causes warnings
                "import flora_get_DT_var_datatype/2 from usermod(flrunify_handlers)",
		"import flora_call_string_command/5 from flrcallflora",
		"import flora_decode_oid_as_atom/2 from flrdecode",
		"import '\\\\load'/1, '\\\\add'/1 from flora2"
	    });
    }
    

    /* Function to execute the initialization commands */
    void executeInitCommands()
    {
        if(commands == null)
            throw new FlrException("System bug: please report");

	for(int i = 0; i < commands.length; i++) {
	    boolean cmdsuccess = simplePrologCommand(commands[i]);
	    if(!cmdsuccess)
		throw new FlrException("Flora-2 startup failed");
	}
    }


    /*
    ** Function to use load{...} to load Flora-2 file into moduleName
    ** fileName   : name of the file to load
    ** moduleName : name of Flora-2 module in which to load;
    ** beware this is parsed by Prolog, so it may need quoting
    */
    public boolean loadFile(String fileName,String moduleName)
    {
	boolean cmdsuccess = false; 
	String jcmd =
            "loadFile(\""
            + fileName + "\", "
            + (moduleName == null ? "null" : "\""+moduleName+"\"")
            + ")";
	String cmd = wrapAsTimedCall("'\\\\load'('"+fileName + "' >> '" + moduleName + "')");
        if (moduleName == null) {
            throw new FlrException(": the command "+ jcmd + " failed; the module argument cannot be null");
        }
	try {
            adjustFloraOutput();
            messageListener.clear();
	    cmdsuccess = engine.deterministicGoal(cmd);
	    // Don't use engine.command: it is not error-sensitive
	    //cmdsuccess = engine.command(cmd);
	}
	catch(IPException e) {
	    throw new FlrException(": the command "+ jcmd + " failed", e);
	}
        return cmdsuccess;
    }


    /* Function to use compile{...} to compile Flora-2 file for moduleName
    ** fileName   : name of the file to compile
    ** moduleName : name of Flora-2 module for which to compile
    */
    public boolean compileFile(String fileName,String moduleName)
    {
	boolean cmdsuccess = false; 
	String jcmd =
            "compileFile(\""
            + fileName + "\", "
            + (moduleName == null ? "null" : "\""+moduleName+"\"")
            + ")";
	String cmd = wrapAsTimedCall("'\\\\compile'('"+fileName + "' >> '" + moduleName+"')");
        if (moduleName == null) {
            throw new FlrException(": the command "+ jcmd + " failed; the module argument cannot be null");
        }
	try {
            adjustFloraOutput();
            messageListener.clear();
	    cmdsuccess = engine.deterministicGoal(cmd);
	    // command is not error-sensitive
	    //cmdsuccess = engine.command(cmd);
	}
	catch(IPException e) {
	    throw new FlrException(": the command "+ jcmd + " failed", e);
	}
        return cmdsuccess;
    }


    /* Function to use add{...} to add Flora-2 file to moduleName
    ** fileName   : name of the file to add
    ** moduleName : name of Flora-2 module to which to add
    */
    public boolean addFile(String fileName,String moduleName)
    {
        boolean cmdsuccess = false;
	String jcmd =
            "addFile(\""
            + fileName + "\", "
            + (moduleName == null ? "null" : "\""+moduleName+"\"")
            + ")";
	String cmd = wrapAsTimedCall("'\\\\add'('"+fileName + "' >> '" + moduleName + "')");
        if (moduleName == null) {
            throw new FlrException(": the command "+ jcmd + " failed; the module argument cannot be a null object");
        }
	try {
            adjustFloraOutput();
            messageListener.clear();
	    cmdsuccess = engine.deterministicGoal(cmd);
	    // Don't use engine.command: it is not error-sensitive
	    //cmdsuccess = engine.command(cmd);
	}
	catch(IPException e) {
	    throw new FlrException(": the command "+ jcmd + " failed", e);
	}
        return cmdsuccess;
    }

    
    /* Function to use compileadd{...} to compile Flora-2 file
    ** for adding to moduleName.
    ** fileName   : name of the file to compile for addition
    ** moduleName : name of Flora-2 module for which to compile-add
    */
    public boolean compileaddFile(String fileName,String moduleName)
    {
	boolean cmdsuccess = false; 
	String jcmd =
            "compileaddFile(\""
            + fileName + "\", "
            + (moduleName == null ? "null" : "\""+moduleName+"\"")
            + ")";
	String cmd = wrapAsTimedCall("'\\\\compileadd'('"+fileName + "' >> '" + moduleName+"')");
        if (moduleName == null) {
            throw new FlrException(": the command "+ jcmd + " failed; the module argument cannot be null");
        }
	try {
            adjustFloraOutput();
            messageListener.clear();
	    cmdsuccess = engine.deterministicGoal(cmd);
	    //cmdsuccess = engine.command(cmd);
	}
	catch(IPException e) {
	    throw new FlrException(": the command "+ jcmd + " failed", e);
	}
        return cmdsuccess;
    }
    
    /** Causes Prolog goal handler(_G) to be called periodically every
        period seconds during file compilation and loading command goals.
        _G will be bound to the current command. If handler is null,
        progress will not be reported
    */
    public void setLoadProgressHandler(String handler,double period){
    	loadProgressHandlerPredicate = handler;
    	loadProgressPeriod = Math.round(period*1000);
    }
        
    public void setLoadProgressHandler(String handler){
    	setLoadProgressHandler(handler,1.0);
    }
        
    private String wrapAsTimedCall(String G) {
        if (loadProgressHandlerPredicate==null) return G;
        else {
            //logger.info("preparing timed_call to "+G);
            return "timed_call(( "+
                loadProgressHandlerPredicate+"(("+G+"),"+HEARTBEAT_STAGE_BEGIN+"),("+G+ ")), repeating("+loadProgressPeriod+"), "+
                loadProgressHandlerPredicate+"(("+G+"),"+HEARTBEAT_STAGE_RUN+"), nesting)";
        }
    }
	
    /* Call the flora_call_string_command/5 predicate of Flora-2
    ** Binds Flora-2 variables to the returned values
    ** and returns an array of answers. Each answer is an Interprolog
    ** term model from which variable bindings can be obtained.
    ** (See findAllMatches and executeQuery/1 for how to do this.)
    **
    ** cmd : Flora query to be executed 
    ** vars : Variables in the Flora query that need to be bound
    */
    public Object[] floraCommand(String cmd,Vector<String> vars)
    {
    	StringBuffer sb = new StringBuffer();
    	String varsString = "";

        if (!cmd.matches(".*\\.[ \t]*$"))
            throw new FlrException("\n   Queries and commands must end with a period.\n   Offending query: `" + cmd + "'");
   	
    	//add other variables
    	for (int i=0; i<vars.size(); i++) {
    		String floravar = vars.elementAt(i);
    		if (!floravar.startsWith("?"))
    			throw new FlrException("Illegal variable name "
    					+ floravar
    					+ ". Variables passed to executeQuery "
    					+ "must be Flora-2 variables and "
    					+ "start with a `?'");
    		if (i > 0)
    			varsString += ",";
    		if (floravar.equals("?XWamState"))
    			varsString += "'" + "?XWamState" + "'=_XWamState";
    		else
    			varsString += "'" + vars.elementAt(i) + "'=__Var" + i;
    	}

    	//add var to capture exception
        if (!(varsString.equals("")))
            varsString += ",";
    	varsString += "'" + "?Ex" + "'=_Ex";
    	
    	varsString = "[" + varsString + "]";

    	String listString = "L_rnd=" + varsString + ",";
    	//String queryString = "S_rnd='" + cmd + "',";
        // Serialize the command atom instead, to avoid atom escaping complications:
    	String queryString = "S_rnd=Cmd,";
    	String solutionsSorter = sortSolutions ? "sort(L_rnds_,L_rnds)" : "L_rnds_=L_rnds";
    	String floraQueryString =
            // send 1st message to get some feedback - even for fast queries
            (loadProgressHandlerPredicate==null?
             "" : loadProgressHandlerPredicate+"(Cmd,"+HEARTBEAT_STAGE_BEGIN+"), ") +
            /*
            // VARIANT 1: handles unbound attributed vars in bindings,
            // but not correctly: see fooExample, query ?X : ?Y @ example.
            "findall(TM_rnd,("+
            "flora_call_string_command(S_rnd,L_rnd,_St,_XWamState,_Ex)"+
            ",buildInitiallyFlatTermModel(L_rnd,TM_rnd)),BL_rnd),ipObjectSpec('ArrayOfObject',BL_rnd,LM)" +
            */
            /*
            // VARIANT 2:
            // Uses a single InitiallyFlatTermModel object: less overhead
            "E = exception(normal), findall(L_rnd,(flora_call_string_command(S_rnd,L_rnd,_St,_XWamState,_Ex),  (_Ex\\==normal -> machine:term_set_arg(E,1,_Ex,1) ; true) ),L_rnds), " +
            "E = exception(Exception), buildTermModel(Exception,ExceptionM), buildInitiallyFlatTermModel(L_rnds,LM)" +
            */
            // VARIANT 3:
            // Uses buildTermModel instead of InitiallyFlatTermModel:
            // more overhead than Valiant 2
            // but does the right thing with atoms that have '-quotes in them
            "catch( (findall(L_rnd,(flora_call_string_command(S_rnd,L_rnd,_St,_XWamState,_Ex), (_Ex\\==normal -> throw(_Ex) ; true) ),L_rnds_), "+solutionsSorter+"), Exception, true), " +
            "(var(Exception)->Exception=normal;L_rnds=[]), buildTermModel(Exception,ExceptionM), "+termBuilder+"(L_rnds,LM)" +
            // send last message to get final feedback - even for fast queries
            (loadProgressHandlerPredicate==null ?
             "" : ", "+loadProgressHandlerPredicate+"(Cmd,"+HEARTBEAT_STAGE_END+")");
    		
    	sb.append(queryString);
    	sb.append(listString);
    	sb.append(floraQueryString);
	
	if (debug) {
	    System.out.println("In PrologFlora: floraCommand(sb) = " + sb);
	    System.out.println("In PrologFlora: floraCommand(cmd) = " + cmd);
        }

	try {
            adjustFloraOutput();
            messageListener.clear();
            /*
            // This one uses VARIANT 1
            Object solutions[] =
                (Object[])engine.deterministicGoal(sb.toString(), "[LM]")[0];
            */

            // This is for VARIANTS 2 and 3
            // NOTE: deterministicGoal(X,Y,Z,W) binds
            //       the Prolog var Cmd to the contents of the string cmd
            //       so S_rnd becomes the command cmd.
            Object[] bindings = engine.deterministicGoal(sb.toString(), "[string(Cmd)]",new Object[]{cmd},"[ExceptionM,LM]");

            if (debug)
                System.out.println("In PrologFlora: floraCommand(bindings) = " + Arrays.toString(bindings));

            if (bindings==null)
                throw new FlrException("\n\t  *** Unexpected problem, probably malformed query.\n\t  *** Use FloraSession.showOutput(); to display errors on the console.");
            TermModel exception = (TermModel)bindings[0];
            TermModel[] solutions = TermModel.flatList((TermModel)bindings[1]);

            if (debug) {
                System.out.println("In PrologFlora: Exception = " + exception);
                System.out.printf("In PrologFlora: # of solutions = %d\n",solutions.length);
                System.out.println("In PrologFlora: solutions = " + Arrays.toString(solutions));
            }

            /* For testing
            System.out.println("*** Listener output: "
                               + messageListener.getString() +  "***");
            */

	    findExceptionCore(exception); 

	    return solutions;
	}
	catch(Exception e) {
	    throw new FlrException("Error in query "+cmd, e);
	}
    }

    protected String termBuilder = "buildTermModel"; // interprolog predicate used to build a TemModel with results
    
    protected boolean sortSolutions = false;
    
    /* A simpler way to call Flora-2 commands that require no variable
    ** bindings to be returned
    **
    ** cmd : Command to be executed
    ** Returns: boolean success/failure in the prolog sense.
    */
    //TODO - may want to add a variable to get exception information back?
    //But that would kind of defeat the simplicity advantage...
    public boolean simpleFloraCommand(String cmd)
    {
	String queryString = "S_rnd=Cmd,";
	String listString = "L_rnd=[],";

        StringBuffer sb = new StringBuffer();
        boolean result = false;

        if (!cmd.matches(".*\\.[ \t]*$"))
            throw new FlrException("\n   Queries and commands must end with a period.\n   Offending query: `" + cmd + "'");
   	
	sb.append(queryString);
	sb.append(listString);
        sb.append("flora_call_string_command(S_rnd,L_rnd,_St,_XWamState,_Ex), _Ex == normal"); // make sure success... means it.

	if (debug) {
	    System.out.println("In PrologFlora: simpleFloraCommand1: " + sb);
	    System.out.println("In PrologFlora: simpleFloraCommand2: " + cmd);
        }

	try {
            adjustFloraOutput();
            messageListener.clear();
            // NOTE: deterministicGoal(X,Y,Z) binds
            //       the Prolog var Cmd to the contents of the string cmd
            //       so S_rnd becomes the command cmd.
	    result = engine.deterministicGoal(sb.toString(),"[string(Cmd)]",new Object[]{cmd});
            if (debug)
                System.out.println("In PrologFlora: simpleFloraCommand result: " + result);
	}
	catch(IPException e) {
	    throw new FlrException(": the command " + cmd + " failed", e);
	}
        return result;
    }


    public boolean simplePrologCommand(String cmd)
    {
        try {
            adjustFloraOutput();
            messageListener.clear();
	    return engine.deterministicGoal(cmd);
	    //return engine.command(cmd);
	}
        catch(IPException ipe) {
	    throw ipe;
	}
    }

    /**
     ** A simplified version of TermModel.toString() that doesn't put commas
     ** between nested children.
     */
    public String toStringNoCommas(TermModel tm)
    {
	if (tm.getChildCount()==0)
	    return tm.node.toString();
	else if (tm.isList())
	    return tm.toString();
	else if ((tm.node.equals("/") && (tm.children.length == 2)))
	    return "" + tm.children[0] + tm.node + tm.children[1];
	else 
	    {
		StringBuffer retval = new StringBuffer();

		for (int i=0; i < tm.children.length; i++)
		    {
			retval.append(toStringNoCommas(tm.children[i]));
		    }

		return retval.toString();
	    }
    }

    public void findException(Object[] solutions) {
    	//look for exception (?Ex binding) in solution and throw it
    	//An exception will look like error(type-of-error(message,...) ...)
    	TermModel objName = null;
    	if (solutions != null && solutions.length != 0) {
	    TermModel tm = (TermModel)solutions[0];
	    objName = PrologFlora.findValue(tm,"?Ex");
	    //logger.info("findException:  " + objName);
	    findExceptionCore(objName);
	    }
    }

    // If objName is "normal" then just return -- NO exception.
    // Otherwise, find the exception's details
    private void findExceptionCore(TermModel objName) {
        if (objName == null)
            throw new FlrException("Flora returned no exception info - probably a bug");
        else if (objName.node instanceof String) {
            if (objName.toString().equals("normal")) {
                // NO EXCEPTION
            } else if (objName.node != null && objName.node.toString().equals("builtin_exception")) {
                Integer id = (Integer) ((TermModel) objName.getChild(0)).node;
                Exception ex = exceptionStore.remove(id);
                throw (FlrException) ex;
            } 
            else {
                if (objName.children == null) {
                    // message from XSB throw(message)
                    throw new FlrException(objName.node.toString());
                } else {
                    switch (objName.children.length) {
                    case 2:
                        // Flora exception
                        throw new FlrException(objName.node,
                                               ((TermModel) objName.getChild(0)).node,
                                               ((TermModel) objName.getChild(1)).node);
                    case 3:
                        // XSB exception - objName.node.equals("error")
                        throw new FlrException(((TermModel) objName.getChild(0)).toString(),
                                               // ((TermModel) objName.getChild(1)),
                                               toStringNoCommas((TermModel) objName.getChild(1)),
                                               ((TermModel) objName.getChild(2)));
                    default:
                        throw new FlrException("Flora returned non-standard exception with " + objName.children.length + " children - " + objName);
                    }
                }
            }
        }
        else
            throw new FlrException("Flora returned non-standard exception info - probably a bug: " + objName.toString());
    	
    }
    
    public int storeException(Exception e) {
    	exceptionStore.put(++numExceptions,e);
    	return numExceptions;
    }
    
    public void removeException(int key) {
    	exceptionStore.remove(key);
    }
    
    /* Query the term model structure 
    **
    ** tm : TermModel to be queried 
    ** name : binding variable name to be queried 
    */
    public static TermModel findValue(TermModel tm, String name)
    {
	if (debug)
	    System.out.println("in findValue, Term model: " + tm);

        for( ; tm.isList(); tm = (TermModel)tm.getChild(1)) {
	    TermModel item = (TermModel)tm.getChild(0);
	    
	    if(name == null
	       || (item.getChild(0).toString().compareTo(name) == 0)) {
		TermModel val = (TermModel)item.getChild(1);
		return val;
	    }

	    if (debug)
		System.out.println("name in findValue: " + name);

	}
	return new TermModel();
    }


    /* Initialise the PrologEngine */
    void initEngine()
    {
	String prologBinDir = System.getProperty("PROLOGDIR");
	String cmdFloraRootDir = System.getProperty("FLORADIR");
        // -DDEBUG may be specified on command line
        String debugRequested = System.getProperty("DEBUG");

	String PrologExecutable = prologBinDir + File.separator + "xsb";

        if(prologBinDir == null || prologBinDir.trim().length() == 0)
            throw new FlrException("Must define PROLOGDIR property");
        
        String FloraRootDir;	
	if (cmdFloraRootDir == null || cmdFloraRootDir.trim().length() == 0) {
	    throw new FlrException("Must define FLORADIR property");
	    }
	else
	    FloraRootDir = cmdFloraRootDir;

        /*
          Debugging output will be triggered if -DDEBUG is given
          on java command line.
        */
        if (debugRequested != null)
            FloraSession.enableDebugging();

	try {
	    String args = "";
	    
	    engine = makePrologEngine(systemSpecificFilePath(PrologExecutable) + args, FloraRootDir, debug);
            /*
              engineType was eliminated in the  Sat Mar 21 00:06:19 2015 -0400
              commit with a message: eliminated native engine, which doesn't
              work in the IP for the most part.
            */
            //engine = new NativeEngine(systemSpecificFilePath(PrologRootDir), args, false, true);
	    
	}
	catch(Exception e2) {
	    throw new FlrException("InterProlog failed to start the engine; PROLOGDIR ("+prologBinDir+") may not be pointing to the XSB binary",e2);
	}

        messageListener = new PrologMessageListener();
        engine.addPrologOutputListener(messageListener);

        // textual error detection seems uneeded: messes up exception handling
    	engine.setDetectErrorMessages(false);



	if (File.separatorChar=='\\')
	    // Windows hack: to avoid incorrect escaping on the Prolog side
	    FloraRootDir = FloraRootDir.replace('\\','/');
	initCommandStrings(FloraRootDir);
	executeInitCommands();

        engineInitialized = true;

        // initially turn verbosity off;
        // can turn on with FloraSession.showMoreOutput()
        engine.deterministicGoal(verbosityOff);

	//load configuration file containing debugger config
	File file = new File(cmdFloraRootDir+"/java/API/javaAPI/initFloraAPI.P");
	engine.load_dynAbsolute(file);
    }

    protected XSBSubprocessEngine makePrologEngine(String path,String FloraRootDir, boolean debug) {
    	return new XSBSubprocessEngine(path, debug);
    }

    /* Constructor function */
    public PrologFlora()
    {
        commands = null;
	initEngine();
	return;
    }
    

    /* Shut down the Prolog Engine */
    public void close()
    {
	// don't exit: let the Java program continue after the shutdown
	engine.shutdown();
        engineInitialized = false;
    }
    
    public String systemSpecificFilePath(String p){
    	char nonSeparatorChar = 'a';
    	if (File.separator.equals("\\"))
    		nonSeparatorChar = '/';
    	else
    		nonSeparatorChar = '\\';
    			
    	StringBuffer newPath = new
    	StringBuffer(p.length()+10);
    	for(int c=0;c<p.length();c++){
    		char ch = p.charAt(c);
    		if (ch==nonSeparatorChar)
    			newPath.append(File.separatorChar); 
    		//backslashes to forward
    		else newPath.append(ch);
    	}
    	return newPath.toString();
    }

    /*
      If needsMoreFloraOutput or needsLessFloraOutput, execute the
      appropriate verbosity commands
    */
    public void adjustFloraOutput() {
        try{
            if (engine != null && engineInitialized) {
                if (needsMoreFloraOutput)
                    engine.deterministicGoal(verbosityOn);
                if (needsLessFloraOutput)
                    engine.deterministicGoal(verbosityOff);
                needsMoreFloraOutput = false;
                needsLessFloraOutput = false;
            }
        } catch (Exception e) {
        }
    }

    public boolean hasErrors() {
        return (messageListener.getString().contains("++Error[")
                ||
                messageListener.getString().contains("++Abort[")
                );
    }
    public boolean hasWarnings() {
        return messageListener.getString().contains("++Warning[");
    }
    
}
