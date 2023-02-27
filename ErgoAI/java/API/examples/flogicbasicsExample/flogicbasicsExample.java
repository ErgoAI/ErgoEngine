/* File:      flogicbasicsExample.java
**
** Author(s): Aditi Pandit, Michael Kifer
**
** Contact:   see  ../CONTACTS.txt
**
** Copyright (C) by
**      The Research Foundation of the State University of New York, 1999-2018.
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




/***********************************************************************
     This file contains examples of the uses of low-level API followed
     by the examples of the uses of high-level API.
***********************************************************************/

import java.util.*;
import net.sf.flora2.API.*;
import net.sf.flora2.API.util.*;

public class flogicbasicsExample {
	
    public static void main(String[] args) {
        /* Initializing the session */
    	FloraSession session = new FloraSession();
        FloraSession.showOutput();
	System.out.println("Flora-2 session started");	
		
        // Assume that Java was called with -DINPUT_FILE=the-file-name
	String fileName = System.getProperty("INPUT_FILE");
	if(fileName == null || fileName.trim().length() == 0) {
	    System.out.println("Invalid path to example file!");
	    System.exit(0);
	}
        /* Loading the flora file */
	if (session.loadFile(fileName,"example"))
	    System.out.println("Example loaded successfully!");
	else
	    System.out.println("Error loading the example file!");

	
	/* Examples of uses of the low-level Java-Flora-2 API */
	
	// Querying persons
	String command = "?X:person@example.";
	System.out.println("Query:"+command);
	Iterator<FloraObject> personObjs = session.executeQuery(command);
	/* Printing out persons  */
    	while (personObjs.hasNext()) {
	    Object personObj = personObjs.next();
	    System.out.println("Person Id: "+personObj);
	}

		
	command = "person[instances -> ?X]@example.";
	System.out.println("Query:"+command);
	personObjs = session.executeQuery(command);
		
	/* Printing out persons  */
    	while (personObjs.hasNext()) {
	    Object personObj = personObjs.next();
	    System.out.println("Person Id: "+personObj);
	}

	/* Example of query with two variables */
	Vector<String> vars = new Vector<String>();
	vars.add("?X");
	vars.add("?Y");
		
	Iterator<HashMap<String,FloraObject>> allmatches =
	    session.executeQuery("?X[believes_in -> ?Y]@example.",vars);
	System.out.println("Query:?X[believes_in -> ?Y]@example.");

	HashMap<String,FloraObject> firstmatch;
	while (allmatches.hasNext()) {	
	    firstmatch = allmatches.next();
	    FloraObject Xobj = firstmatch.get("?X");
	    FloraObject Yobj = firstmatch.get("?Y");	
	    System.out.println(Xobj+" believes in: "+Yobj);	
	}

	FloraObject personObj = new FloraObject("person",session);
	Iterator<FloraMethod> methIter = personObj.getMethods("example");
	while (methIter.hasNext()) {
	    System.out.println((methIter.next()).methodDetails());
	}

	// instances of the person class
	Iterator<FloraObject> instanceIter = personObj.getInstances("example");
	System.out.println("Person instances:");
	while (instanceIter.hasNext())
	    System.out.println("    " + instanceIter.next());


	instanceIter = personObj.getDirectInstances("example");
	System.out.println("Person direct instances:");
	while (instanceIter.hasNext())
	    System.out.println("    " + instanceIter.next());

	Iterator<FloraObject> subIter = personObj.getSubClasses("example");
	System.out.println("Person subclasses:");
	while (subIter.hasNext())
	    System.out.println("    " + subIter.next());

	subIter = personObj.getDirectSubClasses("example");
	System.out.println("Person direct subclasses:");
	while (subIter.hasNext())
	    System.out.println("    " + subIter.next());

	Iterator<FloraObject> supIter = personObj.getSuperClasses("example");
	System.out.println("Person superclasses:");
	while (supIter.hasNext())
	    System.out.println("    " + supIter.next());

	supIter = personObj.getDirectSuperClasses("example");
	System.out.println("Person direct superclasses:");
	while (supIter.hasNext())
	    System.out.println("    " + supIter.next());

	/*****************************************************************
	 ******** Examples of uses of the high-level Java-Flora-2 API
	 *****************************************************************/

	/* Printing out people's names and information about their kids
	   using the high-level API. Note that the high-level person-object 
	   is obtained here out of the low-level FloraObject personObj
	*/
	person currPerson = null;
    	while (personObjs.hasNext()) {
	    personObj = (personObjs.next());
	    System.out.println("Person name:"+personObj);

	    currPerson =new person(personObj,"example");
	    Iterator<FloraObject> kidsItr = currPerson.getVDN_kids();

	    while(kidsItr.hasNext()) {
		FloraObject kidObj = (kidsItr.next());
		System.out.println("Person Name: " + personObj
				   + " has kid: " + kidObj);
		    
		person kidPerson = null;
		kidPerson = new person(kidObj,"example");
		
		Iterator<FloraObject> hobbiesItr = kidPerson.getVDN_hobbies();

		while(hobbiesItr.hasNext()) {
		    FloraObject hobbyObj = null;
		    hobbyObj = (hobbiesItr.next());
		    System.out.println("Kid:"+kidObj
				       + " has hobby: " + hobbyObj);
		}
	    }
	}


	FloraObject age;
	currPerson = new person("father(mary)", "example", session);
	Iterator<FloraObject> maryfatherItr = currPerson.getVDN_age();
	age = maryfatherItr.next();
	System.out.println("Mary's father is " + age + " years old");

	currPerson = new person("mary", "example", session);
	Iterator<FloraObject> maryItr = currPerson.getVDN_age();
	age = maryItr.next();
	System.out.println("Mary is " + age + " years old");

	// person instances using high-level interface
	person personClass = new person("person", "example", session);
	instanceIter = personClass.getInstances();
	System.out.println("Person instances using high-level API:");
	while (instanceIter.hasNext())
	    System.out.println("    " + instanceIter.next());


	Iterator<FloraObject> subclassIter = personClass.getSubClasses();
	System.out.println("Person subclasses using high-level API:");
	while (subclassIter.hasNext())
	    System.out.println("    " + subclassIter.next());


	// Close session and good bye
	session.close();
	System.exit(0);
    }
}
