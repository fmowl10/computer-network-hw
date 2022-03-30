/**
 * @file httpMessage.h
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief http message parser
 * @version 0.0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022
 *
 */
#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include <stdlib.h>

#define NUMSUPPORTEDMETHOD 5

/**
 * @brief Request method enum
 *
 */
typedef enum _method
{
    GET,
    HEAD,
    POST,
    PUT,
    METHOD_NONE
} Method;

/**
 * @brief method raw string
 *
 */
const char *MethodRaw[] = {"GET", "HEAD", "POST", "PUT", NULL};

/**
 * @brief status Code enum
 *
 */
typedef enum _StatusCode
{
    OK,
    BadRequest,
    NotFound,
    STATUSCODE_NONE
} StatusCode;

/**
 * @brief  status code enum to phrase
 *
 */
const char *StatusCodePhrase[] = {"OK", "Bad Request", "Not Found", NULL};

/**
 * @brief status code enum to raw string
 *
 */
const char *StatusCodeRaw[] = {"200", "400", "404", NULL};

const char *HTTP1 = "HTTP1";

/**
 * @brief HeaderNode
 *
 */
typedef struct _HeaderNode
{
    const char *key;
    const char *value;
    struct _HeaderNode *next;
} HeaderNode; // add addHeaderNode

HeaderNode *newHeaderNode(const char *key, const char *value);
int addHeaderNode(HeaderNode *parent, HeaderNode *child);

/**
 * @brief request struct
 *
 */
typedef struct _request
{
    Method method;
    const char *URI;
    const char *protocol;
    const char *body;
    HeaderNode *header;
} Request;

/**
 * @brief response struct
 *
 */
typedef struct _response
{
    const char *protocol;
    StatusCode code;
    HeaderNode *header;
    const char *body;
} Response;

int requestParse(const char *rawRequest, Request *request);
int requestStringfy(const Request *request, const char *destStr);

int responseParse(const char *rawResponse, Response *response);
int responseStringfy(const Response *response, const char *destStr);

#endif