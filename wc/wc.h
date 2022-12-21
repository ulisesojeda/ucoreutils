#ifndef WC_HEADER
#define WC_HEADER

enum mode {CHARS, LINES};

char* wc(char* f_name, enum mode m);
void usage();

#endif
