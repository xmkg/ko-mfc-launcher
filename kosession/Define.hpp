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
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#	include <string>
#	include <cassert>
#include <stdarg.h>
#include "SRW_RAII.h"

//#define TEST_MODE
#	define ASSERT assert

inline void FormattedDebugString(const char * fmt, ...)
{
	char buf[4096], *p = buf;
	va_list args;
	int n;

	va_start(args, fmt);
	n = _vsnprintf(p, sizeof(buf) - 3, fmt, args); // allow for proper linefeed & null terminator
	va_end(args);
	p += (n < 0) ? sizeof buf - 3 : n;
	while (p > buf && isspace(p[-1]))
		*--p = '\0';
	*p++ = '\r';
	*p++ = '\n';
	*p = '\0';

    OutputDebugStringA(buf);
}

#define TRACE FormattedDebugString






typedef signed __int64 int64;
typedef signed __int32 int32;
typedef signed __int16 int16;
typedef signed __int8 int8;
typedef unsigned __int64 uint64;
typedef unsigned __int32 uint32;
typedef unsigned __int16 uint16;
typedef unsigned __int8 uint8;

#	define INLINE __forceinline
#	define swap16(p) p
#	define swap32(p) p
#	define swap64(p) p
#	define swapfloat(p) p
#	define swapdouble(p) p

#pragma optimize( "", off )

static std::string EncryptPassword(std::string & pass)
{

	static const char key[100] = { 0x1A, 0x1F, 0x11, 0xA, 0x1E, 0x10, 0x18, 0x2, 0x1D, 0x8, 0x14, 0xF, 0x1C, 0xB, 0xD, 0x4, 0x13, 0x17, 0x0, 0xC, 0xE, 0x1B, 0x6, 0x12, 0x15, 0x3, 0x9, 0x7, 0x16, 0x1, 0x19, 0x5, 0x12, 0x1D, 0x7, 0x19, 0xF, 0x1F, 0x16, 0x1B, 0x9, 0x1A, 0x3, 0xD, 0x13, 0xE, 0x14, 0xB, 0x5, 0x2, 0x17, 0x10, 0xA, 0x18, 0x1C, 0x11, 0x6, 0x1E, 0x0, 0x15, 0x8, 0x4, 0x1 };
	static const char hash[40] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	unsigned int  intPass, tmp, destCounter = 0, uzunluk = strlen(pass.c_str());
	char passExt[40] = "";
	char buffer[40] = "";
	if ((uzunluk % 4) != 0) uzunluk = uzunluk + 4 - (uzunluk % 4);
	strcpy(passExt, pass.c_str());

	for (int i = 0; i < uzunluk / 4; i++) {
		intPass = 0;
		tmp = *reinterpret_cast<unsigned int*>(passExt + i * 4) + 0x3e8;
		int counter = 0;
		while (tmp) {
			if (tmp % 2) intPass += 1 << key[counter];
			tmp = tmp >> 1;
			counter++;
		}
		for (int j = 0; j < 7; j++)
		{
			unsigned long long mulres = unsigned long long(intPass) * unsigned long long(0x38E38E39);
			unsigned __int32 high = (mulres & 0xFFFFFFFF00000000ULL) >> 32;
			//unsigned __int32 low = mulres & 0xFFFFFFFF;

			tmp = high >> 3;
			buffer[destCounter] = hash[intPass - ((tmp * 9) << 2)];
			intPass = tmp;
			destCounter++;
		}
	}
	return std::string(buffer);
}

#pragma optimize( "", on ) 




