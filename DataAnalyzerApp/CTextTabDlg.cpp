// CTextTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataAnalyzerApp.h"
#include "afxdialogex.h"
#include "CTextTabDlg.h"


// CTextTabDlg dialog

IMPLEMENT_DYNAMIC(CTextTabDlg, CDialogEx)

CTextTabDlg::CTextTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_TEXT, pParent)
{

}

CTextTabDlg::~CTextTabDlg()
{
}

void CTextTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTextTabDlg, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_TEXT, &CTextTabDlg::OnStnClickedStaticText)
END_MESSAGE_MAP()


// CTextTabDlg message handlers

void CTextTabDlg::OnStnClickedStaticText()
{
	// TODO: Add your control notification handler code here
}

