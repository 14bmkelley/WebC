#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "util.h"

#define SIZE_STRING_DEFAULT 10

void report_errno(char *file, int line) {
   fprintf(stderr, "%s, line %d: ", file, line);
   perror(NULL);
   exit(EXIT_FAILURE);
}

char *read_string(int fd, char terminate) {

   int length = 0, size = SIZE_STRING_DEFAULT;
   char *result = malloc(size * sizeof(char)), current;

   read(fd, &current, sizeof(char));
   while (isspace(current) != 0 && current != terminate) {
      read(fd, &current, sizeof(char));
   }

   while (current != terminate) {

      if (length == size - 1) {
         result = realloc(result, (size *= 2) * sizeof(char));
      }

      result[length++] = current;
      read(fd, &current, sizeof(char));

   }

   result = realloc(result, ++length * sizeof(char));
   result[length - 1] = '\0';
   return result;

}
