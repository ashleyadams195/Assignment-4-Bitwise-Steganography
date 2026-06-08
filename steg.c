#include "steg.h"
#include "utils.h"
#include <stdio.h>
#include <stdlib.h>

static void write_size_bits(PPM_Image *img, unsigned long long size, int *bit_index) {
    printf("Debug: Writing size %llu\n", size);
    for (int i = 63; i >= 0; i--) {
        int bit = (size >> i) & 1;
        int pixel_idx = (*bit_index)++;
        unsigned char *g = &img->data[pixel_idx * 3 + 1];
        unsigned char *b = &img->data[pixel_idx * 3 + 2];
        
        int old_g = (*g) & 1;
        int old_b = (*b) & 1;
        
        write_bit(g, b, bit);
        
        int new_g = (*g) & 1;
        int new_b = (*b) & 1;
        
        if (i >= 60 || i <= 5) {  // print only high and low bits
            printf("  Bit %2d = %d | old(g,b)=(%d,%d) -> new(g,b)=(%d,%d)\n", 
                   i, bit, old_g, old_b, new_g, new_b);
        }
    }
}

static void write_payload_bits(PPM_Image *img, unsigned char *payload, long payload_size, int *bit_index) {
    for (long i=0; i < payload_size; i++) {
        for (int j=7; j>=0; j--) {
            int bit = (payload[i]>>j)&1;
            int pixel_idx = (*bit_index)++;
            unsigned char *g = &img->data[pixel_idx *3 +1];
            unsigned char *b = &img->data[pixel_idx *3 +2];
            write_bit(g,b,bit);
        }
    }
}

// encode the payload into the ppm image
void encode(const char *input_ppm, const char *payload_file, const char *output_ppm) {
    PPM_Image *img = read_ppm(input_ppm);
    if (!img) return;

    //read the payload
    FILE *pf = fopen(payload_file, "rb");
    if (!pf) {
        fprintf(stderr, "There was an error opening the payload file.\n");
        free_ppm(img);
        return;
    }

    fseek(pf, 0, SEEK_END);
    long payload_size = ftell(pf);
    fseek(pf, 0, SEEK_SET);

    if (payload_size > (img->width * img-> height -64LL)/8){
        fprintf(stderr, "The payload is too large for the image.\n");
        fclose(pf);
        free_ppm(img);
        return;
    }

    unsigned char *payload = malloc(payload_size);
    if (!payload) {
        fclose(pf);
        free_ppm(img);
        return;
    } 
    
    fread(payload, 1, payload_size, pf);
    fclose(pf);

    int bit_index = 0;
    write_size_bits(img, (unsigned int) payload_size, &bit_index);
    write_payload_bits(img, payload, payload_size, &bit_index);
    
    write_ppm(output_ppm, img);

    free(payload);
    free_ppm(img);
    printf("Image encoding completed,\n");
}


// decode

void decode(const char *input_ppm, const char *output_file) {
    PPM_Image *img = read_ppm(input_ppm);
    if(!img) return;

    int bit_index = 0;
    unsigned long long size = 0;
    
    for (int i=0; i <64;i++) {
        int pixel_idx = bit_index++;
        if (pixel_idx >= img->width *img->height) {
            fprintf(stderr, "The image is too small for the header.\n");
            free_ppm(img);
            return;
        }
        unsigned char g = img->data[pixel_idx *3 +1];
        unsigned char b = img->data[pixel_idx *3 +2];
        int bit = read_bit(g,b);
        size = (size << 1) | bit;

    }

    printf("Debug: Decoded size = %llu bytes\n", size);


    if (size ==0 || size > (unsigned long long) (img->width * img->height -64)/8) {
        fprintf(stderr, "The decoded size to too big.\n");
        free_ppm(img);
        return;
    }

    FILE *out = fopen(output_file, "wb");
    if (!out) {
        fprintf(stderr, "There was an error creating the output file.\n");
        free_ppm(img);
        return;
    }

    for(unsigned int i=0; i < size; i++) {
        unsigned char byte = 0;
        for (int j=0; j<8; j++) {

            int pixel_idx = bit_index++;
            unsigned char g = img->data[pixel_idx *3 +1];
            unsigned char b = img->data[pixel_idx *3 +2];
            int bit = read_bit(g,b);
            byte = (byte << 1) | bit;
        }
        fwrite(&byte, 1, 1, out);
    }
    fclose(out);
    free_ppm(img);
    printf("Decoding completed. The message was saved to file path '%s'\n", output_file);

}

