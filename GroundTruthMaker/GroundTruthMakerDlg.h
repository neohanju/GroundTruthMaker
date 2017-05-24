/******************************************************************************
* Title        : CGroundTruthMakerDlg.h
* Author       : Haanju Yoo
* Initial Date : 2017.04.19
* Version Num. : 0.9
* Description  : Main dialog of this application
******************************************************************************/

#pragma once

#include "opencv2/opencv.hpp"


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
	bool check_box_1 = FALSE;
	int check_box_part;
protected:
	HICON m_hIcon;

	// Video streaming related
	CStatic m_csVideoFrame;
	CImage* m_pVideoFrameImage;

	bool m_bVideoOnRead;
	cv::VideoCapture *m_pVideoCapture;	
	int m_nNumVideoFrames;
	int m_nCurVideoFrame;
	cv::Mat m_matVideoFrame;

	// slider
	CSliderCtrl m_ctrVideoSlider;
public:
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrev();
	afx_msg void OnNMReleasedcaptureSliderVideo(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnClickedCheck1();
	afx_msg void OnClickedCheckHead();
	afx_msg void OnBnClickedHandL();
	afx_msg void OnClickedHandR();
	afx_msg void OnClickedFootL();
	afx_msg void OnClickedFootR();
	afx_msg void OnClickedCheck2();
};
