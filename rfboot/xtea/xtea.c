#ifndef XTEA_H
#define XTEA_H


#include <stdint.h>
#include "xtea.h"


void xtea_encipher( uint32_t v[2], const uint32_t key[4] ) {
    static const uint8_t num_rounds=32;
    uint32_t v0=v[0], v1=v[1], sum=0, delta=0x9E3779B9;
    uint8_t i;
    for (i=0; i < num_rounds; i++) {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += delta;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
    }
    v[0]=v0; v[1]=v1;
}

void xtea_encipher_cbc( uint32_t v[2], const uint32_t key[4], uint32_t iv[2] ) {
    v[0] ^= iv[0];
    v[1] ^= iv[1];
    xtea_encipher(v,key);
    iv[0]=v[0];
    iv[1]=v[1];
}

void xtea_decipher( uint32_t v[2], const uint32_t key[4] ) {
    static const uint8_t num_rounds=32;
    uint32_t v0=v[0], v1=v[1], delta=0x9E3779B9, sum=delta*num_rounds;
    uint8_t i;
    for (i=0; i < num_rounds; i++) {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum>>11) & 3]);
        sum -= delta;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0]=v0; v[1]=v1;
}


void xtea_decipher_cbc( uint32_t v[2], const uint32_t key[4], uint32_t iv[2] ) {
    uint32_t c0=v[0];
    uint32_t c1=v[1];
    xtea_decipher(v,key);
    v[0] ^= iv[0];
    v[1] ^= iv[1];
    iv[0]=c0;
    iv[1]=c1;
}

#endif
