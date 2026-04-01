#ifndef MD5_UTILS_H
#define MD5_UTILS_H

#include <stddef.h>

size_t calculate_padded_msg_length(size_t original_len);

unsigned char* md5_pad_msg(const unsigned char*, size_t original_len, size_t* final_len);

void md5_append_length(unsigned char* padded_msg, size_t original_len, size_t* total_padded_len);

#endif