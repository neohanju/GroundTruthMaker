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
/*
class CGTObjectInfo
{
public:
	CGTObjectInfo() : id(0), objectBoxX(0), objectBoxY(0), objectBoxW(0), objectBoxH(0) {}
	
	int id;
	int category;
	int objectBoxX;
	int objectBoxY;
	int objectBoxW;
	int objectBoxH;
	int headX;
	int headY;
	int rightHandX;
	int rightHandY;
	int leftHandX;
	int leftHandY;
	int rightFootX;
	int rightFootY;
	int leftFootX;
	int leftFootY;
};

class CGTMetadata
{
public:
	CGTMetadata() : numObject(0), frameIndex(0) {}
	void clear()
	{
		this->numObject = 0;
		this->vecObjects.clear();
	}
	bool writefile(const CString strPath);
	bool readfile(const CString strPath);

	int numObject;
	int frameIndex;
	std::vector<CGTObjectInfo> vecObjects;
};
*/

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

protected:
	bool OpenVideo(const std::string strVideoPath);
	bool CloseVideo();
	bool ReadFrame(int position = -2);
	void ShowFrame();
	void AdjustVideSlider(int position);

	//---------------------------------------------------------------------
	// VARIABLES
	//---------------------------------------------------------------------
	bool m_Cursor;
	int top, bottom, right, left;
	bool m_bDataChanged;
	//CGTMetadata m_cCurMetadata;
	CString strMetadataFilePath;
protected:
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
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	UINT m_radio;
	afx_msg void OnClickedRadioBox(UINT msg);
	void SetPointValue(CPoint clickedpoint);
	CPoint prePosition;
	//void SaveMetadata();
	//void CreateMetadata();
};
