#ifndef MD5_UTILS_H
#define MD5_UTILS_H

#include <stddef.h>
#include <stdint.h>


size_t calculate_padded_msg_length(size_t original_len);

unsigned char* md5_pad_msg(const unsigned char*, size_t original_len, size_t* final_len);

void md5_append_length(unsigned char* padded_msg, size_t original_len, size_t* total_padded_len);

uint32_t F_function(uint32_t x, uint32_t y, uint32_t z);
uint32_t G_function(uint32_t x, uint32_t y, uint32_t z);
uint32_t H_function(uint32_t x, uint32_t y, uint32_t z);
uint32_t I_function(uint32_t x, uint32_t y, uint32_t z);


#endif