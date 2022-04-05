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

/**
 * @brief set number of supported method
 *
 */
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

/**
 * @brief http1 raw string
 *
 */
const char *HTTP1 = "HTTP/1.0";

/**
 * @brief HeaderNode
 *
 */
typedef struct _HeaderNode
{
    const char *key;
    const char *value;
    struct _HeaderNode *next;
} HeaderNode;

/**
 * @brief allocate new HeaderNode
 *
 * @param key
 * @param value
 * @return HeaderNode*
 */
HeaderNode *newHeaderNode(const char *key, const char *value);

/**
 * @brief Add child to end of parent
 *
 * @param parent
 * @param child
 * @return int
 */
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
 * @brief parse raw requset string to Request
 *
 * @param rawRequest in
 * @param request  out
 * @return int  error status
 */
int requestParse(const char *rawRequest, Request *request);

/**
 * @brief stringfy request struct to raw string
 *
 * @param request in
 * @param destStr out
 * @return int  error status
 */
int requestStringfy(const Request *request, const char *destStr);

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

/**
 * @brief parse raw reponse string to Response
 *
 * @param rawResponse in
 * @param response out
 * @return int error status
 */
int responseParse(const char *rawResponse, Response *response);

/**
 * @brief stringfy struct response to raw string
 *
 * @param response in
 * @param destStr out
 * @return int error status
 */
int responseStringfy(const Response *response, const char *destStr);

#endif