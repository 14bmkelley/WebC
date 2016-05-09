#ifndef UTIL_H
#define UTIL_H

void report_errno(char *file, int line);
char *read_string(int fd, char terminate);

#endif
