#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>

// TODO
// Read from stdin
// Bug when file # lines < # lines to read
// Follow option

void usage() {
  fputs("Usage: utail [-n/--lines] [-h/--help] [FILE]\n\n", stdout);
}

int readlines(FILE *f, long lines) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;

  while (lines > 0) {
    nread = getline(&line, &len, f);    
    if (nread != -1) {
      printf("%s", line);
    }
    lines--;
  }

  free(line);
  fclose(f);
  return 0;
}

FILE* rewind_lines(char* fname, int lines) {
  int n_eol = 0, c;
  FILE *f = fopen(fname, "r");

  fseek(f, -1, SEEK_END);

  while(n_eol < lines + 1) {
    c = getc(f);
    if ( c == '\n' ) n_eol++;
    fseek(f, -2, SEEK_CUR);
  }

  fseek(f, 2, SEEK_CUR);
  return f;
}


int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"lines", required_argument, 0, 'n'},
        {"follow", no_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  long lines = 10;
  char *endptr;
  bool follow = false;

  while ((c = getopt_long(argc, argv, "n:hf", long_options, NULL)) != -1) {
    switch(c) {
      case 'n':
        lines = strtol(optarg, &endptr, 10);
        break;
      case 'h':
        usage();
        exit(0);
        break;
      case 'f':
        follow = true;
        break;
      default:
        usage();
        exit(1);
    }
  }
 
  FILE* f = rewind_lines(argv[optind], lines);
  int exit_code = readlines(f, lines);
  exit(exit_code);
  
}
