/*
  File: C_calling_Ergo.c
  Author: Michael Kifer (michael.kifer@coherentknowledge.com)

  EXAMPLE of starting Ergo from a C program, querying, loading Ergo files,
  exception handling, etc.

  This example will also work with Flora-2, if ergo_query/5 is replaced
  with flora_query/5.

  THIS FILE IS IN THE PUBLIC DOMAIN.

  IT IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED,
  INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY CLAIM, DAMAGES OR
  OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
  ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
  OTHER DEALINGS IN THE SOFTWARE.
*/

/*
  INSTRUCTIONS:

  Use the Ergo query system{archdir=?E} to find the correct architecture
  dependent paths for your system, which are used below for compilation and
  linking.

  ### Linux/Mac
  # Compile then link: replace "$HOME/XSB" with where your XSB is.

  *** The gcc commands below are for Ubuntu Linux 64 bits.

      gcc -I$HOME/XSB/XSB/emu -I$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu -Wall -c C_calling_Ergo.c 
      gcc -o C_calling_Ergo C_calling_Ergo.o $HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/saved.o/xsb.o -L$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/lib -lm -ldl -Wl,-export-dynamic -lpthread
  
  # OR compile & link in one command:
    Using static linking with XSB:

      gcc -I$HOME/XSB/XSB/emu -I$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu -Wall C_calling_Ergo.c -o C_calling_Ergo $HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/saved.o/xsb.o -L$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/lib -lm -ldl -Wl,-export-dynamic -lpthread

  *** On a Mac (with gcc), change the architecture directory as appropriate.
      Eg, i386-apple-darwin17.3.0 instead of x86_64-unknown-linux-gnu.
      Also change linking options, as shown below.
      On a Mac with clang - figure out.

      gcc -I$HOME/XSB/XSB/emu -I$HOME/XSB/XSB/config/i386-apple-darwin17.3.0 -Wall C_calling_Ergo.c -o C_calling_Ergo $HOME/XSB/XSB/config/i386-apple-darwin17.3.0/saved.o/xsb.o -L$HOME/XSB/XSB/config/i386-apple-darwin17.3.0/lib -lm -ldl -lpthread

    Using dynamic linking with XSB (on the Mac, change .so to .dylib and also
    change the architecture directory and linking options, as above):

      gcc -I$HOME/XSB/XSB/emu -I$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu -Wall C_calling_Ergo.c -o C_calling_Ergo $HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/bin/libxsb.so -L$HOME/XSB/XSB/config/x86_64-unknown-linux-gnu/lib -lm -ldl -Wl,-export-dynamic -lpthread
  
  # To run:
       C_calling_Ergo  small-pos-int
    or
       C_calling_Ergo  small-pos-int xsb-installdir ergo-installdir
  
  ###WINDOWS
  # Compile then link: replace "h:\XSB" with where your XSB is.

      cl.exe /DWINDOWS_IMP /Ih:\XSB\XSB\emu /Ih:\XSB\XSB\config\x64-pc-windows /c C_calling_Ergo.c 
      link.exe /nologo /STACK:10000000 C_calling_Ergo.obj xsb.lib /libpath:"h:\XSB\XSB\config\x64-pc-windows\bin"
  
  # OR compile & link in one command

      cl.exe C_calling_Ergo.c /F10000000 /DWINDOWS_IMP /Ih:\XSB\XSB\emu /Ih:\XSB\XSB\config\x64-pc-windows h:\XSB\XSB\config\x64-pc-windows\bin\xsb.lib
  
  # To run:
      set PATH=h:\xsb\xsb\config\x64-pc-windows\bin;%PATH% & C_calling_Ergo small-pos-int
    or
       set PATH=h:\xsb\xsb\config\x64-pc-windows\bin;%PATH% & C_calling_Ergo small-pos-int xsb-installdir ergo-installdir
  
  ### This example illustrates all the important aspects of querying Ergo and
  getting the results. It is recommended that Ergo queries are arranged so as
  to return only primitive bindings to variables: integer, float, or string
  (atom). More complex structures should be handled inside Ergo itself.

  Only very advanced users should attempt to deal with complex terms as
  variable bindings in a C program (rather than in Ergo).
  Details on how to use XSB's C API are in the XSB Manual, vol. 1 -- see
  Chapters "Embedding XSB in a Process" and "Foreign Language Interface".
*/

// KEEP THESE!!
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// KEEP THESE!!
#include "xsb_config.h"
#include "cinterf.h"

/*
** Tell where the XSB/Ergo top folders are. Change for your installation!
** Windows: use double backslashes to separate names in paths.
** The variables __XSB_HOME__, __ERGO_HOME__ will be used unless alternative
** locations are passed as parameters in argv[2], argv[3], respectively.
*/

// **** Change these or provide the required directories as arguments when
//      invoking this program  -- see above on how to run this program.
//
//  __XSB_HOME__ -- must be the XSB installation directory.
//  __ERGO_HOME__ -- must be the top Ergo directory (this is a child directory
//      called Ergo inside the ErgoAI installation directory)
// The right value for your system can be obtained by running this Ergo query:
//      system{xsbdir=?E}.
//      system{installdir=?E}.

#ifdef WIN_NT
// Example for Windows
#define __XSB_HOME__    "h:\\XSB\\XSB"
#define __ERGO_HOME__   "h:\\ERGO\\Ergo"
#else
// Example for Linux/Mac
#define __XSB_HOME__    "/home/path-to-xsb/XSB"
#define __ERGO_HOME__   "/home/path-to-xsb/Ergo"
#endif

// these are fixed; used below
#define  ERGO_INIT_CMDS_NUMBER  2
#define  XSB_PARAM_NUMBER       5

// KEEP THESE!!
// gets the term to which variable #VarNum is bound
#define xsb_var_term(VarNum) (p2p_arg(reg_term(CTXTc 2),VarNum))
// renders the binding of variable #VarNum as a C string
#define xsb_var_to_string(VarNum) (print_pterm_fun(xsb_var_term(VarNum)))
// checks if Var number VarNum is bound to an XSB atom String
#define xsb_var_strequal(VarNum,String)  \
  (strcmp(xsb_var_to_string(VarNum),String) == 0)

int main(int argc, char** argv)
{
    int iterations;
    char *xsb_home, *ergo_home;

    // set xsb_home and ergo_home
    if (argc == 2) {
        iterations = atoi(argv[1]);
        xsb_home  = __XSB_HOME__;
        ergo_home = __ERGO_HOME__;
    } else if (argc == 4) {
        iterations = atoi(argv[1]);
        xsb_home  = argv[2];
        ergo_home = argv[3];
    } else {
      fprintf (stderr,"Usage: %s <iterations>\n", argv[0]);
      fprintf (stderr,"Usage: %s <iterations> <XSB-installdir> <Ergo-installdir>\n", argv[0]);
      exit(0);
    }

    printf("Number of iterations: %d\n", iterations);

    // PART 1: initialize XSB
    // DON'T CHANGE THESE unless equivalent!!!
    char *new_argv[XSB_PARAM_NUMBER];
    // Only new_argv[0] is mandatory and must be set to XSB's installation dir.
    // Other parameters just remove XSB chatter.
    // Don't change new_argv[1] - new_argv[4] unless you are very advanced.
    new_argv[0] = xsb_home;
    new_argv[1] = "--noprompt";
    new_argv[2] = "--quietload";
    new_argv[3] = "--nofeedback";
    new_argv[4] = "--nobanner";

    /*
    **  xsb_init() returns XSB_ERROR or XSB_SUCCESS
    */
    
    // DON'T CHANGE THESE except for the error message!!!
    if (xsb_init(XSB_PARAM_NUMBER, new_argv) == XSB_ERROR) {
      fprintf(stderr,"*** Cannot start XSB - possibly wrong installation folder given: %s\n", new_argv[0]);
      return XSB_ERROR;
    }
    
    // PART 2: initialize Ergo
    // DON'T CHANGE THESE (except, maybe, consistent renaming of variables)!!!
    char set_home_libdir_cmd[500], load_ccinit_cmd[200];
    static char *init_cmds[ERGO_INIT_CMDS_NUMBER];
    sprintf(set_home_libdir_cmd,"asserta(library_directory('%s')).",ergo_home);
    sprintf(load_ccinit_cmd,"['%s/cc/flrcc_init'].", ergo_home);
    init_cmds[0] = set_home_libdir_cmd;
    init_cmds[1] = load_ccinit_cmd;
    
    /*
      NOTE: xsb_command_string(), bvelow, returns
                               XSB_ERROR, XSB_SUCCESS, XSB_FAILURE.
            These are integers defined in XSB's cinterf.h.
    */
    // DON'T CHANGE THESE!!! (except, possibly, the error message)
    // Execute Ergo initialization commands
    for (int i = 0; i < ERGO_INIT_CMDS_NUMBER; i++) {
      if (xsb_command_string(init_cmds[i]) == XSB_ERROR) {
        fprintf(stderr, "*** Error initializing Ergo in init command: %s\n",
                init_cmds[i]);
        return XSB_ERROR;
      } 
    }

    /*
      NOTE: If additional (custom) initialization is needed, put the desired
      XSB commands in a file, e.g., myinit.P, and load it here like this:

          xsb_command_string("['path-to-myinit/myinit.P'].");
    */

    fprintf(stderr,"Ergo initialization finished.\n");

    // THE REST SHOWS how to execute various commands and queries in Ergo,
    // getting query results, checking for errors.
    
    // PART 3: execute Ergo commands and queries; get results
    // insert data into predicate p/3
    char cmd_string[500];
    char *query;
    for (int i=0; i<iterations; ++i) {
      // construct insertion command
      sprintf(cmd_string,
              "ergo_query('insert{p(a(%d),b(abc%d),c(%f))}.',[],_,_,_).",
              i,i,(float)i);

      /*
        NOTE: xsb_query_string(), xsb_next(), below, both return
                                  XSB_ERROR, XSB_SUCCESS, XSB_FAILURE.
              xsb_close_query() returns XSB_ERROR, XSB_SUCCESS.
      */
      // execute insertion command
      if (XSB_ERROR == xsb_query_string(cmd_string)) {
        fprintf(stderr, "Error in query: %s\n", cmd_string);
      }
      // close insertion command
      xsb_close_query();

      // a more complicated way to set variables - through XSB rather than C
      xsb_make_vars(5); // 5 because X+Y and the three _,_,_ in ergo_query(...)
      xsb_set_var_int(i,1);
      xsb_set_var_float(i+1.2,2);
      query = "ergo_query('insert{q(?X,?Y)}.',['?X'=X,'?Y'=Y],_,_,_).";
      if (XSB_ERROR == xsb_query_string(query)) {
        fprintf(stderr, "Error in query: %s\n", query);
      }
      // close the insertion query
      xsb_close_query();
    }

    printf ("Done inserting. Now do querying.\n");
      
    // execute query Q1
    query = "ergo_query('p(a(?X),b(?Y),c(?Z)).',['?X'=X,'?Y'=Y,'?Z'=Z],CompileStatus,IsUndef,Exception), flora_get_message_from_errorball(Exception,ExceptionMsg).";
    // The next one aborts on purpose -- to test exceptions.
    //query = "ergo_query('p(a(?X),b(?Y),c(?Z)), abort(''test abort'')@\\\\sys.',['?X'=X,'?Y'=Y,'?Z'=Z],CompileStatus,IsUndef,Exception), flora_get_message_from_errorball(Exception,ExceptionMsg).";

    int rc = xsb_query_string(query);
    if (rc == XSB_ERROR) {
      fprintf(stderr, "Error in query Q1: %s\n", query);
    }

    // Here we are showing how to check for exceptions during Ergo runtime.
    // Also, how to check if a query unswer is True or Undefined.

    // Note: 4 here is var #4 in the query == CompileStatus.
    //       6 is variable #6 in Q1: Exception.
    // Exception value "normal" means everything is ok.
    // If CompileStatus has "success" in it, compilation was successful.
    // Otherwise, it will have "failure" in it.
    // If compilation was UNsuccessful, Exception will be "compilation_failed".
    // Note: compilation may succeed, but the query may still have a runtime
    // exception.
    printf("CompileStatus: %s\n", xsb_var_to_string(4));
    printf("Raw exception: %s\n", xsb_var_to_string(6));
    // Var #7 is ExceptionMsg - the human-readable form of exception
    printf("Readable exception: %s\n", xsb_var_to_string(7));

    // var #6 is Exception. "normal" means no exceptions at Ergo runtime.
    if (xsb_var_strequal(6,"normal"))
      printf("no exceptions\n");
    else
      printf("has an exception\n");

    // Var #6 is Exception in query Q1
    if (!xsb_var_strequal(6,"normal"))
      fprintf(stderr, "Q1 has runtime exception: %s\n", xsb_var_to_string(7));
    else if (rc == XSB_SUCCESS)
      do {
        // print query results
        printf("Answer Q1: %d, %s, %f, truth val: %s\n",
               (int)xsb_var_int(1),       // arg1 in p/3 is an int
               (char*)xsb_var_string(2),  // because arg2 in p/3 is an atom
               (float)xsb_var_float(3),   // because arg3 in p/3 is a float
               // Var #5 is IsUndef, the indicator telling if the truth value of
               // this answer is True or Undefined. 0 means True.
               ((int)xsb_var_int(5) == 0 ? "true" : "undefined")
               );
      } while (xsb_next() == XSB_SUCCESS); // get next answer
    else
      printf("Q1 has no results.\n");
    
    // close query Q1
    xsb_close_query();

    // another query: Q2
    query = "ergo_query('q(?X,?Y).',['?X'=X,'?Y'=Y],_,_,_).";
    rc = xsb_query_string(query);
    if (rc == XSB_ERROR) {
      fprintf(stderr, "Error in query Q2: %s\n", query);
    }

    if (rc == XSB_SUCCESS)
      do {
        // print query2 results
        printf("Answer Q2: %d, %f\n",
               (int)xsb_var_int(1),       // arg1 in q/2 is an int
               (float)xsb_var_float(2));  // because arg2 in q/2 is a float
      } while (xsb_next() == XSB_SUCCESS); // get next answer
    else
      printf("Q2 has no results.\n");

    // close query Q2
    xsb_close_query();
     
    printf ("Done querying. Now do deletions.\n");
    
    for (int i=0; i<iterations; ++i) {
      // construct deletion command for p/3 and q/2
      sprintf(cmd_string,
              "ergo_query('delete{p(a(%d),b(_),c(_)), q(%d,_)}.',[],_,_,_).",
              i, i);
      // execute deletion command
      if (XSB_ERROR == xsb_query_string(cmd_string)) {
        fprintf(stderr, "Error in query: %s\n", cmd_string);
      }
      // close the deletion query
      xsb_close_query();
    }
    printf ("Done with deletions.\n");

    // load a demo file
    // Pay attention to '[''demos/family_obj''].'
    // The outer quotes needed because the query argument to ergo_query must be
    // inside '...'. The inner quotes needed because the file has special chars
    // (ie, /), so it must be quoted.
    // These quotes are doubled because they are inside '...'.
    printf ("Loading a file...\n");
    if (XSB_ERROR == xsb_query_string("ergo_query('[''demos/family_obj''].',[],_,_,_).")) {
      fprintf(stderr, "Error while loading the demo family_obj.flr\n");
    }
    xsb_close_query();
}

