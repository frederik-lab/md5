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

void test_calculate_padded_msg_length(void) {
    TEST_ASSERT_EQUAL_INT(64, calculate_padded_msg_length(7));
    TEST_ASSERT_EQUAL_INT(64, calculate_padded_msg_length(55));
    TEST_ASSERT_EQUAL_INT(128, calculate_padded_msg_length(56));
    TEST_ASSERT_EQUAL_INT(0, calculate_padded_msg_length((SIZE_MAX-8)));
}

void test_md5_pad_msg(void) {\
    const char* input = "abc";
    size_t original_len = strlen(input);
    size_t final_len = 0;

    unsigned char* result = md5_pad_msg((const unsigned char*)input, original_len, &final_len);
    
    TEST_ASSERT_NOT_NULL(result);
    TEST_ASSERT_EQUAL_INT(64, final_len);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(input, result, original_len);
    TEST_ASSERT_EQUAL_HEX8(0x80, result[original_len]);

    for (size_t i = original_len + 1; i < final_len - 8; i++) {
        TEST_ASSERT_EQUAL_HEX8(0x00, result[i]);
    }

    free(result);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_calculate_padded_msg_length);
    RUN_TEST(test_md5_pad_msg);
    
    return UNITY_END();
}