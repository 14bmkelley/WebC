#ifndef PARSE_H
#define PARSE_H

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

struct request *parse_request(char *);
void free_request(struct request *);

#endif
