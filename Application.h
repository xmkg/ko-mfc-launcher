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

#ifndef __AFXWIN_H__
	#error include��stdafx.h��
#endif

#include "resource.h"


// CTranslucentApp:
//

class CTranslucentApp : public CWinApp
{
public:
	CTranslucentApp();

public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
	virtual int ExitInstance();
};

extern CTranslucentApp theApp;
