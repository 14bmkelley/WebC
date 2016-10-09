/*
 * config.c
 * Sets the default configuration of a server.
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

#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>

#include "config.h"
#include "util.h"

#define PROTOCOL 0
#define PORT 8000

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
      report_errno();
   }

   /* Try to configure IP socket to reuse addresses */
   set_option = setsockopt(*svr_socket, SOL_SOCKET, SO_REUSEADDR, &set_option,
      sizeof(int));

   if (set_option < 0) {
      report_errno();
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
      report_errno();
   }

   /* Print a message to the console */
   printf("Server root bound to localhost:%d/\n", PORT);

}

/*
 * Configure the server with default settings.
 * Params:
 *    struct svr_info *svr: The server to be configured
 */
void config_server(struct svr_info *svr) {

   /* Configure and bind server */
   set_socket_options(&svr->socket);
   set_addr_options(&svr->addr);
   bind_server(svr);

}
