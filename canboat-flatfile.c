#include <stdio.h>
#include <string.h>
#include <time.h> 
#include <stdlib.h>

#define BUFSIZE 	2048
#define ROTATEINTERVAL	600

int
main(int argc, char **argv)
{

  if (argc < 2) {
    printf("usage: %s <basename> [boatId] [done]\n", argv[0]);
    return 1;
  }

  char str[BUFSIZE];
  FILE *f = NULL;
  char prevfile[256];
  prevfile[0] = 0;
  time_t prevtime = 0;

  while (fgets(str, sizeof str, stdin) != NULL) {
    // File handling
    char fname[256];
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
   
    
    if (!f || t > (prevtime + ROTATEINTERVAL)) {
      prevtime = t;

      // Changing file
      if (f) {
	fclose(f);
  	strcpy(transfername, fname);
	
	if (fork() == 0) {
          char cmd[2048];

	  // Zip it
          sprintf(cmd, "nice -n 10 gzip %s", transfername);
          system(cmd);

	  if (argc == 4) {
	    // Upload it
	    sprintf(cmd, "nice -n 10 ./upload.js %s %s", argv[2], transfername);
	    int ret = system(cmd);
	    if (!ret) {
	      // Move to done
	      sprintf(cmd, "nice -n 10 mv %s %s", transfername, argv[3]); 
	      system(cmd);
	    } 
	  }

	  exit(0);
	}
      }

      sprintf(fname, "%s-%d%02d%02d-%02d%02d%02d.log", argv[1], tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

      f = fopen(fname, "a"); 
      if (!f) {
        fprintf(stderr, "FATAL: Could not open file %s for appending.", fname);
        return 1;
      }
    }
    
    // Print to stdout for piping
    fprintf(stdout, "%s", str);
    fflush(stdout);

    // Save to log
    fprintf(f, "%s", str);
    fflush(f);

    str[strlen(str) - 1] = 0;
  }

  fclose(f); 
  return 0;
}
