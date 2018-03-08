/*!
 * @file cryptocoin.h
 * @brief This is the external i/f for the CRYPTO coin example
 */
 
/* 
 * Copyright (c) 2017 stephen.farrell@cs.tcd.ie
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 */

#ifndef CRYPTOCOIN_H_INCLUDED
#define CRYPTOCOIN_H_INCLUDED

#define CRYPTOCOIN_BUFSIZE	1024 /// the max size of a CRYPTOCOIN
#define CRYPTOCOIN_PROGRESS /// define this to get a mining progress indicator on stdout

#define CRYPTOCOIN_GOOD		0 /// a good coin result
#define CRYPTOCOIN_BAD		1 /// a bad coin result

#define CRYPTOCOIN_GENERR 	-1 /// an error to use if we've nothing better

#define CRYPTOCOIN_MAXITER 1024*1024 /// the highest number of attepts we'll make

/*!
 * @brief our basic crypto coin type
 * 
 * This structure describes a crypto coin.
 * Fields are flattened as usual, lengths use network byte order.
 * The hash is over the fields that preceed it in the struct.
 * The rightmost 'bits' bits of the hash value must be zero.
 * The signature is over the fields that proceed it in the struct.
 * All length fields, except 'bits' are in octets
 *
 */
typedef struct cryptocoin_t_defn {
	/// specifies all algorithms 
	int ciphersuite; 
	/// specifies the zero-bit run length needed in the hashval
	int bits; 
	/// length of the public key
	int keylen; 
	/// public key value
	unsigned char *keyval; 
	/// length of nonce
	int noncelen; 
	unsigned char *nonceval;
	/// length of hashval
	int hashlen; 
	/// hash value with 'bits' of the LSBs having a value of zero 
	unsigned char *hashval; 
	/// signature length
	int siglen; 
	/// signature value
	unsigned char *sigval; 
} cryptocoin_t;

# define CRYPTOCOIN_CS_0 0 /// ciphersuite zero, ecdsa/p256/sha256/

/*!
 * @brief return out best guess error string
 * @param errno is an errno, presumably returned from somewhere else in this API
 * @return error string (a const)
 *
 * Error string handler. Good to use unique error numbers for all different conditions
 */
const char *cryptocoin_err(int errno);

/*!
 * @brief make a coin
 * @param bits specifies how many bits need to be zero in the hash-output
 * @param buf is an allocated buffer for the coid
 * @param buflen is an in/out parameter reflecting the buffer-size/actual-coin-size 
 * @return zero for success, non-zero for fail (note: success != good coin!)
 *
 * Make me a coin of the required quality/strength
 *
 */
int cryptocoin_make(int bits, unsigned char *buf, int *buflen);

/*!
 * @brief check a coin
 * @param bits specifies how many bits need to be zero in the hash-output
 * @param buf is an allocated buffer for the coid
 * @param buflen specifies the input coin size
 * @param res contains the result of checking the coin
 * @return zero for success, non-zero for fail (note: success != good coin!)
 *
 * Make me a coin of the required quality/strength
 *
 */
int cryptocoin_check(int bits, unsigned char *buf, int buflen, int *res);

#endif /* CRYPTOCOIN_H_INCLUDED */


