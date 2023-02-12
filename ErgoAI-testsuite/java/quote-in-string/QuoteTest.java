
import java.io.FileReader;
import java.util.HashMap;
import java.util.Iterator;
import java.util.Vector;

import net.sf.flora2.API.FloraObject;
import net.sf.flora2.API.FloraSession;

public class QuoteTest {
	
    private static FloraSession session = null; 
    
    public static void main(String[] args) {
        try {
            session = new FloraSession();
            System.out.println("Engine session started");
            
            /*
              tested with run -DDEBUG and get
              PROLOG stdout:aaaa = aaa '80' bbb
              So, the quote is passed to Ergo correctly.
            */
            String query = "?X = 'aaa ''80'' bbb', writeln(aaaa=?X)@\\plg.";
            Vector<String> vars = new Vector<String>();
            vars.add("?X");
            System.out.println("Query:" + query);
            Iterator<HashMap<String,FloraObject>> answers = session.executeQuery(query, vars);
            for (; answers.hasNext();) {
                HashMap<String,FloraObject> ans = answers.next();
                FloraObject xvar = ans.get("?X");
                System.out.printf("Answer: ?X = %s\n", xvar);
                System.out.println("");
            }
				
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
