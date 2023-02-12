/* File:      fooExample.java
**
** Author(s): Aditi Pandit
*/

/***********************************************************************
    Examples of the low-level API followed by examples of the uses of
    the high-level API.
***********************************************************************/

import java.util.*;
import net.sf.flora2.API.*;
import net.sf.flora2.API.util.*;

public class fooExample{
	
    public static void main(String[] args) {
		
    	FloraSession session = new FloraSession();
	System.out.println("Flora-2 session started");	
		
	String fileName = System.getProperty("INPUT_FILE");
	if(fileName == null || fileName.trim().length() == 0) {
	    System.out.println("Invalid path to example file");
	    System.exit(0);
	}
	session.loadFile(fileName,"example");
	
	/***********************************************************
                    Low-level API examples
	***********************************************************/

	String command = "?Y : foo @ example.";
	Iterator<FloraObject> fooObjs = session.executeQuery(command);
		
	System.out.println("Query: ?Y : foo @ example.");
	FloraObject johnSuper = null; 
    	while (fooObjs.hasNext()) {
	    johnSuper = fooObjs.next();
	    if (johnSuper != null) {
		System.out.println("FOO obj Id:"+johnSuper);
	    } else
		System.out.println("FOO obj Id: wrong answer!");
	}

	 
	Vector<String> vars = new Vector<String>();
	vars.add("?X");
	vars.add("?Y");

	// Use of executeQuery/2
        // This query tests ?X being unbound attributed variable
	Iterator<HashMap<String,FloraObject>> allmatches =
            session.executeQuery("?X = ?Z^^\\symbol, ?X:?Y@example.",vars);
	HashMap<String,FloraObject> firstmatch;
	System.out.println("Query: ?X = ?Z^^\\symbol, ?X:?Y@ example.");
	while (allmatches.hasNext()) {	
	    firstmatch = allmatches.next();
	    Object Xobj = firstmatch.get("?X");
	    Object Yobj = firstmatch.get("?Y");	
	    System.out.println("    " + Xobj + " : " + Yobj);	
	}

	// Another use of executeQuery/2: with data types
	Vector<String> vars2 = new Vector<String>();
	vars2.add("?X");
	Iterator<HashMap<String,FloraObject>> allmatches2 = session.executeQuery("p(?X) @ example.",vars2);
	HashMap<String,FloraObject> firstmatch2;
	System.out.println("Query: p(?X) @ example.");
	while (allmatches2.hasNext()) {	
	    firstmatch2 = allmatches2.next();
	    Object Xobj = firstmatch2.get("?X");
	    System.out.println("?X = " + Xobj);	
	}
		
	/*******************************************************************
                Examples of the high-level API
	*******************************************************************/

	foo objJohn = new foo("john","example",session);
	boolean check = objJohn.getBDN_boolean();
	if (check)
	    System.out.println("boolean is true");
	else
	    System.out.println("boolean is false");

	check = objJohn.getBDN_boolean2();
	if(check)
	    System.out.println("boolean2 is true");
	else
	    System.out.println("boolean2 is false");

	check = objJohn.getPDN_procedural();
	if (check)
	    System.out.println("procedural is true");
	else
	    System.out.println("procedural is false");

	check = objJohn.getPDN_procedural2();
	if (check)
	    System.out.println("procedural2 is true");
	else
	    System.out.println("procedural2 is false");
			

	objJohn.setVDN_bonus("2004","Jan","5000");
	System.out.println("Set John's bonus");
	objJohn.setVDN_age("2004","50");
	System.out.println("Set John's age");
	Iterator<FloraObject> ageValues = objJohn.getVDN_age("2004");
	while (ageValues.hasNext()) {
	    System.out.println("John's age: "+ageValues.next());
	}
		
	Vector<String> ancestors = new Vector<String>();
	ancestors.add("mary");
	ancestors.add("sally");
		
	objJohn.setVDN_ancestors(ancestors);
	System.out.println("John obj: "+objJohn);

	Iterator<FloraObject> ancestorVals = objJohn.getVDN_ancestors();
	while (ancestorVals.hasNext()) {
	    System.out.println("John's ancestors: " + ancestorVals.next());
	}
	 
	// good bye
	session.close();
	System.exit(0);
    }
}
