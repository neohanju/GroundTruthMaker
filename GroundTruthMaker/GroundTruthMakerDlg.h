/******************************************************************************
* Title        : CGroundTruthMakerDlg.h
* Author       : Haanju Yoo
* Initial Date : 2017.04.19
* Version Num. : 0.9
* Description  : Main dialog of this application
******************************************************************************/

#pragma once

#include "opencv2/opencv.hpp"
#include <vector>

enum PARTS { BOUNDING_BOX = -1, PART_HEAD = 0, PART_LEFT_HAND, PART_RIGHT_HAND, PART_LEFT_FOOT, PART_RIGHT_FOOT, NUM_PARTS };

class CGTObjectInfo
{
public:
	CGTObjectInfo()
	{
		Init();
	}
	void Init()
	{
		id = 0;
		category = 0;
		valid = false;
		for (int i = 0; i < NUM_PARTS; i++)
		{
			partPoints[i] = CPoint(0, 0);
		}
	}
	bool valid;
	int id;
	int category;
	CRect boundingBox;
	CPoint partPoints[NUM_PARTS];
};

class CGTMetadata
{
public:
	CGTMetadata() : frameIndex(0) {}
	void clear()
	{		
		this->vecObjects.clear();
	}
	void insert(CGTObjectInfo newObject);
	CGTObjectInfo *GetObjectInfo(int id);
	bool writefile(const CString strPath);
	bool readfile(const CString strPath);	
	int frameIndex;
	std::vector<CGTObjectInfo> vecObjects;
};


class CGroundTruthMakerDlg : public CDialogEx
{
	//---------------------------------------------------------------------
	// METHODS
	//---------------------------------------------------------------------
public:
	CGroundTruthMakerDlg(CWnd* pParent = NULL);	// standard constructor

	// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GROUNDTRUTHMAKER_DIALOG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

protected:
	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonLoad();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickedRadioBox(UINT msg);

protected:
	bool OpenVideo(const std::string strVideoPath);
	bool CloseVideo();
	bool ReadFrame(int position = -2);
	void ShowFrame();
	void AdjustVideSlider(int position);
	void SaveMetadata();
	void CGroundTruthMakerDlg::Showbox(int x, int y, int width, int height);
	void SetPointValue(CPoint clickedpoint);

	//---------------------------------------------------------------------
	// VARIABLES
	//---------------------------------------------------------------------
	bool m_Cursor;
	int top, bottom, right, left;
	bool m_bDataChanged;
	CGTMetadata m_cCurMetadata;	
	CGTObjectInfo* m_ptCurObject;
	CString m_strVideoName;
	CString m_strMetadataFileDir;
	
	HICON m_hIcon;

	// Video streaming related
	CStatic m_csVideoFrame;
	CImage* m_pVideoFrameImage;

	bool m_bVideoOnRead;
	cv::VideoCapture *m_pVideoCapture;	
	int m_nNumVideoFrames;
	int m_nCurVideoFrame;
	int m_nCurID;
	cv::Mat m_matVideoFrame;

	// slider
	CSliderCtrl m_ctrVideoSlider;

	CPoint m_ptPrePosition;
	UINT m_nRadioButton;
public:
	afx_msg void OnBnClickedButtonClear();
};
