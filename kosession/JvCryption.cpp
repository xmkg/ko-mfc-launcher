/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

#include "Stdafx.h"
#include "JvCryption.h"

// Cryption
#if GAMESERVER_VERSION >= 1700
#define g_private_key 0x1207500120128966
#elif GAMESERVER_VERSION == 1298
#define g_private_key 0x1234567890123456
#else
//#define g_private_key 0x7467148522014785
#endif
//#define g_private_key 0x1257091582190465

//#define g_private_key 0x7412580096385200

void CJvCryption::Init() { m_tkey = m_public_key ^ m_private_key; }



void CJvCryption::SetPublicKey(unsigned long long val)
{
	m_public_key = val;
}

void CJvCryption::SetPrivateKey(unsigned long long val)
{
	m_private_key = val;
}

void CJvCryption::JvEncryptionFast(int len, unsigned char *datain, unsigned char *dataout)
{
	unsigned char *pkey, lkey, rsk;
	int rkey = 2157;

	pkey = (unsigned char *)&m_tkey;
	lkey = (len * 157) & 0xff;

	for (int i = 0; i < len; i++)
	{
		rsk = (rkey >> 8) & 0xff;
		dataout[i] = ((datain[i] ^ rsk) ^ pkey[(i % 8)]) ^ lkey;
		rkey *= 2171;
	}
}

void CJvCryption::JvEncryptionFastBetweenIndex(int start, int len, unsigned char *datain, unsigned char *dataout)
{
	unsigned char *pkey, lkey, rsk;
	int rkey = 2157;

	pkey = (unsigned char *)&m_tkey;
	lkey = (len * 157) & 0xff;

	for (int i = start; i < start + len; i++)
	{
		rsk = (rkey >> 8) & 0xff;
		dataout[i] = ((datain[i] ^ rsk) ^ pkey[((i - start) % 8)]) ^ lkey;
		rkey *= 2171;
	}
}

int CJvCryption::JvDecryptionWithCRC32(int len, unsigned char *datain, unsigned char *dataout)
{
	int result;
	JvDecryptionFast(len, datain, dataout);

	if (crc32(dataout, len - 4, -1) == *(unsigned __int32 *)(len - 4 + dataout))
		result = len - 4;
	else
		result = -1;

	return result;
}
