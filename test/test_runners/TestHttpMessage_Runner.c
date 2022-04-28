/* AUTOGENERATED FILE. DO NOT EDIT. */

/*=======Automagically Detected Files To Include=====*/
#include "unity.h"
#include <http/httpMessage.h>
#include <stdlib.h>
#include <string.h>

/*=======External Functions This Runner Calls=====*/
extern void setUp(void);
extern void tearDown(void);
extern void test_parseMethod(void);
extern void test_parseStatusCode(void);
extern void test_isPorotocolVersValid(void);
extern void test_addHeaderNode(void);
extern void test_getHeaderNodeItem(void);
extern void test_parseHeaderNode(void);
extern void test_stringfyHeaderNode(void);
extern void test_parseRequest_get(void);
extern void test_parseRequest_post(void);
extern void test_stringfyRequest(void);


/*=======Mock Management=====*/
static void CMock_Init(void)
{
}
static void CMock_Verify(void)
{
}
static void CMock_Destroy(void)
{
}

/*=======Test Reset Options=====*/
void resetTest(void);
void resetTest(void)
{
  tearDown();
  CMock_Verify();
  CMock_Destroy();
  CMock_Init();
  setUp();
}
void verifyTest(void);
void verifyTest(void)
{
  CMock_Verify();
}

/*=======Test Runner Used To Run Each Test=====*/
static void run_test(UnityTestFunction func, const char* name, UNITY_LINE_TYPE line_num)
{
    Unity.CurrentTestName = name;
    Unity.CurrentTestLineNumber = line_num;
#ifdef UNITY_USE_COMMAND_LINE_ARGS
    if (!UnityTestMatches())
        return;
#endif
    Unity.NumberOfTests++;
    UNITY_CLR_DETAILS();
    UNITY_EXEC_TIME_START();
    CMock_Init();
    if (TEST_PROTECT())
    {
        setUp();
        func();
    }
    if (TEST_PROTECT())
    {
        tearDown();
        CMock_Verify();
    }
    CMock_Destroy();
    UNITY_EXEC_TIME_STOP();
    UnityConcludeTest();
}

/*=======MAIN=====*/
int main(void)
{
  UnityBegin("test/TestHttpMessage.c");
  run_test(test_parseMethod, "test_parseMethod", 14);
  run_test(test_parseStatusCode, "test_parseStatusCode", 23);
  run_test(test_isPorotocolVersValid, "test_isPorotocolVersValid", 33);
  run_test(test_addHeaderNode, "test_addHeaderNode", 40);
  run_test(test_getHeaderNodeItem, "test_getHeaderNodeItem", 50);
  run_test(test_parseHeaderNode, "test_parseHeaderNode", 65);
  run_test(test_stringfyHeaderNode, "test_stringfyHeaderNode", 87);
  run_test(test_parseRequest_get, "test_parseRequest_get", 112);
  run_test(test_parseRequest_post, "test_parseRequest_post", 130);
  run_test(test_stringfyRequest, "test_stringfyRequest", 150);

  return UnityEnd();
}
