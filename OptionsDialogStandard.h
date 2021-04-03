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

#include "StandardDialog.h"
#include "afxwin.h"
#include "afxcmn.h"
#include <set>

// COptionsDialogStandard dialog

struct ScreenResolution {
public:
	int Width;
	int Height;
	int ColorDepth;
};

class COptionsDialogStandard : public CStandardDialog
{
	DECLARE_DYNAMIC(COptionsDialogStandard)

public:
	COptionsDialogStandard(CWnd* pParent = NULL);   // standard constructor
	virtual ~COptionsDialogStandard();

	enum { IDD = IDD_OPTIONSDIALOGSTANDARD };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual BOOL OnInitDialog();
	virtual BOOL PreCreateWindow(CREATESTRUCT & cs);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnDestroy();
private:
	void PopulateResolutionCombobox();
	void GetSupportedResolotions(std::set<ScreenResolution*> & resList);
	void LoadOptions();
	void SaveOptions();
public:
	CComboBox m_screenResolution;
	CSpinButtonCtrl m_spinButtonViewRange;
	CEdit m_spinEditViewRange;

	CButton m_chbWindowedMode;
	CEdit m_seBufferLine;
	CSpinButtonCtrl m_sbBufferLine;
	CButton m_chbCursor;
	CButton m_chbMovies;
	CComboBox m_cbCharacter;
	CComboBox m_cbObject;
	CComboBox m_cbTerrain;
	CComboBox m_cbShadows;
	CEdit m_seMaximumCount;
	CSpinButtonCtrl m_sbMaximumCount;
	CEdit m_seParticleCount;
	CSpinButtonCtrl m_sbParticleCount;
	CButton m_chbWeaponEffects;
	CButton m_chbBalloonVisible;
	CEdit m_seBalloonAlpha;
	CSpinButtonCtrl m_sbBalloonAlpha;
	CButton m_chbAutoloot;
	CEdit m_seCoinThreshold;
	CSpinButtonCtrl m_sbCoinThreshold;
	CButton m_chbBackground;
	CButton m_chbEffectSounds;
	CButton m_chbDuplicateSound;
	CButton m_chbTalkSound;
	CEdit m_seSoundDistance;
	CSpinButtonCtrl m_sbSoundDistance;
	CButton m_chbEnable;
	CEdit m_tbAccountID;
	CButton m_btnSetPassword;
};
