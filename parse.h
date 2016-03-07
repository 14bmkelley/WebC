#ifndef PARSE_H
#define PARSE_H

struct request *parse_request(char *);
void free_request(struct request *);
typedef char *(*parse_url_path)(char **);
typedef uint8_t (*is_dir)(char *);

char *parse_url_path_def(char **);
uint8_t is_dir_def(char *);

struct request {
  char *type;
  char *url;
  struct request_header *headers;
  char *(*parse_url_path)(char **);
  uint8_t (*is_dir)(char *);
};

struct request_header {
  char *key;
  char *val;
};

#endif
