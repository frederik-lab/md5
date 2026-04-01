#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5_utils.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage is: %s <message>\n", argv[0]);
        return 1;
    }
    const char* msg_str = argv[1];
    
    if (strlen(msg_str) < 1) {
        fprintf(stderr, "ERROR: empty message\n");
        return 1;
    }
    size_t original_msg_length = strlen(msg_str); 

    unsigned char* msg = (unsigned char*)msg_str;
    size_t padded_msg_length = 0; //intial length set to zero since we dont know how long yet.

    uint32_t result_blocks[4]; 

    unsigned char* padded_msg = md5_pad_msg(msg, original_msg_length, &padded_msg_length); // we want to overwrite padded length, therefore we give the ADDR of padded_length

    md5_append_length(padded_msg, original_msg_length, &padded_msg_length);
    
    md5_process_msg_blocks(padded_msg, result_blocks, padded_msg_length);
    
    md5_print_digest(result_blocks);
    
    free(padded_msg);
    return 0;
}