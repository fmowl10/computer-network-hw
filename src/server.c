/**
 * @file server.c
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief server program
 * @version 0.0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "http/httpMessage.h"
#include "http/httpServer.h"

const char *helloGetBody = "body";

/**
 * @brief GET /hello
 *
 */
int helloGet(const Request *req, Response *res)
{
    res->code = OK;
    res->protocol = HTTP1;

    if (req->method == GET)
        res->body = helloGetBody;

    res->header = newHeaderNode("Content-Type", "text/plain");

    char bodyLength[10] = {
        0,
    };

    sprintf(bodyLength, "%s", helloGetBody);

    addHeaderNode(res->header, newHeaderNode("Content-length", bodyLength));

    return 0;
}

const char *rootBody =
    "<!DOCTYPE html>\n"
    "<html>\n"
    "<head>\n"
    "<title>http implement</title>\n"
    "</head>\n"
    "<body>\n"
    "<h1>hello</h1>\n"
    "</body>\n"
    "</html>\n";

int root(const Request *req, Response *res)
{
    res->code = OK;
    res->protocol = HTTP1;

    if (req->method == GET)
    {
        res->body = rootBody;
    }

    res->header = newHeaderNode("Content-Type", "text/html");

    char bodyLength[10] = {
        0,
    };

    sprintf(bodyLength, "%s", rootBody);

    addHeaderNode(res->header, newHeaderNode("Content-length", bodyLength));

    return 0;
}

int main(int argc, char **argv)
{
    if (argc != 2)
    {
        printf("pass port number ./server [portnumber]");
        return EXIT_FAILURE;
    }

    int portNumber = atoi(argv[1]);

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

    // /
    addRoute(server, "/", GET, root);
    addRoute(server, "/", HEAD, root);

    // run server loop
    return startServer(server);
}