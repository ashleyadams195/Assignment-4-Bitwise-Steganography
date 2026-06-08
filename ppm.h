#ifndef PPM_H
#define PPM_H

typedef struct {
    int width;
    int height;
    unsigned char *data;
} PPM_Image;

PPM_Image* read_ppm(const char *filename);
int write_ppm(const char *filename, PPM_Image *img);
void free_ppm(PPM_Image *img);

#endif
