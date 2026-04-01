#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5_utils.h"


void print_buffer_hex(const unsigned char* buffer, size_t len) {
    for (size_t i = 0; i < len; i++) {
        printf("%02x ", buffer[i]);
        // Add a newline every 16 bytes for readability
        if ((i + 1) % 16 == 0) printf("\n");
    }
    printf("\n");
}

int main() {
    const char* msg_str = "testmsg";
    size_t original_msg_length = strlen(msg_str); 

    unsigned char* msg = (unsigned char*)msg_str;
    size_t padded_msg_length = 0; //intial length set to zero since we dont know how long yet.


    // ############################################# bit padding msg
    printf("original length: %zu \n", original_msg_length); 
    print_buffer_hex(msg, original_msg_length);

    unsigned char* padded_msg = md5_pad_msg(msg, original_msg_length, &padded_msg_length); // we want to overwrite padded length, therefore we give the ADDR of padded_length
    
    printf("padded length: %zu\n", padded_msg_length);
    print_buffer_hex(padded_msg, padded_msg_length);
    // ############################################# 


    // ############################################# appending len to msg
    print_buffer_hex(msg, original_msg_length);
    md5_append_length(padded_msg, original_msg_length, &padded_msg_length);

    print_buffer_hex(padded_msg, padded_msg_length);
    //print_buffer_hex(msg_with_len, padded_msg_length);
    //print_buffer_hex(padded_msg, padded_msg_length);
    // ############################################# 


    free(padded_msg);
    return 0;
}