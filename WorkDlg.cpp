// WorkDlg.cpp : 实现文件
#include "stdafx.h"
#include "Moniter.h"
#include "WorkDlg.h"
#include "afxdialogex.h"

// WorkDlg 对话框
IMPLEMENT_DYNAMIC(WorkDlg, CDialogEx)

WorkDlg::WorkDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(WorkDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);

	HighWorkDetecting = FALSE;
	BehaviorDetecting = FALSE;
	MillDetecting = FALSE;


	CString Date= "D:\\Record\\"+GetDate();
	CreateDirectory(Date,NULL);
	ClearPhoneFile();

	try
	{
		NoPic = imread("NoPic.jpg");
	}
	catch (...)
	{
		AfxMessageBox("图片文件缺失！");
	}

	if (NoPic.empty()==false)
	{
		CString path = "D:\\施工现场监控信息\\notificationPhoto\\" + GetSaveTime()+".jpg";
		cvSaveImage(path,
			&(IplImage)NoPic);
	}


}




WorkDlg::~WorkDlg()
{

}




void WorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, HighPic);
	DDX_Control(pDX, IDC_LIST1_HIGH, HighGrid);
	DDX_Control(pDX, IDC_LIST1, m_gridcom);
	DDX_Control(pDX, IDC_MSCOMM1, m_Comm);
	DDX_Control(pDX, IDC_COMBO1, m_ComSel);
	DDX_Control(pDX, IDC_LIST1_HIGH2, BehaviorGrid);
	DDX_Control(pDX, IDC_LIST1_HIGH3, MillGrid);
	DDX_Control(pDX, IDC_PICTURE2, BehaviorPic);
	DDX_Control(pDX, IDC_PICTURE3, MillPic);
	DDX_Control(pDX, IDC_PICTURE4, MorePic);
}




BEGIN_MESSAGE_MAP(WorkDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON1, &WorkDlg::OnLinkHighVideo)
	ON_BN_CLICKED(IDC_BUTTON2, &WorkDlg::OnExitHighVideo)
	ON_BN_CLICKED(IDC_BUTTON3, &WorkDlg::OnSelectMoniterArea)
	ON_BN_CLICKED(IDC_BUTTON4, &WorkDlg::OnCancelSelect)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EMPTY_GRID, &WorkDlg::OnEmptyGrid)
	ON_COMMAND(ID_REFRESH, &WorkDlg::OnRefresh)
	ON_CBN_SELCHANGE(IDC_COMBO1, &WorkDlg::OnComSelect)
	ON_BN_CLICKED(IDC_BUTTON13, &WorkDlg::OnDetectAngle)
	ON_BN_CLICKED(IDC_BUTTON14, &WorkDlg::OnAngleStop)
	ON_WM_TIMER()
	ON_COMMAND(ID_RUN_IN_BACK, &WorkDlg::OnRunInBack)
	ON_COMMAND(ID_32771, &WorkDlg::OnExit)
	ON_COMMAND(ID_32784, &WorkDlg::OnStartPhoneSystem)
	ON_MESSAGE(WM_HIDE_RUN,OnHideRun)
	ON_COMMAND(ID_32793, &WorkDlg::OnFaceRecognize)
	ON_COMMAND(ID_32794, &WorkDlg::OnOpenDoc)
	ON_COMMAND(ID_32795, &WorkDlg::OnOrgDoc)
	ON_BN_CLICKED(IDC_BUTTON21, &WorkDlg::OnLinkBehaviorVideo)
	ON_BN_CLICKED(IDC_BUTTON27, &WorkDlg::OnLinkMillVideo)
	ON_BN_CLICKED(IDC_BUTTON30, &WorkDlg::OnLinkMoreVideo)
	ON_BN_CLICKED(IDC_BUTTON9, &WorkDlg::OnExitBehaviorVideo)
	ON_BN_CLICKED(IDC_BUTTON26, &WorkDlg::OnExitMillVideo)
	ON_BN_CLICKED(IDC_BUTTON29, &WorkDlg::OnExitMoreVideo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON38, &WorkDlg::OnRecordHighVideo)
	ON_BN_CLICKED(IDC_BUTTON37, &WorkDlg::OnStopHighRecord)
	ON_BN_CLICKED(IDC_BUTTON36, &WorkDlg::OnRecordBehavior)
	ON_BN_CLICKED(IDC_BUTTON35, &WorkDlg::OnStopBehavior)
	ON_BN_CLICKED(IDC_BUTTON34, &WorkDlg::OnMillRecord)
	ON_BN_CLICKED(IDC_BUTTON33, &WorkDlg::OnStopMillRecord)
	ON_BN_CLICKED(IDC_BUTTON31, &WorkDlg::OnMoreRecord)
	ON_BN_CLICKED(IDC_BUTTON32, &WorkDlg::OnStopMoreRecord)
	ON_BN_CLICKED(IDC_BUTTON41, &WorkDlg::OnLookDiary)
END_MESSAGE_MAP()




// WorkDlg 消息处理程序
BOOL WorkDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	// TODO:  在此添加额外的初始化
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	m_ComSel.SetCurSel(0);


	//状态栏
	StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, 
		CRect(0,0,0,0), this, 0);

	int strPartDim[5]= {100,300,500,700,-1}; //分割数量,使用坐标
	StatusBar.SetParts(5, strPartDim);

	//设置状态栏文本
	StatusBar.SetText("系统状态", 0, 0);
	StatusBar.SetText("就绪", 1, 0);
	StatusBar.SetText("就绪", 2, 0);
	StatusBar.SetText("就绪", 3, 0);
	StatusBar.SetText("就绪", 4, 0);

	StatusBar.ShowWindow(SW_SHOW);


	//onsize大小随着对话框大小变化而变化
	CRect rectSize;     
	GetClientRect(&rectSize);     //取客户区大小   
	Old.x=rectSize.right-rectSize.left; 
	Old.y=rectSize.bottom-rectSize.top; 


	InitializeCriticalSection(&DetectionCritical);

	//设置list显示
	HighGrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	HighGrid.ModifyStyle( 0, LVS_REPORT );

	HighGrid.InsertColumn(0,"检测时间");        //插入列
	HighGrid.InsertColumn(1,"高空作业情况");   
	   
	HighGrid.SetColumnWidth(0,125);       //设置列的宽度。
	HighGrid.SetColumnWidth(1,160);


	//行为
	BehaviorGrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	BehaviorGrid.ModifyStyle( 0, LVS_REPORT );

	BehaviorGrid.InsertColumn(0,"检测时间");        //插入列
	BehaviorGrid.InsertColumn(1,"施工现场行为检测");   

	BehaviorGrid.SetColumnWidth(0,125);       //设置列的宽度。
	BehaviorGrid.SetColumnWidth(1,160);


	//绞磨机
	MillGrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	MillGrid.ModifyStyle( 0, LVS_REPORT );

	MillGrid.InsertColumn(0,"检测时间");        //插入列
	MillGrid.InsertColumn(1,"绞磨机作业检测");   

	MillGrid.SetColumnWidth(0,125);       //设置列的宽度。
	MillGrid.SetColumnWidth(1,160);


	m_gridcom.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);
	m_gridcom.ModifyStyle( 0, LVS_REPORT );

	m_gridcom.InsertColumn(0,"检测时间");        //插入列
	m_gridcom.InsertColumn(1,"抱杆倾角"); 
	m_gridcom.SetColumnWidth(0,125);       //设置列的宽度。
	m_gridcom.SetColumnWidth(1,170);


	//创建日志
	CFileFind fileExist;

	CString m_date = GetDate();
	CString SaveDataPath = "D:\\施工现场监控信息\\作业情况"
		+m_date+".txt";

	BOOL IsFind = fileExist.FindFile(SaveDataPath);

	if (!IsFind)
	{
		CFile file(SaveDataPath,CFile::modeCreate);
		file.Close();
	}

	//SDK初始化
	H264_DVR_Init(NULL,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



void WorkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	CDialogEx::OnPaint();	
}


//开始选择，重新选择
void WorkDlg::OnSelectMoniterArea()
{
	// TODO: 在此添加控件通知处理程序代码

}





//取消选择
void WorkDlg::OnCancelSelect()
{
	// TODO: 在此添加控件通知处理程序代码

}




void WorkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(1);
	H264_DVR_Cleanup();
}




void WorkDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	if(nType==SIZE_RESTORED||nType==SIZE_MAXIMIZED) 
	{ 
		float fsp[2]; 
		POINT Newp; //获取现在对话框的大小 
		CRect recta;     
		GetClientRect(&recta);     //取客户区大小   
		Newp.x=recta.right-recta.left; 
		Newp.y=recta.bottom-recta.top; 
		fsp[0]=(float)Newp.x/Old.x; 
		fsp[1]=(float)Newp.y/Old.y; 
		CRect Rect; 
		int woc; 
		CPoint OldTLPoint,TLPoint; //左上角 
		CPoint OldBRPoint,BRPoint; //右下角 
		HWND  hwndChild=::GetWindow(m_hWnd,GW_CHILD);  //列出所有控件   
		while(hwndChild)     
		{     
			woc=::GetDlgCtrlID(hwndChild);//取得ID 
			GetDlgItem(woc)->GetWindowRect(Rect);   
			ScreenToClient(Rect);   
			OldTLPoint = Rect.TopLeft();   
			TLPoint.x = long(OldTLPoint.x*fsp[0]);   
			TLPoint.y = long(OldTLPoint.y*fsp[1]);   
			OldBRPoint = Rect.BottomRight();   
			BRPoint.x = long(OldBRPoint.x *fsp[0]);   
			BRPoint.y = long(OldBRPoint.y *fsp[1]);  

			//对组合框控件进行特殊处理
			HWND hwnd1 = m_ComSel.GetSafeHwnd(); // 取得控件的句柄
			if (hwndChild==hwnd1)
			{
				BRPoint.y = BRPoint.y+100;
			}
			//处理组合框控件完毕

			Rect.SetRect(TLPoint,BRPoint);   
			GetDlgItem(woc)->MoveWindow(Rect,TRUE); 
			hwndChild=::GetWindow(hwndChild, GW_HWNDNEXT);     
		} 
		Old=Newp; 
		Invalidate();
	} 

}




BOOL WorkDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == 0x1b)
	{
		return TRUE;
	}


	if (pMsg->message == WM_KEYDOWN   &&   pMsg->wParam == 0x0d)
	{
		return TRUE;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}





void WorkDlg::OnContextMenu(CWnd* pWnd, CPoint point)
{
	// TODO: 在此处添加消息处理程序代码
	CMenu menu;
	menu.LoadMenu(IDR_RIGHT_MENU); //读取资源
	menu.GetSubMenu(0)->TrackPopupMenu(TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON, 
		point.x, point.y, this);
}




void WorkDlg::OnEmptyGrid()
{
	// TODO: 在此添加命令处理程序代码
	HighGrid.DeleteAllItems();
	m_gridcom.DeleteAllItems();
	BehaviorGrid.DeleteAllItems();
	MillGrid.DeleteAllItems();
	
	UpdateData(FALSE);
}





void WorkDlg::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate(TRUE);

}




//传感器
BEGIN_EVENTSINK_MAP(WorkDlg, CDialogEx)
	ON_EVENT(WorkDlg, IDC_MSCOMM1, 1, WorkDlg::OnCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()





void WorkDlg::OnCommMscomm()
{
	// TODO: 在此处添加消息处理程序代码
	VARIANT m_input1; //读取缓冲区内的数据
	COleSafeArray m_input2;

	long length,i;
	BYTE data[1024];

	switch (m_Comm.get_CommEvent())
	{	
	case 2://接受数据
		{
			m_input1 = m_Comm.get_Input();
			m_input2 = m_input1;

			//将VARIANT型变量转换为ColeSafeArray型变量
			length=m_input2.GetOneDimSize();//确定数据长度

			for(i=0;i<length;i++)
				m_input2.GetElement(&i,data+i);
			//将数据转换为BYTE型数组

			for(i=0;i<length;i++)//将数组转换为Cstring型变量
			{
				BYTE a=* (int *)(data+i);

				m_ReceiveByte[i] = a;
			}


			//检验指令的正确性
			BOOL IsWright = TRUE;
			IsWright = IsWrightCode(m_ReceiveByte);


			//如果指令正确
			if (IsWright)
			{
				double x = DecodeX(m_ReceiveByte);
				double y = DecodeY(m_ReceiveByte);

				double m_Angle =180*AngCosReturn(x,y)/3.1415927;

				CString m_time = GetTime();  //时间
				CString m_CurrentAngle;  //倾角
				m_CurrentAngle.Format("倾角为%f度",m_Angle);

				UpdateData(TRUE);

				if (m_Angle>15) 
				{
					CString m_IsOver = "倾角大于15度";
					m_gridcom.InsertItem(0,"");
					m_gridcom.SetItemText(0,0,m_time);
					m_gridcom.SetItemText(0,1,m_IsOver);


					//手机报警系统—120s内不重复发送
					static time_t LastTime = 0;
					static time_t PresentTime = 0;
					static time_t Span = 0;
					PresentTime = time(0);
					Span = PresentTime - LastTime;
					 if (Span>4)
					 {
						LastTime = PresentTime;
						WriteToCommandLine("7");
						if (NoPic.empty()==false)
						{
							cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\NoPic.jpg",
								&(IplImage)NoPic);
						}
	
					 }

				}
				else 
				{

					static int i = 0;
					i++;
					if (i==8)
					{
						m_gridcom.InsertItem(0,"");
						m_gridcom.SetItemText(0,0,m_time);
						m_gridcom.SetItemText(0,1,m_CurrentAngle);
						i = 0;
					}

				}
				UpdateData(FALSE);

			}
		}
		break;
	}

}





void WorkDlg::OnComSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	try
	{
		if (m_Comm.get_PortOpen())
		{
			m_Comm.put_PortOpen(FALSE); //关闭串口
		}
		m_Comm.put__CommPort(m_ComSel.GetCurSel()+1); //选择串口

		if (!m_Comm.get_PortOpen())
		{
			m_Comm.put_PortOpen(TRUE); //打开串口
		}
		m_Comm.put_Settings("9600,n,8,1"); //设置通信参数
		m_Comm.put_InputMode(1);//设置为二进制读取方式
		m_Comm.put_InputLen(0);//设置当前接收区的长度为零
		m_Comm.get_Input();//先预读缓冲区以清除残留数据
		m_Comm.put_RThreshold(14);//设置接受触发字节数
	}
	catch (...)
	{
		MessageBox("无效串口");
	}

}





void WorkDlg::OnDetectAngle()
{
	// TODO: 在此添加控件通知处理程序代码
	SetTimer(1,500,NULL);

}


void WorkDlg::OnAngleStop()
{
	// TODO: 在此添加控件通知处理程序代码
	KillTimer(1);

}


void WorkDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (nIDEvent==1)
	{
		if( m_Comm.get_PortOpen())
		{
			try
			{
				CByteArray data ;
				CString SendStr = "68 04 00 04 08";
				int len = Str2Hex(SendStr,data);
				m_Comm.put_InputLen(0);
				m_Comm.get_Input();//清空接收区
				m_Comm.put_Output(COleVariant(data));//发送16进制数数据
			}
			catch (...)
			{
				KillTimer(1);
				MessageBox("串口已拔除");
			}

		}
		else
		{
			KillTimer(1);
		}
	}

	CDialogEx::OnTimer(nIDEvent);

}



//菜单事件
void WorkDlg::OnRunInBack()
{
	// TODO: 在此添加命令处理程序代码
	NOTIFYICONDATA nid;
	nid.cbSize = (DWORD)sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDI_ICON1;
	nid.uFlags = NIF_ICON|NIF_MESSAGE|NIF_TIP ;

	nid.uCallbackMessage = WM_HIDE_RUN;
	nid.hIcon = LoadIcon(AfxGetInstanceHandle(),
		MAKEINTRESOURCE(IDI_ICON1));

	strcpy_s(nid.szTip,"后台运行");

	Shell_NotifyIcon(NIM_ADD,&nid);

	ShowWindow(SW_HIDE);

}


LRESULT WorkDlg::OnHideRun( WPARAM wParam,LPARAM lParam )
{
	if (wParam!=IDI_ICON1)
	{
		return 1;
	}

	switch(lParam)
	{
	case WM_RBUTTONUP: //右键弹出快捷菜单
		{
			LPPOINT lpoint = new tagPOINT;
			::GetCursorPos(lpoint);
			CMenu menu;
			menu.CreatePopupMenu();//声明一个弹出式菜单
			menu.AppendMenu(MF_STRING,IDCANCEL,"退出");

			//确定弹出式菜单的位置
			menu.TrackPopupMenu(TPM_LEFTALIGN,lpoint->x,
				lpoint->y,this);

			//资源回收
			HMENU hmenu = menu.Detach();
			menu.DestroyMenu();
			delete lpoint;

			break;
		}
	case WM_LBUTTONDOWN:
		{
			this->ShowWindow(SW_SHOW);
			break;
		}
	default:
		break;

	}

	return 0;

}


void WorkDlg::OnExit()
{
	// TODO: 在此添加命令处理程序代码
	OnClose();
	OnOK();
}



void WorkDlg::OnStartPhoneSystem()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open",
		//"F:\\NTF8-8\\NTFClient.jar",
		"E:\\监控系统相关资料\\NTFClient\\NTFClient.exe",
		//F:\NTF8-8
		NULL,
		NULL,SW_SHOWNORMAL);
}



void WorkDlg::OnFaceRecognize()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open",
		"E:\\人脸打卡系统\\FaceRegSystem\\Debug\\FaceRegSystem.exe",
		NULL,
		NULL,SW_SHOWNORMAL);
}



//打开日志
void WorkDlg::OnOpenDoc()
{
	// TODO: 在此添加命令处理程序代码
	CString m_date = GetDate();
	CString SaveDataPath = "D:\\施工现场监控信息\\作业情况"+m_date+".txt";

	//调用记事本打开故障信息
	ShellExecute(NULL,"open",(LPCSTR)SaveDataPath,NULL,
		NULL,SW_SHOWNORMAL);

}


//管理日志
void WorkDlg::OnOrgDoc()
{
	// TODO: 在此添加命令处理程序代码
	CString SaveDataPath = "D:\\施工现场监控信息";

	//调用记事本打开故障信息
	ShellExecute(NULL,"open",(LPCSTR)SaveDataPath,NULL,
		NULL,SW_SHOWNORMAL);
}



void WorkDlg::OnLinkBehaviorVideo()
{
	// TODO: 在此添加控件通知处理程序代码



}


void WorkDlg::OnLinkMillVideo()
{
	// TODO: 在此添加控件通知处理程序代码




}

void WorkDlg::OnLinkHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码

	HighIP = "192.168.1.2";
	HighPort = 0;
	HighUser = "admin";
	HighPass = "admin";
	H264_DVR_SetConnectTime(3000, 1);//设置尝试连接1次，等待时间3s

	HighLoginID = H264_DVR_Login(HighIP, HighPort, 
			HighUser, HighPass, NULL, NULL,NULL);

	//连接视频流
	H264_DVR_CLIENTINFO playstru;

	playstru.nChannel = 0;
	playstru.nStream = 0;
	playstru.nMode = 0;
	playstru.hWnd = HighPic.m_hWnd;
	HighPlayhandle = H264_DVR_RealPlay(HighLoginID, &playstru);	

	StatusBar.SetText("请选择监控区域", 1, 0);

}


void WorkDlg::OnLinkMoreVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	


}



void WorkDlg::OnExitBehaviorVideo()
{
	// TODO: 在此添加控件通知处理程序代码



}


void WorkDlg::OnExitMillVideo()
{
	// TODO: 在此添加控件通知处理程序代码



}


void WorkDlg::OnExitMoreVideo()
{
	// TODO: 在此添加控件通知处理程序代码

}


void WorkDlg::OnExitHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码

	H264_DVR_StopRealPlay(HighPlayhandle,HighPic.m_hWnd); 

}


void WorkDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
}


void WorkDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	HighWorkDetecting = FALSE;
	BehaviorDetecting = FALSE;
	MillDetecting = FALSE;

	KillTimer(1);
	KillTimer(2);
	Sleep(200);
	CDialogEx::OnClose();
}



void WorkDlg::OnRecordHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	//H264_DVR_StartDVRRecord(HighPlayhandle, 0 ,SDK_RECORD_MANUAL);
	CString RecordPath = "D:\\Record\\"+GetDate()
		+"\\"+GetSaveTime();
	//开始本地录像
	H264_DVR_StartLocalRecord(HighPlayhandle,
		RecordPath.GetBuffer(0),0);

}


void WorkDlg::OnStopHighRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	//关闭本地录像
	H264_DVR_StopLocalRecord(HighPlayhandle);
	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnRecordBehavior()
{
	// TODO: 在此添加控件通知处理程序代码

}


void WorkDlg::OnStopBehavior()
{
	// TODO: 在此添加控件通知处理程序代码

	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnMillRecord()
{
	// TODO: 在此添加控件通知处理程序代码

}


void WorkDlg::OnStopMillRecord()
{
	// TODO: 在此添加控件通知处理程序代码

	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnMoreRecord()
{
	// TODO: 在此添加控件通知处理程序代码

}


void WorkDlg::OnStopMoreRecord()
{
	// TODO: 在此添加控件通知处理程序代码

	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnLookDiary()
{
	// TODO: 在此添加控件通知处理程序代码
	CString SaveDataPath = "D:\\施工现场监控信息";

	//调用记事本打开故障信息
	ShellExecute(NULL,"open",(LPCSTR)SaveDataPath,NULL,
		NULL,SW_SHOWNORMAL);

}
