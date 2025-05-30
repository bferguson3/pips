/*
*  patch.c
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
    long offset;
    int size;
    u8* bytes;
} patch;

void not_ips(){
    printf("Not an IPS patch file!");
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Usage:\n patch <base-file> <patch-file>.ips\n");
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
    long ips_size = ftell(f);
    fseek(f, 0, SEEK_SET);
    unsigned char *ips = (unsigned char *)malloc(ips_size);
    fread(ips, 1, ips_size, f);
    fclose(f);

    int changes_capacity = 100;
    int changes_size = 0;
    patch* changes = (patch *)malloc(changes_capacity * sizeof(patch));

    int i = 0;
    if(ips[i] != 'P') { not_ips(); return 1; }
    else if(ips[i+1] != 'A') { not_ips(); return 1; }
    else if(ips[i+2] != 'T') { not_ips(); return 1; }
    else if(ips[i+3] != 'C') { not_ips(); return 1; }
    else if(ips[i+4] != 'H') { not_ips(); return 1; }
    
    for(i = 5; i < ips_size; i++) {
        if(ips[i] == 'E')
            if(ips[i+1] == 'O')
                if(ips[i+2] == 'F')
                    break;
        changes[changes_size].offset = (ips[i] << 16) | (ips[i+1] << 8) | (ips[i+2]);
        i += 3;
        changes[changes_size].size = (ips[i]<<8)|ips[i+1];
        i += 2;
        if(changes[changes_size].size == 0){
            // RLE byte 
            int rle_len = (ips[i] << 8) | ips[i + 1];
            changes[changes_size].bytes = (u8*)malloc(sizeof(u8) * rle_len);
            for(int v = 0; v < rle_len; v++){
                changes[changes_size].bytes[v] = ips[i + 2];
            }
            i += 3 + 1; // i-- below
        }
        else{
            changes[changes_size].bytes = (u8*)malloc(sizeof(u8) * changes[changes_size].size);
            for(int j = 0; j < changes[changes_size].size; j++){
                changes[changes_size].bytes[j] = ips[i];
                i++;
            }
        }
        i--; // balance bytes.size iterator
        printf("%lu %d\n", changes[changes_size].offset, changes[changes_size].size);
        changes_size++;
        if(changes_size >= changes_capacity){
            changes_capacity += 100;
            changes = (patch *)realloc(changes, changes_capacity * sizeof(patch));
        }
        
    }
    // now adjust input bytes 
    for(i = 0; i < changes_size; i++){
        printf("a");
        int p = 0;
        for(int j = changes[i].offset; j < changes[i].offset + changes[i].size; j++){
            base[j] = changes[i].bytes[p++];
        }
    }
        
    f = fopen("out.bin", "wb");
    fwrite(base, 1, base_size, f);
    fclose(f);
    printf("out.bin file written.\n");

    free(base);
    free(ips);
    for(int i = 0; i < changes_size; i++){
        free(changes[i].bytes);
    }
    free(changes);

    return 0;
}
