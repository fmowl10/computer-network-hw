/**
 * @file httpMessage.h
 * @author Jinseok Kim (jinseok1001@hotmail.com)
 * @brief http message parser
 * @version 0.0.1
 * @date 2022-03-30
 *
 * @copyright Copyright (c) 2022, Jinseok Kim
 *
 */

#ifndef __HTTPMESSAGE_H__
#define __HTTPMESSAGE_H__

#include <stdbool.h>
#include <stddef.h>

/**
 * @brief parse error numbers
 *
 */
typedef enum
{
    ParseError_NONE = 0,
    ParseError,
    ReadMore,
} ParseErrorno;

typedef enum
{
    StringfyError_NONE = 0,
    Stringfyerror,
    SHORTBUFFER,
} StringfyErrorno;

/**
 * @brief set number of supported method
 *
 */
#define NUMSUPPORTEDMETHOD 4

/**
 * @brief Request method enum
 *
 */
typedef enum
{
    GET,
    HEAD,
    POST,
    PUT,
    Method_NONE
} Method;

Method parseMethod(const char *rawMethod);

/**
 * @brief method raw string
 *
 */
extern const char *RawMethods[NUMSUPPORTEDMETHOD];

#define NUMSUPPORTEDSTATUS 3

/**
 * @brief status Code enum
 *
 */
typedef enum
{
    OK,
    BadRequest,
    NotFound,
    StatusCode_NONE
} StatusCode;

/**
 * @brief  status code enum to phrase
 *
 */
extern const char *StatusCodeMessages[NUMSUPPORTEDSTATUS];

/**
 * @brief status code enum to raw string
 *
 */
extern const int RawStatusCodes[NUMSUPPORTEDSTATUS];

StatusCode parseStatusCode(int rawCode, const char *message);

#define NUMSUPPORTEDPROTOCOL 2

/**
 * @brief https raw string
 *
 */
extern const char *ProtocolVersions[NUMSUPPORTEDPROTOCOL];

bool isProtocolVersionValid(const char *rawProtocol);

/**
 * @brief HeaderNode
 *
 */
typedef struct HeaderNode
{
    char *key;
    char *value;
    struct HeaderNode *next;
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
 * @brief free header list
 *
 * @param parent
 */
void freeHeaderList(HeaderNode **parent);

/**
 * @brief Add child to end of parent
 *
 * @param parent
 * @param child
 * @return int
 */
int addHeaderNode(HeaderNode **parent, HeaderNode *child);

/**
 * @brief search Header Node by Key
 *
 * @param root
 * @param key
 * @return const char* Null if header not find.
 */
const char *getHeaderNodeItem(HeaderNode *root, const char *key);

ParseErrorno parseHeaderNode(char **rawHeader, HeaderNode **hn);

StringfyErrorno stringfyHeaderNode(HeaderNode *root, char *destStr, size_t sz);

/**
 * @brief request struct
 *
 */
typedef struct
{
    Method method;
    char *URI;
    char *protocol;
    char *body;
    HeaderNode *header;
} Request;

/**
 * @brief init new request
 *
 * @return Request*
 */
Request *newRequest();

/**
 * @brief free request
 *
 * @param request
 */
void freeRequest(Request *request);

/**
 * @brief parse raw requset string to Request
 *
 * @param rawRequest in
 * @param request  out
 * @return int  error status
 */
ParseErrorno parseRequest(char *rawRequest, Request *request);

/**
 * @brief stringfy request struct to raw string
 *
 * @param request in
 * @param destStr out
 * @return int  error status
 */

StringfyErrorno stringfyRequest(const Request *request, char *destStr, size_t sz);

/**
 * @brief response struct
 *
 */
typedef struct
{
    char *protocol;
    StatusCode code;
    HeaderNode *header;
    char *body;
} Response;

/**
 * @brief init new response
 *
 * @return Response*
 */
Response *newResponse();

/**
 * @brief free response
 *
 * @param response
 */
void freeResponse(Response *response);

/**
 * @brief parse raw reponse string to Response
 *
 * @param rawResponse in
 * @param response out
 * @return int error status
 */
ParseErrorno parseResponse(char *rawResponse, Response *response);

/**
 * @brief stringfy struct response to raw string
 *
 * @param response in
 * @param destStr out
 * @return int error status
 */
StringfyErrorno stringfyResponse(const Response *response, char *destStr, size_t sz);

#endif