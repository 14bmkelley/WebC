#ifndef PARSE_H
#define PARSE_H

struct request *parse_request(char *);
char *parse_url_path();
void free_request(struct request *);

struct request {
  char *type;
  char *url;
  struct request_header *headers;
};

struct request_header {
  char *key;
  char *val;
};

#endif
