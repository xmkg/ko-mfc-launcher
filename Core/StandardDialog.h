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

// CStandardDialog dialog

class CStandardDialog : public CDialog
{
	DECLARE_DYNAMIC(CStandardDialog)

public:
	CStandardDialog(UINT nIDTemplate, COLORREF clrTransparent = RGB(0, 255, 0), CWnd* pParentWnd = NULL);

	virtual ~CStandardDialog();

	void ShowDialog();

	DECLARE_MESSAGE_MAP()
	
private:
	COLORREF m_clrTransparent;
	CBrush m_brush;

protected:
	virtual void OnOK();
	virtual void OnCancel();
	void EndDialog(int nResult);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
};

