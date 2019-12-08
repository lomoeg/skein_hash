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

void ubi_first_call(unsigned char *res ,unsigned char *g, unsigned char *c_config, int t_type) {
    uint64_t tweak_words[2];
    tweak_words[0] = 0x0400000000000000;    // T_cfg * 2^120 = 2^122, 64-127 bits
    tweak_words[1] = 0x0000000000000000;    // 0-63 last bits of T_s
    
    tweak_words[1] += 0x0000000000000020;   // T_s + min(N_m, (i+1)*N_b)
    //N_m = N_b = 32, i = 0
    //32 = 0x0000000000000020
    
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
    //------------------------------------------
    
    threefish(res, g, tweak, c_config);
}


// here we calculate C_config and G0 (page 15)
void init(context *ctx) {
    unsigned char g0[32];
    for(unsigned char i=0; i<32; i++) g0[i] = 0x00;
    
    // configure c_config_const
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
    //end of configure c_config_const
    
    // make K' = 0^32
    unsigned char g[32];
    for(unsigned char i=0; i<32; i++) g[i] = 0x00;
    
    // call ubi
    ubi(g0, g, c_config_const, 4);
    
    //print g0
    for (int i = 0; i < 32; i++) {
        printf("%x ", g0[i]);
        //printf("%s", "hey look ma" );
    }
}

void update(unsigned char *a, int len, context *ctx) {
    
}

void finalize(unsigned char *a, context *ctx) {
    //printf("%u", a);
}

