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

#include <ctype.h>
#include <netdb.h>
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
    const char *method = NULL;
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
            method = argv[i + 1];
        }
        else if (strncmp(argv[i], "-d", 2) == 0)
        {
            data = argv[i + 1];
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
    const char *port = NULL;
    char *tempString = NULL;

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

    // port parse
    strcpy(host, strtok(host, ":"));
    port = strtok(NULL, ":");
    if (port != NULL)
    {
        char *iterTempString = port;
        while (isdigit(*iterTempString))
            iterTempString++;

        if (*iterTempString != '\0')
        {
            printf("invalid port:%s\n", port);
            return EXIT_FAILURE;
        }
    }
    else
    {
        port = "80";
    }
    struct addrinfo hints;
    struct addrinfo *servinfo;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    getaddrinfo(host, port, &hints, &servinfo);
    for (; servinfo != NULL; servinfo = servinfo->ai_next)
    {
        printf("%s\n", servinfo->ai_canonname);
    }
    freeaddrinfo(servinfo);
    return EXIT_SUCCESS;
}