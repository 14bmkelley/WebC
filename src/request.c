/*
 * request.c
 * Parses a request from a c-style string to a useful request struct. Functions
 *    are prototyped in request.h.
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "request.h"
#include "util.h"
#include "hashtable.h"

#define DEFAULT_HEADER_LEN 10

/*
 * Parses a set of request headers from a raw request c-style string.
 * Parameters:
 *   int socket: The web socket to read the request headers from.
 * Returns:
 *   request_header **result: set of request headers parsed from raw.
 */

static void parse_headers(struct request *request, int socket) {

   char *test, *key, *val;

   test = fdgets(socket, '\n');

   while (strlen(test) > 1) {

      key = substring(&test, ':');
      val = substring(&test, '\r');
      set(request->headers, key, val, (strlen(val) + 1) * sizeof(char));
      free(test);
      test = fdgets(socket, '\n');

   }

   free(test);

}

/*
 * Parses c-style string containing a url for next path and removes it from url.
 * Parameters:
 *   char **url: the url to parse for next path.
 * Returns:
 *   char *result: the path found.
 */
static char *parse_url_path_def(char **url) {
   return substring(url, '/');
}

/*
 * Parses a complete http request from a client into a useful struct.
 * Parameters:
 *   int socket: the web socket to read the request from.
 * Returns:
 *   struct request parsed: the parsed request.
 */
struct request *parse_request(int socket) {

   struct request *parsed = malloc(sizeof(struct request));
   memset(parsed, 0, sizeof(struct request));

   /* Read in type and url of the request */
   parsed->type = fdgets(socket, ' ');
   parsed->url  = fdgets(socket, ' ');

   /* Skip over the http/1.1 part */
   free(fdgets(socket, '\n'));

   /* Read in request headers */
   parsed->headers = create_hashtable();
   parse_headers(parsed, socket);

   /* Add function pointer and return request */
   parsed->parse_url_path = parse_url_path_def;
   return parsed;

}

/*
 * Frees all memory used by a request.
 * Params:
 *    struct request *req: The request to be completely freed
 */
void free_request(struct request *req) {

   /* Free everything */
   free(req->type);
   free(req->url);
   free_hashtable(req->headers);
   free(req);

}

/*
 * Prints a request to the console.
 * Params:
 *    struct request *request: The request to be printed
 */
void log_request(struct request *request) {
   printf("%s %s successful\n", request->type, request->url);
}
