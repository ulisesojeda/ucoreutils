#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <pwd.h>


void usage() {
  fputs("Usage: uwhoami [-h/--help] [FILE]\n\n", stdout);
}

int whoami() {
  uid_t uid = getuid();
  struct passwd *pass = getpwuid(uid);
  printf("%s\n", pass->pw_name);
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
 
  int exit_code = whoami();
  exit(exit_code);
  
}
