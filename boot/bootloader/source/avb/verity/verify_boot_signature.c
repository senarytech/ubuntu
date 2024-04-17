/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include "com_type.h"
#include "io.h"
#include "string.h"
#include "debug.h"
#include "sha2.h"
#include "DRM_Crypto_API.h"
#include "DRM_Crypto_BigNum.h"
#include  "DRM_Crypto.h"
#include "DRM_Crypto_RSA_Cipher.h"
#include "asn1_parse.h"

#define RSANUMBYTES 256           /* 2048 bit key length */
#define RSANUMWORDS (RSANUMBYTES / sizeof(uint32_t))

typedef struct RSAPublicKey {
	int len;					/* Length of n[] in number of uint32_t */
	uint32_t n0inv;			/* -1 / n[0] mod 2^32 */
	uint32_t n[RSANUMWORDS];	/* modulus as little endian array */
	uint32_t rr[RSANUMWORDS]; /* R^2 as little endian array */
	uint8_t exponent[4]; 			/* 3 or 65537 */
	//int exponent; 			/* 3 or 65537 */
}RSAPublicKey;


extern void NN_Decode (UINT32 *a, UINT32 digits, const UINT8 *b, const UINT32 len);
extern UINT32 NN_Digits (UINT32 *a, UINT32 digits);
extern INT32 NN_Cmp (UINT32 *a, UINT32 *b, UINT32 digits);
extern void NN_ModExp (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 cDigits, UINT32 *d, UINT32 dDigits);
extern void NN_Encode (UINT8 *a, UINT32 len, UINT32 *b, UINT32 digits);

static void sha256_caculate(unsigned char * data, unsigned int size,
								unsigned char * authattr, unsigned int size1,
								unsigned char * digest)
{
	SHA256_CTX context;

	SHA256_Init(&context);

	SHA256_Update(&context, data, size);

	SHA256_Update(&context, authattr, size1);

	SHA256_Final(digest, &context);
}

#define NN_DIGIT_BITS 32
#define DIGIT_2MSB(x) (UINT32)(((x) >> (NN_DIGIT_BITS - 2)) & 3)



#define MAX_RSA_MODULUS_BITS 2048
#define MAX_RSA_MODULUS_LEN ((MAX_RSA_MODULUS_BITS + 7) / 8)
#define MAX_RSA_PRIME_BITS ((MAX_RSA_MODULUS_BITS + 1) / 2)
#define MAX_RSA_PRIME_LEN ((MAX_RSA_PRIME_BITS + 7) / 8)
#define MAX_NN_DIGITS  0x40

/********************************************************************
 *Para:
 * output:    Pointer to the buffer of output data
 * outputLen: Pointer to the size of output data
 * input:     Pointer to the buffer of input data
 * inputLen:  the size of input data
 * u_d:       Pointer to the buffer of RSA Key d value
 * u_n:       Pointer to the buffer of RSA Key n value
 *
 *
********************************************************************/
HRESULT MV_RSAPrivateBlock (UINT8 *output, UINT32 *outputLen, UINT8 *input, UINT32 inputLen, UINT8*u_d, UINT8* u_n )
{
	UINT32 c[MAX_NN_DIGITS], e[MAX_NN_DIGITS], m[MAX_NN_DIGITS], n[MAX_NN_DIGITS];
	UINT32 eDigits, nDigits;
	int	i = 0;

	NN_Decode(m, MAX_NN_DIGITS, input, inputLen);
	NN_Decode(n, MAX_NN_DIGITS, u_n, MAX_RSA_MODULUS_LEN);
	NN_Decode(e, MAX_NN_DIGITS, u_d, MAX_RSA_MODULUS_LEN);

	nDigits = NN_Digits(n, MAX_NN_DIGITS);
	eDigits = NN_Digits(e, MAX_NN_DIGITS);

	if (NN_Cmp(m, n, nDigits) >= 0)
		return 1;
	for(i = 0; i < 10;i++){
		/* Compute c = m^e mod n. */
		NN_ModExp(c, m, e, eDigits, n, nDigits);
	}

	*outputLen = inputLen;
	NN_Encode(output, *outputLen, c, nDigits);


    return 0;
}

static int verify(uint8_t * src, uint32_t src_len,
				uint8_t * signature, uint8_t * keystore,
				uint8_t * buff)
{
	unsigned char digest[SHA256_DIGEST_LENGTH];
	unsigned int output_len = 0;
	RSAPublicKey * key = (RSAPublicKey *)keystore;
	uint8_t key_e[256];
	uint8_t key_n[256];
	uint8_t * n = (uint8_t *)key->n;
	uint32_t i = 0;
	int32_t ret = 0;
	asn1_info info;

	uint32_t len_header = 0;
	uint8_t * authattr = NULL;
	uint32_t len_authattr = 0;

	//dbg_printf(PRN_RES,"start to verify\n");

	if(0 != simple_asn1_parse(signature, &info, 3))
		return 1;

	authattr = info.start;
	len_authattr = info.tag_len + info.len_len + info.data_len;
	//1.  caculate hash
	//dumpmem(authattr, len_authattr);
	//dbg_printf(PRN_RES,"######sha start [%6d] \n", get_time_ms());
	sha256_caculate((uint8_t *)src, src_len, authattr, len_authattr, digest);
	//dbg_printf(PRN_RES,"######sha end [%6d] \n", get_time_ms());
	//dumpmem(digest, SHA256_DIGEST_LENGTH);
	//2. generate e
	for(i = 0; i < 252; i++) {
		key_e[i] = 0;
	}
	key_e[252] = key->exponent[3];
	key_e[253] = key->exponent[2];
	key_e[254] = key->exponent[1];
	key_e[255] = key->exponent[0];

	//3. revert n
	for(i = 0; i < 256; i++) {
		key_n[i] = n[255 - i];
	}
	//4.  caculate c
	if(0 != simple_asn1_parse((signature + len_header), &info, 4))
		return 1;

	//dumpmem(info.data, info.data_len);
	ret = MV_RSAPrivateBlock(buff, &output_len, info.data, info.data_len, key_e, key_n);
	//dbg_printf(PRN_RES,"######mod end [%6d] \n", get_time_ms());
	//5. pkcs padding
	//FIXME: maybe not needed

	//6. compare hash
	//dumpmem(buff, 256);
	for(i = 0; i < 32; i++) {
		if(digest[i] != buff[256 -32 + i])
			ret = 1;
	}
	return ret;
}

int android_verify(uint8_t * src, uint32_t src_len,
						uint8_t * signature, uint8_t * keystore, uint8_t * buff)
{
	return verify(src, src_len, signature, keystore, buff);
}
