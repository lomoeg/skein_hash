/* this file gets replaced with our own driver when we grade your submission.
 * So do what you want here but realize it won't persist when we grade it.
 */

#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include "skein.h"
#include <inttypes.h>

#define CLOCK(x) (x >> 8 | (x ^ (x >> 7) ^ (x >> 6) ^ (x >> 2)) << 24)
void rnd(unsigned char *b, int len) {
	static uint32_t s = 0xFEEDFACE;
	for(int i=0; i<len; i++, s=CLOCK(s)) b[i] = (unsigned char)s;
}

int main() {

	context ctx;
	unsigned char h[32]; // holds the result of the hash function, 256 bits.
	unsigned char m[64]; // message string of length up to 2^99-8 bits, here we have 64bytes (512bits)
	for(unsigned char i=0; i<64; i++) m[i] = 0xFF - i;

	/* test vector 1
	 * note: 0x is hexadecimal, e.g. 0x0B = 0000 1011 = 1 byte = 8 bits
	 * h0[] = 8 * 32 = 256 bits, similar to h[32]
	 * in this case, message m = 0xFF
	*/
	unsigned char h0[] = {
		0x0B,0x98,0xDC,0xD1,0x98,0xEA,0x0E,0x50,0xA7,0xA2,0x44,0xC4,0x44,0xE2,0x5C,0x23,
		0xDA,0x30,0xC1,0x0F,0xC9,0xA1,0xF2,0x70,0xA6,0x63,0x7F,0x1F,0x34,0xE6,0x7E,0xD2 };
	init(&ctx);
	update(m, 1, &ctx);
	finalize(h, &ctx);
	printf("%s\n", !memcmp(h, h0, 32) ? "PASS \n" : "FAIL \n");

	// test vector 2
	// message is FF FE FD FC FB FA F9 F8 F7 F6 F5 F4 F3 F2 F1 F0
    //            EF EE ED EC EB EA E9 E8 E7 E6 E5 E4 E3 E2 E1 E0
	unsigned char h1[] = {
		0x8D,0x0F,0xA4,0xEF,0x77,0x7F,0xD7,0x59,0xDF,0xD4,0x04,0x4E,0x6F,0x6A,0x5A,0xC3,
		0xC7,0x74,0xAE,0xC9,0x43,0xDC,0xFC,0x07,0x92,0x7B,0x72,0x3B,0x5D,0xBF,0x40,0x8B };
	init(&ctx);
	update(m, 32, &ctx);
	finalize(h, &ctx);
	printf("%s\n", !memcmp(h, h1, 32) ? "PASS \n" : "FAIL \n");

	// test vector 3
	// message is FF FE FD FC FB FA F9 F8 F7 F6 F5 F4 F3 F2 F1 F0
    //            EF EE ED EC EB EA E9 E8 E7 E6 E5 E4 E3 E2 E1 E0
    //            DF DE DD DC DB DA D9 D8 D7 D6 D5 D4 D3 D2 D1 D0
    //            CF CE CD CC CB CA C9 C8 C7 C6 C5 C4 C3 C2 C1 C0
	unsigned char h2[] = {
		0xDF,0x28,0xE9,0x16,0x63,0x0D,0x0B,0x44,0xC4,0xA8,0x49,0xDC,0x9A,0x02,0xF0,0x7A,
		0x07,0xCB,0x30,0xF7,0x32,0x31,0x82,0x56,0xB1,0x5D,0x86,0x5A,0xC4,0xAE,0x16,0x2F };
	init(&ctx);
	update(m, 64, &ctx);
	finalize(h, &ctx);
	printf("%s\n", !memcmp(h, h2, 32) ? "PASS \n" : "FAIL \n");
//
	// custom iterated test vector
	printf("------------------------ Fourth Case -------------------------------------------\n");
	unsigned char h3[] = {
		0x32,0x53,0x79,0x06,0xdc,0x03,0x06,0xe4,0xb7,0x67,0xf8,0xeb,0x96,0x60,0x06,0x8a,
		0x4a,0xe3,0x33,0x99,0x2c,0xf0,0xe1,0x6a,0xfe,0x96,0xa6,0x24,0x91,0x3a,0xbe,0xe5 };
	init(&ctx);
	for(int i=0; i<0x100; i++) { // runs 256 times
		int len = m[0] % 64;
		printf("\niteration number %d, len is %u \n" , i, len);
		rnd(m, len);
		update(m, len, &ctx);

	}
	finalize(h, &ctx);
//	printf("number of UBI called in total: %d", ctx->)
	printf("%s\n", !memcmp(h, h3, 32) ? "PASS" : "FAIL");

	return 0;
}

