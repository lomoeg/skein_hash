#include <stdint.h>

/* Hash instance context, i.e., hash state.
 * The buffer should be used to store the bytes left from partial
 * message blocks which should be reused in the next iteration.
 * context includes:
 * - buffered message bytes
 * - current offset within said buffer
 * - chaining value
 * - tweak
 */
typedef struct {
    size_t  hashBitLen;  // size of hash result, in bits
    size_t  bCnt;        // current byte count in buffer b[]
    uint64_t T[2];      // tweak words: T[0]=byte cnt, T[1]=flags
    uint64_t chainingValues[4];       // chaining values
    uint8_t b[32];       // partial block buffer
} context;

/* blkSize =  256 bits. hashSize =  256 bits */
// output of the configuration UBI. if you are using Skein as a normal hash function,
// you can use these IV values as constants and skip the configuration step entirely
//uint64_t IV_VALUES[4] =
//        {0xFC9DA860D048B449, 0x2FCA66479FA7D833, 0xB33BC3896656840F, 0x6A54E920FDE8DA69};
//
//unsigned char aa[8], bb[8], cc[8], dd[8];
//unsigned char arr[4][8] = {aa[8], bb[8], cc[8], dd[8]};
//memcpy(arr[0], &IV_VALUES[0], sizeof(uint64_t));
//memcpy(arr[1], &IV_VALUES[1], sizeof(uint64_t));
//memcpy(arr[2], &IV_VALUES[2], sizeof(uint64_t));
//memcpy(arr[3], &IV_VALUES[3], sizeof(uint64_t));
//
//unsigned char c_config_const[32];
//for (int i = 0; i < 4; i++) {
//    for (int j = 0; j < 8; j++) {
//        c_config_const[i*8 + j] = arr[i][j];
//    }
//}

#define UBI_TYPE_KEY 0   // key (for MAC and KDF)
#define UBI_TYPE_CFG 4   // configuration block
#define UBI_TYPE_PRS 8   // personalization
#define UBI_TYPE_PK  12  // public key (for signature hashing)
#define UBI_TYPE_KDF 16  // key identifier (for KDF)
#define UBI_TYPE_NON 20  // nonce (for stream cipher or randomized hashing)
#define UBI_TYPE_MSG 48  // message
#define UBI_TYPE_OUT 63  // output

/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/* Initialize fresh context ctx */
void init(context *ctx);

/* Under context ctx, incrementally process len bytes at a */
void update(unsigned char *a, int len, context *ctx);

/* Under context ctx, finalize the hash and place the digest at a */
void finalize(unsigned char *a, context *ctx);

