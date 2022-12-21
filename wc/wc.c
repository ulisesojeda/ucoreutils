#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#include "wc.h"


void usage() {
  fputs("Usage: uwc [-m/--chars] [-l/--lines] [-h/--help] [FILE]\n\n", stdout);
}


char* wc(char* f_name, enum mode m) {
  int MAX_BUFF = 100;
  char *line = NULL;
  size_t len, lines = 0, chars = 0;
  ssize_t nread;
  char* output = (char*)malloc(sizeof(char) * MAX_BUFF);
  
  FILE* f = fopen(f_name, "r");
  if (f == NULL) {
    perror("Error opening file");
    return NULL;
  }
  while ((nread = getline(&line, &len, f)) != -1) {
    lines++;
    chars += nread;
  }

  fclose(f);
  int nwrite = sprintf(output, "  %ld %ld %s\n", lines, chars, f_name);
  
  return nwrite > 0 ? output: NULL; 
}

