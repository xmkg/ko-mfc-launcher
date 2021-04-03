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


#include "SevenZipCompressor.h"
#include "SevenZipExtractor.h"

// MBCS DEBUG
// MT - MD
// MBCS RELEASE
// MT - MD
// UNICODE DEBUG
// MT - MD
// UNICODE RELEASE
// MT - MD


#ifdef _DEBUG
	#ifdef _UNICODE
		#ifdef _MT 
			#ifdef _DLL
				#pragma comment ( lib, "7zpp_unicode_md_d.lib" )
			#else
				#pragma comment ( lib, "7zpp_unicode_mt_d.lib" )
			#endif
		#endif
	#else
		#ifdef _MT 
			#ifdef _DLL
				#pragma comment ( lib, "7zpp_ascii_md_d.lib" )
			#else
				#pragma comment ( lib, "7zpp_ascii_mt_d.lib" )
			#endif
		#endif
	#endif
#else
	#ifdef _UNICODE
		#ifdef _MT
			#ifdef _DLL
				#pragma comment ( lib, "7zpp_unicode_md.lib" )
			#else
				#pragma comment ( lib, "7zpp_unicode_mt.lib" )
			#endif
		#endif
	#else
		#ifdef _MT 
			#ifdef _DLL
				#pragma comment ( lib, "7zpp_ascii_md.lib" )
			#else
				#pragma comment ( lib, "7zpp_ascii_mt.lib" )
			#endif
		#endif
	#endif
#endif
