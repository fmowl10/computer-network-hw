#include <sys/socket.h>

#include <http/httpMessage.h>
#include <http/httpServer.h>

#include <unity.h>

void setUp(void)
{
}

void tearDown(void)
{
}

void test_parsePathSeg()
{
    const char *path = "/hello/world";
    const char *paths[] = {path, path + 1, path + 7};
    PathSegNode *head = parsePathSeg(path);
    PathSegNode *cur = head;

    TEST_ASSERT(head != NULL);
    for (int i = 0; i < 3; i++)
    {
        TEST_ASSERT_EQUAL_PTR(paths[i], cur->start);
        cur = cur->next;
    }

    freePathSegList(head);
}

void test_addPahtNode()
{
    const char *path = "/hello/world";

    const char *expected = "hello";
    PathNode *pn = NULL;

    TEST_ASSERT(addPathNode(&pn, path) == 0);
    TEST_ASSERT_EQUAL_STRING(expected, getPathNode(pn, "/hello")->dir);
    TEST_ASSERT_EQUAL_STRING("world", getPathNode(pn, path)->dir);

    TEST_ASSERT(addPathNode(&pn, "/guy") == 0);
    TEST_ASSERT_EQUAL_STRING("guy", getPathNode(pn, "/guy")->dir);
    TEST_ASSERT_EQUAL_PTR(pn->child->next, getPathNode(pn, "/guy"));
    TEST_ASSERT_EQUAL_STRING("world", getPathNode(pn, "/hello/world")->dir);
    TEST_ASSERT_EQUAL_STRING(pn->child->child->dir, getPathNode(pn, "/hello/world")->dir);

    TEST_ASSERT_EQUAL_PTR(pn, getPathNode(pn, "/"));

    TEST_ASSERT_EQUAL_PTR(NULL, getPathNode(pn, "/hello/worl"));
    TEST_ASSERT_EQUAL_PTR(NULL, getPathNode(pn, "/hell/world"));
    TEST_ASSERT_EQUAL_PTR(NULL, getPathNode(pn, "/gui"));
    TEST_ASSERT_EQUAL_PTR(NULL, getPathNode(pn, "/guk"));

    freePathNodeList(pn);
}
