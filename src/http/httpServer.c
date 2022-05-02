/**
 * @file httpServer.c
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief http server
 * @version 0.1
 * @date 2022-04-25
 *
 * @copyright Copyright (c) 2022, Jinseok Kim
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netinet/in.h>
#include <netinet/ip.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <http/httpMessage.h>
#include <http/httpServer.h>

char *defaultNotFound = "<html><body><h1>Not Found 404</h1></body></html>";
char *defaultBadRequest = "<html><body><h1>Bad Request 400</h1></body></html>";

PathSegNode *newPathSegNode()
{
    PathSegNode *tmp = malloc(sizeof(PathSegNode));
    tmp->length = 0;
    tmp->next = NULL;
    tmp->start = NULL;
    return tmp;
}

PathSegNode *parsePathSeg(const char *path)
{
    const char *sep = "/";
    if (strpbrk(path, sep) != path)
        return NULL;

    PathSegNode *list = newPathSegNode();

    // root
    list->length = 1;
    list->start = path;
    if (strlen(path) == 1)
    {
        return list;
    }
    list->next = newPathSegNode();

    PathSegNode *cur = list->next;

    path++;
    const char *pre = NULL;

    do
    {
        pre = path;
        path = strpbrk(path, sep);
        cur->start = pre;

        if (path)
        {
            cur->length = path - pre;
            path += strspn(path, sep);
            // return null if end with '/'
            if (strlen(path) == 0)
            {
                freePathSegList(list);
                return NULL;
            }
            cur->next = newPathSegNode();
            cur = cur->next;
        }
        else
        {
            cur->length = strlen(pre);
        }

    } while (path && *path);

    return list;
}

void freePathSegList(PathSegNode *node)
{
    PathSegNode *tmp = node;
    while (node != NULL)
    {
        tmp = node;
        node = node->next;
        free(tmp);
    }
}

PathNode *newPathNode()
{
    PathNode *tmp = (PathNode *)malloc(sizeof(PathNode));
    tmp->dir = NULL;
    tmp->child = NULL;
    tmp->next = NULL;
    for (int i = 0; i < NUMSUPPORTEDMETHOD; i++)
    {
        tmp->handlers[i] = NotFound_func;
    }
    return tmp;
}

int addPathNode(PathNode **root, const char *path)
{
    if (*root == NULL)
    {
        *root = newPathNode();
        (*root)->dir = strdup("/");
    }

    if (getPathNode(*root, path) != NULL)
        return 1;

    PathSegNode *segs = parsePathSeg(path);
    PathSegNode *segCur = segs;
    PathNode *pathCur = *root;

    // enter "/" (root)
    if (strncmp(pathCur->dir, segCur->start, segCur->length) == 0)
    {
        segCur = segCur->next;
        if (pathCur->child != NULL)
            pathCur = pathCur->child;
    }

    // run until parents
    while (segCur->next != NULL && segCur->next->next != NULL)
    {
        if (strncmp(pathCur->dir, segCur->start, segCur->length) == 0)
        {
            segCur = segCur->next;
            if (pathCur->child != NULL)
                pathCur = pathCur->child;
        }
        else
        {
            pathCur = pathCur->next;
        }
    }

    if (pathCur->child != NULL)
    {
        while (pathCur->next != NULL)
        {
            if (strncmp(pathCur->dir, segCur->start, segCur->length) != 0)
            {
                pathCur = pathCur->next;
            }
        }

        pathCur->next = newPathNode();
        pathCur = pathCur->next;
        pathCur->dir = strndup(segCur->start, segCur->length);
        segCur = segCur->next;
    }

    while (segCur != NULL)
    {
        pathCur->child = newPathNode();
        pathCur = pathCur->child;
        pathCur->dir = strndup(segCur->start, segCur->length);

        segCur = segCur->next;
    }

    freePathSegList(segs);

    return 0;
}

PathNode *getPathNode(PathNode *root, const char *path)
{
    // not start with / return NULL
    PathSegNode *segs = parsePathSeg(path);
    if (segs == NULL)
        return NULL;

    PathNode *pathCur = root;
    PathSegNode *segCur = segs;

    while (pathCur != NULL && segCur != NULL)
    {
        if (strncmp(pathCur->dir,
                    segCur->start,
                    strlen(pathCur->dir)) == 0)
        {
            if (segCur->next == NULL)
            {
                if (strncmp(pathCur->dir,
                            segCur->start,
                            strlen(pathCur->dir)) == 0)
                {
                    freePathSegList(segs);
                    return pathCur;
                }
                else if (strncmp(pathCur->child->dir,
                                 segCur->start,
                                 strlen(pathCur->dir)) == 0)
                {
                    freePathSegList(segs);
                    return pathCur->child;
                }
                pathCur = pathCur->next;
                while (pathCur != NULL)
                {
                    if (strncmp(pathCur->dir,
                                segCur->start,
                                strlen(pathCur->dir)) == 0)
                    {
                        freePathSegList(segs);
                        return pathCur;
                    }
                    pathCur = pathCur->next;
                }
            }

            pathCur = pathCur->child;
            segCur = segCur->next;
        }
        else
        {
            pathCur = pathCur->next;
        }
    }
    freePathSegList(segs);
    return NULL;
}

void freePathNodeList(PathNode *root)
{
    if (root == NULL)
        return;
    if (root->child != NULL)
        freePathNodeList(root->child);
    if (root->next != NULL)
        freePathNodeList(root->next);

    free(root->dir);
    free(root);
}

Server *newServer(int portNumber)
{
    Server *temp = (Server *)malloc(sizeof(Server));
    temp->fd = 0;
    temp->node = NULL;
    temp->port = portNumber;
    FD_ZERO(&(temp->readSet));
    return temp;
}

void freeServer(Server *server)
{
    if (server == NULL)
        return;
    if (server->node != NULL)
        freePathNodeList(server->node);

    close(server->fd);
    server->node = NULL;
    free(server);
}

int startServer(Server *server)
{
    struct sockaddr_in serverAddr, clientAddr;

    int clientFd = 0;
    memset(&clientAddr, 0, sizeof(clientAddr));
    socklen_t addrLen = 0;

    server->fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(server->port);
    serverAddr.sin_addr.s_addr = htonl(INADDR_ANY);

    fd_set tempFdSet;
    FD_ZERO(&tempFdSet);
    FD_SET(server->fd, &(server->readSet));

    if (bind(server->fd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == -1)
    {
        printf("error : bind error\n");
        return 1;
    }

    if (listen(server->fd, SOMAXCONN) == -1)
    {
        printf("error : listen error\n");
        return 1;
    }
    printf("server started\n");

    int maxFd = server->fd;
    int fdNum;
    char message[BUFLENGTH] = {
        0,
    };
    int readN = 0;

    Request *req = NULL;
    Response *res = NULL;

    while (true)
    {
        readN = 0;
        tempFdSet = server->readSet;
        fdNum = select(maxFd + 1, &tempFdSet, NULL, NULL, NULL);
        if (fdNum == -1)
        {
            printf("error : select error\n");
            return 1;
        }
        if (FD_ISSET(server->fd, &tempFdSet))
        {
            addrLen = sizeof(clientAddr);
            clientFd = accept(server->fd,
                              (struct sockaddr *)&clientAddr, &addrLen);

            FD_SET(clientFd, &(server->readSet));

            if (clientFd > maxFd)
                maxFd = clientFd;
            printf("client in\n");
            continue;
        }

        memset(message, '\0', BUFLENGTH);

        for (int i = 0; i <= maxFd; i++)
        {
            if (FD_ISSET(i, &tempFdSet))
            {
                readN = read(i, message, BUFLENGTH - 1);
                if (readN == 0)
                {
                    close(i);
                    FD_CLR(i, &(server->readSet));
                    continue;
                }
                req = newRequest();
                res = newResponse();

                int result = 0;

                switch (parseRequest(message, req))
                {
                case ParseError_NONE:
                    result = route(server, req, res);
                    break;
                case ParseError:
                    result = BadRequest_func(req, res);
                    break;
                case ReadMore:
                    memset(message, '\0', BUFLENGTH);
                    read(i, message, BUFLENGTH);
                    int length = atoi(getHeaderNodeItem(req->header, "Content-Length"));
                    char *temp = (char *)malloc(length);
                    memset(temp, '\0', length);
                    strcat(temp, req->body);
                    strcat(temp, message);

                    break;
                }
                if (result == 1)
                {
                    goto EXIT;
                }
                memset(message, '\0', BUFLENGTH);
                if (stringfyResponse(res, message, BUFLENGTH) == SHORTBUFFER)
                {
                    printf("error : short buffer size\n");
                    goto EXIT;
                }
                write(i, message, strlen(message));
                freeRequest(req);
                freeResponse(res);
                req = NULL;
                res = NULL;
            }
        }
    }
EXIT:
    freeRequest(req);
    freeResponse(res);
    freeServer(server);
    return 1;
}

int addRoute(Server *server, const char *path, Method method, Handler handler)
{
    if (server == NULL || path == NULL || handler == NULL || method == Method_NONE)
    {
        return 1;
    }

    PathNode *node = getPathNode(server->node, path);
    if (node == NULL)
    {
        addPathNode(&(server->node), path);
        node = getPathNode(server->node, path);
    }

    node->handlers[method] = handler;

    return 0;
}

int route(const Server *server, const Request *req, Response *res)
{
    if (server == NULL || server->node == NULL || req == NULL || res == NULL)
    {
        return 1;
    }
    PathNode *node = getPathNode(server->node, req->URI);
    if (node == NULL)
    {
        NotFound_func(req, res);
        return 0;
    }
    node->handlers[req->method](req, res);
    return 0;
}

int NotFound_func(const Request *req, Response *res)
{
    res->code = NotFound;
    res->protocol = strdup(ProtocolVersions[0]);
    res->body = strdup(defaultNotFound);

    char length[3] = {
        0,
    };
    snprintf(length, 3, "%ld", strlen(defaultNotFound));

    addHeaderNode(&(res->header), newHeaderNode("Content-Length", length));
    addHeaderNode(&(res->header), newHeaderNode("Content-Type", "text/html"));

    return 0;
}

int BadRequest_func(const Request *req, Response *res)
{
    res->code = BadRequest;
    res->protocol = strdup(ProtocolVersions[0]);
    res->body = strdup(defaultBadRequest);

    char length[3] = {
        0,
    };
    snprintf(length, 3, "%ld", strlen(defaultBadRequest));

    addHeaderNode(&(res->header), newHeaderNode("Content-Length", length));
    addHeaderNode(&(res->header), newHeaderNode("Content-Type", "text/html"));

    return 0;
}