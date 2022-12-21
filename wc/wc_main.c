#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include "wc.h"


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
 
  char* wc_out = wc(argv[optind], m);
  if (wc_out)
    printf("%s", wc_out);
  else
    printf("ERROR\n");
  
  return wc_out != NULL;
  
}
