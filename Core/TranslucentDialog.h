/**
 * ______________________________________________________
 * This file is part of ko-ascension-launcher project.
 * 
 * @author       Mustafa Kemal Gılor <mustafagilor@gmail.com> (2015)
 * .
 * SPDX-License-Identifier:	MIT
 * ______________________________________________________
 */

// TranslucentDialog.h : ͷ�ļ�
//

#pragma once

#include "StandardDialog.h"
#include "TranslucentWnd.h"

#include <list>

typedef std::list<CTranslucentWnd*> TranslucentWndList; 

// CTranslucentDialog �Ի���
class CTranslucentDialog : public CDialog, public IRenderListener
{
	DECLARE_DYNAMIC(CTranslucentDialog);
public:
	
	CTranslucentDialog(UINT nIDTemplate, LPCTSTR lpszFile, CWnd* pParent = NULL);
	CTranslucentDialog(UINT nIDTemplate, UINT nImgID, LPCTSTR lpszType = _T("PNG"), HINSTANCE hResourceModule = NULL, CWnd* pParent = NULL);
	
	virtual ~CTranslucentDialog();

	void SetEnableDrag(bool bEnableDrag);
	void SetCenterAligned(bool bCenterAligned);

	virtual CStandardDialog* CreateStandardDialog() = 0;
	virtual void OnInitChildrenWnds() = 0;

	void RegisterTranslucentWnd(CTranslucentWnd* translucentWnd);
	void UnregisterTranslucentWnd(CTranslucentWnd* translucentWnd);

	void StartBuffered();
	void EndBuffered();
	
	virtual void OnRenderEvent(CTranslucentWnd* translucentWnd);

	void UpdateView();

// ʵ��
protected:
	BLENDFUNCTION m_blend;
	Gdiplus::Image* m_pImage;
	//RECT m_rcWindow;
	bool m_bEnableDrag;
	bool m_bCenterAligned;
	bool m_bBuffered;

	CStandardDialog* m_pStandardDlg;

	TranslucentWndList m_translucentWndList;

	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
private:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnMove(int x, int y);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
};
