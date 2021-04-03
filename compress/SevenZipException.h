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


#include <exception>
#include "String.h"


namespace SevenZip
{
	TString StrFmt( const TCHAR* format, ... );
	TString GetWinErrMsg( const TString& contextMessage, DWORD lastError );
	TString GetCOMErrMsg( const TString& contextMessage, HRESULT lastError );


	class SevenZipException
	{
	protected:

		TString m_message;

	public:

		SevenZipException();
		SevenZipException( const TString& message );
		virtual ~SevenZipException();

		const TString& GetMessage() const;
	};
}
