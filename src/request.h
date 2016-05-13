/*
 * request.h
 * Makes available the use of the request struct, as well as related functions.
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

#ifndef REQUEST_H
#define REQUEST_H

typedef char *(*parse_url_path)(char **);

struct request_header {
  char *key;
  char *val;
};

struct request {
  char *type;
  char *url;
  struct request_header **headers;
  int num_headers;
  char *(*parse_url_path)(char **);
};

/*
 * Parses a complete http request from a client into a useful struct.
 * Parameters:
 *   int socket: The web socket to read the request from
 * Returns:
 *   struct request parsed: the parsed request.
 */
struct request *parse_request(int);

/*
 * Frees all memory used by a request.
 * Params:
 *    struct request *req: The request to be completely freed
 */
void free_request(struct request *);

/*
 * Prints a request to the console.
 * Params:
 *    struct request *request: The request to be printed
 */
void log_request(struct request *);

#endif
