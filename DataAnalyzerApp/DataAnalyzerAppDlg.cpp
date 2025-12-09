
// DataAnalyzerAppDlg.cpp : implementation file
//

#include "stdafx.h"
#include "framework.h"
#include "DataAnalyzerApp.h"
#include "DataAnalyzerAppDlg.h"
#include "afxdialogex.h"
#include "CPlotTabDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CDataAnalyzerAppDlg dialog



CDataAnalyzerAppDlg::CDataAnalyzerAppDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DATAANALYZERAPP_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CDataAnalyzerAppDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_DATA, m_listData);
	DDX_Control(pDX, IDC_TREE_HEADERS, m_treeHeaders);
	DDX_Control(pDX, IDC_TAB_MAIN, m_tabCtrl);
}

BEGIN_MESSAGE_MAP(CDataAnalyzerAppDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CDataAnalyzerAppDlg::OnBnClickedButton1)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_MAIN, &CDataAnalyzerAppDlg::OnSelchangeTabMain)
END_MESSAGE_MAP()


// CDataAnalyzerAppDlg message handlers

BOOL CDataAnalyzerAppDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	// Set up list control styles
	m_listData.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES);

	m_plotTab.Create(IDD_TAB_PLOT, &m_tabCtrl);
	m_textTab.Create(IDD_TAB_TEXT, &m_tabCtrl);

	m_tabCtrl.InsertItem(0, _T("tab-1"));
	m_tabCtrl.InsertItem(1, _T("tab-2"));

	//ResizeTabPages();

	CRect rc;
	m_tabCtrl.GetClientRect(&rc);
	m_tabCtrl.AdjustRect(FALSE, &rc);
	rc.DeflateRect(4, 4);
	m_plotTab.MoveWindow(&rc);
	m_textTab.MoveWindow(&rc);

	m_plotTab.ShowWindow(SW_SHOW);
	m_textTab.ShowWindow(SW_HIDE);
	m_tabCtrl.SetCurSel(0);

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CDataAnalyzerAppDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CDataAnalyzerAppDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CDataAnalyzerAppDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CDataAnalyzerAppDlg::OnBnClickedButton1()
{
	// TODO: Add your control notification handler code here
	// 1. Clear list completely
	m_listData.DeleteAllItems();
	while (m_listData.DeleteColumn(0));   // remove old columns

	// 2. File open dialog
	CFileDialog dlg(TRUE, _T("csv"), nullptr,
		OFN_HIDEREADONLY | OFN_FILEMUSTEXIST,
		_T("CSV Files (*.csv)|*.csv|TEXT Files (*.txt*)|*.txt*| (*.text*)| *.text*|"), this);

	if (dlg.DoModal() != IDOK) return;

	CString filePath = dlg.GetPathName();

	// 3. Open file as text
	CStdioFile file;
	if (!file.Open(filePath, CFile::modeRead | CFile::typeText))
	{
		AfxMessageBox(_T("Cannot open file!"));
		return;
	}

	CString line;
	int row = 0;
	int columnCount = 0;
	CStringArray headerNames;

	//Populate the tree
	m_treeHeaders.SetRedraw(FALSE);
	m_treeHeaders.DeleteAllItems();

	//Root Node
	HTREEITEM hRoot = m_treeHeaders.InsertItem(_T("Columns"), TVI_ROOT);
	m_treeHeaders.SetItemState(hRoot, TVIS_BOLD, TVIS_BOLD);

	while (file.ReadString(line))
	{

		if (line.IsEmpty()) continue;

		// Split by comma
		CStringArray fields;
		int pos = 0;
		CString token = line.Tokenize(_T(","), pos);
		while (!token.IsEmpty())
		{
			token.Trim();           // remove spaces around values
			fields.Add(token);
			token = line.Tokenize(_T(","), pos);
		}

		// First non-empty line = column Header
		if (row == 0)
		{
			m_plotTab.FillComboBoxes(fields); //send header to combo box


			columnCount = fields.GetSize();
			for (int c = 0; c < columnCount; ++c)
			{
				CString header = fields[c];
				m_treeHeaders.InsertItem(header, hRoot);
				headerNames.Add(header);

				m_listData.InsertColumn(c, fields[c], LVCFMT_LEFT, 100);
			}

			//Expand the root(optional)
			m_treeHeaders.Expand(hRoot, TVE_EXPAND);

			row++;
			continue;               // skip adding header as data row
		}

		//Expand root so headers are visible
		m_treeHeaders.ModifyStyle(0, TVS_HASBUTTONS | TVS_LINESATROOT | TVS_HASLINES);
		m_treeHeaders.Expand(hRoot, TVE_EXPAND);
		m_treeHeaders.SetRedraw(TRUE);
		m_treeHeaders.Invalidate();

		// Data rows
		if (fields.GetSize() > 0)
		{
			int nItem = m_listData.InsertItem(m_listData.GetItemCount(), fields[0]);

			for (int c = 1; c < fields.GetSize() && c < columnCount; ++c)
			{
				m_listData.SetItemText(nItem, c, fields[c]);
			}
		}
		row++;
	}

	file.Close();

	// ========== COPY DATA TO m_data FOR PLOT TAB ==========
	m_data.clear();  // Clear old data

	int colCount = m_listData.GetHeaderCtrl()->GetItemCount();
	int rowCount = m_listData.GetItemCount();

	// Resize m_data: one vector per column
	m_data.resize(colCount);

	for (int col = 0; col < colCount; ++col)
	{
		m_data[col].reserve(rowCount);  // faster

		for (int row = 0; row < rowCount; ++row)
		{
			CString text = m_listData.GetItemText(row, col);
			double value = _tstof(text);  // convert text ? double
			m_data[col].push_back(value);
		}
	}

	m_plotTab.FillChartData(m_data);
}

void CDataAnalyzerAppDlg::OnSelchangeTabMain(NMHDR* pNMHDR, LRESULT* pResult)
{
	// TODO: Add your control notification handler code here
	int sel = m_tabCtrl.GetCurSel();

	m_plotTab.ShowWindow(sel == 0 ? SW_SHOW : SW_HIDE);
	m_textTab.ShowWindow(sel == 1 ? SW_SHOW : SW_HIDE);

	*pResult = 0;
}
