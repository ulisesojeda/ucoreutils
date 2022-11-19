#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdint.h>

#define SHOW_NUMBER    (1 << 0) // 1
#define SHOW_ENDS      (1 << 1) // 2
#define SHOW_TABS      (1 << 2) // 4

#define SET_FLAG(n, f) ((n) |= (f)) 
#define CLR_FLAG(n, f) ((n) &= ~(f)) 
#define TGL_FLAG(n, f) ((n) ^= (f)) 
#define CHK_FLAG(n, f) ((n) & (f))

void usage() {
  fputs("Usage: ucat [-A/--show-all] [-n/--number] [-E/--show-ends] [-T/--show-tabs] [-h/--help] [FILE]...\n\n", stdout);
}

int cat(char** f_names, int len, uint8_t options) {
  FILE **files = (FILE**)malloc(sizeof(FILE*) * len);
  setvbuf(stdout, NULL, _IONBF, 0);
  int ok = 0;
 
  for(int i = 0; i < len; i++) {
    files[i] = fopen(f_names[i], "r");
  }

  int bytes_read;
  size_t r_len = 0;
  int BUFSIZE = 8 * 1024;
  char *lineptr = NULL;
  char *line;
  char buffer[BUFSIZE];
  int written = 0;
  int line_number = 0;

  for(int i = 0; i < len; i++) {
    if (files[i] != NULL) {
      if (CHK_FLAG(options, SHOW_NUMBER) || CHK_FLAG(options, SHOW_TABS) || CHK_FLAG(options, SHOW_ENDS))
      {
        while((bytes_read = getline(&lineptr, &r_len, files[i])) != -1) {
          line = (char*)malloc(strlen(lineptr) - 1);
          strncpy(line, lineptr, strlen(lineptr) - 1);

           if (CHK_FLAG(options, SHOW_NUMBER))
             printf("%d ", (line_number++ + 1));

           if (CHK_FLAG(options, SHOW_TABS)) {
             for(int i=0; i<strlen(line); i++) {
               if (line[i] == '\t')
                 fputs("^I", stdout);
               else
                 fputc(line[i], stdout);
             }
           }
           else
             fputs(line, stdout);
           
           if (CHK_FLAG(options, SHOW_ENDS))
             fputc('$', stdout);
         
           fputs("\n", stdout);
           free(line);
        }
        free(lineptr);
      }
      else {
        while((bytes_read = fread(buffer, 1, sizeof(buffer), files[i])) > 0) {
          written = fwrite(buffer, bytes_read, 1, stdout);
          if (written != 1) {
            perror("Error writing to stdout");
            ok = 1;
          }
        }
      }
    }
  }

  for(int i = 1; i < len; i++) {
    if (files[i] != NULL) {
      int c = fclose(files[i]);
      if (c != 0) {
        perror("Error closing file");
        ok = 1;
      }
    }
  }

  free(files);
  return ok; 
}

int main(int argc, char **argv){
  static struct option long_options[] =
      {
        {"number", no_argument, 0, 'n'},
        {"show-ends", no_argument, 0, 'E'},
        {"show-tabs", no_argument, 0, 'T'},
        {"show-all", no_argument, 0, 'A'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  uint8_t options = 0;

  while ((c = getopt_long(argc, argv, "nETAh", long_options, NULL)) != -1) {
    switch(c) {
      case 'n':
        SET_FLAG(options, SHOW_NUMBER);
        break;
      case 'E':
        SET_FLAG(options, SHOW_ENDS);
        break;
      case 'T':
        SET_FLAG(options, SHOW_TABS);
        break;
      case 'A':
        SET_FLAG(options, SHOW_ENDS);
        SET_FLAG(options, SHOW_TABS);
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
 
  int exit_code = cat(&argv[optind], argc - optind, options);
  exit(exit_code);
  
}
