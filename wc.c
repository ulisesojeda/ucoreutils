#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


enum mode {CHARS, LINES};


void usage() {
  fputs("Usage: uwc [-m/--chars] [-l/--lines] [-h/--help] [FILE]\n\n", stdout);
}


int wc(char* f_name, enum mode m) {
  char *line = NULL;
  size_t len, lines, chars = 0;
  ssize_t nread;
  
  FILE* f = fopen(f_name, "r");
  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }
  while ((nread = getline(&line, &len, f)) != -1) {
    lines++;
    chars += nread;
  }

  printf("  %ld %ld %s\n", lines, chars, f_name);

  fclose(f);
  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"chars", no_argument, 0, 'm'},
        {"lines", no_argument, 0, 'l'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  enum mode m;
  

  while ((c = getopt_long(argc, argv, "mlh", long_options, NULL)) != -1) {
    switch(c) {
      case 'm':
        m = CHARS;
        break;
      case 'l':
        m = LINES;
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
 
  int exit_code = wc(argv[optind], m);
  exit(exit_code);
  
}
