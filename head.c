#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>


void usage() {
  fputs("Usage: uhead [-n/--lines] [-h/--help] [FILE]\n\n", stdout);
}

int head(char* file, long lines) {
  char *line = NULL;
  size_t len = 0;
  ssize_t nread;
  FILE *f = fopen(file, "r");

  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }

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

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"lines", required_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  long lines = 10;
  char *endptr;

  while ((c = getopt_long(argc, argv, "n:h", long_options, NULL)) != -1) {
    switch(c) {
      case 'n':
        lines = strtol(optarg, &endptr, 10);
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
 
  int exit_code = head(argv[optind], lines);
  exit(exit_code);
  
}
