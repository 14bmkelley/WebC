#ifndef RESPONSE_H
#define RESPONSE_H

#include "request.h"

struct response {
   struct request *initial_request;
   int status_code;
   struct hashtable *headers;
};

struct response *create_response();
void free_response(struct response *);
void log_response(struct response *);

#endif
