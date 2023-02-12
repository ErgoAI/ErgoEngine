/* File:      javaAPI.flh
**
** Author(s): Michael Kifer
**
** Template included with every high-level Java object in the FLORA-2 Java API.
**
*/


import java.util.*;
import com.declarativa.interprolog.TermModel;
import net.sf.flora2.API.util.*;
import net.sf.flora2.API.*;
public class foo extends FloraConstants {

    static TermModel floraClassName = new TermModel("foo");
    FloraObject sourceFloraObject;
    String moduleName;

    public foo(FloraObject sourceFloraObject,String moduleName) {
	this.sourceFloraObject = sourceFloraObject;
	if (sourceFloraObject == null)
	    throw new FlrException("Cannot create Java class " + floraClassName
				   + ". Null FloraObject passed to "
				   + floraClassName
				   + "(sourceFloraObject,moduleName)");
	    this.moduleName = moduleName;
    }

    public foo(String floraOID,String moduleName,FloraSession session) {
	this.sourceFloraObject = new FloraObject(floraOID,session);
	this.moduleName = moduleName;
    }

    public String toString() {
        return sourceFloraObject.toString();
    }


    // Sub/Superclass methods
    public Iterator<FloraObject> getDirectInstances() {
	return sourceFloraObject.getDirectInstances(moduleName);
    }

    public Iterator<FloraObject> getInstances() {
	return sourceFloraObject.getInstances(moduleName);
    }

    public Iterator<FloraObject> getDirectSubClasses() {
	return sourceFloraObject.getDirectSubClasses(moduleName);
    }

    public Iterator<FloraObject> getSubClasses() {
	return sourceFloraObject.getSubClasses(moduleName);
    }

    public Iterator<FloraObject> getDirectSuperClasses() {
	return sourceFloraObject.getDirectSuperClasses(moduleName);
    }

    public Iterator<FloraObject> getSuperClasses() {
	return sourceFloraObject.getSuperClasses(moduleName);
    }


    // Java proxy methods for FLORA-2 methods
    public boolean getBDI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean",INHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDIall_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",INHERITABLE,DATA,pars);
    }

    public boolean setBDI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean",INHERITABLE,DATA,pars);
    }

    public boolean deleteBDI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean",INHERITABLE,DATA,pars);
    }

    public boolean getBDI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean2",INHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDIall_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean2",INHERITABLE,DATA,pars);
    }

    public boolean setBDI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean2",INHERITABLE,DATA,pars);
    }

    public boolean deleteBDI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean2",INHERITABLE,DATA,pars);
    }

    public boolean getBDN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDNall_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean setBDN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean deleteBDN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean getBDN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean2",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDNall_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean2",NONINHERITABLE,DATA,pars);
    }

    public boolean setBDN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean2",NONINHERITABLE,DATA,pars);
    }

    public boolean deleteBDN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean2",NONINHERITABLE,DATA,pars);
    }

    public boolean getBDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDNall_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean setBDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean deleteBDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean getBDN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBDNall_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean setBDN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.setboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean deleteBDN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.deleteboolean(moduleName,"boolean",NONINHERITABLE,DATA,pars);
    }

    public boolean getBSI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSIall_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",INHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean",INHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSI_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean",INHERITABLE,SIGNATURE,pars);
    }

    public boolean getBSI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean2",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSIall_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean2",INHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean2",INHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSI_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean2",INHERITABLE,SIGNATURE,pars);
    }

    public boolean getBSN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSNall_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSN_boolean()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getBSN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"boolean2",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSNall_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"boolean2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"boolean2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSN_boolean2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"boolean2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getBSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getboolean(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSNall_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getbooleanAll(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setboolean(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteboolean(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getBSN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getBSNall_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getbooleanAll(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setBSN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.setboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deleteBSN_boolean(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.deleteboolean(moduleName,"boolean",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getPDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getproceduralAll(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean setPDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean deletePDN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean getPDN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getprocedural(moduleName,"procedural2",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPDN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getproceduralAll(moduleName,"procedural2",NONINHERITABLE,DATA,pars);
    }

    public boolean setPDN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setprocedural(moduleName,"procedural2",NONINHERITABLE,DATA,pars);
    }

    public boolean deletePDN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteprocedural(moduleName,"procedural2",NONINHERITABLE,DATA,pars);
    }

    public boolean getPDN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPDN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getproceduralAll(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean setPDN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.setprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean deletePDN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.deleteprocedural(moduleName,"procedural",NONINHERITABLE,DATA,pars);
    }

    public boolean getPSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getproceduralAll(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setPSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deletePSN_procedural()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getPSN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getprocedural(moduleName,"procedural2",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPSN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getproceduralAll(moduleName,"procedural2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setPSN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setprocedural(moduleName,"procedural2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deletePSN_procedural2()
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deleteprocedural(moduleName,"procedural2",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean getPSN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallPSN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.getproceduralAll(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setPSN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.setprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean deletePSN_procedural(Object person)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(person);
      return sourceFloraObject.deleteprocedural(moduleName,"procedural",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVDI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"ancestors",INHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"ancestors",INHERITABLE,DATA,pars);
    }

    public boolean setVDI_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",INHERITABLE,DATA,pars,value);
    }

    public boolean setVDI_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"inheritableMeth",INHERITABLE,DATA,pars);
    }

    public boolean setVDI_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars,value);
    }

    public boolean setVDI_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalue(moduleName,"age",INHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalueAll(moduleName,"age",INHERITABLE,DATA,pars);
    }

    public boolean setVDI_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",INHERITABLE,DATA,pars,value);
    }

    public boolean setVDI_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"ancestors",NONINHERITABLE,DATA,pars);
    }

    public boolean setVDN_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars,value);
    }

    public boolean setVDN_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars);
    }

    public boolean setVDN_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars,value);
    }

    public boolean setVDN_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalue(moduleName,"age",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalueAll(moduleName,"age",NONINHERITABLE,DATA,pars);
    }

    public boolean setVDN_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",NONINHERITABLE,DATA,pars,value);
    }

    public boolean setVDN_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVDN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.getvalue(moduleName,"bonus",NONINHERITABLE,DATA,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVDN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.getvalueAll(moduleName,"bonus",NONINHERITABLE,DATA,pars);
    }

    public boolean setVDN_bonus(Object year, Object month,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.setvalue(moduleName,"bonus",NONINHERITABLE,DATA,pars,value);
    }

    public boolean setVDN_bonus(Object year, Object month,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.setvalue(moduleName,"bonus",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_bonus(Object year, Object month,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_bonus(Object year, Object month,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,DATA,pars,value);
    }

    public boolean deleteVDN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,DATA,pars);
    }

    public Iterator<FloraObject> getVSI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSI_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSI_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSI_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSI_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalue(moduleName,"age",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalueAll(moduleName,"age",INHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSI_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSI_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSI_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",INHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSN_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSN_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_ancestors(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_ancestors(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_ancestors()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"ancestors",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.getvalueAll(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSN_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSN_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.setvalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_inheritableMeth(Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_inheritableMeth(Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_inheritableMeth()    {
      Vector<Object> pars = new Vector<Object>();
      return sourceFloraObject.deletevalue(moduleName,"inheritableMeth",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.getvalueAll(moduleName,"age",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSN_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSN_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.setvalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_age(Object year,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_age(Object year,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_age(Object year)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      return sourceFloraObject.deletevalue(moduleName,"age",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<FloraObject> getVSN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.getvalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars);
    }

    public Iterator<HashMap<String,FloraObject>> getallVSN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.getvalueAll(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars);
    }

    public boolean setVSN_bonus(Object year, Object month,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.setvalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean setVSN_bonus(Object year, Object month,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.setvalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_bonus(Object year, Object month,Vector<Object> value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_bonus(Object year, Object month,Object value)
    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars,value);
    }

    public boolean deleteVSN_bonus(Object year, Object month)    {
      Vector<Object> pars = new Vector<Object>();
      pars.add(year);
      pars.add(month);
      return sourceFloraObject.deletevalue(moduleName,"bonus",NONINHERITABLE,SIGNATURE,pars);
    }

}
