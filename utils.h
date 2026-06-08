#ifndef UTILS_H
#define UTILS_H

void set_lsb(unsigned char *value, int bit);
int get_lsb(unsigned char value);
void write_bit(unsigned char *g, unsigned char *b, int bit);
int read_bit(unsigned char g, unsigned char b);

#endif
