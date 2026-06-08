#include "utils.h"

//set least sig bit of a value
void set_lsb(unsigned char *value, int bit) {
    *value = (*value & ~1) | (bit & 1);
}

//return lsb 
int get_lsb(unsigned char value){
    return value & 1;
}

//write a message bit into lsb of blue and green
void write_bit(unsigned char *g, unsigned char *b, int bit) {
    int current_xor = get_lsb(*g) ^ get_lsb(*b);

    if(current_xor != bit) {
        
        //flip lsb of the blue
        *b ^=1; 
    }
}

int read_bit(unsigned char g, unsigned char b) {
    return get_lsb(g) ^ get_lsb(b);

}

