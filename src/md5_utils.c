
#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include "md5_utils.h" 
#include "md5_constants.h"

#define R1(a, b, c, d, k, s, i) round1(X, T, &a, b, c, d, k, s, i-1)
#define R2(a, b, c, d, k, s, i) round2(X, T, &a, b, c, d, k, s, i-1)
#define R3(a, b, c, d, k, s, i) round3(X, T, &a, b, c, d, k, s, i-1)
#define R4(a, b, c, d, k, s, i) round4(X, T, &a, b, c, d, k, s, i-1)

uint32_t left_rotate(uint32_t x, uint32_t s) {
    return (x << s) | (x >> (32 - s));
}

void print_buffer_hex(const unsigned char* buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", buffer[i]);
        // Add a newline every 16 bytes for readability
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

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
    return ((x & z) | (y & (~z)));
}

uint32_t H_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return (x ^ y ^ z);
}

uint32_t I_function(const uint32_t x, const uint32_t y, const uint32_t z) {
    return (y ^ (x | (~z)));
}

void round1(uint32_t *X, const uint32_t* T, uint32_t* a, uint32_t b, uint32_t c, uint32_t d, int k, int s, int i) {
    *a = b + left_rotate((*a + F_function(b,c,d) + X[k] + T[i]), s);
}

void round2(uint32_t *X, const uint32_t* T, uint32_t* a, uint32_t b, uint32_t c, uint32_t d, int k, int s, int i) {
    *a = b + left_rotate((*a + G_function(b,c,d) + X[k] + T[i]), s);
}

void round3(uint32_t *X, const uint32_t* T, uint32_t* a, uint32_t b, uint32_t c, uint32_t d, int k, int s, int i) {
    *a = b + left_rotate((*a + H_function(b,c,d) + X[k] + T[i]), s);
}

void round4(uint32_t *X, const uint32_t* T, uint32_t* a, uint32_t b, uint32_t c, uint32_t d, int k, int s, int i){
    *a = b + left_rotate((*a + I_function(b,c,d) + X[k] + T[i]), s);
}

void md5_process_msg_blocks(const unsigned char* padded_msg, uint32_t* block_arr , const size_t msg_len) {
    uint32_t A = INITIAL_A;
    uint32_t B = INITIAL_B;
    uint32_t C = INITIAL_C;
    uint32_t D = INITIAL_D;
    
    for(uint32_t i=0; i<=((msg_len / 64)-1); i++) {
        
        uint32_t X[16] = {0};
        for(uint32_t j=0; j<=15; j++) {
            X[j] = ((uint32_t)padded_msg[i*64 + j*4 + 0]) |
                    ((uint32_t)padded_msg[i*64 + j*4 + 1] << 8) |
                    ((uint32_t)padded_msg[i*64 + j*4 + 2] << 16)  |
                    (uint32_t)padded_msg[i*64 + j*4 + 3] << 24;
        }
        uint32_t AA = A;
        uint32_t BB = B;
        uint32_t CC = C;
        uint32_t DD = D;

        // round1
        R1(A, B, C, D, 0, 7, 1);
        R1(D, A, B, C, 1, 12, 2);
        R1(C, D, A, B, 2, 17, 3);
        R1(B, C, D, A, 3, 22, 4);

        R1(A, B, C, D, 4, 7, 5);
        R1(D, A, B, C, 5, 12, 6);
        R1(C, D, A, B, 6, 17, 7);
        R1(B, C, D, A, 7, 22, 8);

        R1(A, B, C, D, 8, 7, 9);
        R1(D, A, B, C, 9, 12, 10);
        R1(C, D, A, B, 10, 17, 11);
        R1(B, C, D, A, 11, 22, 12);

        R1(A, B, C, D, 12, 7, 13);
        R1(D, A, B, C, 13, 12, 14);
        R1(C, D, A, B, 14, 17, 15);
        R1(B, C, D, A, 15, 22, 16);

        // round2 
        R2(A, B, C, D, 1, 5, 17);
        R2(D, A, B, C, 6, 9, 18);
        R2(C, D, A, B, 11, 14, 19);
        R2(B, C, D, A, 0, 20, 20);

        R2(A, B, C, D, 5, 5, 21);
        R2(D, A, B, C, 10, 9, 22);
        R2(C, D, A, B, 15, 14, 23);
        R2(B, C, D, A, 4, 20, 24);

        R2(A, B, C, D, 9, 5, 25);
        R2(D, A, B, C, 14, 9, 26);
        R2(C, D, A, B, 3, 14, 27);
        R2(B, C, D, A, 8, 20, 28);

        R2(A, B, C, D, 13, 5, 29);
        R2(D, A, B, C, 2, 9, 30);
        R2(C, D, A, B, 7, 14, 31);
        R2(B, C, D, A, 12, 20, 32);

        // round3
        R3(A, B, C, D, 5, 4, 33);
        R3(D, A, B, C, 8, 11, 34);
        R3(C, D, A, B, 11, 16, 35);
        R3(B, C, D, A, 14, 23, 36);

        R3(A, B, C, D, 1, 4, 37);
        R3(D, A, B, C, 4, 11, 38);
        R3(C, D, A, B, 7, 16, 39);
        R3(B, C, D, A, 10, 23, 40);

        R3(A, B, C, D, 13, 4, 41);
        R3(D, A, B, C, 0, 11, 42);
        R3(C, D, A, B, 3, 16, 43);
        R3(B, C, D, A, 6, 23, 44);

        R3(A, B, C, D, 9, 4, 45);
        R3(D, A, B, C, 12, 11, 46);
        R3(C, D, A, B, 15, 16, 47);
        R3(B, C, D, A, 2, 23, 48);

        // round4
        R4(A, B, C, D, 0, 6, 49);
        R4(D, A, B, C, 7, 10, 50);
        R4(C, D, A, B, 14, 15, 51);
        R4(B, C, D, A, 5, 21, 52);

        R4(A, B, C, D, 12, 6, 53);
        R4(D, A, B, C, 3, 10, 54);
        R4(C, D, A, B, 10, 15, 55);
        R4(B, C, D, A, 1, 21, 56);

        R4(A, B, C, D, 8, 6, 57);
        R4(D, A, B, C, 15, 10, 58);
        R4(C, D, A, B, 6, 15, 59);
        R4(B, C, D, A, 13, 21, 60);

        R4(A, B, C, D, 4, 6, 61);
        R4(D, A, B, C, 11, 10, 62);
        R4(C, D, A, B, 2, 15, 63);
        R4(B, C, D, A, 9, 21, 64);

        A = A + AA;
        B = B + BB;
        C = C + CC;
        D = D + DD;
    }
    
    block_arr[0] = A;
    block_arr[1] = B;
    block_arr[2] = C;
    block_arr[3] = D;
}

void md5_print_digest(uint32_t* result_blocks) {
    for (int i = 0; i < 4; i++) {
        printf("%02x", (result_blocks[i])       & 0xFF);
        printf("%02x", (result_blocks[i] >> 8)  & 0xFF);
        printf("%02x", (result_blocks[i] >> 16) & 0xFF);
        printf("%02x", (result_blocks[i] >> 24) & 0xFF);
    }
    printf("\n");
}