#include "sha.h"
#include "sha3.h"

#include <cstdlib>
#include <cstring>

int sha_blockLen[8] = {
    64,
    64,
    64,
    128,
    128,
    1344 >> 3,
    1088 >> 3,
    576 >> 3
};

int sha_retLen[8] = {
    160 >> 3,
    224 >> 3,
    256 >> 3,
    384 >> 3,
    512 >> 3,
    128 >> 3,
    256 >> 3,
    512 >> 3
};

int sha_getModeNum(char *mode) {
    if (!strcmp(mode, SHA3_128_STR)) {
        return SHA3_128;
    }
    if (!strcmp(mode, SHA3_256_STR)) {
        return SHA3_256;
    }
    // default SHA3_512
    return SHA3_512;
}

int sha_getBlockLen(char *mode) {
    return sha_blockLen[sha_getModeNum(mode)];
}

int sha_getBlockLenIdx(int mode) {
    if (mode > SHA3_512) {
        mode = SHA3_512;
    }

    return sha_blockLen[mode];
}

int sha_getRetLen(char *mode) {
    return sha_retLen[sha_getModeNum(mode)];
}

int sha_getRetLenIdx(int mode) {
    if (mode > SHA3_512) {
        mode = SHA3_512;
    }

    return sha_retLen[mode];
}

void *sha_initContextStr(char *mode) {
    return sha_initContext(sha_getModeNum(mode));
}

void *sha_initContext(int mode) {
    void *ctx;
    switch (mode) {
        case SHA3_128:
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(reinterpret_cast<sha3_context *>(ctx), SHA3_128);
            return ctx;
        case SHA3_256:
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(reinterpret_cast<sha3_context *>(ctx), SHA3_256);
            return ctx;
        default:
            ctx = malloc(sizeof(sha3_context));
            sha3_initContext(reinterpret_cast<sha3_context *>(ctx), SHA3_512);
            return ctx;
    }
}


void sha_updateStr(char *mode, void *ctx, unsigned char *in, int n) {
    int i = sha_getModeNum(mode);
    sha_update(i, ctx, in, n);
}

void sha_update(int mode, void *ctx, unsigned char *in, int n) {
    sha3_update(reinterpret_cast<sha3_context *>(ctx), in, n);
}


void sha_digestStr(char *mode, void *ctx, unsigned char **out) {
    sha_digest(sha_getModeNum(mode), ctx, out);
}

void sha_digest(int mode, void *ctx, unsigned char **out) {
    sha3_digest(reinterpret_cast<sha3_context *>(ctx), out);
}


void sha_free(void *ctx) {
    free(ctx);
}
