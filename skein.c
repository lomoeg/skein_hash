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

void ubi(unsigned char *res ,unsigned char *g, unsigned char *m, int t_type) {
    /*
     * res: hold the result of this function
     * g: the starting value of Nb bytes
     * m: a message string of arbitrary length up to 2^99 - 8 bits, encoded in a string of bytes
     * t_type: the type value that specifies the TreeLevel and Type fields
     */
    int Nb = 32; // for skein_256, number of bytes in the state = 32 bytes

    // if the number of bits in the message string is a multiply of 8:
    unsigned char *M_prime;
    memcpy(M_prime, m, sizeof(m));  // set M_prime = m
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
    unsigned char g0[32];
    for(unsigned char i=0; i<32; i++) g0[i] = 0x00;
    
    // ---- configure c_config_const ------
    uint64_t IV_VALUES[4] =
    {0xFC9DA860D048B449, 0x2FCA66479FA7D833, 0xB33BC3896656840F, 0x6A54E920FDE8DA69};
    
    unsigned char aa[8], bb[8], cc[8], dd[8];
    unsigned char arr[4][8] = {aa[8], bb[8], cc[8], dd[8]};
    memcpy(arr[0], &IV_VALUES[0], sizeof(uint64_t));
    memcpy(arr[1], &IV_VALUES[1], sizeof(uint64_t));
    memcpy(arr[2], &IV_VALUES[2], sizeof(uint64_t));
    memcpy(arr[3], &IV_VALUES[3], sizeof(uint64_t));

    unsigned char c_config_const[32];
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 8; j++) {
            c_config_const[i*8 + j] = arr[i][j];
        }
    }

    printf("%s", "k: ");
    for (int i = 0; i < 32; ++i) {
        printf("%x ", c_config_const[i]);
    }
    printf("\n");
    // ---- end of configure c_config_const ----
    
    // make K' = 0^32
    unsigned char k[32];
    for(unsigned char i=0; i<32; i++) k[i] = 0x00;
    
    // call ubi
    ubi_first_call(g0, k, c_config_const, 4);
    
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
    printf("%s", "message before splitting: ");
    for (int i = 0; i < 32; ++i) {
        printf("%x ", a[i]);
    }
    printf("\n");

    // --- split the message and pad according to the len given (page 13) ---
    int Nb = 32;
    int Nm = len;
    int p = 0;
    if (Nm == 0) {
        p = Nb;
    }
    else {
        p = mod(-Nm, Nb);
    }
    printf("padding is %d \n", p);
    // find out the number of blocks
    int num_blocks = 0;
    if ((len % Nb) == 0) {
        num_blocks = len/Nb;
    } else {
       num_blocks = len/Nb + 1;
    }
    printf("num blocks is %d \n", num_blocks);
    // M_2primes contains the num_blocks with each of Nb bytes
    unsigned char M_2primes[num_blocks][Nb];
    // process the non-last blocks
    for (int i = 0; i < num_blocks; ++i) {
        memcpy(M_2primes[i], m[], sizeof(uint32_t));
    }
    // process the last block: save the left to the

}

void finalize(unsigned char *a, context *ctx) {
    //printf("%u", a);
}

