#include <stdint.h>

/* This is your hash instance context, i.e., hash state.
 * You could put the following stuff inside here:
 * - buffered message bytes
 * - current offset within said buffer
 * - chaining value
 * - tweak
 * - leprechaun gold
 * - next week's correct lotto numbers
 */
typedef struct {

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

