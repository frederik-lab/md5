#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "md5_utils.h"

int main(int argc, char **argv) {
    if (argc < 2) {
        fprintf(stderr, "Usage is: %s <message>\n"
                        "          %s -f <file>\n", argv[0], argv[0]);
        return 1;
    }

    int is_file = (strcmp(argv[1], "-f") == 0);
    unsigned char* msg = NULL;
    size_t original_msg_length = 0;

    if (is_file) {
        if (argc < 3) {
            fprintf(stderr, "ERROR: -f requires a filename\n");
            return 1;
        }
        FILE* f = fopen(argv[2], "rb");
        if (!f) {
            fprintf(stderr, "ERROR: could not open '%s'\n", argv[2]);
            return 1;
        }
        fseek(f, 0, SEEK_END);
        original_msg_length = ftell(f);
        rewind(f);
        msg = malloc(original_msg_length);
        size_t bytes_read = fread(msg, 1, original_msg_length, f);
        if (bytes_read != original_msg_length) {
            fprintf(stderr, "ERROR: could not read file\n");
            free(msg);
            fclose(f);
            return 1;
        }        
        fclose(f);
    } else {
        if (strlen(argv[1]) < 1) {
            fprintf(stderr, "ERROR: empty message\n");
            return 1;
        }
        msg = (unsigned char*)argv[1];
        original_msg_length = strlen(argv[1]);
    }

    size_t padded_msg_length = 0;
    uint32_t result_blocks[4];
    unsigned char* padded_msg = md5_pad_msg(msg, original_msg_length, &padded_msg_length);
    md5_append_length(padded_msg, original_msg_length, &padded_msg_length);
    md5_process_msg_blocks(padded_msg, result_blocks, padded_msg_length);
    md5_print_digest(result_blocks);
    printf("\n");

    if (is_file) free(msg);
    free(padded_msg);
    return 0;
}