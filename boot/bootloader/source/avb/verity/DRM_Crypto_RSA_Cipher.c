/*
 * NDA AND NEED-TO-KNOW REQUIRED
 *
 * Copyright © 2013-2018 Synaptics Incorporated. All rights reserved.
 *
 * This file contains information that is proprietary to Synaptics
 * Incorporated ("Synaptics"). The holder of this file shall treat all
 * information contained herein as confidential, shall use the
 * information only for its intended purpose, and shall not duplicate,
 * disclose, or disseminate any of this information in any manner
 * unless Synaptics has otherwise provided express, written
 * permission.
 *
 * Use of the materials may require a license of intellectual property
 * from a third party or from Synaptics. This file conveys no express
 * or implied licenses to any intellectual property rights belonging
 * to Synaptics.
 *
 * INFORMATION CONTAINED IN THIS DOCUMENT IS PROVIDED "AS-IS," AND
 * SYNAPTICS EXPRESSLY DISCLAIMS ALL EXPRESS AND IMPLIED WARRANTIES,
 * INCLUDING ANY IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE, AND ANY WARRANTIES OF NON-INFRINGEMENT OF ANY
 * INTELLECTUAL PROPERTY RIGHTS. IN NO EVENT SHALL SYNAPTICS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, PUNITIVE, OR
 * CONSEQUENTIAL DAMAGES ARISING OUT OF OR IN CONNECTION WITH THE USE
 * OF THE INFORMATION CONTAINED IN THIS DOCUMENT, HOWEVER CAUSED AND
 * BASED ON ANY THEORY OF LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * NEGLIGENCE OR OTHER TORTIOUS ACTION, AND EVEN IF SYNAPTICS WAS
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. IF A TRIBUNAL OF
 * COMPETENT JURISDICTION DOES NOT PERMIT THE DISCLAIMER OF DIRECT
 * DAMAGES OR ANY OTHER DAMAGES, SYNAPTICS' TOTAL CUMULATIVE LIABILITY
 * TO ANY PARTY SHALL NOT EXCEED ONE HUNDRED U.S. DOLLARS.
 */
////////////////////////////////////////////////////////////////////////////////
//! \file DRM_Crypto_RSA_Cipher.c
//! \brief implement of RSA algorithm relative functions for Crypto library. 
//!
//! Purpose:
//!
//!  Note:
////////////////////////////////////////////////////////////////////////////////
#include "DRM_Crypto.h"
#include "DRM_Crypto_SHA1.h"
#include "string.h"

/***************************************************************
*	RSA
*
*			 RSA Public Key Type: R_RSA_PUBLIC_KEY
*			RSA Private Key Type: R_RSA_PRIVATE_KEY
***************************************************************/
/* RSA key lengths.*/
///////////////////////////////////////////////////////////////////////////
//		Natural Numbers Routines
///////////////////////////////////////////////////////////////////////////


/* 
Constants.

   Note: MAX_NN_DIGITS is long enough to hold any RSA modulus, plus
   one more digit as required by R_GeneratePEMKeys (for n and phiN,
   whose lengths must be even). All natural numbers have at most
   MAX_NN_DIGITS digits, except for double-length intermediate values
   in NN_Mult (t), NN_ModMult (t), NN_ModInv (w), and NN_Div (c).
*/
/* Length of digit in bits */
#define NN_DIGIT_BITS 32
#define NN_HALF_DIGIT_BITS 16
/* Length of digit in bytes */
#define NN_DIGIT_LEN (NN_DIGIT_BITS / 8)
/* Maximum length in digits */
#define MAX_NN_DIGITS \
  ((MAX_RSA_MODULUS_LEN + NN_DIGIT_LEN - 1) / NN_DIGIT_LEN + 1)
/* Maximum digits */
#define MAX_NN_DIGIT 0xffffffff
#define MAX_NN_HALF_DIGIT 0xffff

/* Macros. */
#define LOW_HALF(x) ((x) & MAX_NN_HALF_DIGIT)
#define HIGH_HALF(x) (((x) >> NN_HALF_DIGIT_BITS) & MAX_NN_HALF_DIGIT)
#define TO_HIGH_HALF(x) (((UINT32)(x)) << NN_HALF_DIGIT_BITS)
#define DIGIT_MSB(x) (UINT32)(((x) >> (NN_DIGIT_BITS - 1)) & 1)
#define DIGIT_2MSB(x) (UINT32)(((x) >> (NN_DIGIT_BITS - 2)) & 3)

/* 
CONVERSIONS
   NN_Decode (a, digits, b, len)   Decodes character string b into a.
   NN_Encode (a, len, b, digits)   Encodes a into character string b.

ASSIGNMENTS
   NN_Assign (a, b, digits)        Assigns a = b.
   NN_ASSIGN_DIGIT (a, b, digits)  Assigns a = b, where b is a digit.
   NN_AssignZero (a, b, digits)    Assigns a = 0.
   NN_Assign2Exp (a, b, digits)    Assigns a = 2^b.
     
ARITHMETIC OPERATIONS
   NN_Add (a, b, c, digits)        Computes a = b + c.
   NN_Sub (a, b, c, digits)        Computes a = b - c.
   NN_Mult (a, b, c, digits)       Computes a = b * c.
   NN_LShift (a, b, c, digits)     Computes a = b * 2^c.
   NN_RShift (a, b, c, digits)     Computes a = b / 2^c.
   NN_Div (a, b, c, cDigits, d, dDigits)  Computes a = c div d and b = c mod d.

NUMBER THEORY
   NN_Mod (a, b, bDigits, c, cDigits)  Computes a = b mod c.
   NN_ModMult (a, b, c, d, digits) Computes a = b * c mod d.
   NN_ModExp (a, b, c, cDigits, d, dDigits)  Computes a = b^c mod d.
   NN_ModInv (a, b, c, digits)     Computes a = 1/b mod c.
   NN_Gcd (a, b, c, digits)        Computes a = gcd (b, c).

OTHER OPERATIONS
   NN_EVEN (a, digits)             Returns 1 iff a is even.
   NN_Cmp (a, b, digits)           Returns sign of a - b.
   NN_EQUAL (a, digits)            Returns 1 iff a = b.
   NN_Zero (a, digits)             Returns 1 iff a = 0.
   NN_Digits (a, digits)           Returns significant length of a in digits.
   NN_Bits (a, digits)             Returns significant length of a in bits.
*/

#if 0
static void TraceHex(const unsigned char* pbData, unsigned int uSize )
{
    int i = 0, j = 0;
        for ( i = 0; i + 8 < uSize; i += 8 )
        {
            for ( j = 0; j < 8; j ++)
            {
                printf("%02x,", pbData[i + j]);
            }
            printf("\n");
        }
        if ( j < uSize )
        {
            for ( j = i; j < uSize; j ++)
            {
                printf("%02x,", pbData[j]);
            }
            printf(",\n");
        }
}
#endif

void NN_Decode(UINT32 *, UINT32, const UINT8 *, const UINT32);
void NN_Encode(UINT8 *, UINT32, UINT32 *, UINT32);

void NN_Assign(UINT32 *, UINT32 *, UINT32);
void NN_AssignZero(UINT32 *, UINT32);
void NN_Assign2Exp(UINT32 *, UINT32, UINT32);

UINT32 NN_Add(UINT32 *, UINT32 *, UINT32 *, UINT32);
UINT32 NN_Sub(UINT32 *, UINT32 *, UINT32 *, UINT32);
void NN_Mult(UINT32 *, UINT32 *, UINT32 *, UINT32);
void NN_Div(UINT32 *, UINT32 *, UINT32 *, UINT32, UINT32 *, UINT32);
UINT32 NN_LShift(UINT32 *, UINT32 *, UINT32, UINT32);
UINT32 NN_RShift(UINT32 *, UINT32 *, UINT32, UINT32);

void NN_Mod(UINT32 *, UINT32 *, UINT32, UINT32 *, UINT32);
void NN_ModMult(UINT32 *, UINT32 *, UINT32 *, UINT32 *, UINT32);
void NN_ModExp(UINT32 *, UINT32 *, UINT32 *, UINT32, UINT32 *, UINT32);
void NN_ModInv(UINT32 *, UINT32 *, UINT32 *, UINT32);
void NN_Gcd(UINT32 *, UINT32 *, UINT32 *, UINT32);

INT32 NN_Cmp(UINT32 *, UINT32 *, UINT32);
INT32 NN_Zero(UINT32 *, UINT32);
UINT32 NN_Bits(UINT32 *, UINT32);
UINT32 NN_Digits(UINT32 *, UINT32);

///////////////////////////////////////////////////////////////////////////
//		Digit Arithmetic Routines
///////////////////////////////////////////////////////////////////////////

#define NN_ASSIGN_DIGIT(a, b, digits) {NN_AssignZero (a, digits); a[0] = b;}
#define NN_EQUAL(a, b, digits) (! NN_Cmp (a, b, digits))
#define NN_EVEN(a, digits) (((digits) == 0) || ! (a[0] & 1))

void NN_DigitMult(UINT32 [2], UINT32, UINT32);
void NN_DigitDiv(UINT32 *, UINT32 [2], UINT32);


#if 0
static UINT32  l_ReverseMsg1024(UINT8* pbMsg, UINT32 uLen)
{
	UINT32 i = 0;
	UINT8 tmp = 0;
	UINT8* pbMsgEnd = pbMsg + uLen -1;
	if(uLen%2 != 0)
		printf("Invalid arguments for ReverseMsg1024\n");
	for(i=0; i<(uLen/2); i++)
	{
		tmp = (*pbMsgEnd);
		(*pbMsgEnd) = (*pbMsg);
        	(*pbMsg) = tmp;
		pbMsg ++;
		pbMsgEnd --;
	}
	return 0;

}
#endif

///////////////////////////////////////////////////////////////////////////
//		RSAES_PKCS1
///////////////////////////////////////////////////////////////////////////

//#if SEPERATE_RSA_PADDING
#if 1
#if 0
UINT32 emsa_pkcs1_cipher_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen)
{

	UINT32 i = 0;
	UINT8  PS = 0;

    	EM[0] = 0x00;
	EM[1] = 0x02;
	srand( (UINT32)time(NULL) );
	for (i = 0; i < emLen- mLen - 3; i++)
	{
		PS = rand() & 0xff;
		while (PS == 0)
			PS = rand() & 0xff;

		EM[2 + i] = PS;
	}
	EM[2 + i] = 0x00;
	for (i = 0; i < mLen; i++)
		EM[emLen - mLen + i] = M[i];
       return S_OK;
}
#endif

UINT32 emsa_pkcs1_cipher_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32* mLen)
{
       UINT32 i = 0, j = 0;

	if ((EM[0] != 0x00) || (EM[1] != 0x02))
		return E_FAIL;
	j = 2;
	while ((j < emLen) && (EM[j] != 0))
		j++;
	if (j == emLen)
		return E_FAIL;
	for (i = 0; i < emLen - j; i++)
		M[i] = EM[j + 1 + i];
	*mLen = emLen - j - 1;
       return S_OK;
}


UINT32 emsa_pkcs1_md5sha1_Encode( UINT8 *M,  UINT32 mLen, UINT8 *EM,  UINT32 emLen)
{
	UINT32 uSize_FF = 0; /*0xFF size*/	
	UINT32 i = 0;
    
	EM[0] = 0x00;
	EM[1] = 0x01;
	
	uSize_FF = emLen - 2 - 36 -1; /*2 is the header, 36 is md5+sha1, and 1 is end flag: 0x00*/
	
	for(i=0; i < uSize_FF; i++)
	{
		EM[2+i] = 0xff;
	}	
	EM[2+ uSize_FF] =0x00;

	memcpy(EM+2+uSize_FF+1, M, mLen);

        return S_OK;
}


UINT32 emsa_pkcs1_md5sha1_Verify( UINT8 *EM, UINT32 emLen, UINT8 *M,  UINT32 mLen, BOOL *pbResult)
{

	UINT32 i = 0;
	UINT32 uSize_FF = 0;

	*pbResult = TRUE;
	uSize_FF = emLen - 2 - mLen - 1;
	/*emsa_pkcs1_Encode(message, mLen, EMPrime, emLen);*/
	if(EM[0] != 0x00 || EM[1] != 0x01)
		*pbResult = FALSE;
	
	/*Check the header flag*/
	if(EM[0] != 0x00 || EM[1] != 0x01)
		*pbResult = FALSE;
	
	
	/*check the 0xff body*/
	for(i= 0; i<uSize_FF; i++)
		if(EM[2+i] != 0xff)
			*pbResult = FALSE;

	/*check the end flag*/
	if(EM[2+uSize_FF] != 0x00)
		*pbResult = FALSE;
	/*check the md5+sha1 digest*/
	for(i=0; i<mLen; i++)
		if(EM[2+uSize_FF + 1 +i] != M[i])
			*pbResult = FALSE;
        return S_OK;
}


#endif


///////////////////////////////////////////////////////////////////////////
//		RSASSA_PKCS1
///////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
//    Function Name : EncryptWithRSAPKCS1
//    Author        : Ya Bin Dang
//    Data          : Thu., Sep 23, 2004
//    Return Value  : 
//		true		: Operation successful
//		false		: Operation failed
//    Parameters    :
//    [IN]     RSA_PUBLIC_KEY *pPublicKey : Public key
//    [IN]     UINT8 *pbPlaintext      : Plaintext
//    [IN]     UINT32 uPltLen          : Length of plaintext
//    [OUT]    UINT8 *pbCiphertext     : Ciphertext
//    [IN/OUT] UINT32 *puCptLen        : Input ciphertext buffer's length,
//										    Output ciphertext's length
//    Remarks       :
//		Length of plaintext SHOULD be no more than RSA_MODULUS_LEN-11.
//		Length of ciphertext SHOULD be RSA_MODULUS_LEN.
////////////////////////////////////////////////////////////////////////////



////////////////////////////////////////////////////////////////////////////
//    Function Name : SignWithRSAPKCS1
//    Author        : Ya Bin Dang
//    Data          : Thu., Sep 23, 2004
//    Return Value  : 
//		true		: Operation successful
//		false		: Operation failed
//    Parameters    :
//    [IN]     RSA_PRIVATE_KEY *pPrivateKey : Private key
//    [IN]     UINT8 *pbMsgDgt           : Message digest
//    [IN]     UINT32 uMsgDgtLen         : Length of message digest
//    [OUT]    UINT8 *pbSignature        : Signature
//    [IN/OUT] UINT32 *puSgnLen          : Input signature buffer's length,
//											  Output signature's length
//    Remarks       :
//		Length of message digest SHOULD be SHA1_DIGEST_LEN.
//		Length of signature SHOULD be RSA_MODULUS_LEN.
////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
//    Function Name : VerifyWithRSAPKCS1
//    Author        : Ya Bin Dang
//    Data          : Thu., Sep 23, 2004
//    Return Value  : 
//		true		: Operation successful
//		false		: Operation failed
//    Parameters    :
//    [IN]  RSA_PUBLIC_KEY *pPublicKey : Public key
//    [IN]  UINT8 *pbMsgDgt         : Message digest
//    [IN]  UINT32 uMsgDgtLen       : Length of message digest
//    [IN]  UINT8 *pbSignature      : Signature
//    [IN]  UINT32 uSgnLen          : Length of signature
//    Remarks       :
//		Length of message digest SHOULD be SHA1_DIGEST_LEN.
//		Length of signature SHOULD be RSA_MODULUS_LEN.
////////////////////////////////////////////////////////////////////////////


HRESULT MV_RSA_PKCS1_SetKeyParm(const UCHAR *pbData, 
                           UINT uSize, 
                           MV_CRYPTO_PARM uParm, 
                           PMV_CRYPTO_KEY_CTX pKeyCtx )
{
	UINT uModSize = uSize/2;
	if ( pbData == NULL || uSize <= 0 || !IS_VALID_KEY_CTX(pKeyCtx))
	{
		MV_CRYPTO_ERROR(CRYPTO_LEVEL1 "Invalid argument\n");
		return E_INVALIDARG;
	}
	if ( uParm == MV_RSA_PUBKEY_PARM )
	{
		if ( uModSize != RSA_MODULUS_LEN/2 && uModSize != RSA_MODULUS_LEN &&  uModSize != 2*RSA_MODULUS_LEN  )
		{
			MV_CRYPTO_ERROR(CRYPTO_LEVEL1 "RSA PKCS1 public key length is wrong\n");
			return E_NOTIMPL;
		}
		pKeyCtx->rsaKeyData.pub_key.bits= (uSize/2)*8;

		memcpy( pKeyCtx->rsaKeyData.pub_key.exponent, pbData, uModSize);

		memcpy( pKeyCtx->rsaKeyData.pub_key.modulus, pbData + uModSize, uModSize);
		 //TraceHex(pKeyCtx->rsaKeyData.pub_key.exponent, 512 );

	}
	else if ( uParm == MV_RSA_PRVKEY_PARM )
	{
		if ( uModSize != RSA_MODULUS_LEN/2 && uModSize != RSA_MODULUS_LEN &&  uModSize != 2*RSA_MODULUS_LEN )
		{
			MV_CRYPTO_ERROR(CRYPTO_LEVEL1 
				"RSA PKCS1 private key length is wrong\n");
			return E_NOTIMPL;
		}
		pKeyCtx->rsaKeyData.priv_key.bits= (uSize/2)*8;

		memcpy( pKeyCtx->rsaKeyData.priv_key.exponent, pbData, uModSize);

		memcpy( pKeyCtx->rsaKeyData.priv_key.modulus, pbData + uModSize, uModSize);
	}
	else
	{
		MV_CRYPTO_ERROR(CRYPTO_LEVEL1 "Unknown parameter type 0x%x\n", uParm);
		return E_NOTIMPL;
	}
	return S_OK;
}

/*
//ROM API
HRESULT MV_DecWithRSAPKCS1(const RSA_PRIVATE_KEY *pPrivateKey, 
									const UINT8 *pbCiphertext, const UINT32 uCptLen, 
									UINT8 *pbPlaintext, UINT32 *puPltLen)
{
	if (pPrivateKey == NULL)
		return E_FAIL;
	if (pbCiphertext == NULL)
		return E_FAIL;
	if (pbPlaintext == NULL)
		return E_FAIL;
	if (uCptLen > RSA_MODULUS_LEN)
		return E_FAIL;
	if (puPltLen == NULL)
		return E_FAIL;

	if (rsa_pkcs1_Decryption((const R_RSA_PRIVATE_KEY*)pPrivateKey, pbPlaintext, puPltLen, pbCiphertext, uCptLen) != S_OK)
		return E_FAIL;

	return S_OK;
}


*/

///////////////////////////////////////////////////////////////////////////
//		Digit Arithmetic Routines
///////////////////////////////////////////////////////////////////////////

/******************************
*	Computes a = b * c, where b and c are digits.
*
*   Lengths: a[2].
******************************/
void NN_DigitMult (UINT32 a[2], UINT32 b, UINT32 c)
{
  UINT32 t, u;
  UINT16 bHigh, bLow, cHigh, cLow;

  bHigh = (UINT16)HIGH_HALF (b);
  bLow = (UINT16)LOW_HALF (b);
  cHigh = (UINT16)HIGH_HALF (c);
  cLow = (UINT16)LOW_HALF (c);

  a[0] = (UINT32)bLow * (UINT32)cLow;
  t = (UINT32)bLow * (UINT32)cHigh;
  u = (UINT32)bHigh * (UINT32)cLow;
  a[1] = (UINT32)bHigh * (UINT32)cHigh;
  
  if ((t += u) < u)
    a[1] += TO_HIGH_HALF (1);
  u = TO_HIGH_HALF (t);
  
  if ((a[0] += u) < u)
    a[1]++;
  a[1] += HIGH_HALF (t);
}

/******************************
*	Sets a = b / c, where a and c are digits.
*
*	Lengths: b[2].
*	Assumes b[1] < c and HIGH_HALF (c) > 0. For efficiency, c should be normalized.
******************************/
void NN_DigitDiv (UINT32 *a, UINT32 b[2], UINT32 c)
{
  UINT32 t[2], u, v;
  UINT16 aHigh, aLow, cHigh, cLow;

  cHigh = (UINT16)HIGH_HALF (c);
  cLow = (UINT16)LOW_HALF (c);

  t[0] = b[0];
  t[1] = b[1];

  /* Underestimate high half of quotient and subtract. */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aHigh = (UINT16)HIGH_HALF (t[1]);
  else
    aHigh = (UINT16)(t[1] / (cHigh + 1));
  u = (UINT32)aHigh * (UINT32)cLow;
  v = (UINT32)aHigh * (UINT32)cHigh;
  if ((t[0] -= TO_HIGH_HALF (u)) > (MAX_NN_DIGIT - TO_HIGH_HALF (u)))
    t[1]--;
  t[1] -= HIGH_HALF (u);
  t[1] -= v;

  /* Correct estimate. */
  while ((t[1] > cHigh) ||
         ((t[1] == cHigh) && (t[0] >= TO_HIGH_HALF (cLow)))) {
    if ((t[0] -= TO_HIGH_HALF (cLow)) > MAX_NN_DIGIT - TO_HIGH_HALF (cLow))
      t[1]--;
    t[1] -= cHigh;
    aHigh++;
  }

  /* Underestimate low half of quotient and subtract. */
  if (cHigh == MAX_NN_HALF_DIGIT)
    aLow = (UINT16)LOW_HALF (t[1]);
  else
    aLow = 
      (UINT16)((TO_HIGH_HALF (t[1]) + HIGH_HALF (t[0])) / (cHigh + 1));
  u = (UINT32)aLow * (UINT32)cLow;
  v = (UINT32)aLow * (UINT32)cHigh;
  if ((t[0] -= u) > (MAX_NN_DIGIT - u))
    t[1]--;
  if ((t[0] -= TO_HIGH_HALF (v)) > (MAX_NN_DIGIT - TO_HIGH_HALF (v)))
    t[1]--;
  t[1] -= HIGH_HALF (v);

  /* Correct estimate. */
  while ((t[1] > 0) || ((t[1] == 0) && t[0] >= c)) {
    if ((t[0] -= c) > (MAX_NN_DIGIT - c))
      t[1]--;
    aLow++;
  }
  
  *a = TO_HIGH_HALF (aHigh) + aLow;
}

///////////////////////////////////////////////////////////////////////////
//		Natural Numbers Routines
///////////////////////////////////////////////////////////////////////////

static UINT32 NN_AddDigitMult(UINT32 *, UINT32 *, UINT32, UINT32 *, UINT32);
static UINT32 NN_SubDigitMult(UINT32 *, UINT32 *, UINT32, UINT32 *, UINT32);

static UINT32 NN_DigitBits (UINT32);

/******************************
*	Decodes character string b into a, where character string is ordered from most to least significant.
*
*	Lengths: a[digits], b[len].
*	Assumes b[i] = 0 for i < len - digits * NN_DIGIT_LEN. (Otherwise most significant bytes are truncated.)
******************************/
void NN_Decode (UINT32 *a, UINT32 digits, const UINT8 *b, const UINT32 len)
{
  UINT32 t;
  INT32 j;
  UINT32 i, u;
  
  for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
    t = 0;
    for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
      t |= ((UINT32)b[j]) << u;
    a[i] = t;
  }
  
  for (; i < digits; i++)
    a[i] = 0;
}

/******************************
*	Encodes b into character string a, where character string is ordered from most to least significant.
*
*	Lengths: a[len], b[digits].
*	Assumes NN_Bits (b, digits) <= 8 * len. (Otherwise most significant digits are truncated.)
******************************/
void NN_Encode (UINT8 *a, UINT32 len, UINT32 *b, UINT32 digits)
{
  UINT32 t;
  INT32 j;
  UINT32 i, u;

  for (i = 0, j = len - 1; i < digits && j >= 0; i++) {
    t = b[i];
    for (u = 0; j >= 0 && u < NN_DIGIT_BITS; j--, u += 8)
      a[j] = (UINT8)(t >> u);
  }

  for (; j >= 0; j--)
    a[j] = 0;
}

/******************************
*	Assigns a = b.
*
*	Lengths: a[digits], b[digits].
******************************/
void NN_Assign (UINT32 *a, UINT32 *b, UINT32 digits)
{
  UINT32 i;

  for (i = 0; i < digits; i++)
    a[i] = b[i];
}

/******************************
*	Assigns a = 0.
*
*	Lengths: a[digits].
******************************/
void NN_AssignZero (UINT32 *a, UINT32 digits)
{
  UINT32 i;

  for (i = 0; i < digits; i++)
    a[i] = 0;
}

/******************************
*	Assigns a = 2^b.
*
*	Lengths: a[digits].
*	Requires b < digits * NN_DIGIT_BITS.
******************************/
void NN_Assign2Exp (UINT32 *a, UINT32 b, UINT32 digits)
{
  NN_AssignZero (a, digits);

  if (b >= digits * NN_DIGIT_BITS)
    return;

  a[b / NN_DIGIT_BITS] = (UINT32)1 << (b % NN_DIGIT_BITS);
}

/******************************
*	Computes a = b + c. Returns carry.
*
*	Lengths: a[digits], b[digits], c[digits].
******************************/
UINT32 NN_Add (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 digits)
{
  UINT32 ai, carry;
  UINT32 i;

  carry = 0;

  for (i = 0; i < digits; i++) {
    if ((ai = b[i] + carry) < carry)
      ai = c[i];
    else if ((ai += c[i]) < c[i])
      carry = 1;
    else
      carry = 0;
    a[i] = ai;
  }

  return (carry);
}

/******************************
*	Computes a = b - c. Returns borrow.
*
*	Lengths: a[digits], b[digits], c[digits].
******************************/
UINT32 NN_Sub (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 digits)
{
  UINT32 ai, borrow;
  UINT32 i;

  borrow = 0;

  for (i = 0; i < digits; i++) {
    if ((ai = b[i] - borrow) > (MAX_NN_DIGIT - borrow))
      ai = MAX_NN_DIGIT - c[i];
    else if ((ai -= c[i]) > (MAX_NN_DIGIT - c[i]))
      borrow = 1;
    else
      borrow = 0;
    a[i] = ai;
  }

  return (borrow);
}

/******************************
*	Computes a = b * c.
*
*	Lengths: a[2*digits], b[digits], c[digits].
*	Assumes digits < MAX_NN_DIGITS.
******************************/
void NN_Mult (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 digits)
{
  UINT32 t[2*MAX_NN_DIGITS];
  UINT32 bDigits, cDigits, i;

  NN_AssignZero (t, 2 * digits);
  
  bDigits = NN_Digits (b, digits);
  cDigits = NN_Digits (c, digits);

  for (i = 0; i < bDigits; i++)
    t[i+cDigits] += NN_AddDigitMult (&t[i], &t[i], b[i], c, cDigits);
  
  NN_Assign (a, t, 2 * digits);
  
  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)t, 0, sizeof (t));
}

/******************************
*	Computes a = b * 2^c (i.e., shifts left c bits), returning carry.
*
*	Lengths: a[digits], b[digits].
*	Requires c < NN_DIGIT_BITS.
******************************/
UINT32 NN_LShift (UINT32 *a, UINT32 *b, UINT32 c, UINT32 digits)
{
  UINT32 bi, carry;
  UINT32 i, t;
  
  if (c >= NN_DIGIT_BITS)
    return (0);
  
  t = NN_DIGIT_BITS - c;

  carry = 0;

  for (i = 0; i < digits; i++) {
    bi = b[i];
    a[i] = (bi << c) | carry;
    carry = c ? (bi >> t) : 0;
  }
  
  return (carry);
}

/******************************
*	Computes a = c div 2^c (i.e., shifts right c bits), returning carry.
*
*	Lengths: a[digits], b[digits].
*	Requires: c < NN_DIGIT_BITS.
******************************/
UINT32 NN_RShift (UINT32 *a, UINT32 *b, UINT32 c, UINT32 digits)
{
  UINT32 bi, carry;
  INT32 i;
  UINT32 t;
  
  if (c >= NN_DIGIT_BITS)
    return (0);
  
  t = NN_DIGIT_BITS - c;

  carry = 0;

  for (i = digits - 1; i >= 0; i--) {
    bi = b[i];
    a[i] = (bi >> c) | carry;
    carry = c ? (bi << t) : 0;
  }
  
  return (carry);
}

/******************************
*	Computes a = c div d and b = c mod d.
*
*	Lengths: a[cDigits], b[dDigits], c[cDigits], d[dDigits].
*	Assumes d > 0, cDigits < 2 * MAX_NN_DIGITS, dDigits < MAX_NN_DIGITS.
******************************/
void NN_Div (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 cDigits, UINT32 *d, UINT32 dDigits)
{
  UINT32 ai, cc[2*MAX_NN_DIGITS+1], dd[MAX_NN_DIGITS], t;
  INT32 i;
  UINT32 ddDigits, shift;
  
  ddDigits = NN_Digits (d, dDigits);
  if (ddDigits == 0)
    return;
  
  /* Normalize operands. */
  shift = NN_DIGIT_BITS - NN_DigitBits (d[ddDigits-1]);
  NN_AssignZero (cc, ddDigits);
  cc[cDigits] = NN_LShift (cc, c, shift, cDigits);
  NN_LShift (dd, d, shift, ddDigits);
  t = dd[ddDigits-1];
  
  NN_AssignZero (a, cDigits);

  for (i = cDigits-ddDigits; i >= 0; i--) {
    /* Underestimate quotient digit and subtract. */
    if (t == MAX_NN_DIGIT)
      ai = cc[i+ddDigits];
    else
      NN_DigitDiv (&ai, &cc[i+ddDigits-1], t + 1);
    cc[i+ddDigits] -= NN_SubDigitMult (&cc[i], &cc[i], ai, dd, ddDigits);

    /* Correct estimate. */
    while (cc[i+ddDigits] || (NN_Cmp (&cc[i], dd, ddDigits) >= 0)) {
      ai++;
      cc[i+ddDigits] -= NN_Sub (&cc[i], &cc[i], dd, ddDigits);
    }
    
    a[i] = ai;
  }
  
  /* Restore result. */
  NN_AssignZero (b, dDigits);
  NN_RShift (b, cc, shift, ddDigits);

  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)cc, 0, sizeof (cc));
  //R_memset ((POINTER)dd, 0, sizeof (dd));
}

/******************************
*	Computes a = b mod c.
*
*	Lengths: a[cDigits], b[bDigits], c[cDigits].
*	Assumes c > 0, bDigits < 2 * MAX_NN_DIGITS, cDigits < MAX_NN_DIGITS.
******************************/
void NN_Mod (UINT32 *a, UINT32 *b, UINT32 bDigits, UINT32 *c, UINT32 cDigits)
{
  UINT32 t[2 * MAX_NN_DIGITS];
  
  NN_Div (t, a, b, bDigits, c, cDigits);
  
  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)t, 0, sizeof (t));
}

/******************************
*	Computes a = b * c mod d.
*
*	Lengths: a[digits], b[digits], c[digits], d[digits].
*	Assumes d > 0, digits < MAX_NN_DIGITS.
******************************/
void NN_ModMult (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 *d, UINT32 digits)
{
  UINT32 t[2*MAX_NN_DIGITS];

  NN_Mult (t, b, c, digits);
  NN_Mod (a, t, 2 * digits, d, digits);
  
  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)t, 0, sizeof (t));
}

/******************************
*	Computes a = b^c mod d.
*
*	Lengths: a[dDigits], b[dDigits], c[cDigits], d[dDigits].
*	Assumes d > 0, cDigits > 0, dDigits < MAX_NN_DIGITS.
******************************/
void NN_ModExp (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 cDigits, UINT32 *d, UINT32 dDigits)
{
  UINT32 bPower[3][MAX_NN_DIGITS], ci, t[MAX_NN_DIGITS];
  INT32 i;
  UINT32 ciBits, j, s;

  /* Store b, b^2 mod d, and b^3 mod d. */
  NN_Assign (bPower[0], b, dDigits);
  NN_ModMult (bPower[1], bPower[0], b, d, dDigits);
  NN_ModMult (bPower[2], bPower[1], b, d, dDigits);
  
  NN_ASSIGN_DIGIT (t, 1, dDigits);

  cDigits = NN_Digits (c, cDigits);
  for (i = cDigits - 1; i >= 0; i--) {
    ci = c[i];
    ciBits = NN_DIGIT_BITS;
    
    /* Scan past leading zero bits of most significant digit. */
    if (i == (INT32)(cDigits - 1)) {
      while (! DIGIT_2MSB (ci)) {
        ci <<= 2;
        ciBits -= 2;
      }
    }

    for (j = 0; j < ciBits; j += 2, ci <<= 2) {
      /* Compute t = t^4 * b^s mod d, where s = two MSB's of ci. */
      NN_ModMult (t, t, t, d, dDigits);
      NN_ModMult (t, t, t, d, dDigits);
      if ((s = DIGIT_2MSB (ci)) != 0)
        NN_ModMult (t, t, bPower[s-1], d, dDigits);
    }
  }
  
  NN_Assign (a, t, dDigits);
  
  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)bPower, 0, sizeof (bPower));
  //R_memset ((POINTER)t, 0, sizeof (t));
}

/******************************
*	Compute a = 1/b mod c, assuming inverse exists.
*
*	Lengths: a[digits], b[digits], c[digits].
*	Assumes gcd (b, c) = 1, digits < MAX_NN_DIGITS.
******************************/
void NN_ModInv (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 digits)
{
  UINT32 q[MAX_NN_DIGITS], t1[MAX_NN_DIGITS], t3[MAX_NN_DIGITS],
    u1[MAX_NN_DIGITS], u3[MAX_NN_DIGITS], v1[MAX_NN_DIGITS],
    v3[MAX_NN_DIGITS], w[2*MAX_NN_DIGITS];
  INT32 u1Sign;

  /* Apply extended Euclidean algorithm, modified to avoid negative numbers. */
  NN_ASSIGN_DIGIT (u1, 1, digits);
  NN_AssignZero (v1, digits);
  NN_Assign (u3, b, digits);
  NN_Assign (v3, c, digits);
  u1Sign = 1;

  while (! NN_Zero (v3, digits)) {
    NN_Div (q, t3, u3, digits, v3, digits);
    NN_Mult (w, q, v1, digits);
    NN_Add (t1, u1, w, digits);
    NN_Assign (u1, v1, digits);
    NN_Assign (v1, t1, digits);
    NN_Assign (u3, v3, digits);
    NN_Assign (v3, t3, digits);
    u1Sign = -u1Sign;
  }
  
  /* Negate result if sign is negative. */
  if (u1Sign < 0)
    NN_Sub (a, c, u1, digits);
  else
    NN_Assign (a, u1, digits);

  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)q, 0, sizeof (q));
  //R_memset ((POINTER)t1, 0, sizeof (t1));
  //R_memset ((POINTER)t3, 0, sizeof (t3));
  //R_memset ((POINTER)u1, 0, sizeof (u1));
  //R_memset ((POINTER)u3, 0, sizeof (u3));
  //R_memset ((POINTER)v1, 0, sizeof (v1));
  //R_memset ((POINTER)v3, 0, sizeof (v3));
  //R_memset ((POINTER)w, 0, sizeof (w));
}

/******************************
*	Computes a = gcd(b, c).
*
*	Lengths: a[digits], b[digits], c[digits].
*	Assumes b > c, digits < MAX_NN_DIGITS.
******************************/
void NN_Gcd (UINT32 *a, UINT32 *b, UINT32 *c, UINT32 digits)
{
  UINT32 t[MAX_NN_DIGITS], u[MAX_NN_DIGITS], v[MAX_NN_DIGITS];

  NN_Assign (u, b, digits);
  NN_Assign (v, c, digits);

  while (! NN_Zero (v, digits)) {
    NN_Mod (t, u, digits, v, digits);
    NN_Assign (u, v, digits);
    NN_Assign (v, t, digits);
  }

  NN_Assign (a, u, digits);

  /* Zeroize potentially sensitive information.
   */
  //R_memset ((POINTER)t, 0, sizeof (t));
  //R_memset ((POINTER)u, 0, sizeof (u));
  //R_memset ((POINTER)v, 0, sizeof (v));
}

/******************************
*	Returns sign of a - b.
*
*	Lengths: a[digits], b[digits].
******************************/
INT32 NN_Cmp (UINT32 *a, UINT32 *b, UINT32 digits)
{
  INT32 i;
  
  for (i = digits - 1; i >= 0; i--) {
    if (a[i] > b[i])
      return (1);
    if (a[i] < b[i])
      return (-1);
  }

  return (0);
}

/******************************
*	Returns nonzero iff a is zero.
*
*	Lengths: a[digits].
******************************/
INT32 NN_Zero (UINT32 *a, UINT32 digits)
{
  UINT32 i;
  
  for (i = 0; i < digits; i++)
    if (a[i])
      return (0);
    
  return (1);
}

/******************************
*	Returns the significant length of a in bits.
*
*	Lengths: a[digits].
******************************/
UINT32 NN_Bits (UINT32 *a, UINT32 digits)
{
  if ((digits = NN_Digits (a, digits)) == 0)
    return (0);

  return ((digits - 1) * NN_DIGIT_BITS + NN_DigitBits (a[digits-1]));
}

/******************************
*	Returns the significant length of a in digits.
*
*	Lengths: a[digits].
******************************/
UINT32 NN_Digits (UINT32 *a, UINT32 digits)
{
  INT32 i;
  
  for (i = digits - 1; i >= 0; i--)
    if (a[i])
      break;

  return (i + 1);
}

/******************************
*	Computes a = b + c*d, where c is a digit. Returns carry.
*
*	Lengths: a[digits], b[digits], d[digits].
******************************/
static UINT32 NN_AddDigitMult (UINT32 *a, UINT32 *b, UINT32 c, UINT32 *d, UINT32 digits)
{
  UINT32 carry, t[2];
  UINT32 i;

  if (c == 0)
    return (0);

  carry = 0;
  for (i = 0; i < digits; i++) {
    NN_DigitMult (t, c, d[i]);
    if ((a[i] = b[i] + carry) < carry)
      carry = 1;
    else
      carry = 0;
    if ((a[i] += t[0]) < t[0])
      carry++;
    carry += t[1];
  }
  
  return (carry);
}

/******************************
*	Computes a = b - c*d, where c is a digit. Returns borrow.
*
*	Lengths: a[digits], b[digits], d[digits].
******************************/
static UINT32 NN_SubDigitMult (UINT32 *a, UINT32 *b, UINT32 c, UINT32 *d, UINT32 digits)
{
  UINT32 borrow, t[2];
  UINT32 i;

  if (c == 0)
    return (0);

  borrow = 0;
  for (i = 0; i < digits; i++) {
    NN_DigitMult (t, c, d[i]);
    if ((a[i] = b[i] - borrow) > (MAX_NN_DIGIT - borrow))
      borrow = 1;
    else
      borrow = 0;
    if ((a[i] -= t[0]) > (MAX_NN_DIGIT - t[0]))
      borrow++;
    borrow += t[1];
  }
  
  return (borrow);
}

/******************************
*	Returns the significant length of a in bits, where a is a digit.
******************************/
static UINT32 NN_DigitBits (UINT32 a)
{
  UINT32 i;
  
  for (i = 0; i < NN_DIGIT_BITS; i++, a >>= 1)
    if (a == 0)
      break;
    
  return (i);
}


