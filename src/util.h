/*
 * util.h
 * Makes available general purpose functions.
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

#ifndef UTIL_H
#define UTIL_H

/*
 * Provides easy error reporting.
 * Params:
 *    char *file: Name of the file error occurred in
 *    int line: Line number the error occurred on
 */
void report_errno(char *, int);

/*
 * Reads and allocates a string from the given file descriptor until the
 *    terminate character is reached.
 * Params:
 *    int fd: The file descriptor to read from
 *    char terminate: The character that ends reading
 * Returns:
 *    char *result: The string read from the file descriptor
 */
char *fdgets(int, char);

/*
 * Appends the second parameter to the first, resizing as needed.
 * Params:
 *    char **dest: The resulting string to add src to
 *    char *src: The string to append to dest
 */
void append_string(char **, char *);

#endif
