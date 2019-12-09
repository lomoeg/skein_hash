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
    // initialize g0 with zeros, we use Skein256_256, so the output will be 32 bytes = 256 bits
//    unsigned char g0[32];
//    for(unsigned char i=0; i<32; i++) g0[i] = 0x00;
    
    // ---- configure c_config_const ------
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
    // print the config out
//    printf("%s", "first chaining value (config): ");
//    for (int i = 0; i < 32; ++i) {
//        printf("%x ", ctx->chaining_value[i]);
//    }
//    printf("\n");
    // ---- end of configure c_config_const ----
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
    if (processed_bytes < Nb) {
        a = 1;
    }

    int last_byte = b*pow(2,7) + a*pow(2, 6) + msg_type;
    res_tweak[15] = (unsigned char) last_byte;

    // Calculating the first two bytes -- position
    int first_value = processed_bytes + min(n_m, (1 + i_step_of_ubi) * 32);
//    printf("%u \n", processed_bytes);
//    printf("%u \n", i_step_of_ubi);
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
    ctx->message_len = len;

    //    printf("%s", "==== BEGIN THREEFISH CALL ==== \n");
//     --- printing the whole message (in a)----
    printf("len is %d \n", len);
    printf("%s", "message before splitting: ");
    for (int i = 0; i < len; ++i) {
        printf("%x ", a[i]);
    }
    printf("\n");
    // --- end printing the message ---

    // --- implementing the algorithm ---
    if (len == 0) { // if len is 0 - special case
        printf("len is 0, no UBI \n");
        ctx->padding = Nb;
        ctx->blockCounter = 0;
//        ctx->posCounter = 0;
    } else if (len <= Nb && len > 0) { // only 1 block, the first one is also the last one
        printf("len is less than 32, no UBI \n");
        for (int i = 0; i < len; ++i) {
            ctx->lastBlockPartial[i] = a[i];
            if (len % Nb == 0 && len > 0) { // in cases where len = 32 or 64
                ctx->padding = 0;
            }
            else {
                ctx->padding = Nb - len; // cases where len is not a multiply of 32
            }
        }
        ctx->blockCounter = len;
        if (ctx->threefish_count == 0) {
//            ctx->posCounter = 0;
        }
    } else { // we have more than 1 block
        unsigned char Mi[Nb]; // the current whole block that we are working on
        int posCounter = 0;
//        ctx->threefish_count = 0;
        while (posCounter + Nb < len) {
            printf("%s", "current block: ");
            // copy the data in the message `a` into the current block we are working on
            for (int i = 0; i < Nb; ++i) {
                Mi[i] = a[posCounter + i];
                printf("%x ", Mi[i]);
            }
            printf("\n");

            printf("%s", "chaining value: ");
            // copy the data in the message `a` into the current block we are working on
            for (int i = 0; i < Nb; ++i) {
                printf("%x ", ctx->chaining_value[i]);
            }
            printf("\n");

            // do the threefish on the current block to get the key for the next block,
            // save the chaining values to context
            unsigned char Hi[Nb];
            unsigned char Ti[16]; // tweak for the current block
            unsigned char Hi_tmp[Nb];
//            posCounter += Nb;

//            if (ctx->threefish_count == 0) {
//                // Hi is the config
//                for (int i = 0; i < Nb; ++i) {
//                    ctx->chaining_value[i] = ctx->config[i];
//                }
//            }
            tweak_gen(Ti, 48, posCounter, ctx->message_len, 0, ctx->threefish_count);
            threefish(Hi_tmp, Mi, ctx->chaining_value, Ti); // argument order: ciphertext, plaintext, key, tweak
            for (int i = 0; i < Nb; ++i) {
                ctx->chaining_value[i] = Hi_tmp[i] ^ Mi[i];
            }
            posCounter += Nb;
            // save the result to context
//            for (int i = 0; i < Nb; ++i) {
//                ctx->chaining_value[i] = Hi[i];
//            }
            ctx->threefish_count++;
        }
        ctx->posCounter = posCounter;
        // the last block
        int blockCounter = 0;
        while (posCounter < len){
            ctx->lastBlockPartial[blockCounter] = a[posCounter];
            blockCounter++;
            posCounter++;
        }
        ctx->blockCounter = blockCounter;
        ctx->padding = Nb - blockCounter;
    }
}

void finalize(unsigned char *a, context *ctx) {
    unsigned char h[32];
    for (int i = 0; i < ctx->blockCounter; ++i) {
        h[i] = ctx->lastBlockPartial[i];
    }
    // pad the last block
    for (int i = 0; i < ctx->padding; ++i) {
        h[ctx->blockCounter + i] = 0x00;
    }
    printf("blockCounter is %d \n", ctx->blockCounter);
    printf("padding is %d \n", ctx->padding);
    printf("position counter (num bytes processed so far): %d \n", ctx->posCounter);
    printf("%s", "last block (padded): ");
    for (int i = 0; i < Nb; ++i) {
       printf("%x ", h[i]);
    }
    printf("\n");

    unsigned char last_tweak[16] = {
            0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
    };

    // do threefish call
    unsigned char h1[32];
    unsigned char first_tweak[16];
    tweak_gen(first_tweak, 48, ctx->posCounter, ctx->message_len, 1, 0);
    threefish(h1, h, ctx->chaining_value, first_tweak);
    unsigned char res[32];
    for (int i = 0; i < Nb; ++i) {
        res[i] = h1[i] ^ h[i];
        printf("%x ", res[i]);
    }
    printf("\n");

    unsigned char last_p[32] = {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    threefish(h1, last_p, res, last_tweak);
//    unsigned char final_res[32];
    for (int i = 0; i < Nb; ++i) {
        a[i] = h1[i] ^ last_p[i];
        printf("%x ", a[i]);
    }
    printf("\n");

}

