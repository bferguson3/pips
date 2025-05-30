/*
*  chksum.c
*
*  (c)2025 bferguson3
*     @retrodevdiscord
*
*  THIS DOES NOT IGNORE THE ROM HEADER YET
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argn, char** argv)
{
    if (argn < 2) {
        printf("Usage:\n chksum <file>\n");
        return 1; // Not enough arguments
    }

    FILE *f;
    f = fopen(argv[1], "rb");
    if (!f) return 1; // Error opening file
    fseek(f, 0, SEEK_END);
    long f_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *base = (unsigned char *)malloc(f_size);
    fread(base, 1, f_size, f);
    fclose(f);

    int sum = 0x0000;
    for(long i = 0; i < f_size; i++){
        sum += base[i];
        if(sum > 0xffff)
            sum %= 0x10000;
    }

    printf("Checksum: 0x%x (%d)\n", sum, sum);
    return 0;
}