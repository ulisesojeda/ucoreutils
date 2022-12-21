#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


void usage() {
  fputs("Usage: uuniq [-h/--help] [FILE]\n\n", stdout);
}


int uniq(char* f_name) {
  char *line, *last = NULL;
  size_t len = 0;
  ssize_t nread;
  
  FILE* f = fopen(f_name, "r");
  if (f == NULL) {
    perror("Error opening file");
    return 1;
  }

  while ((nread = getline(&line, &len, f)) != -1) {
    if (last == NULL) {
        printf("%s", line);   
    }
    else {
        if ( strcmp(line, last) )
          printf("%s", line);   
    }

    last = (char*)realloc(last, strlen(line) + 1);
    strncpy(last, line, strlen(line));
  }

  fclose(f);
  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;

  while ((c = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
    switch(c) {
      case 'h':
        usage();
        exit(0);
        break;
      default:
        usage();
        exit(1);
    }
  }
 
  int exit_code = uniq(argv[optind]);
  exit(exit_code);
  
}
