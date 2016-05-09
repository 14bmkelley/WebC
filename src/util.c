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
#include <unistd.h>

#include "util.h"

#define SIZE_STRING_DEFAULT 10

/*
 * Provides easy error reporting.
 * Params:
 *    char *file: Name of the file error occurred in
 *    int line: Line number the error occurred on
 */
void report_errno(char *file, int line) {

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
char *read_string(int fd, char terminate) {

   int length = 0, size = SIZE_STRING_DEFAULT;
   char *result = malloc(size * sizeof(char)), current;

   /* Skip preceding whitespace */
   read(fd, &current, sizeof(char));
   while (isspace(current) != 0 && current != terminate) {
      read(fd, &current, sizeof(char));
   }

   /* Add characters until terminate is reached */
   while (current != terminate) {

      /* Current memory allocation is full, resize */
      if (length == size - 1) {
         result = realloc(result, (size *= 2) * sizeof(char));
      }

      /* Add character and read the next one */
      result[length++] = current;
      read(fd, &current, sizeof(char));

   }

   /* Reallocate to the exact amount necessary, add '\0' and return */
   result = realloc(result, ++length * sizeof(char));
   result[length - 1] = '\0';
   return result;

}
