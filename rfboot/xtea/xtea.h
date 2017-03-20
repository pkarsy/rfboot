#ifndef XTEA_H
#define XTEA_H


#include <stdint.h>

void xtea_encipher(uint32_t v[2], const uint32_t key[4] ) ;
void xtea_encipher_cbc( uint32_t v[2], const uint32_t key[4], uint32_t iv[2] );
void xtea_decipher( uint32_t v[2], const uint32_t key[4] );
void xtea_decipher_cbc( uint32_t v[2], const uint32_t key[4], uint32_t iv[2] );

#endif
