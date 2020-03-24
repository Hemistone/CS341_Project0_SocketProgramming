#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>
#include <stdio.h>
#define MAXSIZE 10000000

struct message {
    uint8_t op;
    uint8_t shift;
    uint16_t chksum;
    uint32_t length;
    char data[MAXSIZE - 16];
};

uint16_t ip_checksum(char* vdata, size_t length, int mode) {
    //Initialise the accumulator.
    uint32_t acc = 0x0000;

    // Handle complete 16-bit blocks.
    for (size_t i = 0; i + 1 < length; i += 2) {
        uint16_t word;
        memcpy(&word, vdata + i, 2);
        acc += word;
        if (acc > 0xffff) {
            acc -= 0xffff;
        }
    }

    // Handle any partial block at the end of the data.
    if (length & 1) {
        uint16_t word = 0;
        memcpy(&word, vdata + length - 1, 1);
        acc += word;
        if (acc > 0xffff) {
            acc -= 0xffff;
        }
    }

    if (mode == 0)
        return (uint16_t)~acc;
    else
        return (uint16_t)acc;
};

#endif