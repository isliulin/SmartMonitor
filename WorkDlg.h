#pragma once
#include "afxwin.h"
#include "afxcmn.h"
#include "resource.h"

//后台运行
#define WM_HIDE_RUN  (WM_USER+102)
// WorkDlg 对话框

class WorkDlg : public CDialogEx
{
	DECLARE_DYNAMIC(WorkDlg)

public:
	WorkDlg(CWnd* pParent = NULL); //标准构造函数
	virtual ~WorkDlg();

    // 对话框数据
	enum { IDD = IDD_CAMERADLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);   
	// DDX/DDV 支持
	HICON m_hIcon;
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	
public:
	//后台运行
	LRESULT OnHideRun(WPARAM wParam,LPARAM lParam);
	POINT Old;
	//清理程序
	afx_msg void OnDestroy();
	//初始化程序界面
	virtual BOOL OnInitDialog();
	//最大化，最小化
	afx_msg void OnSize(UINT nType,int cx,int cy);
	
	//高空检测摄像头连接
	afx_msg void OnLinkHighVideo();
	//退出高空检测
	afx_msg void OnExitHighVideo();

	//选择监控区域
	afx_msg void OnSelectMoniterArea();
	//取消监控区域
	afx_msg void OnCancelSelect();


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnEmptyGrid();
	afx_msg void OnRefresh();


	//传感器
	CListCtrl m_gridcom;
	int m_ReceiveNum;
	BYTE m_ReceiveByte[14];
	DECLARE_EVENTSINK_MAP()
	void OnCommMscomm();
	CMSComm m_Comm;
	CComboBox m_ComSel;
	afx_msg void OnComSelect();
	afx_msg void OnDetectAngle();
	afx_msg void OnAngleStop();
	afx_msg void OnTimer(UINT_PTR nIDEvent);


public:
	afx_msg void OnRunInBack();
	afx_msg void OnExit();
	afx_msg void OnStartPhoneSystem();
	afx_msg void OnFaceRecognize();

	afx_msg void OnOpenDoc();
	afx_msg void OnOrgDoc();

	afx_msg void OnLinkBehaviorVideo();
	afx_msg void OnLinkMillVideo();
	afx_msg void OnLinkMoreVideo();
	afx_msg void OnExitBehaviorVideo();
	afx_msg void OnExitMillVideo();
	afx_msg void OnExitMoreVideo();
	virtual void OnOK();

public:
	Mat NoPic;
	afx_msg void OnClose();
	afx_msg void OnRecordHighVideo();
	afx_msg void OnStopHighRecord();
	afx_msg void OnRecordBehavior();
	afx_msg void OnStopBehavior();
	afx_msg void OnMillRecord();
	afx_msg void OnStopMillRecord();
	afx_msg void OnMoreRecord();
	afx_msg void OnStopMoreRecord();
	//查看记录
	afx_msg void OnLookDiary();


};
