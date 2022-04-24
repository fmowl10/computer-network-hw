#include <http/httpMessage.h>
#include <unity.h>

void setUp(void) {}

void tearDown(void) {}

void test_newRequest(void)
{
    TEST_ASSERT_EQUAL(NULL, newRequest()->body);
}