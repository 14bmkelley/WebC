#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "parse.h"

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
 * Determines the length of some null-terminated set of memory.
 * Parameters:
 *   void *mem: memory allocated.
 *   size_t alloc_size: size of individual elements to count.
 * Returns:
 *   int result: total number of alloc_size elements in mem.
 */
 /*
static int mem_len(void *mem, size_t alloc_size) {
   int index = -1, consistent = 0;
   char *bytes = (char *) mem;
   while (consistent != alloc_size)
      consistent = (bytes[++index] == 0) ? consistent + 1 : 0;
   return index % alloc_size;
}
*/
/*
 * Parses the number of occurrences of a char in a c-style string.
 * Parameters:
 *   char *chars: c-style string to search for chosen char.
 *   char chosen: the char to search for in provided chars.
 * Returns:
 *   int result: total occurrences of chosen in chars.
 */
static int parse_count(char *chars, char chosen) {
   int total = 0;
   while (*chars != '\0') {
      if (*chars++ == chosen)
         total++;
   }
   return total;
}

/*
 * Parses a set of request headers from a raw request c-style string.
 * Parameters:
 *   char *raw_headers: request headers in c-style string form.
 * Returns:
 *   request_header **result: set of request headers parsed from raw.
 */
static void parse_headers(struct request *req, char *raw_headers) {
   int total = parse_count(raw_headers, '\n');
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
 * Parses a complete http request from a client into a useful struct
 * Parameters:
 *   char *raw: the request to be parsed.
 * Returns:
 *   struct request parsed: the parsed request.
 */
struct request *parse_request(char *raw) {
   
   struct request *parsed = malloc(sizeof(struct request));
   char *raw_headers, *raw_request = malloc(strlen(raw) * sizeof(char));
   strcpy(raw_request, raw);
   
   /* Read in type and url of the request */
   parsed->type = parse_segment(&raw_request, ' ');
   parsed->url  = parse_segment(&raw_request, ' ');

   /* Skip over the http/1.1 part */
   free(parse_segment(&raw_request, '\n'));

   /* Read in request headers */
   raw_headers = parse_segment(&raw_request, '\r');
   parse_headers(parsed, raw_headers);

   /* Add function pointer and return request */
   parsed->parse_url_path = parse_url_path_def;
   return parsed;

}

/*
 * Frees all memory used by a request.
 */
void free_request(struct request *req) {
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