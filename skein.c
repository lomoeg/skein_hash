#include <string.h>
#include <stdio.h>
#include "skein.h"
#include "threefish.h"
#include <inttypes.h>

#if 1 /* TODO change to 0 after you complete this task */
/*

TODO FREE TEXT ASSIGNMENT

If there was a FAQ for this assignment, what question and answer would've helped you the most?

Q: <FILL ME IN>

A: <FILL ME IN>

*/
#warning "FAIL you have not completed the free text assignment"
#endif

void init(context *ctx) {
    memset(ctx->chainingValues, 0, sizeof(ctx->chainingValues)); /* zero the chaining variables */


}

void update(unsigned char *a, int len, context *ctx) {
    //  pre-fill a buffer with data before executing the hash function.
    //  In particular, update is not intended to run UBI on its data yet,
    //  since we are implementing the "simple hash" variant of Skein.

//    for (int i = 0; i < 64; i++) {
//        printf("%" PRIx64 " ", a[i]);
//    }
}

void finalize(unsigned char *a, context *ctx) {
    // the finalize function should be used for the last block
    // (we can't know if this is the last block before finalize and this information is
    // required for the tweak). This is why the remaining message bytes should be
    // saved to a buffer in the update function.

//    for (int i = 0; i < 64; i++) {
//        printf("%" PRIu8 " ", a[i]);
//    }
}



