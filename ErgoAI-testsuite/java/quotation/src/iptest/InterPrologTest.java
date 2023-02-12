package iptest;

import java.io.File;
import java.util.Arrays;

import com.declarativa.interprolog.PrologOutputListener;
import com.declarativa.interprolog.XSBSubprocessEngine;

public class InterPrologTest {
    
    static XSBSubprocessEngine engine;
    
    public InterPrologTest(String prologbin, String floradir) throws Exception {
        engine = new XSBSubprocessEngine(prologbin);
        String[] commands = initCommandStrings(floradir);
        for(int i = 0; i < commands.length; i++) {
            boolean cmdsuccess = engine.deterministicGoal(commands[i]);
            if(!cmdsuccess){
                System.err.println("FLORA-2 startup failed");
            }
        }
    }
    
    static String[] initCommandStrings(String FloraRootDir)
    {
        return new String[] {
            "import {}/1 from clpr",
            "(import bootstrap_flora/0 from flora2)",
            "asserta(library_directory('" + FloraRootDir + "'))",   
            "consult(flora2)",
            "bootstrap_flora",
            "import flora_query/5 from flora2",
            "import flora_decode_oid_as_atom/2 from flrdecode"
        };
    }
    
    class PrologMessageListener implements PrologOutputListener {
        private StringBuffer sb = new StringBuffer();
        
        
        public PrologMessageListener(){}
        
        public void clear(){
            sb = new StringBuffer();
        }
        
        @Override
        public void printStderr(final String str) {
            System.out.println(str);
        };
        @Override
        public void printStdout(final String str) {
            // This would cause all stdout to be printed
            //System.out.println(str);
        };
        
        @Deprecated
        public void print(String str) {
            synchronized (sb) {
                sb.append(str);
            }
        }
        
        /**
         * @return Returns the latest output string
         */
        public String getString(){
            sb.append("testing");
            return sb.toString();
        }
        
    }
    
    public void testWeirdModuleName() throws Exception {
        try{
            File tempfloradir = new File("test/.flora_aux_files");
            if (tempfloradir.exists()){
                for (File file : tempfloradir.listFiles())
                    file.delete();
                tempfloradir.delete();
            }
            tempfloradir = new File("test/.ergo_aux_files");
            if (tempfloradir.exists()){
                for (File file : tempfloradir.listFiles())
                    file.delete();
                tempfloradir.delete();
            }
            
            String contentDir = new File(".").toString();
            engine.deterministicGoal("asserta(library_directory('" + contentDir + "'))");
            
            PrologMessageListener listener = new PrologMessageListener();
            engine.addPrologOutputListener(listener);
            String goal = "S_rnd='\\load(''test/sqwrl.flr''>>main), ?X=abc.',L_rnd=['?X' = Y],flora_query(S_rnd,L_rnd,St,Xw,Ex),buildInitiallyFlatTermModel([L_rnd,St,Xw,Ex],TM)";
            Object[] solutions = engine.deterministicGoal(goal, "[TM]");
            System.out.println(Arrays.toString(solutions));
            System.out.println(listener.getString());
            System.out.println("success");
        } catch (Exception ex){
            System.out.println(ex.toString());
            System.out.println("failed");
        }
    }
    
    public static void main(String[] args) throws Exception {
        String prologbin = args[0];
        String floradir = args[1];
        InterPrologTest iptest = new InterPrologTest(prologbin,floradir);
        iptest.testWeirdModuleName();
        iptest.engine.shutdown();
    }
    
}
