/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */


// OptionsDialogStandard.cpp : implementation file
//

#include "stdafx.h"
#include "OptionsDialogStandard.h"
#include "afxdialogex.h"
#include <set>
#include "GameOptions.h"


// COptionsDialogStandard dialog

IMPLEMENT_DYNAMIC(COptionsDialogStandard, CStandardDialog)
std::set<ScreenResolution *> SupportedResolutions;
COptionsDialogStandard::COptionsDialogStandard(CWnd* pParent /*=NULL*/)
	: CStandardDialog(COptionsDialogStandard::IDD, RGB(0, 255, 0), pParent)
{
	GetSupportedResolotions(SupportedResolutions);
}

COptionsDialogStandard::~COptionsDialogStandard()
{
	for (auto itr = SupportedResolutions.begin(); itr != SupportedResolutions.end();) {
		ScreenResolution * pRes = (*itr);
		if (pRes)
		{
			delete pRes;
			itr = SupportedResolutions.erase(itr);
		}
		else
			++itr;
	}
}



void COptionsDialogStandard::DoDataExchange(CDataExchange* pDX)
{
	CStandardDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_screenResolution);
	DDX_Control(pDX, IDC_SPIN3, m_spinButtonViewRange);
	DDX_Control(pDX, IDC_EDIT2, m_spinEditViewRange);
	DDX_Control(pDX, IDC_CHECK1, m_chbWindowedMode);
	DDX_Control(pDX, IDC_EDIT1, m_seBufferLine);
	DDX_Control(pDX, IDC_SPIN1, m_sbBufferLine);
	DDX_Control(pDX, IDC_CHECK2, m_chbCursor);
	DDX_Control(pDX, IDC_CHECK3, m_chbMovies);
	DDX_Control(pDX, IDC_COMBO2, m_cbCharacter);
	DDX_Control(pDX, IDC_COMBO3, m_cbObject);
	DDX_Control(pDX, IDC_COMBO4, m_cbTerrain);
	DDX_Control(pDX, IDC_COMBO5, m_cbShadows);
	DDX_Control(pDX, IDC_EDIT3, m_seMaximumCount);
	DDX_Control(pDX, IDC_SPIN4, m_sbMaximumCount);
	DDX_Control(pDX, IDC_EDIT4, m_seParticleCount);
	DDX_Control(pDX, IDC_SPIN5, m_sbParticleCount);
	DDX_Control(pDX, IDC_CHECK5, m_chbWeaponEffects);
	DDX_Control(pDX, IDC_CHECK6, m_chbBalloonVisible);
	DDX_Control(pDX, IDC_EDIT6, m_seBalloonAlpha);
	DDX_Control(pDX, IDC_SPIN7, m_sbBalloonAlpha);
	DDX_Control(pDX, IDC_CHECK7, m_chbAutoloot);
	DDX_Control(pDX, IDC_EDIT7, m_seCoinThreshold);
	DDX_Control(pDX, IDC_SPIN8, m_sbCoinThreshold);
	DDX_Control(pDX, IDC_CHECK8, m_chbBackground);
	DDX_Control(pDX, IDC_CHECK9, m_chbEffectSounds);
	DDX_Control(pDX, IDC_CHECK10, m_chbDuplicateSound);
	DDX_Control(pDX, IDC_CHECK11, m_chbTalkSound);
	DDX_Control(pDX, IDC_EDIT8, m_seSoundDistance);
	DDX_Control(pDX, IDC_SPIN9, m_sbSoundDistance);
	DDX_Control(pDX, IDC_CHECK12, m_chbEnable);
	DDX_Control(pDX, IDC_EDIT9, m_tbAccountID);
	DDX_Control(pDX, IDC_BUTTON1, m_btnSetPassword);
	PopulateResolutionCombobox();

	m_screenResolution.MoveWindow(145, 139, 120, 24);
	m_spinEditViewRange.MoveWindow(145, 172, 100, 24);
	m_spinButtonViewRange.MoveWindow(245, 172, 20, 24);
	m_chbWindowedMode.MoveWindow(145, 205, 120, 24);
	m_seBufferLine.MoveWindow(145, 238, 100, 24);
	m_sbBufferLine.MoveWindow(245, 238, 20, 24);
	m_chbCursor.MoveWindow(145, 271, 120, 24);
	m_chbMovies.MoveWindow(145, 304, 120, 24);

	m_cbCharacter.MoveWindow(350, 139, 120, 24);
	m_cbObject.MoveWindow(350, 172, 120, 24);
	m_cbTerrain.MoveWindow(350, 205, 120, 24);
	m_cbShadows.MoveWindow(350, 238, 120, 24);
	m_cbCharacter.AddString(L"High");
	m_cbCharacter.AddString(L"Low");
	m_cbObject.AddString(L"High");
	m_cbObject.AddString(L"Low");
	m_cbTerrain.AddString(L"High");
	m_cbTerrain.AddString(L"Low");
	m_cbShadows.AddString(L"High");
	m_cbShadows.AddString(L"Low");

	m_seMaximumCount.MoveWindow(380, 383, 70, 24);
	m_sbMaximumCount.MoveWindow(450, 383, 20, 24);
	m_seParticleCount.MoveWindow(380, 416, 70, 24);
	m_sbParticleCount.MoveWindow(450, 416, 20, 24);
	m_chbWeaponEffects.MoveWindow(380, 449, 90, 24);

	m_chbBalloonVisible.MoveWindow(145, 383, 120, 24);
	m_seBalloonAlpha.MoveWindow(145, 416, 100, 24);
	m_sbBalloonAlpha.MoveWindow(245, 416, 20, 24);
	m_chbAutoloot.MoveWindow(145, 449, 120, 24);
	m_seCoinThreshold.MoveWindow(145, 482, 100, 24);
	m_sbCoinThreshold.MoveWindow(245, 482, 20, 24);

	/* 489 139 */
	m_chbBackground.MoveWindow(590, 139, 70, 24);
	m_chbEffectSounds.MoveWindow(590, 172, 70, 24);
	m_chbDuplicateSound.MoveWindow(590, 205, 70, 24);
	m_chbTalkSound.MoveWindow(590, 238, 70, 24);
	m_seSoundDistance.MoveWindow(590, 271, 50, 24);
	m_sbSoundDistance.MoveWindow(640, 271, 20, 24);

	m_chbEnable.MoveWindow(590, 383, 70, 24);
	m_tbAccountID.MoveWindow(590, 416, 70, 24);
	m_btnSetPassword.MoveWindow(590, 449, 70, 24);

	m_spinButtonViewRange.SetRange32(0, 2048);


	LoadOptions();
}

BOOL COptionsDialogStandard::OnInitDialog()
{
	return CStandardDialog::OnInitDialog();
}

BOOL COptionsDialogStandard::PreCreateWindow(CREATESTRUCT & cs)
{
	cs.cy = 683; // width
	cs.cx = 580; // height

	return CStandardDialog::PreCreateWindow(cs);
}


BEGIN_MESSAGE_MAP(COptionsDialogStandard, CStandardDialog)
	ON_BN_CLICKED(IDOK, &COptionsDialogStandard::OnBnClickedOk)
	ON_WM_CLOSE()
	ON_WM_SYSCOMMAND()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// COptionsDialogStandard message handlers


void COptionsDialogStandard::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
}

void COptionsDialogStandard::OnClose()
{
	SaveOptions();

	CStandardDialog::OnClose();
}

void COptionsDialogStandard::OnSysCommand(UINT nID, LPARAM lParam)
{
	SaveOptions();
	CStandardDialog::OnSysCommand(nID, lParam);
}

void COptionsDialogStandard::OnDestroy()
{
	SaveOptions();
	return;
}

void COptionsDialogStandard::PopulateResolutionCombobox()
{

	for (auto pRes : SupportedResolutions)
	{
		
		if (pRes)
		{
			wchar_t buf[15];
			wnsprintfW(buf, 15, L"%dx%dx%d", pRes->Width, pRes->Height, pRes->ColorDepth);
			m_screenResolution.AddString(buf);
		}

	}
}

void COptionsDialogStandard::GetSupportedResolotions(std::set<ScreenResolution*>& resList)
{
	DEVMODE dm = { 0 };
	dm.dmSize = sizeof(dm);
	int prev_w = 0, prev_h = 0, prev_bpp = 0;
	for (int iModeNum = 0; EnumDisplaySettings(NULL, iModeNum, &dm) != 0; iModeNum++)
	{
		if (prev_w == dm.dmPelsWidth && prev_h == dm.dmPelsHeight && prev_bpp == dm.dmBitsPerPel)
			continue;
		prev_w = dm.dmPelsWidth, prev_h = dm.dmPelsHeight, prev_bpp = dm.dmBitsPerPel;
		ScreenResolution * pResolution = new ScreenResolution();
		pResolution->Width = dm.dmPelsWidth;
		pResolution->Height = dm.dmPelsHeight;
		pResolution->ColorDepth = dm.dmBitsPerPel;
		resList.insert(pResolution);
	}
}

void COptionsDialogStandard::LoadOptions()
{
	int index = 0;
	for (auto p : SupportedResolutions) {
		if (p->Width == GameOptions::Window::Width && 
			p->Height == GameOptions::Window::Height && 
			p->ColorDepth == GameOptions::Window::ColorDepth)
		{
			m_screenResolution.SetCurSel(index);
			break;
		}
		index++;
	}
	if (m_screenResolution.GetCurSel() == -1){
		/* Set to maximum available */
		m_screenResolution.SetCurSel(0);
	}
	m_spinButtonViewRange.SetRange32(0, 4096);
	m_spinButtonViewRange.SetPos32(GameOptions::Window::ViewDistance);
	m_chbWindowedMode.SetCheck(GameOptions::Window::WindowedMode);
	m_sbBufferLine.SetRange32(0, 20);
	m_sbBufferLine.SetPos32(GameOptions::Window::BuffLine);
	m_chbCursor.SetCheck(GameOptions::Window::Cursor);
	m_chbMovies.SetCheck(GameOptions::Window::EnableMovies);

	m_chbBalloonVisible.SetCheck(GameOptions::Pet::ShowBalloon);
	m_sbBalloonAlpha.SetRange32(0, 255);
	m_sbBalloonAlpha.SetPos32(GameOptions::Pet::BalloonAlpha);
	m_chbAutoloot.SetCheck(GameOptions::Pet::EnableAutoLoot);
	m_sbCoinThreshold.SetRange32(0, 32000);
	m_sbCoinThreshold.SetPos32(GameOptions::Pet::LootCoinThreshold);

	m_cbCharacter.SetCurSel(!GameOptions::Texture::EnableLevelOfDetail_Character);
	m_cbObject.SetCurSel(!GameOptions::Texture::EnableLevelOfDetail_Shape);
	m_cbTerrain.SetCurSel(!GameOptions::Texture::EnableLevelOfDetail_Terrain);
	m_cbShadows.SetCurSel(!GameOptions::Shadow::Enable);

	m_sbMaximumCount.SetRange32(0, 4096);
	m_sbMaximumCount.SetPos32(GameOptions::Effect::MaximumEffectRenderCount);
	m_sbParticleCount.SetRange32(0, 16384);
	m_sbParticleCount.SetPos32(GameOptions::Effect::MaximumParticleLoadCount);
	m_chbWeaponEffects.SetCheck(GameOptions::Effect::WeaponEffectsVisible);

	m_chbBackground.SetCheck(GameOptions::Sound::EnableBackgroundMusic);
	m_chbEffectSounds.SetCheck(GameOptions::Sound::EnableEffectMusic);
	m_chbDuplicateSound.SetCheck(GameOptions::Sound::EnableDuplicateMusic);
	m_chbTalkSound.SetCheck(GameOptions::Sound::EnableTalk);
	m_sbSoundDistance.SetRange32(0, 2048);
	m_sbSoundDistance.SetPos32(GameOptions::Sound::Distance);
}

void COptionsDialogStandard::SaveOptions()
{

	if (m_screenResolution.GetCurSel() == -1){
		m_screenResolution.SetCurSel(0);
	}
	auto it = SupportedResolutions.begin();
	std::advance(it, m_screenResolution.GetCurSel());
	auto resolution = *it;

	GameOptions::Window::Width = resolution->Width;
	GameOptions::Window::Height = resolution->Height;
	GameOptions::Window::ColorDepth = resolution->ColorDepth;
	GameOptions::Window::ViewDistance = m_spinButtonViewRange.GetPos32();
	GameOptions::Window::WindowedMode = m_chbWindowedMode.GetCheck() == 1;
	GameOptions::Window::BuffLine = m_sbBufferLine.GetPos32();
	GameOptions::Window::Cursor = m_chbCursor.GetCheck() == 1;
	GameOptions::Window::EnableMovies = m_chbMovies.GetCheck() == 1;

	GameOptions::Pet::ShowBalloon = m_chbBalloonVisible.GetCheck() == 1;
	GameOptions::Pet::BalloonAlpha = m_sbBalloonAlpha.GetPos32();
	GameOptions::Pet::EnableAutoLoot = m_chbAutoloot.GetCheck() == 1;
	GameOptions::Pet::LootCoinThreshold = m_sbCoinThreshold.GetPos32();

	GameOptions::Texture::EnableLevelOfDetail_Character = m_cbCharacter.GetCurSel() == 0;
	GameOptions::Texture::EnableLevelOfDetail_Shape = m_cbObject.GetCurSel() == 0;
	GameOptions::Texture::EnableLevelOfDetail_Terrain = m_cbTerrain.GetCurSel() == 0;
	GameOptions::Shadow::Enable = m_cbShadows.GetCurSel() == 1;



	GameOptions::Effect::MaximumEffectRenderCount = m_sbMaximumCount.GetPos32();
	GameOptions::Effect::MaximumParticleLoadCount = m_sbParticleCount.GetPos32();
	GameOptions::Effect::WeaponEffectsVisible = m_chbWeaponEffects.GetCheck() == 1;


	GameOptions::Sound::EnableBackgroundMusic = m_chbBackground.GetCheck() == 1;
	GameOptions::Sound::EnableEffectMusic = m_chbEffectSounds.GetCheck() == 1;
	GameOptions::Sound::EnableDuplicateMusic = m_chbDuplicateSound.GetCheck() == 1;
	GameOptions::Sound::EnableTalk = m_chbTalkSound.GetCheck() == 1;
	GameOptions::Sound::Distance = m_sbSoundDistance.GetPos32();

	GameOptions::SaveOptions();
}
