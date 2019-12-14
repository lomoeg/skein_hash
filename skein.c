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
    ctx->how_many_bytes_to_end_of_block_left = 0;
    for(unsigned char i=0; i<16; i++) ctx->lastBlockPartial[i] = 0x00;
    
    for (int i=0; i < 100000; i++) {
        for (int j=0; j<32; j++) ctx->whole_message[i][j] = 0;
    }
    
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
               int n_m, int a, int b, int i_step_of_ubi) {
    // bitpad always =0 since we get input msg in bytes
    for(unsigned char i=0; i<16; i++) res_tweak[i] = 0x00;
    
    int last_byte = b*pow(2,7) + a*pow(2, 6) + msg_type;
    res_tweak[15] = (unsigned char) last_byte;
    
    // Calculating the first two bytes -- position
    int first_value = min(n_m, (1 + i_step_of_ubi) * 32);
    if (first_value > 255) {
        res_tweak[0] = first_value % 256;
        res_tweak[1] = first_value / 256;
    } else {
        res_tweak[0] = (unsigned char) first_value;
    }
    
    printf("t: ");
    for (int i=0; i < 16; i++) {
        printf("%x ", res_tweak[i]);
    }
    printf("\n\n");
}


void update(unsigned char *a, int len, context *ctx) {

    int proc_bytes = 0;
    int cc = ctx->how_many_bytes_to_end_of_block_left;
    for (int i= 0; i < len; i++) {
        
        if (cc == 32) {
            ctx->threefish_count++;
            cc = 0;
            //printf("\n");
        }
        
        ctx->whole_message[ctx->threefish_count][cc] = a[i];
        //printf("%x ", ctx->whole_message[ctx->threefish_count][i % 32]);
        cc++;
        proc_bytes++;
    }
    
    ctx->how_many_bytes_to_end_of_block_left = cc;
    ctx->posCounter += proc_bytes;
    //printf("posCounter: %lld \n", ctx->posCounter);
}




void finalize(unsigned char *a, context *ctx) {
    unsigned char h[32];
    //---------------------------------------------------------------
    
    for (int k=0; k < ctx->threefish_count; k++) {
        for(int i=0; i < 32; i++) {
            printf("%x ", ctx->whole_message[k][i % 32]);
        }
        printf("\n");
    }
    
    if (ctx->posCounter % 32 != 0) {
        for (int i = ctx->posCounter % 32; i < 32 - ctx->posCounter % 32; ++i) {
            ctx->whole_message[ctx->threefish_count][i] = 0x00;
        }
    }
    
    //printf("posCounter: %lld \n", ctx->posCounter);
    for (int i = 0; i < ctx->threefish_count; i++) {
        printf("\ninside\n");
        
        printf("p: ");
        for (int v = 0; v < 32; ++v) {
            printf("%x ", ctx->whole_message[i][v]);
        }
        printf("\n");
        
        printf("k: ");
        for (int k = 0; k < Nb; ++k) {
            printf("%x ", ctx->chaining_value[k]);
        }
        printf("\n");
        
        unsigned char tweak[16];
        int a_fir = 0;
        if (i == 0) {
            a_fir = 1;
        }
                             // we don't care
        tweak_gen(tweak, 48, 0, ctx->posCounter, a_fir, 0, i);
        
        threefish(h, ctx->whole_message[i], ctx->chaining_value, tweak);
        
        //unsigned char res[32];
        
        //printf("k: ");
        for (int k = 0; k < Nb; ++k) {
            ctx->chaining_value[k] = h[k] ^ ctx->whole_message[i][k];
            //printf("%x ", ctx->chaining_value[k]);
        }
        //printf("\n");
        
        //ctx->realPosCounter += 32;
        //printf("\nrealPosCounter: %x \n", ctx->realPosCounter);
        
    }

    
    //printf("\n posCounter: %u \n", ctx->posCounter);
//    for (int i=0; i < ctx->posCounter; i++) {
//        if (i % 32 == 0) {
//            printf("\n");
//        }
//        printf("%x ", ctx->whole_message[i / 32][i % 32]);
//
//    }
    
    unsigned char last_tweak[16] = {
        0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
    };
    
//    // do threefish call
    unsigned char h1[32];
    unsigned char first_tweak[16];

    printf("\n------------Finalize output--------------\n");
    int aaaa = 0;
    if (ctx->posCounter <= 32) {
        aaaa = 1;
    }
    //printf("%u ", ctx->threefish_count);
    
//    int poss = 0;
//    int poss2 = 0;
//    if (ctx->threefish_count != 0) {
//        poss = (ctx->threefish_count - 2) * 32;
//        poss2 = ctx->threefish_count - 1;
//    }
    
    printf("p: ");
    for (int i = 0; i < 32; i++) {
        printf("%x ", ctx->whole_message[ctx->threefish_count][i]);
    }
    printf("\n");
    
    printf("k: ");
    for (int k = 0; k < Nb; ++k) {
        printf("%x ", ctx->chaining_value[k]);
    }
    printf("\n");
    
    //printf("\nPosCounter: %u \n", ctx->posCounter);
    //printf("\nthreefish_count: %u \n", ctx->threefish_count);
    tweak_gen(first_tweak, 48, 0, ctx->posCounter, aaaa, 1, ctx->threefish_count);
    
    threefish(h, ctx->whole_message[ctx->threefish_count], ctx->chaining_value, first_tweak);
    unsigned char res[32];
    
    
    unsigned char last_p[32] = {
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    printf("p: ");
    for (int i = 0; i < 32; ++i) {
        printf("%x ", last_p[i]);
    }
    printf("\n");
    
    
    printf("k: ");
    for (int i = 0; i < 32; ++i) {
        res[i] = h[i] ^ ctx->whole_message[ctx->threefish_count][i];
        printf("%x ", res[i]);
    }
    printf("\n");

    threefish(h, last_p, res, last_tweak);

    printf("\n");
    printf("Skein result: ");
    //    unsigned char final_res[32];
    for (int i = 0; i < Nb; ++i) {
        a[i] = h[i] ^ last_p[i];
        printf("%x ", a[i]);
    }
    printf("\n\n");
    
}
