/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


// OptionsDialog.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsDialog.h"
#include "afxdialogex.h"
#include "OptionsDialogStandard.h"


// COptionsDialog dialog

IMPLEMENT_DYNAMIC(COptionsDialog, CTranslucentDialog)



COptionsDialog::COptionsDialog(UINT nImgID, LPCTSTR lpszType, HINSTANCE hResourceModule, CWnd * pParent):
	CTranslucentDialog(COptionsDialog::IDD, nImgID, lpszType, hResourceModule, pParent)
{
	
}

COptionsDialog::~COptionsDialog()
{
}

CStandardDialog * COptionsDialog::CreateStandardDialog()
{
	return ::new COptionsDialogStandard(this);
}

void COptionsDialog::OnInitChildrenWnds()
{

	auto initializeLabel = [=](CTranslucentLabel * p,const int baseX,const int baseY, const int baseW, const int baseH) {
		p->SetFont(L"Lucida Sans Unicode", 12.0f);
		p->SetFormat(Gdiplus::StringAlignment::StringAlignmentNear);
		p->SetColor(Gdiplus::Color::AntiqueWhite);
		p->MoveWindow(baseX, baseY, baseW, baseH);
		RegisterTranslucentWnd(p);

	};
	UINT imgList_Close[] = { IDB_BTNCLOSE_DEFAULT,IDB_BTNCLOSE_DISABLED,IDB_BTNCLOSE_HOVERED,IDB_BTNCLOSE_PRESSED,IDB_BTNCLOSE_PRESSED };
	m_btnClose.LoadImageList(imgList_Close);
	RegisterTranslucentWnd(&m_btnClose);
	//MoveWindow(GetWindowP)
	m_btnClose.MoveWindow(648, 35, 33, 33);
	/* Window option labels*/
	{
		CTranslucentLabel * arrLabels[] = {
			&m_lblResolution,
			&m_lblViewDistance,
			&m_lblWindowedMode,
			&m_lblBufferLine,
			&m_lblCursor,
			&m_lblEnableMovies
		};
			const int baseX = 39, baseY = 139, baseW = 120, baseH = 24;
			const int verticalSpace = 33;
			int index = 0;
			for (auto p : arrLabels) {
				initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
			}
	}
	/* Quality option labels */
	{
		CTranslucentLabel * arrLabels[] = {
			&m_lblCharacter,
			&m_lblObject,
			&m_lblTerrain,
			&m_lblShadow
		};
		const int baseX = 280, baseY = 139, baseW = 120, baseH = 24;
		const int verticalSpace = 33;
		int index = 0;
		for (auto p : arrLabels) {
			initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
		}
	}
	/* Sound option labels */
	{
		CTranslucentLabel * arrLabels[] = {
			&m_lblBackground,
			&m_lblEffectSounds,
			&m_lblDuplicateSound,
			&m_lblTalkSound,
			&m_lblDistance
		};
		const int baseX = 489, baseY = 139, baseW = 120, baseH = 24;
		const int verticalSpace = 33;
		int index = 0;
		for (auto p : arrLabels) {
			initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
		}

	}
	/* Pet option labels */
	{
		CTranslucentLabel * arrLabels[] = {
			&m_lblBalloonVisible,
			&m_lblBalloonAlpha,
			&m_lblAutoloot,
			&m_lblCoinThreshold
		};
		const int baseX = 39, baseY = 383, baseW = 120, baseH = 24;
		const int verticalSpace = 33;
		int index = 0;
		for (auto p : arrLabels) {
			initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
		}
	}
	/* Effect option labels */
	{

		CTranslucentLabel * arrLabels[] = {
			&m_lblMaximumCount,
			&m_lblParticleCount,
			&m_lblWeaponEffects
		};

		const int baseX = 280, baseY = 383, baseW = 120, baseH = 24;
		const int verticalSpace = 33;
		int index = 0;
		for (auto p : arrLabels) {
			initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
		}
	}

	/* Autologin option labels */
	{
		CTranslucentLabel * arrLabels[] = {
			&m_lblEnable,
			&m_lblID,
			&m_lblPassword
		};
		const int baseX = 489, baseY = 383, baseW = 120, baseH = 24;
		const int verticalSpace = 33;
		int index = 0;
		for (auto p : arrLabels) {
			initializeLabel(p, baseX, baseY + (verticalSpace * index++), baseW, baseH);
		}
	}
}

void COptionsDialog::DoDataExchange(CDataExchange* pDX)
{
	CTranslucentDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_1, m_lblResolution);
	DDX_Control(pDX, IDC_STATIC_2, m_lblViewDistance);
	DDX_Control(pDX, IDC_STATIC_3, m_lblWindowedMode);
	DDX_Control(pDX, IDC_STATIC_4, m_lblBufferLine);
	DDX_Control(pDX, IDC_STATIC_5, m_lblCursor);
	DDX_Control(pDX, IDC_STATIC_6, m_lblEnableMovies);
	DDX_Control(pDX, IDC_STATIC_7, m_lblCharacter);
	DDX_Control(pDX, IDC_STATIC_8, m_lblObject);
	DDX_Control(pDX, IDC_STATIC_9, m_lblTerrain);
	DDX_Control(pDX, IDC_STATIC_10, m_lblBackground);
	DDX_Control(pDX, IDC_STATIC_11, m_lblEffectSounds);
	DDX_Control(pDX, IDC_STATIC_12, m_lblDuplicateSound);
	DDX_Control(pDX, IDC_STATIC_13, m_lblTalkSound);
	DDX_Control(pDX, IDC_STATIC_14, m_lblDistance);
	DDX_Control(pDX, IDC_STATIC_15, m_lblBalloonVisible);
	DDX_Control(pDX, IDC_STATIC_16, m_lblBalloonAlpha);
	DDX_Control(pDX, IDC_STATIC_17, m_lblAutoloot);
	DDX_Control(pDX, IDC_STATIC_18, m_lblCoinThreshold);
	DDX_Control(pDX, IDC_STATIC_19, m_lblMaximumCount);
	DDX_Control(pDX, IDC_STATIC_22, m_lblParticleCount);
	DDX_Control(pDX, IDC_STATIC_21, m_lblWeaponEffects);
	DDX_Control(pDX, IDC_STATIC_20, m_lblEnable);
	DDX_Control(pDX, IDC_STATIC_23, m_lblID);
	DDX_Control(pDX, IDC_STATIC_24, m_lblPassword);
	DDX_Control(pDX, IDC_STATIC_25, m_lblShadow);
	DDX_Control(pDX, IDC_BUTTON1, m_btnClose);
}

BOOL COptionsDialog::PreCreateWindow(CREATESTRUCT & cs)
{
	cs.cy = 683; // width
	cs.cx = 580; // height
	//MoveWindow(cs.x, cs.y + 150, 683, 580);
	return CTranslucentDialog::PreCreateWindow(cs);
}





BEGIN_MESSAGE_MAP(COptionsDialog, CTranslucentDialog)
	ON_BN_CLICKED(IDC_BUTTON1, &COptionsDialog::OnBnClickedButton1)
END_MESSAGE_MAP()



void COptionsDialog::OnBnClickedButton1()
{
	SendMessage(WM_CLOSE);
	// TODO: Add your control notification handler code here
}
