/**
 * @file client.c
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief http client
 * @version 0.1
 * @date 2022-03-31
 *
 * @copyright Copyright (c) 2022, Jinseok Kim
 *
 */

#include <arpa/inet.h>
#include <ctype.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <http/httpMessage.h>

int main(int argc, char **argv)
{
    char host[100] = {
        0,
    };
    const char *uri = NULL;
    const char *rawPort = "80";
    const char *rawMethod = NULL;
    const char *data = NULL;

    if (argc == 1 || (argc - 1) % 2 == 1)
    {
        printf("client -h [host] -m [method] -d [data]");
        return EXIT_FAILURE;
    }
    for (int i = 1; i < argc; i += 2)
    {
        if (strncmp(argv[i], "-h", 2) == 0)
        {
            uri = argv[i + 1];
        }
        else if (strncmp(argv[i], "-m", 2) == 0)
        {
            rawMethod = argv[i + 1];
        }
        else if (strncmp(argv[i], "-d", 2) == 0)
        {
            data = argv[i + 1];
        }
    }
    if (rawMethod == NULL)
    {
        if (data == NULL || strlen(data) == 0)
        {
            rawMethod = "GET";
        }
        else
        {
            rawMethod = "POST";
        }
    }
    if (uri == NULL)
    {
        printf("pass right\n");
        return EXIT_FAILURE;
    }
    char path[100] = {
        0,
    };

    int count = sscanf(uri, "http://%[^/]%s", host, path);
    if (count == 0)
    {
        count = sscanf(uri, "%[^/]%s", host, path);
        if (count == 0)
        {
            printf("input uri\n ex: ./testhttp http://example.com/hello/world");
            return EXIT_FAILURE;
        }
    }

    if (strlen(path) == 0)
    {
        memset(path, '\0', 100);
        strcpy(path, "/");
    }

    // port parse
    strcpy(host, strtok(host, ":"));
    rawPort = strtok(NULL, ":");
    if (rawPort != NULL)
    {
        const char *iterTempString = rawPort;
        while (isdigit(*iterTempString))
            iterTempString++;

        if (*iterTempString != '\0')
        {
            printf("invalid port:%s\n", rawPort);
            return EXIT_FAILURE;
        }
        if (atoi(rawPort) >= (1 << 16))
        {
            printf("invalid port:%s\n", rawPort);
            return EXIT_FAILURE;
        }
    }

    Method method = parseMethod(rawMethod);

    if (method == Method_NONE)
    {
        printf("input method with in [GET, HEAD, POST, PUT]\n");
        return EXIT_FAILURE;
    }

    if (method == GET || method == HEAD)
    {
        if (data != NULL && strlen(data) != 0)
        {
            printf("do not pass data on %s\n", rawMethod);
            return EXIT_FAILURE;
        }
    }

    if (method == POST || method == PUT)
    {
        if (data == NULL || strlen(data) == 0)
        {
            printf("input a data\n");
            return EXIT_FAILURE;
        }
    }

    struct addrinfo hints, *res;

    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo(host, rawPort, &hints, &res) != 0)
    {
        printf("getaddrinfo error\n");
        return EXIT_FAILURE;
    }

    int fd = socket(res->ai_family, res->ai_socktype, 0);

    if (fd == -1)
    {
        printf("socket error\n");
        return EXIT_FAILURE;
    }

    if (connect(fd, res->ai_addr, res->ai_addrlen) == -1)
    {
        printf("connect error\n");
        return 0;
    }

    char buff[65536] = {
        0,
    };

    Request *req = newRequest();
    req->method = method;
    req->protocol = strdup(ProtocolVersions[0]);
    req->URI = strdup(path);

    strcat(host, ":");
    strcat(host, rawPort);

    addHeaderNode(&(req->header), newHeaderNode("User-Agent", "client"));
    addHeaderNode(&(req->header), newHeaderNode("Host", host));
    addHeaderNode(&(req->header), newHeaderNode("Accept", "*/*"));

    if (req->method == POST || req->method == PUT)
    {
        req->body = strdup(data);
        addHeaderNode(&(req->header), newHeaderNode("Content-Type", "text/plain"));
        char lengthStr[10] = {
            0,
        };
        snprintf(lengthStr, 10, "%ld", strlen(req->body));
        addHeaderNode((&req->header), newHeaderNode("Content-Length", lengthStr));
    }

    stringfyRequest(req, buff, 65536);
    printf("%s\n", buff);

    write(fd, buff, strlen(buff));

    memset(buff, '\0', 65536);

    read(fd, buff, 65536);

    printf("raw response message\n%s\n", buff);

    Response *response = newResponse();

    parseResponse(buff, response);

    printf("status : ");

    StatusCode code = response->code;

    switch (code)
    {
    case OK:
        printf("\033[40m\033[32m%s\033[0m", StatusCodeMessages[code]);
        break;
    case NotFound:

    case BadRequest:
        printf("\033[40m\033[31m%s\033[0m", StatusCodeMessages[code]);
        break;
    default:
        break;
    }
    printf("\n");
    printf("body : %s\n", response->body);

    close(fd);
    freeRequest(req);
    freeResponse(response);
    freeaddrinfo(res);

    return EXIT_SUCCESS;
}