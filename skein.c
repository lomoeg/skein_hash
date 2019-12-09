#include <string.h>
#include <stdio.h>
#include "skein.h"
#include "threefish.h"
#include <inttypes.h>

#if 0 /* TODO change to 0 after you complete this task */
/*
 
 TODO FREE TEXT ASSIGNMENT
 
 If there was a FAQ for this assignment, what question and answer would've helped you the most?
 
 Q: <FILL ME IN>
 
 A: <FILL ME IN>
 
 */
#warning "FAIL you have not completed the free text assignment"
#endif

void test(unsigned char *a){
    // test the first case (len=1) with values according to the skein_states.txt
    unsigned char k_prime[32] = { // first block of the message
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    unsigned char p[32] = { // first block of the message
            0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    unsigned char c[32] = { // the config
            0x49,0xb4,0x48,0xd0,0x60,0xa8,0x9d,0xfc,0x33,0xd8,0xa7,0x9f,0x47,0x66,0xca,0x2f,
            0x0f,0x84,0x56,0x66,0x89,0xc3,0x3b,0xb3,0x69,0xda,0xe8,0xfd,0x20,0xe9,0x54,0x6a
    };
    unsigned char t[16] = { // the tweak
            0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xf0
    };
    unsigned char h0[32];
    threefish(h0, p, c, t);
    for (int i = 0; i < 32; ++i) {
        h0[i] = h0[i] ^ p[i];
    }
    printf("%s", "h0: ");
    for (int i = 0; i < 32; ++i) {
        printf("%x ", h0[i]);
    }
    printf("\n");

    unsigned char p1[32] = {
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
            0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
    };
    unsigned char k1[32] = {
            0x1E,0x03,0x5A,0x91,0xED,0x39,0xAE,0x27,0xD2,0x1D,0x31,0x5D,0x06,0x49,0xAE,0x3f,
            0x6D,0x96,0x4D,0x24,0x0C,0xC8,0xE6,0x76,0x5C,0xFB,0x19,0x62,0x94,0x43,0x7E,0x20
    };
    unsigned char t1[16] = {
            0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
    };
    threefish(a, p1, h0, t1);
    printf("%s", "a: ");
    for (int i = 0; i < 32; ++i) {
        printf("%x ", a[i]);
    }
    printf("\n");
}


void ubi(unsigned char *res ,unsigned char *g, unsigned char *m, int t_type) {
    /*
     * res: hold the result of this function
     * g: the starting value of Nb bytes
     * m: a message string of arbitrary length up to 2^99 - 8 bits, encoded in a string of bytes
     * t_type: the type value that specifies the TreeLevel and Type fields
     */
//    int Nb = 32; // for skein_256, number of bytes in the state = 32 bytes

    // if the number of bits in the message string is a multiply of 8:
    unsigned char *M_prime;
//    memcpy(M_prime, m, sizeof(m));  // set M_prime = m
    int B = 0;
//    printf("%lu", sizeof(m));

    int Nm = sizeof(m); // number of bytes in M'
    int p = 0;
    if (Nm == 0) {

        p = Nb;
    } else {
        p = -Nm % Nb;
    }

}



void ubi_first_call(unsigned char *res ,unsigned char *g, unsigned char *c_config, int t_type) {
    uint64_t tweak_words[2];
    tweak_words[0] = 0x0400000000000000;    // T_cfg * 2^120 = 2^122, bit 64-127 (T_cfg = 4)
    tweak_words[1] = 0x0000000000000000;    // bit 0-63 of T_s
    
    tweak_words[1] += 0x0000000000000020;   // T_s + min(N_m, (i+1)*N_b)
    // N_m = N_b = 32, i = 0
    // 32 = 0x0000000000000020
    
    tweak_words[0] += 0xC000000000000000;   // T_s + 2^126 + 2^127
    
    // return result from uint64_t array back to unsigned char
    //------------------------------------------
    unsigned char aaa[8], bbb[8];
    unsigned char arr2[2][8] = {aaa[8], bbb[8]};
    memcpy(arr2[0], &tweak_words[0], sizeof(uint64_t));
    memcpy(arr2[1], &tweak_words[1], sizeof(uint64_t));
    
    unsigned char tweak[16];
    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 8; j++) {
            tweak[i*8 + j] = arr2[i][j];
        }
    }

    printf("%s ", "t:");
    for (int i = 0; i < 16; ++i) {
        printf("%x ", tweak[i]);
    }
    printf("\n");
    //------------------------------------------

//    threefish(res, g, c_config, tweak);
}


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

    unsigned char c_config_const[32];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            c_config_const[i*8 + j] = arr[i][j];
        }
    }

//    printf("%s", "c_config_const: ");
//    for (int i = 0; i < 32; ++i) {
//        printf("%x ", c_config_const[i]);
//    }
//    printf("\n");
    // ---- end of configure c_config_const ----
    
    // make K' = 0^32
    unsigned char k[32];
    for(unsigned char i=0; i<32; i++) k[i] = 0x00;
    
    // call ubi
//    ubi_first_call(g0, k, c_config_const, 4);
    
    //print g0
//    printf("%lu", sizeof(g0));
//    for (int i = 0; i < 32; i++) {
//        printf("%x ", g0[i]);
//    }
//    ubi(g0, k, c_config_const, 4);
}

int mod(int a, int b)
{
    // helper function with modulo on negative number
    int r = a % b;
    return r < 0 ? r + b : r;
}


void update(unsigned char *a, int len, context *ctx) {

    //    printf("%s", "==== BEGIN THREEFISH CALL ==== \n");
    // --- printing the whole message (in a)----
    printf("len is %d \n", len);
    printf("%s", "message before splitting: ");
    for (int i = 0; i < 64; ++i) {
        printf("%x ", a[i]);
    }
    printf("\n");
    // --- end printing the message ---

    // --- implementing the algorithm ---
    if (len == 0) { // if len is 0 - special case
        ctx->padding = Nb;
        ctx->blockCounter = 0;
    } else if (len <= Nb && len > 0) { // only 1 block, the first one is also the last one
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
    } else { // we have more than 1 block
        unsigned char Mi[Nb]; // the current whole block that we are working on
        int posCounter = 0;
        while (posCounter + Nb < len) {
            printf("%s", "current block: ");
            for (int i = 0; i < Nb; ++i) {
                Mi[posCounter + i] = a[posCounter + i];
                printf("%x ", Mi[posCounter+i]);
            }
            printf("\n");
            posCounter += Nb;
        }
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
    //printf("%u", a);

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
    printf("%s", "last block (padded): ");
    for (int i = 0; i < Nb; ++i) {
       printf("%x ", h[i]);
    }
    printf("\n");


    unsigned char last_tweak[16] = {
            0x08,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF
    };

}

