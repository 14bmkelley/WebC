#include <netinet/in.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "parse.h"
#include "util.h"

#define PROTOCOL 0
#define PORT 8000
#define SIZE_BACKLOG 10

struct svr_info {
   int socket;
   struct sockaddr_in addr;
};

static void set_socket_options(int *svr_socket) {

   int set_option = 1;

   *svr_socket = socket(AF_INET, SOCK_STREAM, PROTOCOL);
   if (*svr_socket < 1) {
      report_errno(__FILE__, __LINE__);
   }

   set_option = setsockopt(*svr_socket, SOL_SOCKET, SO_REUSEADDR, &set_option,
      sizeof(int));

   if (set_option < 0) {
      report_errno(__FILE__, __LINE__);
   }

}

static void set_addr_options(struct sockaddr_in *addr) {
   addr->sin_family = AF_INET;
   addr->sin_addr.s_addr = INADDR_ANY;
   addr->sin_port = htons(PORT);
}

static void bind_server(struct svr_info *svr) {

   socklen_t addr_len = sizeof(svr->addr);
   int result = bind(svr->socket, (struct sockaddr *) &svr->addr, addr_len);

   if (result < 0) {
      report_errno(__FILE__, __LINE__);
   }

}

static struct request *recieve_request(int *request_socket,
   struct svr_info svr) {

   socklen_t addr_len = sizeof(svr.addr);
   char *raw_request;
   struct request *request;

   if (listen(svr.socket, SIZE_BACKLOG) < 0) {
      report_errno(__FILE__, __LINE__);
   }

   *request_socket = accept(svr.socket, (struct sockaddr *) &svr.addr,
      &addr_len);

   if (*request_socket < 0) {
      report_errno(__FILE__, __LINE__);
   }

   raw_request = read_string(*request_socket, '\r');

   request = parse_request(raw_request);
   free(raw_request);

   return request;

}

static void handle_request(int socket, struct request *incoming_request) {
   write(socket, "HTTP/1.1 200 OK\n", 16);
   write(socket, "Content-length: 46\n", 19);
   write(socket, "Content-Type: text/html\n\n", 25);
   write(socket, "<html><body><h1>Hello world</h1></body></html>\n", 47);
   close(socket);
}

int run_server() {

   struct svr_info svr;
   int request_socket;
   struct request *incoming_request;

   set_socket_options(&svr.socket);
   set_addr_options(&svr.addr);
   bind_server(&svr);

   while (1) {
      incoming_request = recieve_request(&request_socket, svr);
      handle_request(request_socket, incoming_request);
      free_request(incoming_request);
   }

   close(svr.socket);
   return EXIT_SUCCESS;

}
