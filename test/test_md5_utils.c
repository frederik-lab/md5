#include "unity/unity.h"
#include "md5_utils.h"
#include <string.h>
#include <stdlib.h>

void setUp(void) {
    // This runs before EACH test. 
    // If you don't need to set anything up, leave it empty.
}

void tearDown(void) {
    // This runs after EACH test.
    // If you don't need to clean anything up, leave it empty.
}

void test_PaddingLength_SmallMessage(void) {
    TEST_ASSERT_EQUAL_INT(64, calculate_padded_msg_length(7));
}

void test_PaddingLength_OneBlockMessage(void) {
    TEST_ASSERT_EQUAL_INT(64, calculate_padded_msg_length(55));
}

void test_PaddingLength_MultipleBlockMessage(void) {
    TEST_ASSERT_EQUAL_INT(128, calculate_padded_msg_length(56));
}

void test_PaddingLength_OverflowReturnsZero(void) {
    TEST_ASSERT_EQUAL_INT(0, calculate_padded_msg_length((SIZE_MAX-8)));
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_PaddingLength_SmallMessage);
    RUN_TEST(test_PaddingLength_OneBlockMessage);
    RUN_TEST(test_PaddingLength_MultipleBlockMessage);
    RUN_TEST(test_PaddingLength_OverflowReturnsZero);
    
    return UNITY_END();
}