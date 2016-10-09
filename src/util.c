/*
 * util.c
 * Utilities for the web server. Functions are prototyped in util.h.
 *
 * Author:   Brandon M. Kelley
 * Date:     May 8, 2016
 * Version:  1.0
 * License:  GNU GPL
 * Copyright (C) 2016 Brandon M. Kelley
 *
 * This file is a part of WebC
 *
 * WebC is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * WebC is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with WebC.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define DEFAULT_WORD_LEN 10

void *safe_malloc(size_t size) {
   void *ptr = malloc(size);
   if (ptr == NULL) {
      perror("safe malloc failed.");
      exit(EXIT_FAILURE);
   }
   return ptr;
}

void *safe_realloc(void *ptr, size_t size) {
   ptr = realloc(ptr, size);
   if (ptr == NULL) {
      perror("safe realloc failed.");
      exit(EXIT_FAILURE);
   }
   return ptr;
}

pid_t safe_fork() {
   pid_t pid = fork();
   if (pid == -1) {
      perror("safe fork failed.");
      exit(EXIT_FAILURE);
   }
   return pid;
}

/*
 * Provides easy error reporting.
 * Params:
 *    char *file: Name of the file error occurred in
 *    int line: Line number the error occurred on
 */
void report_errno_with_data(char *file, int line) {

   /* So snazzy */
   fprintf(stderr, "%s, line %d: ", file, line);
   perror(NULL);
   exit(EXIT_FAILURE);

}

/*
 * Reads and allocates a string from the given file descriptor until the
 *    terminate character is reached.
 * Params:
 *    int fd: The file descriptor to read from
 *    char terminate: The character that ends reading
 * Returns:
 *    char *result: The string read from the file descriptor
 */
char *fdgets(int fd, char terminate) {

   int length = 0, size = DEFAULT_WORD_LEN, read_result;
   char *result = malloc(size * sizeof(char)), current;
   read_result = read(fd, &current, sizeof(char));

   /* Add characters until terminate is reached */
   while (current != terminate && read_result > 0) {

      /* Current memory allocation is full, resize */
      if (length == size - 1) {
         result = realloc(result, (size *= 2) * sizeof(char));
      }

      /* Add character and read the next one */
      result[length++] = current;
      read_result = read(fd, &current, sizeof(char));

   }

   /* Reallocate to the exact amount necessary, add '\0' and return */
   result = realloc(result, ++length * sizeof(char));
   result[length - 1] = '\0';
   return result;

}

/*
 * Parses a segment of the input delimited by a character.
 * Parameters:
 *   char **input: the address of a c-style string to remove the segment from.
 *   char delimiter: the delimiter to use when removing a segment from input.
 * Returned:
 *   char *result: the segment removed from input
 */
char *substring(char **input, char delimiter) {

   size_t allocated = DEFAULT_WORD_LEN, length = 0;
   char *result = (char *) malloc(allocated * sizeof(char));
   char *current = *input, *output;

   /* Increment up to the beginning of a segment */
   while (isspace(*current) != 0 || *current == delimiter) {
      current++;
   };

   /* Loop while the segment hasn't ended and add characters to result */
   while (*current != delimiter && *current != '\0') {
      if (length == allocated - 1) {
         result = (char *) realloc(result, (allocated *= 2) * sizeof(char));
      }
      result[length++] = *current++;
   }

   /* Allocate exactly enough room for a valid c-style string */
   result = (char *) realloc(result, (length + 1) * sizeof(char));
   result[length] = '\0';

   /* Remove the segment from the input */
   output = (char *) malloc((strlen(*input) - length) * sizeof(char));
   strcpy(output, &current[1]);
   free(*input);
   *input = output;

   return result;

}

/*
 * Appends the second parameter to the first, resizing as needed.
 * Params:
 *    char **dest: The resulting string to add src to
 *    char *src: The string to append to dest
 */
void append_string(char **dest, char *src) {
   int dest_len = strlen(*dest), src_len = strlen(src);
   *dest = realloc(*dest, (dest_len + src_len + 1) * sizeof(char));
   strcpy(*dest + dest_len, src);
}
