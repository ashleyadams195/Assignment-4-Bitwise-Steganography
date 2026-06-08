
COMPILING AND RUNNING THE PROGRAM
gcc -c main.c -o main.o
gcc -c ppm.c -o ppm.o
gcc -c steg.c -o steg.o
gcc -c utils.c -o utils.o

or

gcc -c main.c ppm.c utils.c steg.c
gcc -o steg main.o ppm.o utils.o steg.o


gcc -o steg main.o ppm.o steg.o utils.o

ENCODING AND DECODING A MESSAGE
1) save your message in payload.bin

2) Encode by running the program
./steg encode input.ppm payload.bin output.ppm

3) Decode the message by running the program
./steg decode output.ppm decoded.bin

4) To view the decoded message
cat decoded.bin











HERE WAS THE ASSIGNEMENT INSTRUCTIONS FOR MY REFERENCE

Homework: Bitwise Steganography in C
Objective
Build a C program that hides and retrieves data inside an image using bitwise operations.

You will implement a simple covert communication technique by encoding data into the least significant bits (LSBs)of a .ppm image.

Background
Steganography is the art of concealing secret information within a non-suspicious file or medium. Unlike cryptography, which focuses on the protection of data through encryption, steganography seeks to hide the existence of the message itself. It can be thought of as a way of "sneaking" a message past potential adversaries, often in formats such as images, audio files, or text. 

You will build a program that encodes and decodes hidden data inside a PPM image by directly manipulating bits.

Historically, steganography has been used in various forms, such as encoding messages on hidden scrolls or shaving the head of a messenger to inscribe a message onto the scalp. In modern times, steganography is often used to hide messages in digital files, such as images or videos.

 

Your program will hide and recover data using least significant bits (LSBs) of pixel values in a .ppm image. You must use bitwise operators (&, |, ^, ~, <<, >>) to:

Insert bits into image data
Extract bits from image data
Reconstruct hidden messages
 

PPM (Portable Pixmap) format.

PPM Image Format
The PPM format is a simple, uncompressed image format. For this assignment, we will use the ASCII version of PPM for color images with 24-bit color depth (RGB). Each pixel is represented by three 8-bit values (one for red, green, and blue). 

Each pixel stores 1 bit of data.

capacity_bits = width × height
capacity_bytes = capacity_bits / 8
Here is the structure of a PPM image:

P3 # Magic number indicating PPM format width height
# Width and height of the image 255
# Maximum color value R G B # RGB values for each pixel (row-major order)
For example, a 3x2 image would look like:

P3 3 2 255 255 0 0 0 255 0 0 0 255 255 255 0 255 255 255 0 0 0
Each RGB component is an integer in the range of 0 to 255.

Your Program
You will write a C program that can encode and decode messages within a PPM image. The encoding will use the least significant bits (LSBs) of the green and blue components of each pixel to hide the bits of the message.

Program Modes
Your program should operate in two modes:

Encode mode: ./steg encode input.ppm payload.bin output.ppm

Decode mode: ./steg decode input.ppm output.bin

Encoding Process
Payload: The payload is the message (binary or plain text) you want to hide within the image. The first 16 bits of the payload will store the size of the message (in bytes). The remaining bits will represent the actual content of the message.

Recommended implementation: Read your payload file as raw bytes, store the payload size (16 bit unsigned integer), then convert all data into a bit stream
Encoding Logic:

For each pixel, the green and blue values will have their least significant bits (LSBs) modified to encode the payload bit by bit.

ENCODE 1 bit per pixel in the image encode bits in MSB-first order.
Example:

'A' = 01000001
Encoded as: 0 1 0 0 0 0 0 1
Specifically, the exclusive-or (XOR) of the LSBs of green and blue will be used to encode a message bit. If the XOR is 1, the bit will be set to 1, otherwise, it will be set to 0.

Extract LSBs:

g_lsb = green & 1
b_lsb = blue & 1
Encode using XOR:

encoded_bit = g_lsb ^ b_lsb
Modify bits using bitwise ops:
Set LSB:

value = (value & ~1) | bit
Toggle LSB:

value ^= 1
You must ensure:

The XOR of green and blue LSBs matches the message bit
 

The image pixels will be modified row by row, and each pixel will hide 1 bit of the message.

Decoding Process
Decoding Logic:

To decode the message, you will reverse the encoding process.

For each pixel:

bit = (green & 1) ^ (blue & 1)
By reading each pixel’s green and blue components, you will extract the LSBs and XOR them to reconstruct each bit of the payload.

byte |= (bit << position);
Message Retrieval:

The first 16 bits represent the size of the message.

After that, the remaining bits will be the actual message data. You need to save this data in the appropriate output file (binary or text).

Error Handling
Your program should handle several possible error conditions:

Duplicate filenames: If any of the filenames provided (input, payload, or output) are the same, print an error message and terminate the program.

Payload too large: If the payload is too large to fit into the image (based on the number of pixels), print an error message.

PPM format validation: Ensure that the input PPM is in the correct format (i.e., "P3", valid width, height, and 255 max color depth). If the format is invalid, print an error message.

Partial decoding: If the encoded size is larger than the available pixels, handle the case by printing an error and possibly producing partial data.

Program Structure
Your program should be divided into logical components across multiple files:

main.c: Handle user input, processing the command line, and calling the encoding or decoding functions.

ppm.c: Functions for reading and writing PPM images.

steg.c: Functions for encoding and decoding the hidden messages.

utils.c: Helper functions (e.g., bit manipulation, error handling).

steg.h: Header file for function prototypes.

ppm.h: Header file for reading and writing PPM files.

utils.h: Header file for utility functions.

Functions
void encode(const char *input_ppm, const char *payload, const char *output_ppm);

void decode(const char *input_ppm, const char *output_file);

void set_lsb(unsigned char *value, int bit);

int get_lsb(unsigned char value);

void write_bit(unsigned char *g, unsigned char *b, int bit);

int read_bit(unsigned char g, unsigned char b);

 

Hints
PPM File Reading/Writing: Use fscanf and fprintf to read and write the PPM file format.

File Operations: Use fread and fwrite to handle reading and writing binary files (for the payload).

Bit Manipulation: You can use bitwise operators (&, |, ^, ~) to manipulate bits in the RGB components of each pixel.

Command-Line Arguments: Parse the command-line arguments to identify whether the user wants to encode or decode the message.

Utility Functions: Consider creating utility functions for tasks such as checking if a file exists, validating input, or performing bit manipulation.
