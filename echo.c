#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>


void usage() {
  fputs("Usage: uecho [-n] [-h/--help] [FILE]\n\n", stdout);
}

int echo(char** args, int len, bool trailing) {
  for(int i = 0; i < len; i++) {
      char* sep = (i < len - 1) ? " " : "";
      printf("%s%s", args[i], sep);
  }
  
  if (trailing) printf("\n");
  
  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"no-trailing", no_argument, 0, 'n'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  bool trailing = true;

  while ((c = getopt_long(argc, argv, "nh", long_options, NULL)) != -1) {
    switch(c) {
      case 'n':
        trailing = false;
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
 
  int exit_code = echo(&argv[optind], argc - optind, trailing);
  exit(exit_code);
  
}
