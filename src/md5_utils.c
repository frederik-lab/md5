
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "md5_utils.h" 

static const unsigned char A[4] = {0x01, 0x23, 0x45, 0x67};
static const unsigned char B[4] = {0x89, 0xab, 0xcd, 0xef};
static const unsigned char C[4] = {0xfe, 0xdc, 0xab, 0x98};
static const unsigned char D[4] = {0x76, 0x54, 0x32, 0x01};

static const uint32_t T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be, 0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8, 0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c, 0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665, 0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1, 0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391
};

size_t calculate_padded_msg_length(size_t original_len) {
    size_t min_total_length = original_len + 1 + 8; 
    size_t final_total_length = 0;

    if (original_len > (SIZE_MAX - 9)) {
        return 0; // ERROR
    } 

    final_total_length = ((min_total_length + 63) / 64) * 64;
    
    return final_total_length;
}

unsigned char* md5_pad_msg(const unsigned char* msg, size_t original_len, size_t* final_len) {
    *final_len = calculate_padded_msg_length(original_len);

    unsigned char* padded_msg = calloc(*final_len, sizeof(unsigned char)); // sets padded_msg to zeros, in the entire length of all the blocks for the message + padding
    if (!padded_msg) return NULL;

    memcpy(padded_msg, msg, original_len);

    padded_msg[original_len] = 0x80; // sets the final bit after msg to 1

    return padded_msg;
}

void md5_append_length(unsigned char* padded_msg, size_t original_len, size_t* total_padded_len) {\
    uint64_t len_64_bit = (uint64_t)original_len * 8; 
    unsigned char* padded_msg_end_ptr = padded_msg + (*total_padded_len - 8);

    padded_msg_end_ptr[0] = (unsigned char)(len_64_bit & 0xFF);
    padded_msg_end_ptr[1] = (unsigned char)((len_64_bit >> 8) & 0xFF);
    padded_msg_end_ptr[2] = (unsigned char)((len_64_bit >> 16) & 0xFF);
    padded_msg_end_ptr[3] = (unsigned char)((len_64_bit >> 24) & 0xFF);
    padded_msg_end_ptr[4] = (unsigned char)((len_64_bit >> 32) & 0xFF);
    padded_msg_end_ptr[5] = (unsigned char)((len_64_bit >> 40) & 0xFF);
    padded_msg_end_ptr[6] = (unsigned char)((len_64_bit >> 48) & 0xFF);
    padded_msg_end_ptr[7] = (unsigned char)((len_64_bit >> 56) & 0xFF);

}

uint32_t F_function(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & y) | ((~x) & z));
}

uint32_t G_function(uint32_t x, uint32_t y, uint32_t z) {
    return ((x & z) | (y & (~x)));
}

uint32_t H_function(uint32_t x, uint32_t y, uint32_t z) {
    return (x ^ y ^ z);
}

uint32_t I_function(uint32_t x, uint32_t y, uint32_t z) {
    return (y ^ (x | (~z)));
}


