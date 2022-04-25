/**
 * @file httpServer.h
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief http server library
 * @version 0.0.1
 * @date 2022-03-31
 *
 * @copyright Copyright (c) 2022, Jinseok Kim
 *
 */

#ifndef __HTTPSERVER_H__
#define __HTTPSERVER_H__

#include "http/httpMessage.h"

/**
 * @brief  define handler type
 *
 */
typedef void (*Handler)(Request *request, Response *response);

/**
 * @brief define URI tree
 *
 */
typedef struct _PathNode
{
    const char *dir;
    Handler handlers[NUMSUPPORTEDMETHOD];
    PathNode *child;
    PathNode *next;
} PathNode;

/**
 * @brief define server. server hold port, server socket fd and Path
 *
 */
typedef struct _server
{
    int port;
    int fd;
    PathNode *node;
} Server;

/**
 * @todo add defalult response
 *
 */

/**
 * @brief set server address, bind socket, set select
 *
 * @param portNumber
 * @return Server*
 */
Server *newServer(int portNumber);

/**
 * @brief  run server loop
 *
 * @param server
 * @return int
 */
int startServer(const Server *server); // add errono

/**
 * @brief book path and handler
 *
 * @param server
 * @param path  pass full path
 * @param method pass http method
 * @param handler pass handler user defined
 * @return int
 */
int addRoute(Server *server, const char *path, Method method, Handler handler); // add errono

/**
 * @brief parse path
 *
 * @param path
 * @param dir
 * @return int
 */
int parsePath(const char *path, char *dir); // add errono

/**
 * @brief call handler by request
 *
 * @param request
 * @param response
 * @return int
 */
int route(const Request *request, Response *response); // route pass server information to response

// default error handler for parse error, bad request, request a non-existen object

/**
 * @brief if client request not existed path then call this hanlder
 *
 * @param request
 * @param response
 * @return int
 */
int NotFound(Request *request, Response *response);

/**
 * @brief  handle several cases. path have whitespace
 *
 * @param request
 * @param response
 * @return int
 */
int BadRequest(Request *request, Response *response);

#endif