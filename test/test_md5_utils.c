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

void test_md5_append_length(void) {
    const char* input = "abc";
    size_t original_len = strlen(input);
    size_t final_len = 0;
    const unsigned char true_arr[64] = {0x61, 0x62, 0x63, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    unsigned char* input_for_append = md5_pad_msg((const unsigned char*)input, original_len, &final_len);
    md5_append_length(input_for_append, original_len, &final_len); 
    
    TEST_ASSERT_NOT_NULL(input_for_append);
    TEST_ASSERT_EQUAL_HEX8_ARRAY(true_arr, input_for_append, 64);

    free(input_for_append);
}

/*
void test_rounds(void) {
    TEST_ASSERT_EQUAL_INT(round1());
    TEST_ASSERT_EQUAL_INT(round1());
    TEST_ASSERT_EQUAL_INT(round1());
    TEST_ASSERT_EQUAL_INT(round1());
}
*/
int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_calculate_padded_msg_length);
    RUN_TEST(test_md5_pad_msg);
    RUN_TEST(test_md5_append_length);
    
    return UNITY_END();
}