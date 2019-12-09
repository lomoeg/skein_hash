#include <stdint.h>
#include "threefish.h"
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#define INT_BITS 64

#if 0 /* TODO change to 0 after you complete this task */
/*

TODO FREE TEXT ASSIGNMENT

If there was a FAQ for this assignment, what question and answer would've helped you the most?

Q: How you can compile the program?

A: You also should include threefish_driver.c in clang command. clang threefish_driver.c threefish.c -o program

*/
#warning "FAIL you have not completed the free text assignment"
#endif

uint64_t c_words[4];

void divide_block(uint64_t arr[], const unsigned char *l, int parts) {
    int i;
    for (i = 0; i < parts; i++) {
        unsigned char tmp[8];
        int j;
        for (j = i*8; j <= i*8+7; j++) {
            tmp[j % 8] = l[j];
        }
        memcpy(&arr[i], tmp, sizeof(tmp));
    }
}

int R_const[8][2] = {
    {14, 16},
    {52, 57},
    {23, 40},
    {5, 37},
    {25, 33},
    {46, 12},
    {58, 22},
    {32, 32},
};


void mix_func(const uint64_t x0, const uint64_t x1, uint64_t y0, uint64_t y1, int d, int j) {
    y0 = x0 + x1;
    // works fine
    y1 = ((x1 << R_const[d % 8][j]) | (x1 >> (INT_BITS - R_const[d % 8][j]) ) ) ^ y0;
    c_words[2*j] = y0; c_words[2*j + 1] = y1;
}

/* Under key at k and tweak at t, encrypt 32 bytes of plaintext at p and store it at c. */
void threefish(unsigned char *c, const unsigned char *p, const unsigned char *k, const unsigned char *t) {
    printf("=== BEGIN THREEFISH CALL === \n");
    // split key and tweaks to words
    uint64_t key_words[5];
    uint64_t tweak_words[3];
    uint64_t subkeys[4];

    divide_block(key_words, k, 4);
    divide_block(tweak_words, t, 2);
    
    // generate additional words t2 and kNw
    const uint64_t c_key = 0x1BD11BDAA9FC1A22;
    tweak_words[2] = tweak_words[0] ^ tweak_words[1];   // t2
    key_words[4] = c_key ^ key_words[0] ^ key_words[1] ^ key_words[2] ^ key_words[3]; //k_N_w
    
//    for (int i = 0; i < 5; i++) {
//        printf("Key part: %" PRIx64 "\n", key_words[i]);
//    }
    
    // split plaintext into words
    uint64_t p_words[4];
    divide_block(p_words, p, 4);
    
    c_words[0] = p_words[0];
    c_words[1] = p_words[1];
    c_words[2] = p_words[2];
    c_words[3] = p_words[3];
    
    int s = 0;
    for (int r=0; r < 72; r++) {
        
//        printf("%u: ", r);
//        for (int i = 0; i < 4; i++) {
//            printf("%" PRIx64 " ", c_words[i]);
//        }
//        printf("\n");
//
        
        // if r % 4 == 0 then generate and add subkeys
        if (r % 4 == 0) {
            subkeys[0] = key_words[s % 5];
            subkeys[1] = key_words[(s + 1) % 5] + tweak_words[s % 3];
            subkeys[2] = key_words[(s + 2) % 5] + tweak_words[(s + 1) % 3];
            subkeys[3] = key_words[(s + 3) % 5] + s;
            s+=1;
            // lines above work fine
            
            // add subkeys
            c_words[0] += subkeys[0];
            c_words[1] += subkeys[1];
            c_words[2] += subkeys[2];
            c_words[3] += subkeys[3];
        }
        
        // implement mix function
        uint64_t tmp_x0 = c_words[0], tmp_x1 = c_words[1];
        mix_func(c_words[0], c_words[1], c_words[0], c_words[1], r, 0);
        mix_func(c_words[2], c_words[3], c_words[2], c_words[3], r, 1);
        
        // permute
        uint64_t swap = c_words[1];
        c_words[1] = c_words[3];
        c_words[3] = swap;
    }
    
    subkeys[0] = key_words[s % 5];
    subkeys[1] = key_words[(s + 1) % 5] + tweak_words[s % 3];
    subkeys[2] = key_words[(s + 2) % 5] + tweak_words[(s + 1) % 3];
    subkeys[3] = key_words[(s + 3) % 5] + s;
    s+=1;
    // lines above work fine
    
    // add subkeys
    c_words[0] += subkeys[0];
    c_words[1] += subkeys[1];
    c_words[2] += subkeys[2];
    c_words[3] += subkeys[3];
    
//    printf("Final cipher: ");
//    for (int i = 0; i < 4; i++) {
//        printf("%" PRIx64 " ", c_words[i]);
//    }
//    printf("\n");
    
    // result in uint64_t array c_words, just merge it to one variable c
    
    unsigned char aa[8], bb[8], cc[8], dd[8];
    unsigned char arr[4][8] = {aa[8], bb[8], cc[8], dd[8]};
    memcpy(arr[0], &c_words[0], sizeof(uint64_t));
    memcpy(arr[1], &c_words[1], sizeof(uint64_t));
    memcpy(arr[2], &c_words[2], sizeof(uint64_t));
    memcpy(arr[3], &c_words[3], sizeof(uint64_t));
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            c[i*8 + j] = arr[i][j];
        }
    }
    //----------------------------------------------------------------
//    printf("Cipher : ");
//    for (int i = 0; i < 32; i++) {
//        printf("%x ", c[i]);
//    }
//    printf("\n");
    
}
