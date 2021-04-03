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


#include "SevenZipLibrary.h"
#include "CompressionFormat.h"


namespace SevenZip
{
	class SevenZipExtractor
	{
	private:

		const SevenZipLibrary& m_library;
		TString m_archivePath;
		CompressionFormatEnum m_format;

	public:

		SevenZipExtractor( const SevenZipLibrary& library, const TString& archivePath );
		virtual ~SevenZipExtractor();

		void SetCompressionFormat( const CompressionFormatEnum& format );

		virtual void ExtractArchive( const TString& directory );

	private:

		void ExtractArchive( const CComPtr< IStream >& archiveStream, const TString& directory );
	};
}
