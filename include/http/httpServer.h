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

#include <sys/select.h>

#include "http/httpMessage.h"

#define MAXCLIENT 1024

// 16kB
#define BUFLENGTH 65536

extern char *defaultNotFound;
extern char *defaultBadRequest;

/**
 * @brief  define handler type
 *
 */
typedef int (*Handler)(const Request *request, Response *response);

/**
 * @brief split path without modify
 *
 */
typedef struct PathSegNode
{
    const char *start;
    int length;

    struct PathSegNode *next;
} PathSegNode;

/**
 * @brief allocate new PathSegNode
 *
 * @return PathSegNode*
 */
PathSegNode *newPathSegNode();

/**
 * @brief split path
 *
 * @param path
 * @return PathSegNode*
 */
PathSegNode *parsePathSeg(const char *path);

/**
 * @brief free nodes
 *
 * @param node
 */
void freePathSegList(PathSegNode *node);

/**
 * @brief define URI tree
 *
 */
typedef struct PathNode
{
    char *dir;
    Handler handlers[NUMSUPPORTEDMETHOD];
    struct PathNode *child;
    struct PathNode *next;
} PathNode;

/**
 * @brief allocate PathNode
 *
 * @return PathNode*
 */
PathNode *newPathNode();

/**
 * @brief add pathNode
 *
 * @param root
 * @param path
 * @return int
 */
int addPathNode(PathNode **root, const char *path);

/**
 * @brief Get the Path Node object
 *
 * @param root
 * @param path
 * @return PathNode*
 */
PathNode *getPathNode(PathNode *root, const char *path);

/**
 * @brief free pathNodes
 *
 * @param root
 */
void freePathNodeList(PathNode *root);

/**
 * @brief define server. server hold port, server socket fd and Path
 *
 */
typedef struct
{
    int port;
    int fd;
    PathNode *node;
    fd_set readSet;
} Server;

/**
 * @brief set server address, bind socket, set select
 *
 * @param portNumber
 * @return Server*
 */
Server *newServer(int portNumber);

/**
 * @brief
 *
 * @param server
 */
void freeServer(Server *server);

/**
 * @brief  run server loop
 *
 * @param server
 * @return int
 */
int startServer(Server *server); // add errono

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
 * @brief call handler by request
 *
 * @param request
 * @param response
 * @return int
 */
int route(const Server *server, const Request *request, Response *response); // route pass server information to response

// default error handler for parse error, bad request, request a non-existen object

/**
 * @brief if client request not existed path then call this hanlder
 *
 * @param request
 * @param response
 * @return int
 */
int NotFound_func(const Request *request, Response *response);

/**
 * @brief  handle several cases. path have whitespace
 *
 * @param request
 * @param response
 * @return int
 */
int BadRequest_func(const Request *request, Response *response);

#endif
