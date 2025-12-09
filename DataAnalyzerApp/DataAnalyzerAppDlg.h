
// DataAnalyzerAppDlg.h : header file
//

#pragma once
#include "CTextTabDlg.h"
#include "CPlotTabDlg.h"


// CDataAnalyzerAppDlg dialog
class CDataAnalyzerAppDlg : public CDialogEx
{
// Construction
public:
	CDataAnalyzerAppDlg(CWnd* pParent = nullptr);	// standard constructor

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DATAANALYZERAPP_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
protected:
	CListCtrl m_listData;
	CTreeCtrl m_treeHeaders;
	CPlotTabDlg m_plotTab;
	CTextTabDlg m_textTab;

	std::vector<std::vector<double>> m_data;

public:
	CTabCtrl m_tabCtrl;
	afx_msg void OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult);
};
