#include <stdio.h>
#include <stdlib.h>

#include "response.h"
#include "hashtable.h"

struct response *create_response() {
   struct response *new_response = malloc(sizeof(struct response));
   new_response->headers = create_hashtable();
   return new_response;
}

void free_response(struct response *server_response) {
   free(server_response->headers);
   free(server_response);
}

void log_response(struct response *server_response) {
   printf("%s %s %d\n", server_response->initial_request->type,
         server_response->initial_request->url, server_response->status_code);
}
