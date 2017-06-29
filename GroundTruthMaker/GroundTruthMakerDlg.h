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
#include "afxwin.h"
#include "afxcmn.h"

enum PARTS {
	BOUNDING_BOX = -1,
	PART_HEAD = 0,
	PART_LEFT_HAND,
	PART_RIGHT_HAND,
	PART_LEFT_FOOT,
	PART_RIGHT_FOOT,
	NUM_PARTS
};

enum GUI_STATE {
	GUI_STATE_IDLE = 0,
	GUI_STATE_SET_BOX_LT,
	GUI_STATE_SET_BOX_RB,
	GUI_STATE_SET_BODY_PART,
	GUI_STATE_ADJUST_BODY_BOX,
	NUM_GUI_STATE
};

enum ADJUST_POINT {
	AP_LT = 0,
	AP_T,
	AP_RT,
	AP_R,
	AP_RB,
	AP_B,
	AP_LB,
	AP_L,
	NUM_AP
};

enum MOUSE_CURSOR_TYPE {
	MCT_NORMAL = 0,
	MCT_CROSS,
	MCT_SIZE_NESW,
	MCT_SIZE_NS,
	MCT_SIZE_NWSE,
	MCT_SIZE_WE,
	NUM_MCT
};

class CGTObjectInfo
{
public:
	CGTObjectInfo() { Init(); }
	void Init(int _id = 0);
	bool valid;
	int id;
	int category;
	CRect boundingBox;
	CPoint partPoints[NUM_PARTS];
};

class CGTMetadata
{
public:
	CGTMetadata() : nFrameIndex(0), bGarbageDump(false) {}
	void clear() 
	{ 
		this->vecObjects.clear(); 
		this->bGarbageDump = false;
	}
	void insert(CGTObjectInfo newObject);
	CGTObjectInfo *GetObjectInfo(int id);
	bool writefile(const CString strPath);
	bool readfile(const CString strPath);
	int nFrameIndex;
	bool bGarbageDump;
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
	afx_msg void OnBnClickedButtonClear();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnSelchangeComboCategory();
	afx_msg void OnClickedGo();
	afx_msg void OnClickedButtonDelete();
	afx_msg void OnClickedCheckEventGargage();
	afx_msg void OnEnChangeEditId();
	BOOL PreTranslateMessage(MSG *pMsg);


protected:
	bool OpenVideo(const std::string strVideoPath);
	bool CloseVideo();
	bool ReadFrame(int position = -2, bool bShowFrame = true);
	void ShowFrame();
	void UpdateObjectInfoField();
	void UpdateEventField();
	void AdjustVideSlider(int position);
	void SaveMetadata();
	void ReadMetadata();
	void Track();
	void InitBodyPointColor();
	void CheckCorrectBoundingBox();

	//---------------------------------------------------------------------
	// VARIABLES
	//---------------------------------------------------------------------	
	int top, bottom, right, left;
	bool m_bDataChanged;
	CGTMetadata m_cCurMetadata;
	CGTObjectInfo* m_ptCurObject;
	CString m_strVideoName;
	CString m_strMetadataFileDir;

	HICON m_hIcon;
	std::vector<COLORREF> pointColor;

	// Video streaming related
	CStatic m_csVideoFrame;
	CImage* m_pVideoFrameImage;
	CRect m_rectViewer;

	// ID
	CSpinButtonCtrl m_cSpinBtnCtrl;

	bool m_bVideoOnRead;
	cv::VideoCapture *m_pVideoCapture;
	int m_nNumVideoFrames;
	int m_nCurFrameIdx;
	int m_nCurID;
	int m_nCurComboID;
	cv::Mat m_matVideoFrame;

	// slider
	CSliderCtrl m_ctrVideoSlider;

	// object information	
	CComboBox m_comboCategory;
	UINT m_nRadioButton;

	// event information
	CButton m_ctrlCheckGarbageDumping;

	// GUI FSM related
	ADJUST_POINT m_nCurAdjustingPoint;
	MOUSE_CURSOR_TYPE m_nCursorType;
	HCURSOR m_arrCursors[NUM_MCT];
	MOUSE_CURSOR_TYPE m_arrApCursorTypes[NUM_AP];
	GUI_STATE m_nCurrState;
	GUI_STATE m_nNextState;	

};
