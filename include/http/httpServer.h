#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "http/httpMessage.h"

typedef void (*Handler)(Request *request, Response *response);

typedef struct _PathNode
{
    const char *dir;
    Handler handlers[NUMSUPPORTEDMETHOD];
    PathNode *child;
    PathNode *next;
} PathNode;

typedef struct _server
{
    int port;
    int fd;
    PathNode *node;
} Server;

// add defalult response

Server *newServer(int portNumber);
int startServer(const Server *server);                                          // add errono
int addRoute(Server *server, const char *path, Method method, Handler handler); // add errono
int pathParse(const char *path, char *dir);                                     // add errono
int route(const Request *request, Response *response);                          // route pass server information to response

// error handler
int NotFound(Request *request, Response *response);
int BadRequest(Request *request, Response *response);

#endif