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


#include "SevenZipException.h"


namespace SevenZip
{
	class SevenZipLibrary
	{
	private:

		typedef UINT32 (WINAPI * CreateObjectFunc)( const GUID* clsID, const GUID* interfaceID, void** outObject );

		HMODULE				m_dll;
		CreateObjectFunc	m_func;

	public:

		SevenZipLibrary();
		~SevenZipLibrary();

		void Load();
		void Load( const TString& libraryPath );
		void Free();

		void CreateObject( const GUID& clsID, const GUID& interfaceID, void** outObject ) const;
	};
}
