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
    size_t  hashBitLen;  /* size of hash result, in bits */
    size_t  bCnt;        /* current byte count in buffer b[] */
    uint64_t  T[2];         /* tweak words: T[0]=byte cnt, T[1]=flags */
} context;

/* Implement the following API.
 * You can add your own functions above, but don't modify below this line.
 */

/* Initialize fresh context ctx */
void init(context *ctx);

/* Under context ctx, incrementally process len bytes at a */
void update(unsigned char *a, int len, context *ctx);

/* Under context ctx, finalize the hash and place the digest at a */
void finalize(unsigned char *a, context *ctx);

