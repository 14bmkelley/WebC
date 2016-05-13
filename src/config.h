#ifndef SERVER_H
#define SERVER_H

#include <netinet/in.h>

#include "request.h"

struct svr_info {
   int socket;
   struct sockaddr_in addr;
};

/*
 * Configure the server with default settings.
 * Params:
 *    struct svr_info *svr: The server to be configured
 */
void config_server(struct svr_info *);

#endif
