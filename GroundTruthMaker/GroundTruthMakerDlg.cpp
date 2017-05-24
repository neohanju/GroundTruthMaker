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

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


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
// DIALOG
//=========================================================================
CGroundTruthMakerDlg::CGroundTruthMakerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GROUNDTRUTHMAKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}


void CGroundTruthMakerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_VIEWER, m_csVideoFrame);
	DDX_Control(pDX, IDC_SLIDER_VIDEO, m_ctrVideoSlider);
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
	ON_BN_CLICKED(IDC_BUTTON1, &CGroundTruthMakerDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CGroundTruthMakerDlg::OnBnClickedButton2)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_BN_CLICKED(IDC_CHECK1, &CGroundTruthMakerDlg::OnClickedCheck1)
	ON_BN_CLICKED(IDC_CHECK_HEAD, &CGroundTruthMakerDlg::OnClickedCheckHead)
	ON_BN_CLICKED(IDC_HAND_L, &CGroundTruthMakerDlg::OnBnClickedHandL)
	ON_BN_CLICKED(IDC_HAND_R, &CGroundTruthMakerDlg::OnClickedHandR)
	ON_BN_CLICKED(IDC_FOOT_L, &CGroundTruthMakerDlg::OnClickedFootL)
	ON_BN_CLICKED(IDC_FOOT_R, &CGroundTruthMakerDlg::OnClickedFootR)
	ON_BN_CLICKED(IDC_CHECK2, &CGroundTruthMakerDlg::OnClickedCheck2)
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

	m_ctrVideoSlider.SetRangeMin(0);
	m_ctrVideoSlider.SetLineSize(1);    // moving interval of keyboard direction keys
	m_ctrVideoSlider.SetPageSize(100);  // moving interval of page up/down keys 

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
		m_nCurVideoFrame = -1;

		// slider
		m_ctrVideoSlider.SetRangeMax(m_nNumVideoFrames);
		m_ctrVideoSlider.SetPos(0);
		m_ctrVideoSlider.SetTicFreq((int)m_pVideoCapture->get(CV_CAP_PROP_FPS) * 60);  // draw ticks every minutes

	}
	catch (int e)
	{
		// TODO: print message
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
		// TODO: print message
		return false;
	}
	return true;
}


bool CGroundTruthMakerDlg::ReadFrame(int position)
{	
	if (!m_bVideoOnRead) { return false; }
	if (position < -1)
	{
		// move to the next frame (do nothing) | last frame check
		m_nCurVideoFrame++;
	}
	else
	{
		// move to a specific frame
		m_nCurVideoFrame = MAX(0, MIN(position, m_nNumVideoFrames - 1));
		m_pVideoCapture->set(CV_CAP_PROP_POS_FRAMES, (double)m_nCurVideoFrame);		
	}
	(*m_pVideoCapture) >> m_matVideoFrame;
	//m_nCurVideoFrame = (int)m_pVideoCapture->get(CV_CAP_PROP_FRAME_COUNT);
	m_ctrVideoSlider.SetPos(m_nCurVideoFrame);
	CString buff;	buff.Format(_T("%d"), m_nCurVideoFrame);
	SetDlgItemText(IDC_STATIC_FI_FRAME_INDEX, buff);

	// TODO: display time
	//SetDlgItemText(IDC_STATIC_FI_TIME, buff);

	this->ShowFrame();

	return true;
}


void CGroundTruthMakerDlg::ShowFrame()
{
	RECT clientRect;
	m_csVideoFrame.GetClientRect(&clientRect);
	cv::Size rectSize(clientRect.right, clientRect.bottom);
	cv::Mat  matResizedImage;

	//size_t bpp = 8 * m_matVideoFrame.elemSize();
	//assert((bpp == 8 || bpp == 24 || bpp == 32));

	//int padding = 0;
	////32 bit image is always DWORD aligned because each pixel requires 4 bytes
	//if (bpp < 32) { padding = 4 - (m_matVideoFrame.cols % 4); }
	//if (padding == 4) { padding = 0; }	

	//int border = 0;
	////32 bit image is always DWORD aligned because each pixel requires 4 bytes
	//if (bpp < 32)
	//{
	//	border = 4 - (m_matVideoFrame.cols % 4);
	//}

	//if (border > 0 || m_matVideoFrame.isContinuous() == false)
	//{
	//	// Adding needed columns on the right (max 3 px)
	//	cv::copyMakeBorder(m_matVideoFrame, matResizedImage, 0, 0, 0, border, cv::BORDER_CONSTANT, 0);
	//}
	//else
	//{
	//	matResizedImage = m_matVideoFrame;
	//}
	////cv::cvtColor(matResizedImage, matResizedImage, CV_BGR2RGB);
	//cv::resize(matResizedImage, matResizedImage, rectSize);
	//cv::flip(matResizedImage, matResizedImage, 0);

	//if (m_pVideoFrameImage)
	//{
	//	m_pVideoFrameImage->ReleaseDC();
	//	delete m_pVideoFrameImage;
	//	m_pVideoFrameImage = nullptr;
	//}
	//m_pVideoFrameImage = new CImage();
	//m_pVideoFrameImage->Create(rectSize.width, rectSize.height, 24);

	//BITMAPINFO bitInfo;
	//bitInfo.bmiHeader.biBitCount = 24;
	//bitInfo.bmiHeader.biWidth = rectSize.width;
	//bitInfo.bmiHeader.biHeight = rectSize.height;
	//bitInfo.bmiHeader.biPlanes = 1;
	//bitInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	//bitInfo.bmiHeader.biCompression = BI_RGB;
	//bitInfo.bmiHeader.biClrImportant = 0;
	//bitInfo.bmiHeader.biClrUsed = 0;
	//bitInfo.bmiHeader.biSizeImage = 0;
	//bitInfo.bmiHeader.biXPelsPerMeter = 0;
	//bitInfo.bmiHeader.biYPelsPerMeter = 0;

	//StretchDIBits(m_pVideoFrameImage->GetDC(), 0, 0,
	//	rectSize.width, rectSize.height, 0, 0,
	//	rectSize.width, rectSize.height,
	//	matResizedImage.data, &bitInfo, DIB_RGB_COLORS, SRCCOPY
	//);

	//m_pVideoFrameImage->BitBlt(::GetDC(m_csVideoFrame.m_hWnd), 0, 0);
	//m_pVideoFrameImage->ReleaseDC();
	//delete m_pVideoFrameImage;
	//m_pVideoFrameImage = nullptr;


	if (m_pVideoFrameImage)
	{
		m_pVideoFrameImage->ReleaseDC();
		delete m_pVideoFrameImage;
		m_pVideoFrameImage = nullptr;
	}
	m_pVideoFrameImage = new CImage();
	m_pVideoFrameImage->Create(rectSize.width, rectSize.height, 24);

	cv::resize(m_matVideoFrame, matResizedImage, rectSize);
	int stride = ((((rectSize.width * 24) + 31)  &  ~31) >> 3);

	// Allocate a buffer for our DIB bits
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
		bitInfo.bmiHeader.biSizeImage = 0;      //winSize.height * winSize.width * * 3;
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
}



void CGroundTruthMakerDlg::OnBnClickedButtonNext()
{
	this->ReadFrame();	
}

void CGroundTruthMakerDlg::OnBnClickedButtonPrev()
{
	this->ReadFrame(m_nCurVideoFrame - 1);
}


void CGroundTruthMakerDlg::OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult)
{
	int nPos = m_ctrVideoSlider.GetPos();
	this->ReadFrame(nPos);
	*pResult = 0;
}


void CGroundTruthMakerDlg::OnBnClickedButton1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Cursor = TRUE;
}


void CGroundTruthMakerDlg::OnBnClickedButton2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Cursor = FALSE;
}


void CGroundTruthMakerDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	CRect rect;
	CRect FrameRect;
	CString str;

	//m_csVideoFrame.GetClientRect(&FrameRect);    // 사진 나오는 부분 크기 받아오는 코드(맞나? 확인하고 이거 쓸건지 아래꺼 쓸건지 정하기)
	GetDlgItem(IDC_VIEWER)->GetWindowRect(&rect); //사진 나오는 부분 바깥에 출력해주는 viewer 크기 전부다 받아오는 코드
	ScreenToClient(&rect);

	if ((m_Cursor == TRUE) && (point.x >= rect.left) && (point.x <= rect.right) && (point.y >= rect.top) && (point.y <= rect.bottom) )
	{
		SetCursor(::LoadCursor(NULL, IDC_CROSS));
		str.Format(_T("X = %d , Y = %d"),point.x,point.y);
		SetDlgItemText(IDC_EDIT_ID, str);
		str.Format(_T("rect_right = %d, rect_left = %d"), rect.right, rect.left);
		SetDlgItemText(IDC_EDIT_CLASS, str);
	}
	else
	{
		SetCursor(::LoadCursor(NULL, IDC_ARROW));
	}

	CDialogEx::OnMouseMove(nFlags, point);
}




void CGroundTruthMakerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CClientDC dc(this);
	CString str_x,str_y;

	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_Cursor == TRUE ) 
	{
	
		switch (check_box_part)
		{
			case 1:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_HEAD_X, str_x);
				SetDlgItemText(IDC_EDIT_HEAD_Y, str_y);
				break;

			case 2:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_HAND_L_X, str_x);
				SetDlgItemText(IDC_EDIT_HAND_L_Y, str_y);
				break;

			case 3:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_HAND_R_X, str_x);
				SetDlgItemText(IDC_EDIT_HAND_R_Y, str_y);
				break;

			case 4:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_FOOT_L_X, str_x);
				SetDlgItemText(IDC_EDIT_FOOT_L_Y, str_y);
				break;

			case 5:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_FOOT_R_X, str_x);
				SetDlgItemText(IDC_EDIT_FOOT_R_Y, str_y);
				break;

			case 6:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_BB_TOP, str_x);
				SetDlgItemText(IDC_EDIT_BB_LEFT, str_y);
				break;

			case 7:
				str_x.Format(_T("%d"), point.x);
				str_y.Format(_T("%d"), point.y);
				SetDlgItemText(IDC_EDIT_BB_BOTTOM, str_x);
				SetDlgItemText(IDC_EDIT_BB_RIGHT, str_y);
				break;

			// 이런식으로 좌표 받는게 맞는 방법일지? 다른 방법은 없는지?
		}


		

	}

	CDialogEx::OnLButtonDown(nFlags, point);
}



void CGroundTruthMakerDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CDialogEx::OnLButtonUp(nFlags, point);
}


void CGroundTruthMakerDlg::OnClickedCheck1()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 6;

}


void CGroundTruthMakerDlg::OnClickedCheckHead()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 1;
}


void CGroundTruthMakerDlg::OnBnClickedHandL()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 2;
}


void CGroundTruthMakerDlg::OnClickedHandR()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 3;
}


void CGroundTruthMakerDlg::OnClickedFootL()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 4;
}




void CGroundTruthMakerDlg::OnClickedFootR()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 5;
}


void CGroundTruthMakerDlg::OnClickedCheck2()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	check_box_part = 7;
}
