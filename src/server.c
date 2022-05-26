/**
 * @file server.c
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief server program
 * @version 0.0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022, Jinsoek Kim
 *
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <http/httpMessage.h>
#include <http/httpServer.h>

#define setHost(res) addHeaderNode(&(res->header), newHeaderNode("Server", "Server"));

const char *helloGetBody = "body\n";

const char *helloFormat = "hello, %s\n";

/**
 * @brief GET /hello
 *
 */
int helloGet(const Request *req, Response *res)
{
    res->code = OK;
    res->protocol = strdup(req->protocol);

    if (req->method == GET)
        res->body = strdup(helloGetBody);

    addHeaderNode(&(res->header), newHeaderNode("Content-Type", "text/plain"));

    char bodyLength[10] = {
        0,
    };

    sprintf(bodyLength, "%ld", strlen(helloGetBody));

    addHeaderNode(&(res->header), newHeaderNode("Content-Length", bodyLength));

    setHost(res);

    return 0;
}

int helloPost(const Request *req, Response *res)
{
    if (strlen(req->body) == 0)
    {
        BadRequest_func(req, res);
        return 0;
    }

    res->code = OK;
    res->protocol = strdup(req->protocol);

    // remove escape, plus null and \n
    int bodyLength = strlen(helloFormat) + strlen(req->body) - 2 + 1;
    res->body = malloc(bodyLength);

    memset(res->body, '\0', bodyLength);
    snprintf(res->body, bodyLength, helloFormat, req->body);

    char bodyLengthStr[10] = {
        0,
    };
    sprintf(bodyLengthStr, "%ld", strlen(res->body));

    addHeaderNode(&(res->header), newHeaderNode("Content-Length", bodyLengthStr));
    addHeaderNode(&(res->header), newHeaderNode("Content-Type", "text/plain"));

    setHost(res);
    return 0;
}

/**
 * @brief hard coded /index.html
 *
 */
const char *rootBody = {
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<title>http implement</title>\n"
    "</head>\n"
    "<body>\n"
    "<h1>hello</h1>\n"
    "</body>\n"
    "</html>\n"};

int root(const Request *req, Response *res)
{
    res->code = OK;
    res->protocol = strdup(req->protocol);

    // if method is get than add a body
    if (req->method == GET)
    {
        res->body = strdup(rootBody);
    }

    addHeaderNode(&(res->header), newHeaderNode("Content-Type", "text/html"));

    char bodyLength[10] = {
        0,
    };

    sprintf(bodyLength, "%ld", strlen(rootBody));

    addHeaderNode(&(res->header), newHeaderNode("Content-Length", bodyLength));

    setHost(res);

    return 0;
}

int main(int argc, char **argv)
{
    // if user doesn't pass port number than terminate.
    if (argc != 2)
    {
        printf("pass port number ./server [portnumber]");
        return EXIT_FAILURE;
    }

    int portNumber = atoi(argv[1]);

    // if portNumber can not parse or not a proper port number than terminate.
    if (portNumber == 0 || (portNumber > UINT16_MAX))
    {
        printf("pass right port number");
        return EXIT_FAILURE;
    }

    // create server instance
    Server *server = newServer(portNumber);

    // add route

    // /hello
    addRoute(server, "/hello", GET, helloGet);
    addRoute(server, "/hello", HEAD, helloGet);
    addRoute(server, "/hello", POST, helloPost);

    // /(root)
    addRoute(server, "/", GET, root);
    addRoute(server, "/", HEAD, root);

    // run server loop
    return startServer(server);
}
