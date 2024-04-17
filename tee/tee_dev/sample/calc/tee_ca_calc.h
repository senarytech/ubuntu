#ifndef _TEE_CA_CALC_H_
#define _TEE_CA_CALC_H_

/** initialize calc
 *
 * @retval 0	success
 * @retval <0	fail
 */
int CalcInitialize(void);
void CalcFinalize(void);

int CalcAdd(int a, int b);
int CalcSub(int a, int b);
int CalcMul(int a, int b);
int CalcDiv(int a, int b);
int CalcInc(int a);
int CalcDec(int a);

/** output a string in secure world.
 *
 * @param str	string to output
 *
 * @return size have been output
 */
int CalcEcho(const char *str);

/** convert a number to a string in hex, dec, oct, bin.
 *
 * @param buf	buffer to return the result. size must be >= 33.
 * @param a	number to cnvernt
 * @param base	can be 2(bin), 8(oct), 10(dec), 16(hex)
 *
 * @retval 0	fail to convert.
 * @retval !0	length of the string.
 */
int CalcConvert(char *buf, int a, int base);

/** reverser the data bit-by-bit in a buffer.
 *
 * it's for test only.
 *
 * @param out	output buffer
 * @param in	input buffer
 * @param len	size of data
 *
 * @retval 0	fail to reverse the data
 * @retval !0	length of data be reversed, should be same as len
 */
int CalcReverse(void *out, const void *in, int len);

#endif /* _TEE_CA_CALC_H_ */
