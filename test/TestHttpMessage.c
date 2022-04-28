#include <http/httpMessage.h>
#include <stdlib.h>
#include <string.h>
#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_parseMethod(void)
{
    TEST_ASSERT_EQUAL_INT32(GET, parseMethod("GET"));
    TEST_ASSERT_EQUAL_INT32(HEAD, parseMethod("HEAD"));
    TEST_ASSERT_EQUAL_INT32(POST, parseMethod("POST"));
    TEST_ASSERT_EQUAL_INT32(PUT, parseMethod("PUT"));
    TEST_ASSERT_EQUAL_INT32(Method_NONE, parseMethod("DELTET"));
}

void test_parseStatusCode(void)
{
    TEST_ASSERT_EQUAL_INT32(OK, parseStatusCode(200, "OK"));
    TEST_ASSERT_EQUAL_INT32(BadRequest, parseStatusCode(400, "Bad Request"));
    TEST_ASSERT_EQUAL_INT32(NotFound, parseStatusCode(404, "Not Found"));

    TEST_ASSERT_EQUAL_INT32(StatusCode_NONE,
                            parseStatusCode(200, "Bad Request"));
    TEST_ASSERT_EQUAL_INT32(StatusCode_NONE, parseStatusCode(222, "OK"));
}

void test_isPorotocolVersValid(void)
{
    TEST_ASSERT(isProtocolVersionValid("HTTP/1.0"));
    TEST_ASSERT(!isProtocolVersionValid("HTTP/1.1"));
    TEST_ASSERT(!isProtocolVersionValid("HTTP/2.0"));
}

void test_addHeaderNode(void)
{
    HeaderNode *hn = newHeaderNode("hello", "12");
    HeaderNode *child = newHeaderNode("world", "13");
    addHeaderNode(&hn, child);
    TEST_ASSERT_EQUAL_PTR(child, hn->next);
    freeHeaderList(&hn);
    TEST_ASSERT_EQUAL_PTR(NULL, hn);
}

void test_getHeaderNodeItem(void)
{
    HeaderNode *hn = NULL;
    HeaderNode *child1 = newHeaderNode("hello", "12");
    HeaderNode *child2 = newHeaderNode("world", "13");
    HeaderNode *child3 = newHeaderNode("noll", "22");

    addHeaderNode(&hn, child1);
    addHeaderNode(&hn, child3);
    addHeaderNode(&hn, child2);

    TEST_ASSERT_EQUAL_STRING("13", getHeaderNodeItem(hn, "world"));
    freeHeaderList(&hn);
}

void test_parseHeaderNode(void)
{
    HeaderNode *hn = NULL;
    char *headers = strdup(
        "Host: example.com\r\n"
        "User-Agent: curl/7.78.0\r\n"
        "Accept: */*\r\n\r\nhello");
    char *headersLine = headers;

    parseHeaderNode(&headersLine, &hn);

    // body test
    TEST_ASSERT_EQUAL_STRING("hello", headersLine);

    TEST_ASSERT_EQUAL_STRING("example.com", getHeaderNodeItem(hn, "Host"));
    TEST_ASSERT_EQUAL_STRING("curl/7.78.0",
                             getHeaderNodeItem(hn, "User-Agent"));
    TEST_ASSERT_EQUAL_STRING("*/*", getHeaderNodeItem(hn, "Accept"));

    freeHeaderList(&hn);
    free(headers);
}

void test_stringfyHeaderNode(void)
{
    char *headerout = (char *)malloc(60);
    memset(headerout, '\0', 60);
    char *headerOutShort = (char *)malloc(20);
    memset(headerOutShort, '\0', 20);
    HeaderNode *hn = NULL;

    addHeaderNode(&hn, newHeaderNode("Host", "example.com"));
    addHeaderNode(&hn, newHeaderNode("User-Agent", "curl/7.78.0"));
    addHeaderNode(&hn, newHeaderNode("Accept", "*/*"));

    TEST_ASSERT_EQUAL_INT32(ParseError_NONE,
                            stringfyHeaderNode(hn, headerout, 60));
    TEST_ASSERT_EQUAL_STRING("Host: example.com\r\n"
                             "User-Agent: curl/7.78.0\r\n"
                             "Accept: */*\r\n\r\n",
                             headerout);

    TEST_ASSERT_EQUAL(TOO_SHORT, stringfyHeaderNode(hn, headerOutShort, 20));

    free(headerout);
    free(headerOutShort);
    freeHeaderList(&hn);
}

void test_parseRequest_get(void)
{
    char *httpMessage = strdup("GET / HTTP/1.0\r\n"
                               "Host: example.com\r\n"
                               "User-Agent: curl/7.78.0\r\n"
                               "Accept: */*\r\n\r\n");
    char *httpMessageLine = httpMessage;
    Request *req = newRequest();

    TEST_ASSERT_EQUAL_INT32(ParseError_NONE,
                            parseRequest(httpMessageLine, req));
    TEST_ASSERT_EQUAL_INT32(GET, req->method);
    TEST_ASSERT(isProtocolVersionValid(req->protocol));
    TEST_ASSERT_EQUAL_STRING("example.com",
                             getHeaderNodeItem(req->header, "Host"));

    free(httpMessage);
    freeRequest(req);
}

void test_parseRequest_post(void)
{
    char *httpMessage = strdup("POST /hello HTTP/1.0\r\n"
                               "Host: example.com\r\n"
                               "User-Agent: curl/7.78.0\r\n"
                               "Accept: */*\r\n"
                               "Content-Length: 6\r\n\r\nhello\n");
    char *httpMessageLine = httpMessage;
    Request *req = newRequest();

    TEST_ASSERT_EQUAL_INT32(ParseError_NONE, parseRequest(httpMessageLine, req));
    TEST_ASSERT_EQUAL_INT32(POST, req->method);
    TEST_ASSERT(isProtocolVersionValid(req->protocol));
    TEST_ASSERT_EQUAL_STRING("example.com", getHeaderNodeItem(req->header, "Host"));
    TEST_ASSERT_EQUAL_STRING("hello\n", req->body);

    free(httpMessage);
    freeRequest(req);
}

void test_stringfyRequest(void)
{
    char *httpMessageOut = malloc(76);
    memset(httpMessageOut, '\0', 76);

    const char *expected = "GET / HTTP/1.0\r\n"
                           "Host: example.com\r\n"
                           "User-Agent: curl/7.78.0\r\n"
                           "Accept: */*\r\n\r\n";

    Request *req = newRequest();

    char *shortbuffer = malloc(60);
    memset(shortbuffer, '\0', 60);

    req->method = GET;
    req->protocol = strdup(ProtocolVersions[0]);
    req->URI = strdup("/");

    addHeaderNode(&(req->header), newHeaderNode("Host", "example.com"));
    addHeaderNode(&(req->header), newHeaderNode("User-Agent", "curl/7.78.0"));
    addHeaderNode(&(req->header), newHeaderNode("Accept", "*/*"));

    TEST_ASSERT_EQUAL_INT32(StringfyError_NONE,
                            stringfyRequest(req, httpMessageOut, 76));

    TEST_ASSERT_EQUAL_STRING(expected, httpMessageOut);

    TEST_ASSERT_EQUAL_INT32(TOO_SHORT,
                            stringfyRequest(req, shortbuffer,
                                            sizeof(shortbuffer)));

    free(httpMessageOut);
    free(shortbuffer);
    freeRequest(req);
}