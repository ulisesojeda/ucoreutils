#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <string.h>
#include <stdbool.h>


void usage() {
  fputs("Usage: utr [-d/--delete] [h/--help] SET1 [SET2]\n\n", stdout);
}

int tr(char** sets, int len, bool del) {
  if (len < 1 || len > 2) {
    usage();
    exit(1);
  }
  
  bool delete = len==1 ? true : del;
  setvbuf(stdout, NULL, _IONBF, 0);
  char c, outc;
  char *set_1 = sets[0];
  char *set_2 = sets[1];

  while ( (c = getc(stdin)) != EOF ) {
    outc = c;

    for ( int i = 0; i < strlen(sets[0]); i++ ) {
      if ( c == set_1[i])
        outc = delete ? 0 : set_2[i];
    }

    putc(outc, stdout);
  }

  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"delete", no_argument, 0, 'd'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  bool del = false;

  while ( (c = getopt_long(argc, argv, "dh", long_options, NULL)) != -1 ) {
    switch(c) {
      case 'd':
        del = true;
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
 
  int exit_code = tr(&argv[optind], argc - optind, del);
  exit(exit_code);
}
