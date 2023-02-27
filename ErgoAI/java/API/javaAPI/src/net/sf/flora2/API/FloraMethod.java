
/* File:      FloraMethod.java
**
** Author(s): Aditi Pandit
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
 
import java.util.Vector;

/* Class to encapsulate a Flora method */
public class FloraMethod extends FloraConstants
{
	
    public FloraObject methodName;
    public Vector<FloraObject> parameters; // of FloraObject
    public FloraObject returnType;
    public boolean inheritable;
    public int type; // DATA, BOOLEAN, PROCEDURAL
	
    public FloraMethod(FloraObject methodName,
		       Vector<FloraObject> params, FloraObject returnType,
		       boolean inheritable, int type)
    {
	this.methodName = methodName;
	this.parameters = params;
	this.returnType = returnType;
	this.inheritable = inheritable;
	this.type = type;
	return;
    }

    public String toString()
    {
	return methodName.toString();
    }
	
    // mainly for debugging
    public String methodDetails()
    {
	return
	    "Method name:   " + methodName.toString()
	    + "\n    parameters:     " + parameters.toString()
	    + "\n    return type:    " + (returnType==null?
					  "*none*" : returnType.toString())
	    + "\n    inheritability: " + printableInheritability(inheritable)
	    + "\n    method type:    " + printableMethodType(type);
    }

}
