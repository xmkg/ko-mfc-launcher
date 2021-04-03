/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


#pragma once

#define USE_CRYPTION
#include "crc32.h"

class CJvCryption
{
private:
	unsigned long long m_public_key,m_private_key, m_tkey;

public:
	CJvCryption() : m_public_key(0),m_private_key(0) {}

	__inline unsigned long long GetPublicKey() { return m_public_key; }
	void SetPublicKey(unsigned long long);
	void SetPrivateKey(unsigned long long);

	void Init();
	
	void JvEncryptionFast(int len, unsigned char *datain, unsigned char *dataout);
	void JvEncryptionFastBetweenIndex(int start, int len, unsigned char * datain, unsigned char * dataout);
	__inline void JvDecryptionFast(int len, unsigned char *datain, unsigned char *dataout) { JvEncryptionFast(len, datain, dataout); };

	int JvDecryptionWithCRC32(int len, unsigned char *datain, unsigned char *dataout);
};
