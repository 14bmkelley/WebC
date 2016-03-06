#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parse.c"

int main() {
  
  int svr_socket, req_socket;
  int buffer_size = 1024;
  char *buffer = malloc(buffer_size);
  struct sockaddr_in svr_addr;
  socklen_t svr_addr_len;
  int result;
  
  svr_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (svr_socket > 0) {
    printf("The socket was created.\n");
  }
  
  result = setsockopt(svr_socket, SOL_SOCKET, SO_REUSEADDR, &(int) {1}, sizeof(int));
  if (result < 0) {
    perror("Server: set socket options");
  }
  
  svr_addr.sin_family = AF_INET;
  svr_addr.sin_addr.s_addr = INADDR_ANY;
  svr_addr.sin_port = htons(8000);
  svr_addr_len = sizeof(svr_addr);
  
  result = bind(svr_socket, (struct sockaddr *) &svr_addr, svr_addr_len);
  if (result == 0) {
    printf("Binding socket.\n");
  }
  
  while (1) {
    
    result = listen(svr_socket, 10);
    if (result < 0) {
      perror("Server: listen");
      exit(1);
    }
    
    req_socket = accept(svr_socket, (struct sockaddr *) &svr_addr, &svr_addr_len);
    if (req_socket < 0) {
      perror("Server: accept");
      exit(1);
    }
    
    if (req_socket > 0) {
      printf("A client connected...\n");
    }
    
    result = recv(req_socket, buffer, buffer_size, 0);
    
    struct request *parsed_req = parse_request(buffer);
    printf("%s %s %s\n", parsed_req->type, parsed_req->url, parsed_req->headers->key);
    
    free_request(parsed_req);
    
    send(req_socket, "HTTP/1.1 200 OK\n", 16, 0);
    send(req_socket, "Content-length: 46\n", 19, 0);
    send(req_socket, "Content-Type: text/html\n\n", 25, 0);
    send(req_socket, "<html><body><h1>Hello world</h1></body></html>\n", 47, 0);
    close(req_socket);
    
  }
  
  close(svr_socket);
  return 0;
  
}

