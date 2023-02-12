import net.sf.flora2.API.PrologFlora;
//import org.junit.Assert;
//import org.junit.Test;

import java.io.File;

/**
 * Created by elenius on 12/16/16.
 */
public class LoadTest {

    public static void main(String[] args) {
        PrologFlora pf = new PrologFlora();
        File testfile = new File("foo/test2.flr");
        String path = testfile.getAbsolutePath();
        System.out.println("Loading file: " + path);
        //Assert.assertTrue(testfile.exists());
        boolean success = pf.loadFile(path,"main");
        System.out.println("Success = "+success);
        //Assert.assertTrue(success);
	System.exit(0);
    }
}
