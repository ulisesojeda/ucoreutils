#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <dirent.h>


void usage() {
  fputs("Usage: umkdir [-p/--parents] [-m/--mode=MODE] [-h/--help] DIRECTORY... \n\n", stdout);
}


int umkdir(char** f_names, int len, const char* mode, bool parents) {
  // TODO
  // -m a=rwx,u=rwx,g=rx,o=r
  char *token;
  char *scopy;
  const char sep[2] = "/";
  mode_t mask = umask(0);
  umask(mask);

  for (int i=0; i<len; i++) {
    scopy = (char*)malloc(strlen(f_names[i]) + 1);
    strcpy(scopy, f_names[i]);
    token = strtok(scopy, sep);

    if ( token == NULL || strcmp(token, f_names[i]) == 0 ) {
      if ( mkdir(f_names[i], ~mask & 0777) == -1) {
        perror("Error creating directory");
        return 1;
      }
    }
    else {
      char *fpath = (char*)malloc(strlen(f_names[i]) + 1);
      while ( token != NULL ) {
        strcat(fpath, token);
        strcat(fpath, sep);
        if (opendir(fpath) == NULL) {
          mkdir(fpath, ~mask & 0777);
        }
        token = strtok(NULL, sep);
      }
      free(fpath);
    }
  }
  free(scopy);
  return 0; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"parents", no_argument, 0, 'p'},
        {"mode", required_argument, 0, 'm'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  char const *mode = NULL;
  bool parents = false;

  while ((c = getopt_long(argc, argv, "pm:h", long_options, NULL)) != -1) {
    switch(c) {
      case 'p':
        parents = true;
        break;
      case 'm':
        mode = optarg;
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
 
  int exit_code = umkdir(&argv[optind], argc - optind, mode, parents);
  exit(exit_code);
  
}
