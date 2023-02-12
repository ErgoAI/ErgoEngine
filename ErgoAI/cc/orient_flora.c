/* File:      orient_flora.c - find out where FLORA-2 is installed.
** Author(s): kifer
** Contact:   flora-users@lists.sf.net
** 
** Copyright (C)  by
**      The Research Foundation of the State University of New York, 2013-2018.
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
*/

/*
  Parts of this code are cannibalized from XSB's orient_xsb.c and pathnames.c
*/


#ifdef WIN_NT
#include <direct.h>
#include <io.h>
#else
#include <unistd.h>
#endif

#ifdef WIN_NT
#define PATH_SEPARATOR ';'
#define SLASH  '\\'
#else
#define PATH_SEPARATOR ':'
#define SLASH  '/'
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

#ifndef FALSE
#define FALSE  0
#endif
#ifndef TRUE
#define TRUE  (!FALSE)
#endif
#ifndef MAXPATHLEN
#define MAXPATHLEN   1024 
#endif

char executable_path_gl[MAXPATHLEN] = {'\0'};	/* This is set to a real name below */
char *user_home_gl;    	     	     	/* the user $HOME dir or current dir */

char current_dir_gl[MAXPATHLEN];
char flora_dir_gl[MAXPATHLEN];

int is_absolute_filename(char *filename) {

#if defined(WIN_NT) 
  /*  If the file name begins with a "\" or with an "X:", where X is some
   *  character, then the file name is absolute.
   *  We also assume if it starts with a // or /cygdrive/, it's absolute */
  if ( (filename[0] == SLASH) || 
       (isalpha(filename[0]) && filename[1] == ':') ||
       (filename[0] == '/')
       )
    return TRUE;
#else /* Unix */
  if (filename[0] == '/')
    return TRUE;
#endif

  return FALSE;
}


/* transform_cygwin_pathname takes cygwin-like pathnames
/cygdrive/D/..  and transforms them into windows-like pathnames D:\
(in-place).  It assumes that the given pathname is a valid cygwin
absolute pathname */

void transform_cygwin_pathname(char *filename) 
{
  char *pointer;
  char tmp[MAXPATHLEN];
  int diff;

  if (filename[0] == '/') {
    /* MK: unclear what this was supposed to do in case of the files starting
       with // or files of the form /Letter. Changed this to no-op. */
    if (filename[1] == '/') return; /* diff = 1; */
    else if (filename[2] == '\0') return; /* diff = 1; */
    else if (filename[1] == 'c' &&
	     filename[2] == 'y' &&
	     filename[3] == 'g' &&
	     filename[4] == 'd' &&
	     filename[5] == 'r' &&
	     filename[6] == 'i' &&
	     filename[7] == 'v' &&
	     filename[8] == 'e' &&
	     filename[9] == '/') diff = 9;
    else {
      strcpy(tmp,filename);
      strcpy(filename,(char *)user_home_gl);
      strcpy(filename+strlen((char *)user_home_gl),tmp);
      return;
    }

    pointer=filename+diff+1;
    filename[0]=*pointer;
    filename[1]=':';
    filename[2]='\\';
    for(pointer+=1;*pointer;pointer++) 
      if (*pointer == '/')
	*(pointer-diff) = '\\';
      else
	*(pointer-diff) = *pointer;
  
    *(pointer-diff) = '\0';
    return;
  }
}


void set_user_home() {
  char* ret = getcwd(current_dir_gl, MAXPATHLEN-1);
  user_home_gl = (char *) getenv("HOME");
  if ( user_home_gl == NULL ) {
    user_home_gl = (char *) getenv("USERPROFILE"); /* often used in Windows */
    if ( user_home_gl == NULL )
      user_home_gl = current_dir_gl;
  }
#ifdef WIN_NT
  transform_cygwin_pathname(user_home_gl);
#endif
}


// strip two levels from executable_path_gl
char* get_flora_install_dir() {
  char* ptr = NULL;
  strcpy(flora_dir_gl, executable_path_gl);
  ptr = strrchr(flora_dir_gl,SLASH);
  *ptr = '\0';
  ptr = strrchr(flora_dir_gl,SLASH);
  *ptr = '\0';
  return flora_dir_gl;
}


  

/* uses the global executable var */
int main(int argc, char *argv[])
{
  struct stat fileinfo;
  char *path = getenv("PATH");
  int len, found = 0;
  char *pathcounter, save;
  char *myname = argv[0];
  char myname_augmented[MAXPATHLEN];
#ifndef WIN_NT
  int link_len;
#endif

  set_user_home();

#ifndef WIN_NT
#ifndef SIMPLESCALAR
  /* Unix */
  /* if we can read symlink, then it is a symlink */
  if ( (link_len = readlink(myname, myname_augmented, MAXPATHLEN)) > 0 ) {
    /* we can't assume that the value of the link is null-terminated */
    if ( *(myname_augmented+link_len) != '\0' )
      *(myname_augmented+link_len+1) = '\0';
  } else
    strcpy(myname_augmented, myname);
#endif
#else
  /* Windows doesn't seem to have readlink() */
  strcpy(myname_augmented, myname);
  /* if executable doesn't end with .exe, then add it */
  if ( *(myname_augmented + strlen(myname) - 4) != '.'
       || tolower(*(myname_augmented + strlen(myname) - 3)) != 'e'
       || tolower(*(myname_augmented + strlen(myname) - 2)) != 'x'
       || tolower(*(myname_augmented + strlen(myname) - 1)) != 'e' )
    snprintf(myname_augmented, MAXPATHLEN, "%s.exe", myname);
#endif

#ifdef WIN_NT
  /* CygWin32 uses absolute paths like this:
     //<drive letter>/dir1/dir2/...
     actually /cygdrive/<drive letter>/....
     If we find such a path, we transform it to a windows-like pathname.
     This assumes that XSB has been compiled using the native Windows
     API, and is being run from CygWin32 bash (like from the test
     scripts). */
  transform_cygwin_pathname(myname_augmented);
#endif

  if (is_absolute_filename(myname_augmented))
    strcpy(executable_path_gl, myname_augmented);
  else {
    snprintf(executable_path_gl, MAXPATHLEN, "%s%c%s", current_dir_gl, SLASH, myname_augmented);
  }

  /* found executable by prepending cwd. */
  if ((!stat(executable_path_gl, &fileinfo)) && (S_ISREG(fileinfo.st_mode))) {
    printf("%s",get_flora_install_dir());
    return FALSE;
  }

  /* Otherwise, search PATH environment var.
     This code is a modified "which" shell builtin */
  pathcounter = path;
  while (*pathcounter != '\0' && found == 0) {
    len = 0;
    while (*pathcounter != PATH_SEPARATOR && *pathcounter != '\0') {
      len++;
      pathcounter++;
    }

    /* save the separator ':' (or ';' on NT and replace it with \0) */
    save = *pathcounter;
    *pathcounter = '\0';

    /* Now `len' holds the length of the PATH component 
       we are currently looking at.
       `pathcounter' points to the end of this component. */
    snprintf(executable_path_gl, MAXPATHLEN, "%s%c%s", pathcounter - len, SLASH, myname_augmented);

    /* restore the separator and addvance the pathcounter */
    *pathcounter = save;
    if (*pathcounter) pathcounter++;

#ifdef WIN_NT
    found = (0 == access(executable_path_gl, 02));	/* readable */
#else
    found = (0 == access(executable_path_gl, 01));	/* executable */
#endif

    if (found) printf("%s",get_flora_install_dir());
  }

  return FALSE;
}

