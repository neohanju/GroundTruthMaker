/******************************************************************************
* Title        : GroundTruthMakerDlg.cpp
* Author       : Haanju Yoo
* Initial Date : 2017.04.19
* Version Num. : 0.9
* Description  : Main dialog implementation
******************************************************************************/


#include "stdafx.h"
#include "GroundTruthMaker.h"
#include "GroundTruthMakerDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include "kcftracker.hpp"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

bool objectIDAscend(const CGTObjectInfo obj1, const CGTObjectInfo obj2) { return obj1.id < obj2.id; }

cv::Rect CRect2CVRect(CRect rect)
{
	float x = (float)rect.left;
	float y = (float)rect.top;
	float w = (float)(rect.right - rect.left + 1);
	float h = (float)(rect.bottom - rect.top + 1);
	return cv::Rect(x, y, w, h);
}

CRect CVRect2CRect(cv::Rect rect)
{
	int left = (int)rect.x;
	int top = (int)rect.y;
	int right = (int)(rect.x + rect.width - 1);
	int bottom = (int)(rect.y + rect.height - 1);
	return CRect(left, top, right, bottom);
}

//=========================================================================
// CGTObjectInfo
//=========================================================================
void CGTObjectInfo::Init()
{
	id = 0;
	category = 0;
	valid = false;
	for (int i = 0; i < NUM_PARTS; i++)
	{
		partPoints[i] = CPoint(0, 0);
	}
}

void CGTMetadata::insert(CGTObjectInfo newObject)
{
	bool bFound = false;
	for (int i = 0; i < (int)vecObjects.size(); i++)
	{
		if (newObject.id == vecObjects[i].id)
		{
			vecObjects[i] = newObject;
		}
	}
	if (!bFound) { vecObjects.push_back(newObject); }
}


//=========================================================================
// CGTMetadata
//=========================================================================
CGTObjectInfo* CGTMetadata::GetObjectInfo(int id)
{
	bool bFound = false;
	for (int i = 0; i < (int)vecObjects.size(); i++)
	{
		if (id == vecObjects[i].id)
		{
			return &vecObjects[i];
		}
	}
	if (!bFound)
	{
		CGTObjectInfo newObject;
		vecObjects.push_back(newObject);
	}
	return &vecObjects.back();
}

bool CGTMetadata::writefile(const CString strPath)
{
	try
	{
		std::ofstream outputFile;
		outputFile.open(strPath);

		std::vector<CGTObjectInfo> newObjects;
		for (int i = 0; i < (int)vecObjects.size(); i++)
		{
			if (vecObjects[i].valid) { newObjects.push_back(vecObjects[i]); }
		}
		vecObjects = newObjects;

		std::sort(newObjects.begin(), newObjects.end(), objectIDAscend);

		outputFile << (int)vecObjects.size() << "\n";
		for (int i = 0; i < (int)vecObjects.size(); i++)
		{
			if (!vecObjects[i].valid) { continue; }
			outputFile << vecObjects[i].id << " ";
			outputFile << vecObjects[i].category << " ";
			outputFile
				<< vecObjects[i].boundingBox.left << " "
				<< vecObjects[i].boundingBox.top << " "
				<< vecObjects[i].boundingBox.right << " "
				<< vecObjects[i].boundingBox.bottom << " ";
			for (int j = 0; j < NUM_PARTS; j++)
			{
				outputFile 
					<< vecObjects[i].partPoints[j].x << " " 
					<< vecObjects[i].partPoints[j].y << " ";
			}		
			outputFile << std::endl;
		}
		outputFile.close();
	}
	catch (int e)
	{
		printf("error number: %d\n", e);
		return false;
	}
	return true;
}

bool CGTMetadata::readfile(const CString strPath)
{
	CString strCurLine;	
	try
	{
		CStdioFile file(strPath, CFile::modeRead | CFile::typeText);
		file.ReadString(strCurLine);
		int numObject = _ttoi(strCurLine);
		while (true)
		{
			if (!file.ReadString(strCurLine)) { break; }

			CGTObjectInfo newObject;
			newObject.valid = true;

			int curPos = 0;
			CString restoken = strCurLine.Tokenize(_T(" "), curPos);			
			newObject.id = _ttoi(restoken);
			restoken = strCurLine.Tokenize(_T(" "), curPos);
			newObject.boundingBox.left = _ttoi(restoken);
			restoken = strCurLine.Tokenize(_T(" "), curPos);
			newObject.boundingBox.top = _ttoi(restoken);
			restoken = strCurLine.Tokenize(_T(" "), curPos);
			newObject.boundingBox.right = _ttoi(restoken);
			restoken = strCurLine.Tokenize(_T(" "), curPos);
			newObject.boundingBox.bottom = _ttoi(restoken);

			for (int i = 0; i < NUM_PARTS; i++)
			{
				restoken = strCurLine.Tokenize(_T(" "), curPos);
				newObject.partPoints[i].x = _ttoi(restoken);
				restoken = strCurLine.Tokenize(_T(" "), curPos);
				newObject.partPoints[i].y = _ttoi(restoken);
			}
			this->vecObjects.push_back(newObject);
		}
		file.Close();
		assert(numObject == (int)this->vecObjects.size());
	}
	catch (int e)
	{
		printf("error number: %d\n", e);
		return false;
	}
	return true;
}


//=========================================================================
// CAboutDlg
//=========================================================================
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


//=========================================================================
// CGroundTruthMakerDlg
//=========================================================================
CGroundTruthMakerDlg::CGroundTruthMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GROUNDTRUTHMAKER_DIALOG, pParent)
	, m_nRadioButton(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CGroundTruthMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_ID, m_nCurID);
	DDX_Control(pDX, IDC_VIEWER, m_csVideoFrame);
	DDX_Control(pDX, IDC_SLIDER_VIDEO, m_ctrVideoSlider);
	DDX_Control(pDX, IDC_COMBO_CATEGORY, m_comboCategory);
	DDX_Radio(pDX, IDC_RADIO_BOX, (int&)m_nRadioButton);	
}


BEGIN_MESSAGE_MAP(CGroundTruthMakerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CGroundTruthMakerDlg::OnBnClickedButtonLoad)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CGroundTruthMakerDlg::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CGroundTruthMakerDlg::OnBnClickedButtonPrev)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_VIDEO, &CGroundTruthMakerDlg::OnNMReleasedcaptureSliderVideo)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RADIO_BOX, IDC_RADIO_R_FOOT, &CGroundTruthMakerDlg::OnClickedRadioBox)
	ON_BN_CLICKED(IDC_BUTTON_CLEAR, &CGroundTruthMakerDlg::OnBnClickedButtonClear)
END_MESSAGE_MAP()


//=========================================================================
// MESSAGE HANDLERS
//=========================================================================
BOOL CGroundTruthMakerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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
	SetIcon(m_hIcon, FALSE);

	// video streaming
	m_bVideoOnRead = false;
	m_pVideoCapture = NULL;

	// viewer
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&m_rectViewer);
	ScreenToClient(&m_rectViewer);
	
	// slider
	m_ctrVideoSlider.SetRangeMin(0);
	m_ctrVideoSlider.SetLineSize(1);    // moving interval of keyboard direction keys
	m_ctrVideoSlider.SetPageSize(100);  // moving interval of page up/down keys 

	// category
	m_comboCategory.AddString(_T("0: Pedestrian"));
	m_comboCategory.AddString(_T("1: Garbage"));
	m_comboCategory.SetCurSel(0);

	// radio button for select input mode
	CButton* pButton = (CButton*)GetDlgItem(IDC_RADIO_BOX);
	pButton->SetCheck(true);	

	// initialize current object info
	m_nCurID = 0;
	m_ptCurObject = m_cCurMetadata.GetObjectInfo(m_nCurID);

	// for FSM (Finite State Machine)
	m_nCurrState = GUI_STATE_IDLE;
	m_nNextState = GUI_STATE_SET_BOX_LT;

	UpdateData(false);

	return TRUE;
}

void CGroundTruthMakerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CGroundTruthMakerDlg::OnPaint()
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
HCURSOR CGroundTruthMakerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CGroundTruthMakerDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (NULL != m_pVideoCapture) { delete m_pVideoCapture; }
}


void CGroundTruthMakerDlg::OnBnClickedButtonLoad()
{
	TCHAR szFilter[] = _T("Video (*.avi, *.mpeg, *.wmv) | *.AVI;*.MPEG;*.WMV | All Files(*.*)|*.*||");
	CFileDialog dlg(TRUE, NULL, NULL, OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, szFilter);
	dlg.m_ofn.lpstrInitialDir = _T(".");  // default position of file dialog	
	if (IDOK == dlg.DoModal())
	{
		CString strPathName = dlg.GetPathName();
		m_strVideoName = dlg.GetFileName();
		m_strMetadataFileDir = strPathName;		
		m_strMetadataFileDir.Truncate(m_strMetadataFileDir.ReverseFind('\\'));
		CT2CA pszConvertedAnsiString(strPathName);
		this->OpenVideo(std::string(pszConvertedAnsiString));
	}
	this->ReadFrame();	
}


bool CGroundTruthMakerDlg::OpenVideo(const std::string strVideoPath)
{
	try
	{
		if (m_bVideoOnRead) { this->CloseVideo(); }
		m_pVideoCapture = new cv::VideoCapture(strVideoPath);
		m_bVideoOnRead = true;
		m_nNumVideoFrames = (int)m_pVideoCapture->get(CV_CAP_PROP_FRAME_COUNT);
		m_nCurFrameIdx = -1;

		// slider
		m_ctrVideoSlider.SetRangeMax(m_nNumVideoFrames);
		m_ctrVideoSlider.SetPos(0);
		m_ctrVideoSlider.SetTicFreq((int)m_pVideoCapture->get(CV_CAP_PROP_FPS) * 60);  // draw ticks every minutes

		// viewer adjust
		//GetDlgItem(IDC_VIEWER)->GetWindowRect(&m_rectViewer);
		//ScreenToClient(&m_rectViewer);
	}
	catch (int e)
	{
		printf("error number: %d\n", e);
		return false;
	}
	return true;
}


bool CGroundTruthMakerDlg::CloseVideo()
{
	try
	{
		if (NULL != m_pVideoCapture)
		{
			if (m_pVideoCapture->isOpened()) { m_pVideoCapture->release(); }
			delete m_pVideoCapture;
		}
		m_bVideoOnRead = false;
	}
	catch (int e)
	{
		printf("error number: %d\n", e);
		return false;
	}
	return true;
}


bool CGroundTruthMakerDlg::ReadFrame(int position, bool bShowFrame)
{	
	if (!m_bVideoOnRead) { return false; }
	if (position < -1)
	{
		// move to the next frame (do nothing) | last frame check
		m_nCurFrameIdx++;
	}
	else
	{
		// move to a specific frame
		m_nCurFrameIdx = MAX(0, MIN(position, m_nNumVideoFrames - 1));
		m_pVideoCapture->set(CV_CAP_PROP_POS_FRAMES, (double)m_nCurFrameIdx);		
	}
	(*m_pVideoCapture) >> m_matVideoFrame;
	//m_nCurFrameIdx = (int)m_pVideoCapture->get(CV_CAP_PROP_FRAME_COUNT);
	m_ctrVideoSlider.SetPos(m_nCurFrameIdx);
	CString buff; buff.Format(_T("%d"), m_nCurFrameIdx);
	SetDlgItemText(IDC_STATIC_FI_FRAME_INDEX, buff);

	// TODO: display time
	//SetDlgItemText(IDC_STATIC_FI_TIME, buff);

	m_ptCurObject = m_cCurMetadata.GetObjectInfo(m_nCurID);

	if (bShowFrame)
		this->ShowFrame();

	return true;
}


void CGroundTruthMakerDlg::ShowFrame()
{
	if (!m_bVideoOnRead) { return; }

	//----------------------------------------------------------
	// DRAW VIDEO FRAME TO STATIC IMAGE
	//----------------------------------------------------------
	RECT clientRect;
	m_csVideoFrame.GetClientRect(&clientRect);
	cv::Size rectSize(
		clientRect.right - clientRect.left + 1, 
		clientRect.bottom - clientRect.top + 1);

	if (m_pVideoFrameImage)
	{
		m_pVideoFrameImage->ReleaseDC();
		delete m_pVideoFrameImage;
		m_pVideoFrameImage = nullptr;
	}
	m_pVideoFrameImage = new CImage();
	m_pVideoFrameImage->Create(rectSize.width, rectSize.height, 24);


	cv::Mat  matResizedImage;
	cv::resize(m_matVideoFrame, matResizedImage, rectSize);

	// Allocate a buffer for our DIB bits
	int stride = ((((rectSize.width * 24) + 31)  &  ~31) >> 3);	
	uchar* pcDibBits = (uchar*)malloc(rectSize.height * stride);
	if (pcDibBits != NULL)
	{
		// Copy the raw pixel data over to our dibBits buffer.
		// NOTE: Can setup cvImgTmp to add the padding to skip this.
		for (int row = 0; row < matResizedImage.rows; ++row)
		{
			// Get pointers to the beginning of the row on both buffers
			uchar* pcSrcPixel = matResizedImage.ptr<uchar>(row);
			uchar* pcDstPixel = pcDibBits + (row * stride);

			// We can just use memcpy
			memcpy(pcDstPixel,
				pcSrcPixel,
				stride);
		}

		// Initialize the BITMAPINFO structure
		BITMAPINFO bitInfo;
		bitInfo.bmiHeader.biBitCount = 24;
		bitInfo.bmiHeader.biWidth = matResizedImage.cols;
		bitInfo.bmiHeader.biHeight = -matResizedImage.rows;
		bitInfo.bmiHeader.biPlanes = 1;
		bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bitInfo.bmiHeader.biCompression = BI_RGB;
		bitInfo.bmiHeader.biClrImportant = 0;
		bitInfo.bmiHeader.biClrUsed = 0;
		bitInfo.bmiHeader.biSizeImage = 0; //winSize.height * winSize.width * * 3;
		bitInfo.bmiHeader.biXPelsPerMeter = 0;
		bitInfo.bmiHeader.biYPelsPerMeter = 0;

		// Add header and OPENCV image's data to the HDC
		StretchDIBits(m_pVideoFrameImage->GetDC(), 0, 0,
			rectSize.width, rectSize.height, 0, 0,
			rectSize.width, rectSize.height,
			pcDibBits, &bitInfo, DIB_RGB_COLORS, SRCCOPY);
		free(pcDibBits);
	}		
	m_pVideoFrameImage->BitBlt(::GetDC(m_csVideoFrame.m_hWnd), 0, 0);
	m_pVideoFrameImage->ReleaseDC();
	delete m_pVideoFrameImage;
	m_pVideoFrameImage = nullptr;
	

	//----------------------------------------------------------
	// DRAW BOXES
	//----------------------------------------------------------


	CDC *dc = GetDC();
	CBrush brush;
	brush.CreateStockObject(NULL_BRUSH);
	CBrush *oldBrush = dc->SelectObject(&brush);
	
	for (int i = 0; i < (int)m_cCurMetadata.vecObjects.size(); i++)
	{
		if (!m_cCurMetadata.vecObjects[i].valid) { continue; }

		COLORREF penColor = RGB(170, 170, 170);
		if (m_cCurMetadata.vecObjects[i].id == m_nCurID)
		{
			penColor = RGB(255, 0, 0);
		}
		CPen pen;
		pen.CreatePen(PS_DOT, 3, penColor);
		CPen* oldPen = dc->SelectObject(&pen);

		// bounding box
		dc->Rectangle(
			m_cCurMetadata.vecObjects[i].boundingBox.left,
			m_cCurMetadata.vecObjects[i].boundingBox.top,
			m_cCurMetadata.vecObjects[i].boundingBox.right,
			m_cCurMetadata.vecObjects[i].boundingBox.bottom);

		// body parts
		for (int j = 0; j < NUM_PARTS; j++)
		{
			if (0 == m_cCurMetadata.vecObjects[i].partPoints[j].x
				&& 0 == m_cCurMetadata.vecObjects[i].partPoints[j].y)
			{
				continue;
			}
			dc->Rectangle(
				m_cCurMetadata.vecObjects[i].partPoints[j].x - 1,
				m_cCurMetadata.vecObjects[i].partPoints[j].y - 1,
				m_cCurMetadata.vecObjects[i].partPoints[j].x + 1,
				m_cCurMetadata.vecObjects[i].partPoints[j].y + 1);
		}	

		// draw labels
		CString strID;
		strID.Format(_T("%d"), m_cCurMetadata.vecObjects[i].id);
		dc->SetTextColor(penColor);
		dc->SetBkColor(RGB(0, 0, 0));
		dc->TextOut(
			m_cCurMetadata.vecObjects[i].boundingBox.left,
			m_cCurMetadata.vecObjects[i].boundingBox.top - 15,	// 13) 저 좌표값을 움직이면 된다. (하드 코딩한것 고치기)
			strID);
		dc->SelectObject(oldPen);
	}
	dc->SelectObject(oldBrush);

	Invalidate(FALSE);	
}


void CGroundTruthMakerDlg::OnBnClickedButtonNext()
{
	this->SaveMetadata();
	this->ReadFrame(m_nCurFrameIdx + 1);
}

void CGroundTruthMakerDlg::OnBnClickedButtonPrev()
{
	this->ReadFrame(m_nCurFrameIdx - 1);
}


void CGroundTruthMakerDlg::OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPos = m_ctrVideoSlider.GetPos();
	this->ReadFrame(nPos);
	*pResult = 0;
}


void CGroundTruthMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{	
	bool bViewUpdate = false;
	CString strStatic;

	m_nCurrState = m_nNextState;
	switch (m_nCurrState)
	{
	case GUI_STATE_SET_BOX_RB:
		m_ptCurObject->boundingBox.right = MAX(0, MIN(point.x, m_rectViewer.right - 1));
		m_ptCurObject->boundingBox.bottom = MAX(0, MIN(point.y, m_rectViewer.bottom - 1));
		strStatic.Format(_T("(%d, %d, %d, %d)"),
			m_ptCurObject->boundingBox.left,
			m_ptCurObject->boundingBox.top,
			m_ptCurObject->boundingBox.right,
			m_ptCurObject->boundingBox.bottom);
		SetDlgItemText(IDC_STATIC_BOX_INFO, strStatic);
		m_nNextState = m_nCurrState;

		bViewUpdate = true;		
		break;
	default:
		m_nNextState = m_nCurrState;
		break;
	}

	if (bViewUpdate)
	{
		this->ShowFrame();
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CGroundTruthMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	bool bViewUpdate = false;
	CString strStatic;

	if (m_rectViewer.PtInRect(point))
	{
		m_nCurrState = m_nNextState;
		switch (m_nCurrState)
		{
		case GUI_STATE_SET_BOX_LT:
			m_ptCurObject->boundingBox.left = point.x;
			m_ptCurObject->boundingBox.top = point.y;
			m_ptCurObject->boundingBox.right = point.x;
			m_ptCurObject->boundingBox.bottom = point.y;
			m_ptCurObject->valid = true;

			m_nNextState = GUI_STATE_SET_BOX_RB;
			bViewUpdate = true;
			break;
		case GUI_STATE_SET_BODY_PART:			
			m_ptCurObject->partPoints[m_nRadioButton-1] = point;
			strStatic.Format(_T("(%d, %d)"),
				m_ptCurObject->partPoints[m_nRadioButton-1].x,
				m_ptCurObject->partPoints[m_nRadioButton-1].y);
			SetDlgItemText(IDC_STATIC_BOX_INFO + m_nRadioButton, strStatic);
			m_ptCurObject->valid = true;

			m_nNextState = GUI_STATE_SET_BODY_PART;
			bViewUpdate = true;
			break;
		case GUI_STATE_ADJUST_BODY_BOX:
			// TODO: recognize which adjust point is selected
			break;
		default:
			m_nNextState = GUI_STATE_SET_BOX_RB;
			break;
		}
	}

	if (bViewUpdate)
	{
		this->ShowFrame();
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CGroundTruthMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	bool bViewUpdate = false;
	CString strStatic;
	
	m_nCurrState = m_nNextState;
	switch (m_nCurrState)
	{
	case GUI_STATE_SET_BOX_RB:		
		m_nNextState = GUI_STATE_SET_BOX_LT;		
		break;
	default:
		m_nNextState = m_nCurrState;
		break;
	}

	if (bViewUpdate)
	{
		this->ShowFrame();
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CGroundTruthMakerDlg::OnClickedRadioBox(UINT msg)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	UpdateData(TRUE);
	if (m_nRadioButton == 0) 
	{ 
		m_nNextState = GUI_STATE_SET_BOX_LT;
	}
	else
	{
		m_nNextState = GUI_STATE_SET_BODY_PART;
	}
}


void CGroundTruthMakerDlg::SaveMetadata()
{
	if (m_bDataChanged)
	{			
		CString strMetadataFilePath;
		strMetadataFilePath.Format(_T("%s\\%s_%06d"), m_strMetadataFileDir, m_strVideoName, m_nCurFrameIdx);
		m_cCurMetadata.writefile(strMetadataFilePath);
	}
}


void CGroundTruthMakerDlg::OnBnClickedButtonClear()
{
	m_ptCurObject->Init();
}


BOOL CGroundTruthMakerDlg::PreTranslateMessage(MSG *pMsg)
{
	if (m_bVideoOnRead && WM_KEYDOWN == pMsg->message)
	{
		switch (pMsg->wParam)
		{
		case VK_LEFT:
			// OnBnClickedButtonFramePrev();
			break;
		case VK_RIGHT:
			// OnBnClickedButtonFrameNext();
			break;
		case VK_UP:
			// OnBnClickedButtonIdUp();
			break;
		case VK_DOWN:
			// OnBnClickedButtonIdDown();
			break;
		case VK_SPACE:
			Track();
			break;
		default:
			// do nothing
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}


//=========================================================================
// OPERATIONS
//=========================================================================
void CGroundTruthMakerDlg::Track()
{
	// initialize trackers
	std::vector<KCFTracker> vecTrackers(0);
	for (int objIdx = 0; objIdx < m_cCurMetadata.vecObjects.size(); objIdx++)
	{
		bool HOG = true;
		bool FIXEDWINDOW = false;
		bool MULTISCALE = true;
		bool SILENT = true;
		bool LAB = false;

		KCFTracker tracker(HOG, FIXEDWINDOW, MULTISCALE, LAB);
		tracker.init(
			CRect2CVRect(m_cCurMetadata.vecObjects[objIdx].boundingBox),
			m_matVideoFrame);
		vecTrackers.push_back(tracker);
	}

	// proceed to the next frame
	int nPrevFrameIndex = m_nCurFrameIdx;
	this->ReadFrame(nPrevFrameIndex + 1, false);

	if (nPrevFrameIndex != m_nCurFrameIdx)  // if there is another frame 
	{		
		m_cCurMetadata.frameIndex = m_nCurFrameIdx;		
		for (int objIdx = 0; objIdx < vecTrackers.size(); objIdx++)
		{
			m_cCurMetadata.vecObjects[objIdx].boundingBox =
				CVRect2CRect(vecTrackers[objIdx].update(m_matVideoFrame));
		}
	}

	this->ShowFrame();
}


//()()
//('')HAANJU.YOO
