#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "parse.h"

struct request *parse_request(char *req) {
  
  char type[100];
  char url[100];
  struct request_header *headers[100];
  size_t type_len = 0;
  size_t url_len = 0;
  size_t headers_len = 0;
  struct request *parsed_req =
    (struct request *) malloc(sizeof(struct request));
  char *index = req;
  
  // Pick up the type of request
  while (*index != ' ') {
    type[type_len++] = *index++;
  }
  type[type_len++] = '\0';
  
  // Pick up the url of request
  while (*++index != ' ') {
    url[url_len++] = *index;
  }
  url[url_len++] = '\0';
  
  // Skip to next line
  while (*++index != '\n') {}
  
  // Process request headers
  uint8_t reqs = 1;
  while (reqs) {
    
    // Set up a request_header
    struct request_header *header =
      (struct request_header *) malloc(sizeof(struct request_header));
    char key[200];
    char val[200];
    size_t key_len = 0;
    size_t val_len = 0;
    
    // Get the key
    while (*++index != ':') {
      key[key_len++] = *index;
    }
    key[key_len++] = '\0';
    
    // Get past that pesky extra space
    index++;
    
    // Get the value
    while (*++index != '\n') {
      val[val_len++] = *index;
    }
    val[val_len++] = '\0';
    
    // Populate header
    header->key = (char *) malloc(key_len * sizeof(char));
    header->val = (char *) malloc(val_len * sizeof(char));
    memcpy(header->key, key, key_len);
    memcpy(header->val, val, val_len);
    headers[headers_len++] = header;
    
    // Check end of request
    if (*(index + 1) == '\r') {
      reqs = 0;
    }
    
  }
  
  // Populate request
  parsed_req->type = (char *) malloc(type_len * sizeof(char));
  parsed_req->url = (char *) malloc(url_len * sizeof(char));
  parsed_req->headers =
    (struct request_header *) malloc(headers_len * sizeof (struct request_header));
  memcpy(parsed_req->type, type, type_len);
  memcpy(parsed_req->url, url, url_len);
  struct request_header *current = parsed_req->headers;
  for (int i = 0; i < headers_len; i++) {
    *current = *headers[i];
    current++;
  }
  
  return parsed_req;
  
}

void free_request(struct request *req) {
  free(req->type);
  free(req->url);
  size_t headers_len = sizeof(req->headers);
  struct request_header *current = req->headers;
  for (int i = 0; i < headers_len; i++, current++) {
    free(current->key);
    free(current->val);
  }
  free(req);
}

