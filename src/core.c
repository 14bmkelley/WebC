/*
 * core.c
 * The core functionality of the web server is written below. To use, simply
 *    include the prototype below and call the function with this file in the
 *    same working directory.
 *
 *    int run_server();
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

#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
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

/*
 * Show information about the WebC license
 */
static void output_license() {

   printf("\nWebC - a basic web server written in c.\n");
   printf("Copyright (C) 2016 Brandon M. Kelley\n\n");

}

/*
 * Configure the server socket to listen for IP requests.
 * Params:
 *    int *svr_socket: The socket that identifies the internet connection
 */
static void set_socket_options(int *svr_socket) {

   /* Basically just the boolean value of true */
   int set_option = 1;

   /* Try to create an IP socket */
   *svr_socket = socket(AF_INET, SOCK_STREAM, PROTOCOL);

   if (*svr_socket < 1) {
      report_errno(__FILE__, __LINE__);
   }

   /* Try to configure IP socket to reuse addresses */
   set_option = setsockopt(*svr_socket, SOL_SOCKET, SO_REUSEADDR, &set_option,
      sizeof(int));

   if (set_option < 0) {
      report_errno(__FILE__, __LINE__);
   }

}

/*
 * Populate address struct with internet socket settings.
 * Params:
 *    struct sockaddr_in *addr: the struct containing settings, to be filled
 */
static void set_addr_options(struct sockaddr_in *addr) {

   /* Listens for IP from any address on PORT */
   addr->sin_family = AF_INET;
   addr->sin_addr.s_addr = INADDR_ANY;
   addr->sin_port = htons(PORT);

}

/*
 * Binds the address settings established in set_addr_options to the server
 *    socket established in set_socket_options.
 * Params:
 *    struct svr_info *svr: The struct holding the socket and address settings
 */
static void bind_server(struct svr_info *svr) {

   /* Try to bind the address settings to the server socket */
   socklen_t addr_len = sizeof(svr->addr);
   int result = bind(svr->socket, (struct sockaddr *) &svr->addr, addr_len);

   if (result < 0) {
      report_errno(__FILE__, __LINE__);
   }

   /* Print a message to the console */
   printf("Server root bound to localhost:%d/\n", PORT);

}

/*
 * Logs information from each request.
 * Params:
 *    char *request: The raw request to be recorded
 */
static void log_request(char *request) {
   printf("%s\n\n", request);
}

/*
 * Read in a request from the IP socket and parse it.
 * Params:
 *    int *request_socket: The socket opened by an incoming request
 *    struct svr_info svr: The IP socket and address settings
 * Returns:
 *    struct request *parsed_request: The parsed request
 */
static struct request *receive_request(int *request_socket,
   struct svr_info svr) {

   socklen_t addr_len = sizeof(svr.addr);
   char *raw_request;
   struct request *parsed_request;

   /* Try to listen for requests */
   if (listen(svr.socket, SIZE_BACKLOG) < 0) {
      report_errno(__FILE__, __LINE__);
   }

   /* Try to accept a new incoming request */
   *request_socket = accept(svr.socket, (struct sockaddr *) &svr.addr,
      &addr_len);

   if (*request_socket < 0) {
      report_errno(__FILE__, __LINE__);
   }

   /* Read in the request */
   raw_request = read_string(*request_socket, '\r');
   log_request(raw_request);

   /* Parse the request and return */
   parsed_request = parse_request(raw_request);
   free(raw_request);

   return parsed_request;

}

/*
 * Respond to a received request.
 * Params:
 *    int socket: The current request socket
 *    struct request *incoming_request: the current request data
 */
static void handle_request(int socket, struct request *incoming_request) {

   /* Just write hello world for now and close socket */
   write(socket, "HTTP/1.1 200 OK\n", 16);
   write(socket, "Content-length: 46\n", 19);
   write(socket, "Content-Type: text/html\n\n", 25);
   write(socket, "<html><body><h1>Hello world!</h1></body></html>\n", 48);
   close(socket);

}

/*
 * Run the web server.
 * Returns:
 *    int result: Exit code of the server
 */
int run_server() {

   struct svr_info svr;
   int request_socket;
   struct request *incoming_request;

   /* Show license information */
   output_license();

   /* Set up server for listening */
   set_socket_options(&svr.socket);
   set_addr_options(&svr.addr);
   bind_server(&svr);
   printf("Server is now listening\n\n");

   /* Loop forever, processing requests as they occur */
   while (1) {

      /* Parse incoming request */
      incoming_request = receive_request(&request_socket, svr);

      /* Decide how to respond to request and then free it*/
      handle_request(request_socket, incoming_request);
      free_request(incoming_request);

   }

   /* We will sadly never reach here :( */
   close(svr.socket);
   return EXIT_SUCCESS;

}
