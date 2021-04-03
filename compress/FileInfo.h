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


namespace SevenZip
{
namespace intl
{
	struct FileInfo
	{
		TString		FileName;
		FILETIME	LastWriteTime;
		FILETIME	CreationTime;
		FILETIME	LastAccessTime;
		ULONGLONG	Size;
		UINT		Attributes;
		bool		IsDirectory;
	};

	struct FilePathInfo : public FileInfo
	{
		TString		FilePath;
	};
}
}
