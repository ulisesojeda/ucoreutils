#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <errno.h>
#include <string.h>
#include <signal.h>


typedef struct {
 char** buf;
 int len;
} BUF;

char *line = NULL;
FILE *f = NULL;

void signal_callback_handler(int signum) {
   if ( line != NULL) free(line);
   if ( f != NULL) fclose(f);
   exit(signum);
}

void usage() {
  fputs("Usage: utail [-n/--lines] [-h/--help] [FILE]\n\n", stdout);
}

int readlines(FILE *f, long lines, bool follow) {
  size_t len = 0;
  ssize_t nread;
  int exit_code = 0;

  while (lines > 0) {
    nread = getline(&line, &len, f);    
    if (nread == -1) {
      exit_code = 1;
      break;
    }
    printf("%s", line);
    lines--;
  }

  return exit_code;
}

FILE* rewind_lines(char* fname, int lines) {
  int n_eol = 0, c;
  f = fopen(fname, "r");

  fseek(f, -1, SEEK_END);

  while(n_eol < lines + 1) {
    c = getc(f);
    if ( c == '\n' ) n_eol++;
    fseek(f, -2, SEEK_CUR);
    if (ftell(f) == 0) break;
  }

  if (ftell(f) != 0) fseek(f, 2, SEEK_CUR);
  return f;
}


BUF read_stdin() {
  char** buffer = (char**)malloc(sizeof(char*));
  int c_lines = 0;

  size_t len = 0;
  ssize_t nread;

  while (1==1) {
    nread = getline(&buffer[c_lines], &len, stdin);    
    if (nread != -1) {
      buffer = (char**)realloc(buffer, sizeof(char*) * (++c_lines + 1));
    } else {
      break;
    }
  }

  BUF res = {.buf = buffer, .len = c_lines};
  return res;
}

int print_buffer(BUF buf, int lines) {
  int init = lines < buf.len ? buf.len - lines : 0;
  for (int i=init; i<buf.len; i++)
    printf("%s", buf.buf[i]);

  return 0;
}

void follow_stream(FILE* f) {
  size_t len = 0;
  ssize_t nread;
  long cur;
  if ( f != stdin ) cur = ftell(f);

  while (1==1) {
    nread = getline(&line, &len, f);    
    if (nread != -1) { 
      printf("%s", line);
      if ( f != stdin ) cur = ftell(f);
    }
    else {
     if ( f!= stdin ) fseek(f, cur, SEEK_SET);
    }
  }
}

int main(int argc, char **argv){
  signal(SIGINT, signal_callback_handler);

  static struct option long_options[] =
      {
        {"lines", required_argument, 0, 'n'},
        {"follow", no_argument, 0, 'f'},
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
      };
  int c;
  long lines = 10;
  char *endptr;
  bool follow = false;
  int exit_code = 0;
  line = (char*)malloc(sizeof(char*) * 100);

  while ((c = getopt_long(argc, argv, "n:hf", long_options, NULL)) != -1) {
    switch(c) {
      case 'n':
        lines = strtol(optarg, &endptr, 10);
        break;
      case 'h':
        usage();
        exit(0);
        break;
      case 'f':
        follow = true;
        break;
      default:
        usage();
        exit(1);
    }
  }
 
  if (argv[optind] != NULL) {
    f = rewind_lines(argv[optind], lines);
    exit_code = readlines(f, lines, follow);
  }
  else {
    BUF buf = read_stdin();
    f = stdin;
    exit_code = print_buffer(buf, lines);

    for(int i=0; i<buf.len; i++) free(buf.buf[i]);
    free(buf.buf);
  }

  if (follow) {
    follow_stream(f);
  }

  exit(exit_code);
}
