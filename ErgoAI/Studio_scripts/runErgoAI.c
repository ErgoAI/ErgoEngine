
// Compile with: cl.exe runErgoAI.c shlwapi.lib
// Will call without more memory
//    call:  java -jar ErgoEngine\ErgoAI\ergo_lib\ergo2java\java\ergoStudio.jar
// If more memory requested
//    call:  java -Xmx3G -jar ErgoEngine\ErgoAI\ergo_lib\ergo2java\java\ergoStudio.jar

#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <process.h>

#include <tchar.h>
#include <shlwapi.h>
#include <strsafe.h>

#define JARPATH  "ErgoEngine\\ErgoAI\\ergo_lib\\ergo2java\\java\\ergoStudio.jar"
// we assume that the installer also uses the ErgoEngine\\ErgoAI file naming
// scheme, like in Linux, so we do not implement the scheme below
//#define JARPATH2 "ErgoAI\\ergo_lib\\ergo2java\\java\\ergoStudio.jar"

int fileExists(TCHAR * file);

int main(int argc, char *argv[])
{ 
  char javaw[MAX_PATH], cmd[MAX_PATH+2],
    studio_jar_buf[MAX_PATH], studio_jar_quoted[MAX_PATH+2],
    additional_jar_buf[MAX_PATH];
  char *backslash_ptr;
  long bufLen = sizeof(javaw);
  HRESULT hr = AssocQueryString(0,ASSOCSTR_EXECUTABLE,".jar",NULL,javaw,&bufLen);
  int pid, status, path_sz;
  HRESULT retcode;
  int moreMemory = FALSE; // flag that tells if we need to run with more memory
  char
    current_dir_quoted[MAX_PATH+2],
    current_dir[MAX_PATH];

  if (FAILED(hr)) {
    // handle error
    fprintf(stderr,"+++ Error: Java is not properly installed: no handler is associated with JAR files\n");
    return -1;
  }

  sprintf(cmd,"\"%s\"",javaw); // add quotes for proper string

  fprintf(stderr,"Java handler:        %s\n",javaw);
  fprintf(stderr,"Java cmd:            %s\n",cmd);
  fprintf(stderr,"This cmd:            %s\n",argv[0]);

  studio_jar_buf[0] = '\0';
  retcode = StringCbCat(studio_jar_buf,strlen(argv[0])+1,argv[0]);
  if (FAILED(retcode)) {
    fprintf(stderr,"+++ Error: not enough memory (1)\n");
    return -1;
  }

  backslash_ptr = strrchr(studio_jar_buf,'\\');
  if (backslash_ptr==NULL) {
    studio_jar_buf[0] = '\0';
  } else {
    *(backslash_ptr+1) = '\0';
  }
  // Now studio_jar_buf contains the directory of argv[0] - save it
  *current_dir = '\0';
  retcode = StringCbCat(current_dir,strlen(studio_jar_buf)+1,studio_jar_buf);
  if (FAILED(retcode)) {
    fprintf(stderr,"+++ Error: not enough memory (2)\n");
    return -1;
  }

  if (current_dir[0] == '\0') {
    current_dir[0] = '.';
    current_dir[1] = '\0';
  }

  // zap trailing backslash
  if (current_dir[strlen(current_dir) - 1] == '\\')
    current_dir[strlen(current_dir)-1] = '\0';

  sprintf(current_dir_quoted,"\"%s\"",current_dir); // add quotes for cmd line

  fprintf(stderr,"This dir:            %s\n",current_dir);
  fprintf(stderr,"                     %s\n",current_dir_quoted);

  additional_jar_buf[0] = '\0';
  retcode = StringCbCat(additional_jar_buf,strlen(studio_jar_buf)+1,studio_jar_buf);
  if (FAILED(retcode)) {
    fprintf(stderr,"+++ Error: not enough memory (3)\n");
    return -1;
  }

  path_sz = strlen(JARPATH) + strlen(studio_jar_buf) + 1;
  retcode = StringCbCat(studio_jar_buf,path_sz,JARPATH);
  if (FAILED(retcode)) {
    fprintf(stderr,"+++ Error: not enough memory (4)\n");
    return -1;
  }
  sprintf(studio_jar_quoted,"\"%s\"",studio_jar_buf); // add quotes for cmd line

  fprintf(stderr,"ErgoAI Studio jar:     %s\n",studio_jar_buf);
  fprintf(stderr,"                       %s\n",studio_jar_quoted);


  if (moreMemory) {
    pid = _execl(javaw,cmd,"-Xmx1900M",
                 //"-Djava.net.preferIPv4Stack=true", // unused: required for EHcache
                 "-jar",studio_jar_quoted,
                 "-basedir",current_dir_quoted,NULL);
  } else {
    pid = _execl(javaw,cmd,
                 //"-Djava.net.preferIPv4Stack=true", // unused: required for EHcache
                 "-jar",studio_jar_quoted,
                 "-basedir",current_dir_quoted,NULL);
  }
  // wait for subprocess to finish
  _cwait(&status,pid,0);

  fprintf(stderr,"ErgoAI start failed. Error code = %d\n",pid);
  return -1;
}


int fileExists(TCHAR * file)
{
   WIN32_FIND_DATA FindFileData;
   HANDLE handle = FindFirstFile(file, &FindFileData) ;
   int found = handle != INVALID_HANDLE_VALUE;
   if(found) {
       FindClose(handle);
   }
   return found;
}
