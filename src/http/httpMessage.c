#include <http/httpMessage.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Method parseMethod(const char *rawMethod)
{
    for (int i = 0; i < NUMSUPPORTEDMETHOD; i++)
    {
        if (strcmp(RawMethods[i], rawMethod) == 0)
            return (Method)i;
    }
    return Method_NONE;
}

StatusCode parseStatusCode(int rawCode, const char *message)
{
    for (int i = 0; i < NUMSUPPORTEDSTATUS; i++)
    {
        if (RawStatusCodes[i] == rawCode && strcmp(StatusCodeMessages[i], message))
        {
            return (StatusCode)i;
        }
    }
    return StatusCode_NONE;
}

bool isProtocolVersionValid(const char *rawProtocol)
{
    int i = 0;
    for (; i < NUMSUPPORTEDPROTOCOL; i++)
    {
        if (strcmp(ProtocolVersions[i], rawProtocol) == 0)
            return true;
    }
    return false;
}

HeaderNode *newHeaderNode(const char *key, const char *value)
{
    HeaderNode *temp = malloc(sizeof(HeaderNode));
    temp->next = NULL;
    temp->key = strdup(key);
    temp->value = strdup(value);
    return temp;
}

void freeHeaderList(HeaderNode **parent)
{
    HeaderNode *tmp;
    while (*parent != NULL)
    {
        tmp = *parent;
        *parent = (*parent)->next;
        free(tmp->key);
        free(tmp->value);
        free(tmp);
    }

    // double check
    *parent = NULL;
}

int addHeaderNode(HeaderNode **parent, HeaderNode *child)
{
    if (child == NULL)
        return 1;
    if (parent == NULL)
        *parent = child;
    HeaderNode *cur = *parent;
    while (cur->next != NULL)
    {
        cur = cur->next;
    }
    cur->next = child;
    return 0;
}

const char *getHeaderNodeItem(HeaderNode *root, const char *key)
{
    HeaderNode *cur = root;
    while (cur != NULL)
    {
        if (strcmp(cur->key, key) == 0)
            return cur->value;
        cur = cur->next;
    }
    return NULL;
}

ParseErrorno parseHeaderNode(char **rawHeader, HeaderNode **hn)
{
    char *line = strtok(*rawHeader, "\r\n");
    char header[41] = {
        0,
    };
    char value[101] = {
        0,
    };
    while (line != NULL)
    {
        sscanf(line, "%40s: %100[a-zA-Z ]s", header, value);
        if (addHeaderNode(hn, newHeaderNode(header, value)) == 1)
            return ParseError;

        // body start with \r\n\r\n
        if (strncmp(line + strlen(line) + 1, "\n\r\n", 3) == 0)
        {
            *rawHeader = line + strlen(line) + 4;
            break;
        }

        line = strtok(NULL, "\r\n");
    }
    return ParseError_NONE;
}

StringfyErrorno stringfyHeaderNode(HeaderNode *root, char *destStr, size_t sz)
{
    HeaderNode *cur = root;
    int length = 0;
    while (cur != NULL)
    {
        // length check
        length = strlen(destStr) + strlen(cur->key) + strlen(": ") + strlen(cur->value) + strlen("\r\n");
        if (length + 1 > sz)
            return TOO_SHORT;

        strcat(destStr, cur->key);
        strcat(destStr, ": ");
        strcat(destStr, cur->value);
        strcat(destStr, "\r\n");
    }
    return StringfyError_NONE;
}

Request *newRequest()
{
    Request *temp = (Request *)malloc(sizeof(Request));
    temp->method = Method_NONE;
    temp->URI = NULL;
    temp->protocol = NULL;
    temp->body = NULL;
    temp->header = NULL;
    return temp;
}

ParseErrorno parseRequest(char *rawRequset, Request *request)
{
    if (request == NULL || request == NULL)
        return ParseError;

    char rawMethod[6] = {
        0,
    };
    char rawProtocol[11] = {
        0,
    };

    // first line processing
    char *line = NULL;
    request->URI = malloc(sizeof(char) * 201);

    line = strtok(rawRequset, "\r\n");

    // split
    int valueCount = sscanf(line, "%5s %10s %201s",
                            rawMethod, request->URI, rawProtocol);
    if (valueCount != 3)
        return ParseError;

    // method string to enum
    request->method = parseMethod(rawMethod);

    if (request->method == Method_NONE)
        return ParseError;

    // httpversion strin to enum
    if (!isProtocolVersionValid(rawProtocol))
        return ParseError;
    request->protocol = strdup(rawProtocol);

    if (parseHeaderNode(&line, &(request->header)) == ParseError)
        return ParseError;

    // 나머지 body 복사
    int contentLength = atoi(getHeaderNodeItem(request->header, "Content-Length"));
    if (contentLength == 0)
        return ParseError_NONE;

    request->body = strndup(line, contentLength);
    if (strlen(line) < contentLength)
        return ReadMore;

    return ParseError_NONE;
}

StringfyErrorno stringfyRequest(const Request *request, char *destStr, size_t sz)
{
    int len = snprintf(NULL, 0, "%s %s %s\r\n",
                       RawMethods[request->method], request->URI, request->protocol);
    if (len + 1 > sz) // with null
        return TOO_SHORT;

    snprintf(destStr, sz, "%s %s %s\r\n",
             RawMethods[request->method], request->URI, request->protocol);

    if (stringfyHeaderNode(request->header, destStr, sz) == TOO_SHORT)
        return TOO_SHORT;

    len = strlen(destStr) + strlen(destStr) + strlen(request->body);
    if (len + 1 > sz)
        return TOO_SHORT;

    strcat(destStr, "\r\n");
    strcat(destStr, request->body);

    return StringfyError_NONE;
}

Response *newResponse()
{
    Response *temp = (Response *)malloc(sizeof(Response));
    temp->protocol = NULL;
    temp->code = StatusCode_NONE;
    temp->header = NULL;
    temp->body = NULL;
    return temp;
}

ParseErrorno parseResponse(char *rawResponse, Response *response)
{
    if (rawResponse == NULL || response == NULL)
        return ParseError;
    char rawProtocol[11] = {
        0,
    };
    int rawStatuscode = 0;
    char RawStatusCodeMessage[40] = {
        0,
    };

    char *line = NULL;
    line = strtok(rawResponse, "\r\n");
    int valueCount = sscanf(line, "%11s %3d %[a-zA-Z ]",
                            rawProtocol, &rawStatuscode, RawStatusCodeMessage);
    if (valueCount != 0)
        return ParseError;

    response->code = parseStatusCode(rawStatuscode, RawStatusCodeMessage);

    if (response->code == StatusCode_NONE)
        return ParseError;

    if (!isProtocolVersionValid(rawProtocol))
        return ParseError;
    response->protocol = strdup(rawProtocol);

    if (parseHeaderNode(&line, &(response->header)) == ParseError)
        return ParseError;

    int contentLength = atoi(getHeaderNodeItem(response->header, "Content-Length"));
    if (contentLength == 0)
        return ParseError_NONE;
    response->body = strndup(line, contentLength);
    if (strlen(line) < contentLength)
        return ReadMore;
    return ParseError_NONE;
}

StringfyErrorno stringfyResponse(const Response *response, char *destStr, size_t sz)
{
    int len = snprintf(NULL, 0, "%s %d %s\r\n",
                       response->protocol,
                       RawStatusCodes[response->code],
                       StatusCodeMessages[response->code]);
    if (len + 1 > sz)
        return TOO_SHORT;

    snprintf(destStr, sz, "%s %d %s\r\n", response->protocol,
             RawStatusCodes[response->code],
             StatusCodeMessages[response->code]);

    if (stringfyHeaderNode(response->header, destStr, sz) == TOO_SHORT)
        return TOO_SHORT;

    len = strlen(destStr) + strlen("\r\n") + strlen(response->body);
    if (len + 1 > sz)
        return TOO_SHORT;

    strcat(destStr, "\r\n");
    strcat(destStr, response->body);
    return StringfyError_NONE;
}
