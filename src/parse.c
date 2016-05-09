/*
 * parse.c
 * Parses a request from a c-style string to a useful request struct. Functions
 *    are prototyped in parse.h.
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

#include "parse.h"
#include "util.h"

#define DEFAULT_WORD_LEN 10

/*
 * Parses a segment of the input delimited by a character.
 * Parameters:
 *   char **input: the address of a c-style string to remove the segment from.
 *   char delimiter: the delimiter to use when removing a segment from input.
 * Returned:
 *   char *result: the segment removed from input
 */
static char *parse_segment(char **input, char delimiter) {

   size_t allocated = DEFAULT_WORD_LEN, length = 0;
   char *result = (char *) malloc(allocated * sizeof(char));
   char *current = *input, *output;

   /* Increment up to the beginning of a segment */
   while (isspace(*current) != 0 || *current == delimiter) {
      current++;
   };

   /* Loop while the segment hasn't ended and add characters to result */
   while (*current != delimiter && *current != '\0') {
      if (length == allocated - 1)
         result = (char *) realloc(result, (allocated *= 2) * sizeof(char));
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
 * Parses a set of request headers from a raw request c-style string.
 * Parameters:
 *   int socket: The web socket to read the request headers from.
 * Returns:
 *   request_header **result: set of request headers parsed from raw.
 */
static void parse_headers(struct request *req, int socket) {

   char *raw_headers = fread_string(socket, '\r'), *new_headers;
   int header_len = 0, total = 0;

   while (strlen(raw_headers) - header_len > 2) {
      header_len = strlen(raw_headers);
      new_headers = fread_string(socket, '\r');
      append_string(&raw_headers, new_headers);
      free(new_headers);
      total++;
   }

   req->headers = malloc(total * sizeof(struct request_header *));
   req->num_headers = total;

   while (total-- > 0) {
      req->headers[total] = malloc(sizeof(struct request_header));
      req->headers[total]->key = parse_segment(&raw_headers, ':');
      req->headers[total]->val = parse_segment(&raw_headers, '\n');
   }

}

/*
 * Parses c-style string containing a url for next path and removes it from url.
 * Parameters:
 *   char **url: the url to parse for next path.
 * Returns:
 *   char *result: the path found.
 */
static char *parse_url_path_def(char **url) {
  return parse_segment(url, '/');
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

   /* Read in type and url of the request */
   parsed->type = fread_string(socket, ' ');
   parsed->url  = fread_string(socket, ' ');

   /* Skip over the http/1.1 part */
   free(fread_string(socket, '\n'));

   /* Read in request headers */
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
   int index;
   free(req->type);
   free(req->url);

   for (index = 0; index < req->num_headers; index++) {
      free(req->headers[index]->key);
      free(req->headers[index]->val);
      free(req->headers[index]);
   }

   free(req->headers);
   free(req);

}
