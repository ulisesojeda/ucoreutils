#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/utsname.h>

enum OPT {ALL=0, KNAME=1, NNAME=2, KREL=4, KVER=8, MACH=16, PROC=32, HARD=64, OPER=128};

void usage() {
  fputs("Usage: uuname [-h/--help] [FILE]\n\n", stdout);
}

int _uname(int opts) {
  struct utsname uts;
  char res[100];
  uname(&uts);
  if (opts & KNAME) { strcat(res, uts.sysname); strcat(res, " "); }
  if (opts & NNAME) { strcat(res, uts.nodename); strcat(res, " "); }
  if (opts & KREL) { strcat(res, uts.release); strcat(res, " "); }
  if (opts & KVER) { strcat(res, uts.version); strcat(res, " "); }
  if (opts & MACH) { strcat(res, uts.machine); strcat(res, " "); }
  
  printf("%s\n", res);
  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"all", no_argument, 0, 'a'},
        {"kernel-name", no_argument, 0, 's'},
        {"nodename", no_argument, 0, 'n'},
        {"kernel-relase", no_argument, 0, 'r'},
        {"kernel-version", no_argument, 0, 'v'},
        {"machine", no_argument, 0, 'm'},
        {"processor", no_argument, 0, 'p'},
        {"hardware-platform", no_argument, 0, 'i'},
        {"operating-system", no_argument, 0, 'o'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  int opts = 0;

  while ((c = getopt_long(argc, argv, "asnrvmpioh", long_options, NULL)) != -1) {
    switch(c) {
      case 'h':
        usage();
        exit(0);
        break;
      case 'a':
        opts = KNAME | NNAME | KREL | KVER | MACH | PROC | HARD | OPER;
        break;
      case 's':
        opts |= KNAME;
        break;
      case 'n':
        opts |= NNAME;
        break;
      case 'r':
        opts |= KREL;
        break;
      case 'v':
        opts |= KVER;
        break;
      case 'm':
        opts |= MACH;
        break;
      default:
        usage();
        exit(1);
    }
  }
 
  opts = (opts != 0) ? opts : KNAME;
  int exit_code = _uname(opts);
  exit(exit_code);
  
}
