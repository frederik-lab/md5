
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "md5_utils.h" 
#include "md5_constants.h"

size_t calculate_padded_msg_length(const size_t original_len) {
    size_t min_total_length = original_len + 1 + 8; 
    size_t final_total_length = 0;

    if (original_len > (SIZE_MAX - 9)) {
        return 0; // ERROR
    } 

    final_total_length = ((min_total_length + 63) / 64) * 64;
    
    return final_total_length;
}

unsigned char* md5_pad_msg(const unsigned char* msg, const size_t original_len, size_t* final_len) {
    *final_len = calculate_padded_msg_length(original_len);

    unsigned char* padded_msg = calloc(*final_len, sizeof(unsigned char)); // sets padded_msg to zeros, in the entire length of all the blocks for the message + padding
    if (!padded_msg) return NULL;

    memcpy(padded_msg, msg, original_len);

    padded_msg[original_len] = 0x80; // sets the final bit after msg to 1
    return padded_msg;
}

void md5_append_length(unsigned char* padded_msg, const size_t original_len, size_t* total_padded_len) {\
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

uint32_t F_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return ((x & y) | ((~x) & z));
}

uint32_t G_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return ((x & z) | (y & (~x)));
}

uint32_t H_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return (x ^ y ^ z);
}

uint32_t I_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return (y ^ (x | (~z)));
}

uint32_t round1(unsigned char *X, uint32_t* T, unsigned char a, unsigned char b, unsigned char c, unsigned char d, int k, int s, int i) {
    uint32_t result = b + ((a + F_function(b,c,d) + X[k] + T[i]) << s);
    return result;
}

uint32_t round2(unsigned char *X, uint32_t* T, unsigned char a, unsigned char b, unsigned char c, unsigned char d, int k, int s, int i) {
    uint32_t result = b + ((a + G_function(b,c,d) + X[k] + T[i]) << s);
    return result;
}

uint32_t round3(unsigned char *X, uint32_t* T, unsigned char a, unsigned char b, unsigned char c, unsigned char d, int k, int s, int i) {
    uint32_t result = b + ((a + H_function(b,c,d) + X[k] + T[i]) << s);
    return result;
}

uint32_t round4(unsigned char *X, uint32_t* T, unsigned char a, unsigned char b, unsigned char c, unsigned char d, int k, int s, int i) {
    uint32_t result = b + ((a + I_function(b,c,d) + X[k] + T[i]) << s);
    return result;
}
/*
unsigned char* process_msg_blocks(const unsigned char* padded_msg, const size_t msg_len) {
    for(uint32_t i=0; i<=((msg_len / 16)-1); i++) {
        for(uint32_t j=0; j<=15; j++) {
            X[j] = padded_msg[i*16+j]; // copy ith block into X
        }
        unsigned char AA = A;
        unsigned char BB = B;
        unsigned char CC = C;
        unsigned char DD = D;

    }
}
*/