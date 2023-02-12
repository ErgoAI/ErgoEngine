
import java.io.FileReader;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

import net.sf.flora2.API.FloraObject;
import net.sf.flora2.API.FloraSession;

public class TestQuery {
	
    private static FloraSession session = null; 
    
    public static void main(String[] args) {
        try {
            //FloraSession.enableDebugging();
            session = new FloraSession();
            //FloraSession.enableDebugging();
            System.out.println("Engine session started");

            //FloraSession.showOutput();
            //FloraSession.showMoreOutput();
            //FloraSession.showLessOutput();
            
            /*
              tested with run -DDEBUG and get
              PROLOG stdout:aaaa = aaa '80' bbb
              So, the quote is passed to Ergo correctly.
            */
            // test.ergo has a syntax error
            /*
            */
            Iterator<FloraObject> dummy1 = session.executeQuery("writeln(TestQuery=iiiii)@\\plg.");
            if (session.hasWarnings())
                System.out.println("HAS warnings: writeln(TestQuery=iiiii)@\\plg.");
            else
                System.out.println("NO warnings: writeln(TestQuery=iiiii)@\\plg.");
            if (session.hasErrors())
                System.out.println("HAS errors: writeln(TestQuery=iiiii)@\\plg.");
            else
                System.out.println("NO errors: writeln(TestQuery=iiiii)@\\plg.");
            System.out.println("before dummy2");
            Iterator<FloraObject> dummy2 = session.executeQuery("\\false.");
            if (session.hasWarnings())
                System.out.println("HAS warnings: \\false.");
            else
                System.out.println("NO warnings: \\false.");
            if (session.hasErrors())
                System.out.println("HAS errors: \\false.");
            else
                System.out.println("NO errors: \\false.");
            System.out.println("after dummy2");
            
            Iterator<FloraObject> dummy21 = session.executeQuery("insert{p(?X) :- q}.");
            if (session.hasWarnings())
                System.out.println("HAS warnings: insert{p(?X) :- q}.");
            else
                System.out.println("NO warnings: insert{p(?X) :- q}.");
            if (session.hasErrors())
                System.out.println("HAS errors: insert{p(?X) :- q}.");
            else
                System.out.println("NO errors: insert{p(?X) :- q}.");

            Iterator<FloraObject> dummy3 = session.executeQuery("['auxfiles/test2'].");
            if (session.hasWarnings())
                System.out.println("HAS warnings: [test2].");
            else
                System.out.println("NO warnings: [test2].");
            if (session.hasErrors())
                System.out.println("HAS errors: [test2].");
            else
                System.out.println("NO errors: [test2].");
            System.out.println("after loading test2.ergo");

            // query = false, but returns non-null bindings in PrologFlora:325
            //Iterator<FloraObject> dummy4 = session.executeQuery("test.");

            // query = false, but has compile error;
            // returns null bindings in PrologFlora:325
            try{
                Iterator<FloraObject> dummy4 = session.executeQuery("te st.");
            } catch(Exception e) {
            }
            if (session.hasWarnings())
                System.out.println("HAS warnings: te st.");
            else
                System.out.println("NO warnings: te st.");
            if (session.hasErrors())
                System.out.println("HAS errors: te st.");
            else
                System.out.println("NO errors: te st.");
            // flora_call_string_command returns Exception=compilation_failed

            // query = false, but has compile error;
            // returns null bindings in PrologFlora:325
            // flora_call_string_command returns Exception=normal
            Iterator<FloraObject> dummy41 = session.executeQuery("['auxfiles/test'].");
            if (session.hasWarnings())
                System.out.println("HAS warnings: [test].");
            else
                System.out.println("NO warnings: [test].");
            if (session.hasErrors())
                System.out.println("HAS errors: [test].");
            else
                System.out.println("NO errors: [test].");
            Iterator<FloraObject> dummy42 = session.executeQuery("test.");
            if (session.hasWarnings())
                System.out.println("HAS warnings: test.");
            else
                System.out.println("NO warnings: test.");
            if (session.hasErrors())
                System.out.println("HAS errors: test.");
            else
                System.out.println("NO errors: test.");

            try{
                Iterator<FloraObject> dummy5 = session.executeQuery("p@foo.");
            } catch(Exception e) {
            }
            if (session.hasWarnings())
                System.out.println("HAS warnings: p@foo.");
            else
                System.out.println("NO warnings: p@foo.");
            if (session.hasErrors())
                System.out.println("HAS errors: p@foo.");
            else
                System.out.println("NO errors: p@foo.");

            String query = "?X = 'aaa ''80'' bbb', writeln(aaaa=?X)@\\plg.";
            Vector<String> vars = new Vector<String>();
            vars.add("?X");
            System.out.println("Query:" + query);

            Iterator<HashMap<String,FloraObject>> answers = session.executeQuery(query, vars);
            if (session.hasWarnings())
                System.out.println("HAS warnings: ?X = 'aaa ''80'' bbb'.");
            else
                System.out.println("NO warnings: ?X = 'aaa ''80'' bbb'.");
            if (session.hasErrors())
                System.out.println("HAS errors: ?X = 'aaa ''80'' bbb'.");
            else
                System.out.println("NO errors: ?X = 'aaa ''80'' bbb'.");
            for (; answers.hasNext();) {
                HashMap<String,FloraObject> ans = answers.next();
                FloraObject xvar = ans.get("?X");
                System.out.printf("Answer: ?X = %s\n", xvar);
                System.out.println("");
            }
            /*
            */
				
            session.close();
            System.out.println("Finished");
            System.exit(0);
        } catch (Exception e) {
            System.err.printf("Exception %s\n", e.getMessage());
            e.printStackTrace();
            System.exit(0);
        }
    }
	

}
