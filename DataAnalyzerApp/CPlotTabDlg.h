#pragma once
#include "afxdialogex.h"
#include <vector>
//#include "ChartCtrl.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartCtrl.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartLineSerie.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartLabel.h"

#define ID_DOWNLOAD_DATA   9001
#define ID_DOWNLOAD_CHART  9002

// CPlotTabDlg dialog

class CPlotTabDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CPlotTabDlg)

public:
	CPlotTabDlg(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CPlotTabDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TAB_PLOT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	CComboBox m_comboX;
	CComboBox m_comboY;

	CStringArray m_headers; //column names
	int m_lastX = -1; //track to prevent previous selection
	int m_lastY = -1;
	
	
public:
	afx_msg void OnCbnSelchangeComboX();
	afx_msg void OnCbnSelchangeComboY();
	//afx_msg LRESULT OnDelayedInit(WPARAM wParam, LPARAM lParam);
	void FillComboBoxes(const CStringArray& headers);
	void FillChartData(const std::vector<std::vector<double>>& data);
	void UpdateChart();
	std::vector<std::vector<double>> m_data;

	CChartStandardAxis* pBottomAxis;
	CChartStandardAxis* pLeftAxis;
	CChartLineSerie* pSeries;

	//for csv and png download
	//afx_msg void OnRClickChart(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDownloadData();
	afx_msg void OnDownloadChart();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void EnableZooming();
protected:
	CChartCtrl m_chart;

private: 
	bool m_chartReady = false;
	
public:
	afx_msg void OnStnClickedChartctrl();
};
