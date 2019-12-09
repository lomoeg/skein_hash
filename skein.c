#include <string.h>
#include <stdio.h>
#include "skein.h"
#include "threefish.h"
#include <inttypes.h>
#include <math.h>
#include <stdlib.h>

#if 0 /* TODO change to 0 after you complete this task */
/*
 
 TODO FREE TEXT ASSIGNMENT
 
 If there was a FAQ for this assignment, what question and answer would've helped you the most?
 
 Q: <FILL ME IN>
 
 A: <FILL ME IN>
 
 */
#warning "FAIL you have not completed the free text assignment"
#endif

// here we calculate C_config and G0 (page 15)
void init(context *ctx) {
    uint64_t g0[4] =
    {0xFC9DA860D048B449, 0x2FCA66479FA7D833, 0xB33BC3896656840F, 0x6A54E920FDE8DA69};
    
    unsigned char aa[8], bb[8], cc[8], dd[8];
    unsigned char arr[4][8] = {aa[8], bb[8], cc[8], dd[8]};
    memcpy(arr[0], &g0[0], sizeof(uint64_t));
    memcpy(arr[1], &g0[1], sizeof(uint64_t));
    memcpy(arr[2], &g0[2], sizeof(uint64_t));
    memcpy(arr[3], &g0[3], sizeof(uint64_t));
    
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            ctx->chaining_value[i*8 + j] = arr[i][j];
        }
    }
    ctx->threefish_count = 0;
    ctx->posCounter = 0;
    ctx->blockCounter = 0;
    ctx->threefish_count = 0;
    ctx->message_len = 0;
    for(unsigned char i=0; i<16; i++) ctx->lastBlockPartial[i] = 0x00;
}

int mod(int a, int b)
{
    // helper function with modulo on negative number
    int r = a % b;
    return r < 0 ? r + b : r;
}

int min(int a, int b) {
    return b < a ? b : a;
}

void tweak_gen(unsigned char res_tweak[16], int msg_type, int processed_bytes,
               int n_m, int b, int i_step_of_ubi) {
    // bitpad always =0 since we get input msg in bytes
    for(unsigned char i=0; i<16; i++) res_tweak[i] = 0x00;
    
    // Calculating the last byte
    int a = 0;
    if (i_step_of_ubi == 0) {
        a = 1;
    }
    
    int last_byte = b*pow(2,7) + a*pow(2, 6) + msg_type;
    res_tweak[15] = (unsigned char) last_byte;
    
    // Calculating the first two bytes -- position
    int first_value = processed_bytes + min(n_m, (1 + i_step_of_ubi) * 32);
    if (first_value > 255) {
        res_tweak[0] = first_value / 256;
        res_tweak[1] = first_value % 256;
    } else {
        res_tweak[0] = (unsigned char) first_value;
    }
    
    printf("tweak: ");
    for (int i=0; i < 16; i++) {
        printf("%x ", res_tweak[i]);
    }
    printf("\n");
}


void update(unsigned char *a, int len, context *ctx) {

    int a_counter = 0;
    int proc_bytes = 0;
    while (ctx->blockCounter < 32 && a_counter < len) {
        ctx->lastBlockPartial[ctx->blockCounter] = a[a_counter];
        a_counter++;
        ctx->blockCounter++;
        proc_bytes++;
    }
    
    printf("The current block: ");
    for (int i = 0; i < Nb; ++i) {
        //a[i] = h1[i] ^ last_p[i];
        printf("%x ", ctx->lastBlockPartial[i]);
    }
    printf("\n");
    
    printf("blockCounter: %u \n", ctx->blockCounter);
    
    // if we got the complete block and it's not the first one
    if (ctx->blockCounter == 32 && len > 32) {
        // process the complete block with threefish call
        // save the result in context
        
        //unsigned char Hi[Nb];
        unsigned char Ti[16]; // tweak for the current block
        unsigned char Hi_tmp[Nb];
        
        printf("posCounter: %u \n i_step_of_ubi: %u \n", ctx->posCounter, ctx->threefish_count);
        
        // generate the tweak
        // res_tweak[16], msg_type, processed_bytes, n_m, b, i_step_of_ubi)
        tweak_gen(Ti, 48, ctx->posCounter, len, 0, ctx->threefish_count);
        
        // calculate the threefish
        threefish(Hi_tmp, ctx->lastBlockPartial, ctx->chaining_value, Ti);
        // argument order: ciphertext, plaintext, key, tweak
        
        // safe the full result (including XOR to the context)
        for (int i = 0; i < Nb; ++i) {
            ctx->chaining_value[i] = Hi_tmp[i] ^ ctx->lastBlockPartial[i];
        }
        
        ctx->blockCounter = 0;
        ctx->threefish_count++;
    }
    ctx->posCounter+= proc_bytes;
}




void finalize(unsigned char *a, context *ctx) {
    unsigned char h[32];
    //---------------------------------------------------------------
    // we know that last block is in context: ctx->lastBlockPartial
    // and it contains of ctx->blockCounter bytes
    
    // pad the last block
    for (int i = 0; i < 31 - ctx->blockCounter; ++i) {
        ctx->lastBlockPartial[ctx->blockCounter + i] = 0x00;
    }
    
    unsigned char last_tweak[16] = {
        0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
    };
    
    // do threefish call
    unsigned char h1[32];
    unsigned char first_tweak[16];
    
    printf("------------Finalize output--------------\n");
    tweak_gen(first_tweak, 48, ctx->posCounter, ctx->message_len, 1, ctx->threefish_count);
    
    threefish(h1, ctx->lastBlockPartial, ctx->chaining_value, first_tweak);
    unsigned char res[32];
    
    printf("The last threefish call: ");
    for (int i = 0; i < Nb; ++i) {
        res[i] = h1[i] ^ ctx->lastBlockPartial[i];
        printf("%x ", res[i]);
    }
    printf("\n");
    
    unsigned char last_p[32] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    threefish(h1, last_p, res, last_tweak);
    
    printf("The output of the last ubi call: ");
    //    unsigned char final_res[32];
    for (int i = 0; i < Nb; ++i) {
        a[i] = h1[i] ^ last_p[i];
        printf("%x ", a[i]);
    }
    printf("\n");
    
}
