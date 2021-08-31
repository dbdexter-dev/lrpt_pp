#ifndef utils_h
#define utils_h

#ifndef MIN
#define MIN(x, y) (((x)<(y)) ? (x) : (y))
#endif

#ifndef MAX
#define MAX(x, y) (((x)>(y)) ? (x) : (y))
#endif

void usage(const char *pname);
void version();

#endif
