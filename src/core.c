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

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "config.h"
#include "request.h"
#include "util.h"

#define SIZE_BACKLOG 10

/*
 * Show information about the WebC license
 */
static void output_license() {

   printf("\nWebC - a basic web server written in c.\n");
   printf("Copyright (C) 2016 Brandon M. Kelley\n\n");

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

   /* Parse the request, log and return */
   parsed_request = parse_request(*request_socket);
   log_request(parsed_request);

   return parsed_request;

}

/*
 * Respond to a received request.
 * Params:
 *    int socket: The current request socket
 *    struct request *request: the current request data
 */
static void handle_request(int socket, struct request *req) {

   char *dir = malloc(7 * sizeof(char)), *static_html, *dynamic_content_len;
   int static_fd, static_len, static_len_len;

   strcpy(dir, "static");
   append_string(&dir, req->url);
   static_fd = open(dir, O_RDONLY);

   if (static_fd > 0) {
      static_html = fdgets(static_fd, EOF);
      static_len = strlen(static_html);
      static_len_len = ceil(log(static_len) / log(10));
      dynamic_content_len = malloc((18 + static_len_len) * sizeof(char));
      sprintf(dynamic_content_len, "Content-length: %d\n", static_len);
      write(socket, "HTTP/1.1 200 OK\n", 16);
      write(socket, dynamic_content_len, 17 + static_len_len);
      write(socket, "Content-Type: text/html\n\n", 25);
      write(socket, static_html, static_len);
      free(static_html);
      free(dynamic_content_len);
      close(static_fd);
   }

   else {
      write(socket, "HTTP/1.1 200 OK\n", 16);
      write(socket, "Content-length: 48\n", 19);
      write(socket, "Content-Type: text/html\n\n", 25);
      write(socket, "<html><body><h1>Page not found</h1></body></html>\n", 48);
   }

   free(dir);

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
   config_server(&svr);
   printf("Server is now listening\n\n");

   /* Loop forever, processing requests as they occur */
   while (1) {

      /* Parse incoming request */
      incoming_request = receive_request(&request_socket, svr);

      /* Decide how to respond to request and then free it*/
      handle_request(request_socket, incoming_request);
      close(request_socket);
      free_request(incoming_request);

   }

   /* We will sadly never reach here :( */
   close(svr.socket);
   return EXIT_SUCCESS;

}
