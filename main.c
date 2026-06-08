#include "steg.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    if (argc != 4 && argc !=5) {
        fprintf(stderr, "Usage:\n");
        fprintf(stderr, "  Encode: %s encode input.ppm payload.bin output.ppm\n", argv[0]);
        fprintf(stderr, "  Decode: %s decode input.ppm output.bin\n", argv[0]);
        return 1;
    }

    //check duplicate
    if (argc ==5) {
        if (strcmp(argv[2], argv[3]) == 0 || strcmp(argv[2], argv[4]) == 0 || strcmp(argv[3], argv[4]) == 0) {
            fprintf(stderr, "Error: Duplicate file names are not allowed.\n ");
            return 1;
        }
    } else {
        if (strcmp(argv[2], argv[3]) == 0) {
            fprintf(stderr, "Error: Duplicate file names are not allowed.\n");
            return 1;
            
        }
    }
    if (strcmp(argv[1], "encode") == 0 && argc ==5) {
        encode(argv[2], argv[3], argv[4]);
    }
    else if (strcmp(argv[1], "decode") ==0 && argc ==4) {
        decode(argv[2], argv[3]);
    }
    else {
        fprintf(stderr, "The command was invalid or contained the wrong number of arguments.\n");
        return 1;
    }
    return 0;
}
