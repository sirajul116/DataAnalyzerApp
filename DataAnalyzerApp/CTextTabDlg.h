#pragma once
#include "afxdialogex.h"


// CTextTabDlg dialog

class CTextTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CTextTabDlg)

public:
	CTextTabDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CTextTabDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_TEXT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnStnClickedStaticText();
};
