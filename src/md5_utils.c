#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "md5_utils.h" 

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

    uint64_t bit_len = (uint64_t) original_len * 8; // how many bits do i need do get original msg? 
    
    memcpy(padded_msg + (*final_len - 8), &bit_len, 8); // copy the bit length of original msg to the end of padded_msg
    
    return padded_msg;
}
