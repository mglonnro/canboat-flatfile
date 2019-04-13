#include <stdio.h>
#include <strings.h>
#include <time.h> 

#define BUFSIZE 	2048

int
main(int argc, char **argv)
{

  if (argc < 2) {
    printf("usage: %s <basename>\n", argv[0]);
    return 1;
  }

  char str[BUFSIZE];
  FILE *f = NULL;
  char prevfile[256];
  prevfile[0] = 0;

  while (fgets(str, sizeof str, stdin) != NULL) {
    // File handling
    char fname[256];
    
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
   
    sprintf(fname, "%s-%d%02d%02d.log", argv[1], tm.tm_year + 1900, tm.tm_mon+1, tm.tm_mday);
    
    if (strcmp(fname, prevfile) || !f) {
      // Changing fate
      if (f) {
	fclose(f);
      }

      f = fopen(fname, "a"); 
      if (!f) {
        fprintf(stderr, "FATAL: Could not open file %s for appending.", argv[1]);
        return 1;
      }

      strcpy(prevfile, fname);
    }
    
    // Print to stdout for piping
    printf("%s", str);

    // Save to log
    fprintf(f, "%s", str);
    fflush(f);

    str[strlen(str) - 1] = 0;
  }

  fclose(f); 
  return 0;
}
