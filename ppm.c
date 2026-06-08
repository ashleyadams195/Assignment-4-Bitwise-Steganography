#include "ppm.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//read ppm file
PPM_Image* read_ppm(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        perror("There was an error opening the PPM file.\n");
        return NULL;
    }

    char magic[3];
    int width, height, maxval;
    
    //read headder
    if (fscanf(fp, "%2s", magic) != 1 || strcmp(magic, "P3") !=0) {
        fprintf(stderr, "The format is invalid. Please save as a P3 file.\n");
        fclose(fp);
        return NULL;
    }

    fscanf(fp, "%d %d %d", &width, &height, &maxval);
    if (width <= 0 || height <= 0 || maxval != 255) {
        fprintf(stderr, "The dimensions or max vlue is invalid.\n");
        fclose(fp);
        return NULL;
    }

    PPM_Image *img = malloc(sizeof(PPM_Image));
    img-> width = width;
    img-> height = height;
    img-> data = malloc(width * height * 3);


    for (int i =0; i < width * height * 3; i++) {
        int val;
        if(fscanf(fp, "%d", &val) != 1 || val <0 || val >255) {
            fprintf(stderr, "There was an error reading the pixel data.\n");
            free_ppm(img);
            fclose(fp);
            return NULL;
        }
        img -> data[i] = (unsigned char)val;
    }
    fclose(fp);
    return img;

}


//write the ppm 3 file

int write_ppm(const char *filename, PPM_Image *img) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        perror("There was an error creating the PPM file.\n");
        return 0;
    }
    fprintf(fp, "P3\n%d %d\n255\n", img->width, img->height);
    for (int i=0; i <img->width * img->height * 3; i++) {
        fprintf(fp, "%d ", img-> data[i]);
        if ((i+1) % 15 == 0) fprintf(fp, "\n");
    }
    fprintf(fp, "\n");
    fclose(fp);
    return 1;

}

void free_ppm(PPM_Image *img) {
    if (img) {
        free(img->data);
        free(img);
    }
}
