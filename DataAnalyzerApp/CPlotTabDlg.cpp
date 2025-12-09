// CPlotTabDlg.cpp : implementation file
//

#include "stdafx.h"
#include "DataAnalyzerApp.h"
#include "afxdialogex.h"
#include "CPlotTabDlg.h"

#include "../../../../Hridoy/chartctrl/chartctrl/ChartLineSerie.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartPointsSerie.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartSurfaceSerie.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartGrid.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartBarSerie.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartLabel.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartAxisLabel.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartStandardAxis.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartDateTimeAxis.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartCrossHairCursor.h"
#include "../../../../Hridoy/chartctrl/chartctrl/ChartDragLineCursor.h"

static UINT  g_numEncoders = 0;
static UINT  g_encoderSize = 0;

// CPlotTabDlg dialog

IMPLEMENT_DYNAMIC(CPlotTabDlg, CDialogEx)

CPlotTabDlg::CPlotTabDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_TAB_PLOT, pParent)
{

}

CPlotTabDlg::~CPlotTabDlg()
{
}

void CPlotTabDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_X, m_comboX);
	DDX_Control(pDX, IDC_COMBO_Y, m_comboY);
	DDX_Control(pDX, IDC_CHARTCTRL, m_chart);
}


BEGIN_MESSAGE_MAP(CPlotTabDlg, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_X, &CPlotTabDlg::OnCbnSelchangeComboX)
	ON_CBN_SELCHANGE(IDC_COMBO_Y, &CPlotTabDlg::OnCbnSelchangeComboY)
    //ON_NOTIFY_REFLECT(NM_RCLICK, &CPlotTabDlg::OnRClickChart)
    ON_COMMAND(ID_DOWNLOAD_DATA, &CPlotTabDlg::OnDownloadData)
    ON_COMMAND(ID_DOWNLOAD_CHART, &CPlotTabDlg::OnDownloadChart)
    ON_STN_CLICKED(IDC_CHARTCTRL, &CPlotTabDlg::OnStnClickedChartctrl)
END_MESSAGE_MAP()

BOOL CPlotTabDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

	//PostMessage(WM_USER + 100);

	//UpdateChart();

    return TRUE;
}


// CPlotTabDlg message handlers
void CPlotTabDlg::FillComboBoxes(const CStringArray& headers)
{
	m_headers.Copy(headers);

	m_comboX.ResetContent();
	m_comboY.ResetContent();

	for (int i = 0; i < m_headers.GetSize(); ++i)
	{
		m_comboX.AddString(m_headers[i]);
		m_comboY.AddString(m_headers[i]);
	}

	//default: first two different columns
	/*if (m_headers.GetSize() >= 2)
	{
		m_comboX.SetCurSel(0);
		m_comboY.SetCurSel(1);
	}
	else if (m_headers.GetSize() == 1)
	{
		m_comboX.SetCurSel(0);
		m_comboY.SetCurSel(0);
	}

	m_lastX = m_comboX.GetCurSel();
	m_lastY = m_comboY.GetCurSel();*/

}

void CPlotTabDlg::OnCbnSelchangeComboX()
{
	// TODO: Add your control notification handler code here
	int current = m_comboX.GetCurSel();

	//if user select same as Y-> auto seitch Y ot previous X
	if (current == m_comboY.GetCurSel() && current != -1)
	{
		m_comboY.SetCurSel(m_lastX);
	}
	m_lastX = current;
    m_lastY = m_comboY.GetCurSel();

    if (m_lastY != -1 && m_lastX != -1)
        UpdateChart();
	
}

void CPlotTabDlg::OnCbnSelchangeComboY()
{
	// TODO: Add your control notification handler code here
	int current = m_comboY.GetCurSel();

	//if user select same as Y-> auto seitch Y ot previous X
	if (current == m_comboX.GetCurSel() && current != -1)
	{
		m_comboX.SetCurSel(m_lastY);
	}
	m_lastY = current;
    m_lastX = m_comboX.GetCurSel();

    if(m_lastY != -1 && m_lastX != -1)
	    UpdateChart();
	
}

void CPlotTabDlg::FillChartData(const std::vector<std::vector<double>>& data) {
	m_data = data;
	/*if(!m_data.empty())
		UpdateChart();*/
}

void CPlotTabDlg::UpdateChart()
{
    int x = m_comboX.GetCurSel();
    int y = m_comboY.GetCurSel();

    if (x < 0 || y < 0 || x >= (int)m_data.size() || y >= (int)m_data.size())
        return;

    if (m_data[x].empty())
        return;

    // Clear old plot
    m_chart.RemoveAllSeries();

    // Enable refresh (you already do this)
    m_chart.EnableRefresh(true);

    // Create axes (you already do this)
    pBottomAxis = m_chart.CreateStandardAxis(CChartCtrl::BottomAxis);
    pLeftAxis = m_chart.CreateStandardAxis(CChartCtrl::LeftAxis);

    // Auto-scale based on real data
    double minX = *std::min_element(m_data[x].begin(), m_data[x].end()) + 10;
    double maxX = *std::max_element(m_data[x].begin(), m_data[x].end()) + 10;
    double minY = *std::min_element(m_data[y].begin(), m_data[y].end()) - 25;
    double maxY = *std::max_element(m_data[y].begin(), m_data[y].end()) + 10;

    // Add some padding
    double padX = (maxX - minX) * 0.05;
    double padY = (maxY - minY) * 0.05;

    pBottomAxis->SetMinMax(minX - padX, maxX + padX);
    pLeftAxis->SetMinMax(minY - padY, maxY + padY);

    m_chart.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
    m_chart.SendMessage(NIM_SETVERSION, 5);

    // Optional: nice tick spacing
    pBottomAxis->SetTickIncrement(false, (maxX - minX) / 10.0);
    pBottomAxis->SetDiscrete(false);
    pBottomAxis->EnableScrollBar(true);

    // Create the line series
    pSeries = m_chart.CreateLineSerie();

    // Set style
    pSeries->SetWidth(5);
    pSeries->SetColor(RGB(0, 120, 255));  // Beautiful blue

    //// Set name (optional)
    //pSeries->SetName(m_headers[y] + L" vs " + m_headers[x]);

    // === HERE: USE YOUR REAL DATA FROM m_data ===
    size_t count = m_data[x].size();

    // Convert vectors to arrays (ChartCtrl needs raw arrays)
    double* XVal = new double[count];
    double* YVal = new double[count];

    for (size_t i = 0; i < count; ++i)
    {
        XVal[i] = m_data[x][i];
        YVal[i] = m_data[y][i];
    }

    // Plot the real data!
    pSeries->SetPoints(XVal, YVal, (int)count);

    // Clean up memory
    delete[] XVal;
    delete[] YVal;

    // Axis labels
    pBottomAxis->GetLabel()->SetText(m_headers[x].AllocSysString());
    pLeftAxis->GetLabel()->SetText(m_headers[y].AllocSysString());

    // Final refresh
    m_chart.EnableRefresh(true);

   
}

// Right-click menu
BOOL CPlotTabDlg::PreTranslateMessage(MSG* pMsg)
{
    if (pMsg->message == WM_RBUTTONDOWN)
    {
        CPoint pt(LOWORD(pMsg->lParam), HIWORD(pMsg->lParam)); //client coordinates
       
        //check if click is inside the chart area
        CRect chartRect;
        m_chart.GetWindowRect(&chartRect);
        ScreenToClient(&chartRect);

        if (chartRect.PtInRect(pt))
        {
            //Convert to screen coordinates for menu
            CPoint screenPt = pt;
            ClientToScreen(&screenPt);

            CMenu menu;
            menu.CreatePopupMenu();
            menu.AppendMenu(MF_STRING, ID_DOWNLOAD_DATA, _T("Download Data as CSV"));
            menu.AppendMenu(MF_STRING, ID_DOWNLOAD_CHART, _T("Download Chart as PNG"));

            menu.TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, screenPt.x, screenPt.y, this);
            return TRUE;  // Message handled
        }
    }

    return CDialogEx::PreTranslateMessage(pMsg);
}

// Helper for PNG encoder
// === ADD THIS FUNCTION (the one that was missing!) ===
CLSID GetPngClsid()
{
    static CLSID pngClsid = { 0 };

    if (pngClsid.Data1 == 0)  // First time only
    {
        Gdiplus::GetImageEncodersSize(&g_numEncoders, &g_encoderSize);
        if (g_encoderSize == 0) return CLSID_NULL;

        Gdiplus::ImageCodecInfo* pInfo = (Gdiplus::ImageCodecInfo*)malloc(g_encoderSize);
        Gdiplus::GetImageEncoders(g_numEncoders, g_encoderSize, pInfo);

        for (UINT j = 0; j < g_numEncoders; ++j)
        {
            if (wcscmp(pInfo[j].MimeType, L"image/png") == 0)
            {
                pngClsid = pInfo[j].Clsid;
                break;
            }
        }
        free(pInfo);
    }

    return pngClsid;
}

// Download plotted data as CSV
void CPlotTabDlg::OnDownloadData()
{
    int x = m_comboX.GetCurSel();
    int y = m_comboY.GetCurSel();
    if (x < 0 || y < 0) {
        AfxMessageBox(_T("Please select X and Y columns."));
        return;
    }

    CFileDialog dlg(FALSE, _T("csv"), _T("data.csv"),
        OFN_OVERWRITEPROMPT, _T("CSV Files (*.csv)|*.csv||"));
    if (dlg.DoModal() == IDOK) {
        CStdioFile file(dlg.GetPathName(),
            CFile::modeCreate | CFile::modeWrite | CFile::typeText);
        CString line;
        line.Format(_T("%s,%s\n"), m_headers[x], m_headers[y]);
        file.WriteString(line);
        for (size_t i = 0; i < m_data[x].size(); ++i) {
            line.Format(_T("%.10f,%.10f\n"), m_data[x][i], m_data[y][i]);
            file.WriteString(line);
        }
        file.Close();
        AfxMessageBox(_T("CSV saved!"));
    }
}

// Download chart as PNG
void CPlotTabDlg::OnDownloadChart()
{
    CFileDialog dlg(FALSE, _T("png"), _T("chart.png"),
        OFN_OVERWRITEPROMPT, _T("PNG Images (*.png)|*.png||"));
    if (dlg.DoModal() != IDOK) return;

    CRect rect;
    m_chart.GetClientRect(&rect);

    CDC* pDC = m_chart.GetDC();
    CDC memDC;
    memDC.CreateCompatibleDC(pDC);
    CBitmap bitmap;
    bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
    CBitmap* pOld = memDC.SelectObject(&bitmap);
    memDC.BitBlt(0, 0, rect.Width(), rect.Height(), pDC, 0, 0, SRCCOPY);

    HBITMAP hBitmap = (HBITMAP)bitmap.GetSafeHandle();
    Gdiplus::Bitmap gdipBitmap(hBitmap, nullptr);
    gdipBitmap.Save(dlg.GetPathName(), &GetPngClsid(), nullptr);

    memDC.SelectObject(pOld);
    m_chart.ReleaseDC(pDC);

    AfxMessageBox(_T("Chart saved as PNG!"));
}


void CPlotTabDlg::OnStnClickedChartctrl()
{
    // TODO: Add your control notification handler code here
}
