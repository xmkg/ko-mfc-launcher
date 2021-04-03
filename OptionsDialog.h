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

#include "TranslucentDialog.h"
#include "TranslucentButton.h"
#include "TranslucentProgressBar.h"
#include "TranslucentLabel.h"
#include "afxwin.h"
#include "afxcmn.h"

// COptionsDialog dialog



class COptionsDialog :public CTranslucentDialog
{
	DECLARE_DYNAMIC(COptionsDialog)

public:
	COptionsDialog(UINT nImgID, LPCTSTR lpszType = _T("PNG"), HINSTANCE hResourceModule = NULL, CWnd* pParent = NULL);
	virtual ~COptionsDialog();
	virtual CStandardDialog* CreateStandardDialog();
	virtual void OnInitChildrenWnds();
// Dialog Data

	enum { IDD = IDD_OPTIONSDIALOG };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
	DECLARE_MESSAGE_MAP()
private:
	

public:
	CTranslucentLabel m_lblResolution;
	CTranslucentLabel  m_lblViewDistance;
	CTranslucentLabel  m_lblWindowedMode;
	CTranslucentLabel  m_lblBufferLine;
	CTranslucentLabel  m_lblCursor;
	CTranslucentLabel  m_lblEnableMovies;
	CTranslucentLabel m_lblCharacter;
	CTranslucentLabel m_lblObject;
	CTranslucentLabel m_lblTerrain;
	CTranslucentLabel m_lblBackground;
	CTranslucentLabel m_lblEffectSounds;
	CTranslucentLabel m_lblDuplicateSound;
	CTranslucentLabel m_lblTalkSound;
	CTranslucentLabel m_lblDistance;
	CTranslucentLabel m_lblBalloonVisible;
	CTranslucentLabel m_lblBalloonAlpha;
	CTranslucentLabel m_lblAutoloot;
	CTranslucentLabel m_lblCoinThreshold;
	CTranslucentLabel m_lblMaximumCount;
	CTranslucentLabel m_lblParticleCount;
	CTranslucentLabel m_lblWeaponEffects;
	CTranslucentLabel m_lblEnable;
	CTranslucentLabel m_lblID;
	CTranslucentLabel m_lblPassword;
	CTranslucentLabel m_lblShadow;
	CTranslucentButton m_btnClose;
	afx_msg void OnBnClickedButton1();
};
