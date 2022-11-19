#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


void usage() {
  fputs("Usage: utee [-a/--append] [-i/--ignore-interrupts] [-h/--help] [FILE]\n\n", stdout);
}

int tee(char** f_names, int len, char* mode) {
  int f_len = len + 1;
  char **names = (char**)malloc(sizeof(char*) * f_len);
  names[0] = "stdout";
  for(int i=0; i<len; i++) {
    names[i+1] = f_names[i];
  }
   
  FILE **files = (FILE**)malloc(sizeof(FILE*) * f_len);
  setvbuf(stdout, NULL, _IONBF, 0);
  int ok = 0;
 
  files[0] = stdout;
  for(int i = 1; i < f_len; i++) {
    files[i] = fopen(names[i], mode);
    if (files[i] != NULL) {
      setvbuf(files[i], NULL, _IONBF, 0);
    }
    else {
      perror("Error opening file");
      ok = 1;
    }
  }
  
  free(names);

  int bytes_read;
  int BUFSIZE = 8 * 1024;
  char buffer[BUFSIZE];
  int written = 0;

  while((bytes_read = read(STDIN_FILENO, buffer, sizeof buffer)) > 0) {
    for(int i = 0; i < f_len; i++) {
      if (files[i] != NULL) {
        written = fwrite(buffer, bytes_read, 1, files[i]);
        if (written != 1) {
          perror("Error writing to file");
          ok = 1;
        }
      }
    }
  }

  for(int i = 1; i < f_len; i++) {
    if (files[i] != NULL) {
      int c = fclose(files[i]);
      if (c != 0) {
        perror("Error closing file");
        ok = 1;
      }
    }
  }

  free(files);
  return ok; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"append", no_argument, 0, 'a'},
        {"help", no_argument, 0, 'h'},
        {"ignore-interrupts", no_argument, 0, 'i'},
        {0, 0, 0, 0}
      };
  int c;
  char *mode = "w";

  while ((c = getopt_long(argc, argv, "aih", long_options, NULL)) != -1) {
    switch(c) {
      case 'a':
        mode = "a";
        break;
      case 'i':
        signal (SIGINT, SIG_IGN);
        signal (SIGPIPE, SIG_IGN);
        break;
      case 'h':
        usage();
        exit(0);
        break;
      default:
        usage();
        exit(1);
    }
  }
 
  int exit_code = tee(&argv[optind], argc - optind, mode);
  exit(exit_code);
  
}
