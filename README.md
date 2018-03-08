# CryptoCoin
Basic version of a Cryptocurrency coin generating program written in C.

The basic idea is similar to, but a lot simpler than, the bitcoin idea of
mining ["difficulty"](https://en.bitcoin.it/wiki/Difficulty). We require that
each "cryptocoin" be unique and include the inputs to a SHA256 hash whose output value has
a selected number of the low order (rightmost) bits with zero values. Since the
output of a hash function like SHA256 is essentially random, one has to try many
times before one sees such an output, and the longer the run of zeros required,
the more attempts one needs.

To generate such outputs we include a [cryptographic nonce](https://en.wikipedia.org/wiki/Cryptographic_nonce) in the hash input value. We can vary the nonce value until we find a hash output with the required number of bits zero-valued. 

In addition we require each coin to be digitally
signed using the Elliptic Curve Digital Signature Algorithm (ECDSA) with the
p256 NIST curve. 

## Usage

To begin with, clone the repo.
```
git clone https://github.com/dooleyb1/CryptoCoin
```

Explode the mbedtls tarball.
```
tar xzvf mbedtls-2.6.0-apache.tgz
```

Build the mbedtls code.
```
cd mbedtls-2.6.0-apache.tgz
make
```

Go back up a level and compile the cryptocoin program.
```
make
```

Build a _cryptocoin_ (output to **crypto.coin**)
```
./crypto-coin -m 
```

Verify if your coin was built successfully
```
./crypto-coin
```


### Example

Here's a hexdump of an example coin with 20 bits of difficulty: 

		00,00,00,00,00,00,00,14,00,00,00,9e,30,81,9b,30
		10,06,07,2a,86,48,ce,3d,02,01,06,05,2b,81,04,00
		23,03,81,86,00,04,01,c8,46,55,6b,4e,26,bb,6e,22
		d8,7a,f8,2e,1b,15,0b,18,af,98,33,59,00,66,d9,0c
		08,63,75,4a,ea,50,5e,54,7e,72,8e,3d,57,cb,89,15
		0f,bc,10,0b,5a,1b,3a,84,08,9f,73,0a,e7,38,c7,03
		e4,2e,1a,19,45,08,25,f8,01,bd,89,0f,3a,e1,18,3a
		87,51,74,71,94,a2,4c,8a,1e,3a,7c,52,f3,03,6e,91
		fe,97,42,4f,3e,22,b7,c5,72,8c,f8,da,dd,53,ee,42
		ca,af,8d,78,38,70,10,63,e9,8c,51,a5,02,f2,89,f8
		a0,4d,68,7a,a5,96,d4,67,70,12,00,00,00,20,1e,a9
		86,c4,2b,ff,9f,99,00,2d,be,2e,91,c4,5a,ac,b7,49
		e4,7e,1a,7f,65,ae,29,bf,3f,c7,d0,c5,ce,39,00,00
		00,20,2c,55,ee,bd,2c,f0,ad,c8,77,56,cf,b6,15,e8
		5e,2b,18,ce,3e,5c,fc,56,d2,4f,9a,8b,f5,71,a5,10 <- 20 zero bits start with that last nibble 
		00,00,00,00,00,8a,30,81,87,02,41,3b,cb,b3,10,9a
		87,03,89,ec,61,aa,e4,9c,83,1a,7e,27,64,5b,6d,74
		fc,6c,a7,f2,f9,2c,1c,11,c6,56,76,b2,77,aa,92,c8
		cf,de,e8,9d,0f,0f,e3,c0,7a,5b,8f,04,e0,a2,7d,af
		70,27,57,fb,4b,ba,3d,48,c2,fa,e5,ee,02,42,01,86
		ff,a4,93,1e,ba,18,f5,14,65,06,25,86,10,9c,d7,3e
		53,30,c9,39,a3,90,13,b2,7f,a1,ba,10,af,5b,53,c8
		b1,ae,6a,19,ed,2a,a3,3a,ec,8b,01,7c,50,9a,15,8b
		7a,77,7b,28,b4,70,71,1f,77,40,c2,6b,22,0e,6e,fb

### Protocol Data Units (PDUs)

The fields in a cryptocoin are: 

<table>
<tr><th>Offset</th><th>Name</th><th>Length</th><th>Description</th></tr>
<tr><td>0</td><td>Ciphersuite</td><td>4</td><td>Specifies all cryptographic algorithms - value for now fixed at zero</td></tr>
<tr><td>4</td><td>Bits</td><td>4</td><td>Specifies difficulty</td></tr>
<tr><td>8</td><td>Public Key length</td><td>4</td><td>Specifies length of public key</td></tr>
<tr><td>12</td><td>Public Key</td><td>158</td><td>Public key value (fixed for p256 curve)</td></tr>
<tr><td>170</td><td>Nonce len</td><td>4</td><td>length of nonce</td></tr>
<tr><td>174</td><td>Nonce</td><td>32</td><td>nonce used to generate PoW hash</td></tr>
<tr><td>206</td><td>PoW Hash len</td><td>4</td><td>length of proof-of-work hash</td></tr>
<tr><td>210</td><td>PoW Hash</td><td>32</td><td>proof-of-work hash</td></tr>
<tr><td>242</td><td>Signature len</td><td>4</td><td>length of coin self-signature</td></tr>
<tr><td>246</td><td>Signature</td><td>Variable (~138 octets)</td><td>coin self-signature</td></tr>
</table>

</table>

The ciphersuite value of zero means: "use SHA256 for the proof-of-work and use
ECDSA with NIST p256 for the public key and signature." The ciphersuite concept
is used in TLS and various other cryptographic protocols.

All length fields and the bits field are in [network byte order](https://en.wikipedia.org/wiki/Network_byte_order).

### Cryptographic Inputs/Outputs

- Bytes 0..205 inclusive are input to the Proof-of-Work (PoW) hash (which uses SHA256).
- Bytes 206..241 inclusive are the PoW length and hash value
- Bytes 0..241 inclusive are input to the signature 
- Bytes 242..end are the signature length and value

### Example (again)

Breaking the above sample down into these fields we get...

		Ciphersuite (0)
		00,00,00,00,
		Difficulty in terms of bits (20)
		            00,00,00,14,
		Length of public key (4)
		                        00,00,00,9e,
		Public key value (158, DER encoded)
		                                    30,81,9b,30
		10,06,07,2a,86,48,ce,3d,02,01,06,05,2b,81,04,00
		23,03,81,86,00,04,01,c8,46,55,6b,4e,26,bb,6e,22
		d8,7a,f8,2e,1b,15,0b,18,af,98,33,59,00,66,d9,0c
		08,63,75,4a,ea,50,5e,54,7e,72,8e,3d,57,cb,89,15
		0f,bc,10,0b,5a,1b,3a,84,08,9f,73,0a,e7,38,c7,03
		e4,2e,1a,19,45,08,25,f8,01,bd,89,0f,3a,e1,18,3a
		87,51,74,71,94,a2,4c,8a,1e,3a,7c,52,f3,03,6e,91
		fe,97,42,4f,3e,22,b7,c5,72,8c,f8,da,dd,53,ee,42
		ca,af,8d,78,38,70,10,63,e9,8c,51,a5,02,f2,89,f8
		a0,4d,68,7a,a5,96,d4,67,70,12,
		Length of Nonce (4)
		                              00,00,00,20,
		Nonce value (32)
		                                          1e,a9
		86,c4,2b,ff,9f,99,00,2d,be,2e,91,c4,5a,ac,b7,49
		e4,7e,1a,7f,65,ae,29,bf,3f,c7,d0,c5,ce,39,
		Length of Proof-of-Work hash (4)
		                                          00,00
		00,20,
		Proof-of-Work hash (32)
		      2c,55,ee,bd,2c,f0,ad,c8,77,56,cf,b6,15,e8
		5e,2b,18,ce,3e,5c,fc,56,d2,4f,9a,8b,f5,71,a5,10
		00,00,
		Length of Signature (4)
		      00,00,00,8a,
		Signature (138, DER encoded)
		                  30,81,87,02,41,3b,cb,b3,10,9a
		87,03,89,ec,61,aa,e4,9c,83,1a,7e,27,64,5b,6d,74
		fc,6c,a7,f2,f9,2c,1c,11,c6,56,76,b2,77,aa,92,c8
		cf,de,e8,9d,0f,0f,e3,c0,7a,5b,8f,04,e0,a2,7d,af
		70,27,57,fb,4b,ba,3d,48,c2,fa,e5,ee,02,42,01,86
		ff,a4,93,1e,ba,18,f5,14,65,06,25,86,10,9c,d7,3e
		53,30,c9,39,a3,90,13,b2,7f,a1,ba,10,af,5b,53,c8
		b1,ae,6a,19,ed,2a,a3,3a,ec,8b,01,7c,50,9a,15,8b
		7a,77,7b,28,b4,70,71,1f,77,40,c2,6b,22,0e,6e,fb