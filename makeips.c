/*
*  makeips.c
*
*  (c)2025 bferguson3
*     @retrodevdiscord
*
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define u8 unsigned char

typedef struct _patch { 
    u8 offset[3];
    u8 size[2];
    u8* bytes;
} patch;

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n makeips <base-file> <compared-file>\n");
        return 1; // Not enough arguments
    }

    FILE *f;
    f = fopen(argv[1], "rb");
    if (!f) return 1; // Error opening file
    fseek(f, 0, SEEK_END);
    long base_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *base = (unsigned char *)malloc(base_size);
    fread(base, 1, base_size, f);
    fclose(f);

    f = fopen(argv[2], "rb");
    if (!f) {
        free(base);
        return 1; // Error opening file
    }
    fseek(f, 0, SEEK_END);
    long cmp_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *cmp = (unsigned char *)malloc(cmp_size);
    fread(cmp, 1, cmp_size, f);
    fclose(f);

    int changes_capacity = 100;
    int changes_size = 0;
    patch* changes = (patch *)malloc(changes_capacity * sizeof(patch));

    int i = 0;
    if (base_size <= cmp_size) { // file 2 is bigger
        while (i < base_size) {
            if (base[i] != cmp[i]) {
                int ofs = i;
                int o_size = 0;
                while ((base[i] != cmp[i]) && (i < cmp_size) && (o_size < 0xffff)) {
                    o_size++;
                    i++;
                }
                if (changes_size >= changes_capacity) {
                    changes_capacity += 100;
                    changes = (patch*)realloc(changes, changes_capacity * sizeof(patch));
                }
                changes[changes_size].offset[0] = (ofs & 0xff0000) >> 16;
                changes[changes_size].offset[1] = (ofs & 0xff00) >> 8;
                changes[changes_size].offset[2] = (ofs & 0xff);
                changes[changes_size].size[0] = (o_size & 0xff00) >> 8;
                changes[changes_size].size[1] = (o_size & 0xff);
                changes[changes_size].bytes = (u8*)malloc(o_size * sizeof(u8));
                for(int b = 0; b < o_size; b++){
                    changes[changes_size].bytes[b] = cmp[i - o_size + b]; // hmm
                }
                changes_size++;
            }
            i++;
        }
        
        if(base_size < cmp_size){
            int add_size = 0;
            int ro = i;
            while (i < cmp_size) {
                add_size++;
                i++;
            }
            if (changes_size >= changes_capacity) {
                changes_capacity *= 2;
                changes = (patch*)realloc(changes, changes_capacity * sizeof(patch));
            }
            changes[changes_size].offset[0] = (ro & 0xff0000) >> 16; // Offset
            changes[changes_size].offset[1] = (ro & 0xff00) >> 8;
            changes[changes_size].offset[2] = (ro & 0xff);
            changes[changes_size].size[0] = (add_size & 0xff00) >> 8; // Length
            changes[changes_size].size[1] = (add_size & 0xff);
            changes[changes_size].bytes = (u8*)malloc(add_size * sizeof(u8));
            for(int b = 0; b < add_size; b++){
                changes[changes_size].bytes[b] = cmp[i - add_size + b]; // hmm
            }
            changes_size++;
        }
        
    } 
    else { // file 1 is bigger
        printf("File size mismatch: Ensure file #2 is the larger.");
        return 2;
    }
    
    // length will be 5 + (len of changes * 5) + len of each byte arr 
    changes = (patch *)realloc(changes, changes_size * sizeof(patch));
    int tot = 5 + (5 * changes_size);
    
    //printf("%d, %d\n", changes_size, tot);
    for(int p = 0; p < changes_size; p++) {
        int len = (changes[p].size[0] << 8) | (changes[p].size[1]);
        tot += len;
    }
    
    unsigned char * ips = (u8*)malloc(sizeof(u8)*tot);
    int ips_size = 0;

    printf("Creating %d patches...\n", changes_size);

    ips[ips_size++] = 'P';
    ips[ips_size++] = 'A';
    ips[ips_size++] = 'T';
    ips[ips_size++] = 'C';
    ips[ips_size++] = 'H';

    for (i = 0; i < changes_size; i++) {
        ips[ips_size++] = (u8)(changes[i].offset[0]);
        ips[ips_size++] = (u8)(changes[i].offset[1]);
        ips[ips_size++] = (u8)(changes[i].offset[2]);
        int len = (changes[i].size[0] << 8) | (changes[i].size[1]);
        ips[ips_size++] = (u8)(changes[i].size[0]);
        ips[ips_size++] = (u8)(changes[i].size[1]);
        if(len == 0) {
            // RLE 
            ips[ips_size++] = (u8)(changes[i].bytes[0]);
            ips[ips_size++] = (u8)(changes[i].bytes[1]);
            int rle_len = (changes[i].bytes[0] << 8) | (changes[i].bytes[1]);
            for(int j = 0; j < rle_len; j++) ips[ips_size++] = (u8)(changes[i].bytes[2]);
        }
        else {
            for(int j = 0; j < len; j++) {
                ips[ips_size++] = (u8)changes[i].bytes[j];
            }
        }
    }

    ips[ips_size++] = 'E';
    ips[ips_size++] = 'O';
    ips[ips_size++] = 'F';

    f = fopen("out.ips", "wb");
    fwrite(ips, 1, ips_size, f);
    fclose(f);
    printf("out.ips IPS file written OK.\n");

    free(base);
    free(cmp);
    for(int i = 0; i < changes_size; i++){
        free(changes[i].bytes);
    }
    free(changes);

    return 0;
}
