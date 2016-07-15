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
	IsHighPlaying = FALSE;
	HighVideoPath =
		_T("http://admin:admin@192.168.1.2/ipcam/mpeg4.cgi");

	BehaviorVideoPath =
		_T("http://admin:admin@192.168.1.2/ipcam/mpeg4.cgi");

	MillVideoPath =
		_T("http://admin:admin@192.168.1.2/ipcam/mpeg4.cgi");

	MoreVideoPath =
		_T("http://admin:admin@192.168.1.2/ipcam/mpeg4.cgi");

	IsHighPlaying = FALSE;
	IsMillPlaying = FALSE;
	IsHighPlaying = FALSE;
	HighWorkDetecting = FALSE;
	BehaviorDetecting = FALSE;
	MillDetecting = FALSE;
	MillCollecting = FALSE;
	BehaviorCollecting = FALSE;
	HighCollecting = FALSE;
	MoreCollecting = FALSE;
	MillRecording = FALSE;
	BehaviorRecording = FALSE;
	HighRecording = FALSE;
	MoreRecording = FALSE;

	CString Date= "D:\\Record\\"+GetDate();
	CreateDirectory(Date,NULL);

}


WorkDlg::~WorkDlg()
{

}


void WorkDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PICTURE, HighPic);
	DDX_Control(pDX, IDC_LIST1_HIGH, m_highgrid);
	DDX_Text(pDX, IDC_EDIT1, HighVideoPath);
	DDX_Control(pDX, IDC_LIST1, m_gridcom);
	DDX_Control(pDX, IDC_MSCOMM1, m_Comm);
	DDX_Control(pDX, IDC_COMBO1, m_ComSel);
	DDX_Text(pDX, IDC_EDIT2, BehaviorVideoPath);
	DDX_Text(pDX, IDC_EDIT4, MillVideoPath);
	DDX_Text(pDX, IDC_EDIT5, MoreVideoPath);
	DDX_Control(pDX, IDC_LIST1_HIGH2, m_BehaviorGrid);
	DDX_Control(pDX, IDC_LIST1_HIGH3, m_MillGrid);
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
	ON_BN_CLICKED(IDC_BUTTON11, &WorkDlg::OnSelectHighVideo)
	ON_BN_CLICKED(IDC_BUTTON12, &WorkDlg::OnSelectHighVideo)
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
	ON_BN_CLICKED(IDC_BUTTON8, &WorkDlg::OnSelectBehaviorPath)
	ON_BN_CLICKED(IDC_BUTTON25, &WorkDlg::OnSelectMillPath)
	ON_BN_CLICKED(IDC_BUTTON28, &WorkDlg::OnSelectMorePath)
	ON_BN_CLICKED(IDC_BUTTON21, &WorkDlg::OnLinkBehaviorVideo)
	ON_BN_CLICKED(IDC_BUTTON27, &WorkDlg::OnLinkMillVideo)
	ON_BN_CLICKED(IDC_BUTTON30, &WorkDlg::OnLinkMoreVideo)
	ON_BN_CLICKED(IDC_BUTTON9, &WorkDlg::OnExitBehaviorVideo)
	ON_BN_CLICKED(IDC_BUTTON26, &WorkDlg::OnExitMillVideo)
	ON_BN_CLICKED(IDC_BUTTON29, &WorkDlg::OnExitMoreVideo)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON39, &WorkDlg::OnPausePlay)
	ON_BN_CLICKED(IDC_BUTTON40, &WorkDlg::OnResumePlay)
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
	m_choosing = FALSE;
	

	//状态栏
	m_StatusBar.Create(WS_CHILD|WS_VISIBLE|SBT_OWNERDRAW, 
		CRect(0,0,0,0), this, 0);

	int strPartDim[5]= {100,300,500,700,-1}; //分割数量,使用坐标
	m_StatusBar.SetParts(5, strPartDim);

	//设置状态栏文本
	m_StatusBar.SetText("系统状态", 0, 0);
	m_StatusBar.SetText("就绪", 1, 0);
	m_StatusBar.SetText("就绪", 2, 0);
	m_StatusBar.SetText("就绪", 3, 0);
	m_StatusBar.SetText("就绪", 4, 0);

	m_StatusBar.ShowWindow(SW_SHOW);


	//onsize大小随着对话框大小变化而变化
	CRect rectSize;     
	GetClientRect(&rectSize);     //取客户区大小   
	Old.x=rectSize.right-rectSize.left; 
	Old.y=rectSize.bottom-rectSize.top; 


	InitializeCriticalSection(&DetectionCritical);

	//设置list显示
	m_highgrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	m_highgrid.ModifyStyle( 0, LVS_REPORT );

	m_highgrid.InsertColumn(0,"检测时间");        //插入列
	m_highgrid.InsertColumn(1,"高空作业情况");   
	   
	m_highgrid.SetColumnWidth(0,125);       //设置列的宽度。
	m_highgrid.SetColumnWidth(1,160);


	//行为
	m_BehaviorGrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	m_BehaviorGrid.ModifyStyle( 0, LVS_REPORT );

	m_BehaviorGrid.InsertColumn(0,"检测时间");        //插入列
	m_BehaviorGrid.InsertColumn(1,"施工现场行为检测");   

	m_BehaviorGrid.SetColumnWidth(0,125);       //设置列的宽度。
	m_BehaviorGrid.SetColumnWidth(1,160);


	//绞磨机
	m_MillGrid.SetExtendedStyle(LVS_EX_FLATSB
		|LVS_EX_FULLROWSELECT
		|LVS_EX_HEADERDRAGDROP
		|LVS_EX_ONECLICKACTIVATE
		|LVS_EX_GRIDLINES);

	m_MillGrid.ModifyStyle( 0, LVS_REPORT );

	m_MillGrid.InsertColumn(0,"检测时间");        //插入列
	m_MillGrid.InsertColumn(1,"绞磨机作业检测");   

	m_MillGrid.SetColumnWidth(0,125);       //设置列的宽度。
	m_MillGrid.SetColumnWidth(1,160);


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

	SetTimer(2,60000,NULL);

	//SetTimer(3,100,NULL);程序启动后自动连接摄像头
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void WorkDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码

	CDialogEx::OnPaint();	
}

//选择点
void WorkDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_choosing)
	{
		if (m_Dot.size( )!=4)
		{
			m_Dot.push_back(point);
		}
	}
	
	CDialogEx::OnLButtonDown(nFlags, point);
}

//开始选择，重新选择
void WorkDlg::OnSelectMoniterArea()
{
	// TODO: 在此添加控件通知处理程序代码
	m_choosing = TRUE;
	m_Dot.clear();
	PointArray.clear();
	DrawToScreen(HighPic,HighImg);
}

//取消选择
void WorkDlg::OnCancelSelect()
{
	// TODO: 在此添加控件通知处理程序代码
	m_Dot.clear();
	m_choosing = FALSE;
	PointArray.clear();
	//绘制选择区域到控件上
}

//确认选择
void WorkDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_Dot.size()==4)
	{
		//在picture控件中绘图
		int width;
		int height;
		CRect tempRect;
		HighPic.GetWindowRect(&tempRect);
		ScreenToClient(tempRect);
		PointArray.clear();
		for(int i=0;i<4;i++)
		{
			POINT Adder;
			Adder.x = (tempRect.TopLeft()).x;
			Adder.y = (tempRect.TopLeft()).y;
			POINT m_ExactDot;
			//图片控件上的坐标
			m_ExactDot.x = m_Dot[i].x - Adder.x;
			m_ExactDot.y = m_Dot[i].y - Adder.y;

			if (HighImg.empty()==FALSE)
			{
				width = HighImg.cols;
				height = HighImg.rows;
			}
			else
			{
				return;
			}
	     
			POINT m_RealDot;
			m_RealDot.x = m_ExactDot.x*width/tempRect.Width();
			m_RealDot.y = m_ExactDot.y*height/tempRect.Height();
			//原图片上的真实点位
			PointArray.push_back(cvPoint(m_RealDot.x,m_RealDot.y));
		
		}
		//OnLButtonUp要修改
		if (m_choosing==TRUE)
		{
			Mat imgCopy = HighImg.clone();
			DrawSelectedArea(&(IplImage)imgCopy,PointArray);
			//绘制选择区域到控件上
			DrawToScreen(HighPic,imgCopy);
			m_choosing = FALSE;
		}
	
	}
	CDialogEx::OnLButtonUp(nFlags, point);

}


void WorkDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	// TODO: 在此处添加消息处理程序代码
	KillTimer(1);

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
	m_highgrid.DeleteAllItems();
	m_gridcom.DeleteAllItems();
	m_BehaviorGrid.DeleteAllItems();
	m_MillGrid.DeleteAllItems();
	
	UpdateData(FALSE);
}

void WorkDlg::OnRefresh()
{
	// TODO: 在此添加命令处理程序代码
	Invalidate(TRUE);

}

void WorkDlg::OnSelectHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		NULL,AfxGetMainWnd()); //open

	if (dlg.DoModal()==IDOK)
	{
		HighVideoPath = dlg.GetPathName();

		UpdateData(FALSE);
	}

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
					static long LastTime = 0;
					static long PresentTime = 0;
					static long Span = 0;
					PresentTime = time(0);
					Span = PresentTime - LastTime;
					 if (Span>120)
					 {
						LastTime = PresentTime;
						WriteToCommandLine("7");
						Mat Gun = imread("NoPic");
						cvSaveImage("D:\\施工现场监控信息\\notificationPhoto\\Gun.jpg",
							&(IplImage)Gun);
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


	if (nIDEvent==2)
	{
		OnPausePlay();
		Sleep(200);
		OnResumePlay();
	}

	if (nIDEvent==3)
	{
		 KillTimer(3);
		 OnLinkHighVideo();
		 OnLinkBehaviorVideo();
		 OnLinkMillVideo();
		 OnLinkMoreVideo();
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
		"E:\\监控系统相关资料\\NTFClient\\NTFClient.exe",
		NULL,
		NULL,SW_SHOWNORMAL);
}



void WorkDlg::OnFaceRecognize()
{
	// TODO: 在此添加命令处理程序代码
	ShellExecute(NULL,"open",
		"E:\\人脸打卡系统\\Debug\\Moniter.exe",
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


void WorkDlg::OnSelectBehaviorPath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		NULL,AfxGetMainWnd()); //open

	if (dlg.DoModal()==IDOK)
	{
		BehaviorVideoPath = dlg.GetPathName();

		UpdateData(FALSE);
	}


}


void WorkDlg::OnSelectMillPath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		NULL,AfxGetMainWnd()); //open

	if (dlg.DoModal()==IDOK)
	{
		MillVideoPath = dlg.GetPathName();

		UpdateData(FALSE);
	}



}


void WorkDlg::OnSelectMorePath()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog dlg(TRUE,NULL,NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,
		NULL,AfxGetMainWnd()); //open

	if (dlg.DoModal()==IDOK)
	{
		MoreVideoPath = dlg.GetPathName();
		UpdateData(FALSE);
	}

}


void WorkDlg::OnLinkBehaviorVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	IsBehaviorPlaying = FALSE;
	BehaviorCollecting = FALSE;
	Sleep(100);

	CreateThread(NULL,0,BehaviorPlayerProc,NULL,0,NULL);
	
	if (BehaviorDetecting==FALSE)
	{
		CreateThread(NULL,0,BeheviorDetectProc,NULL,0,NULL);
	}

	CreateThread(NULL,0,BehaviorDataGetProc,
		NULL,0,NULL);

}


void WorkDlg::OnLinkMillVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	IsMillPlaying = FALSE;
	MillCollecting = FALSE;
	Sleep(100);

	
	CreateThread(NULL,0,MillPlayerProc,NULL,0,NULL);
	
	if (MillDetecting==FALSE)
	{
		CreateThread(NULL,0,MillDetectProc,NULL,0,NULL);
	}

	CreateThread(NULL,0,MillDataGetProc,NULL,0,NULL);

}

void WorkDlg::OnLinkHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
    IsHighPlaying = FALSE;
	HighCollecting = FALSE;
	Sleep(100);

	CreateThread(NULL,0,HighPlayerProc,NULL,0,NULL);
	
	if (HighWorkDetecting==FALSE)
	{
		CreateThread(NULL,0,HighDetectProc,NULL,0,NULL);
	}

	CreateThread(NULL,0,HighDataGetProc,
		this,0,NULL);

}


void WorkDlg::OnLinkMoreVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(TRUE);
	IsMorePlaying = FALSE;
	MoreCollecting = FALSE;
	Sleep(100);
	CreateThread(NULL,0,MorePlayerProc,NULL,0,NULL);
	CreateThread(NULL,0,MoreDataGetProc,NULL,0,NULL);

}



void WorkDlg::OnExitBehaviorVideo()
{
	// TODO: 在此添加控件通知处理程序代码

	IsBehaviorPlaying = FALSE;
	BehaviorDetecting = FALSE;
	BehaviorCollecting = FALSE;
	BehaviorRecording = FALSE;

}


void WorkDlg::OnExitMillVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	IsMillPlaying = FALSE;
	MillDetecting = FALSE;
	MillCollecting = FALSE;
	MillRecording = FALSE;
}


void WorkDlg::OnExitMoreVideo()
{
	// TODO: 在此添加控件通知处理程序代码
    IsMorePlaying = FALSE;
	MoreCollecting = FALSE;
	MoreRecording = FALSE;
}


void WorkDlg::OnExitHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码

	IsHighPlaying = FALSE;
	HighWorkDetecting = FALSE;
	HighCollecting = FALSE;
	HighRecording = FALSE;

}


void WorkDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类
	CDialogEx::OnOK();
}


void WorkDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	IsHighPlaying = FALSE;
	IsMillPlaying = FALSE;
	IsMorePlaying = FALSE;
	IsBehaviorPlaying = FALSE;
	MillCollecting = FALSE;
	BehaviorCollecting = FALSE;
	HighCollecting = FALSE;
	MoreCollecting = FALSE;
	HighWorkDetecting = FALSE;
	BehaviorDetecting = FALSE;
	MillDetecting = FALSE;
	MillRecording = FALSE;
	BehaviorRecording = FALSE;
	HighRecording = FALSE;
	MoreRecording = FALSE;
	KillTimer(1);
	KillTimer(2);
	Sleep(200);
	CDialogEx::OnClose();
}




void WorkDlg::OnPausePlay()
{
	// TODO: 在此添加控件通知处理程序代码
	IsHighPlaying = FALSE;
	IsMillPlaying = FALSE;
	IsMorePlaying = FALSE;
	IsBehaviorPlaying = FALSE;


}


void WorkDlg::OnResumePlay()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL,0,MorePlayerProc,NULL,0,NULL);
	CreateThread(NULL,0,MillPlayerProc,NULL,0,NULL);
	CreateThread(NULL,0,HighPlayerProc,NULL,0,NULL);
	CreateThread(NULL,0,BehaviorPlayerProc,NULL,0,NULL);

}


void WorkDlg::OnRecordHighVideo()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL,0,HighRecordProc,NULL,0,NULL);
}


void WorkDlg::OnStopHighRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	HighRecording = FALSE;
	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnRecordBehavior()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL,0,BehaviorRecordProc,NULL,0,NULL);
}


void WorkDlg::OnStopBehavior()
{
	// TODO: 在此添加控件通知处理程序代码
	BehaviorRecording = FALSE;
	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnMillRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL,0,MillRecordProc,NULL,0,NULL);
}


void WorkDlg::OnStopMillRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	MillRecording = FALSE;
	AfxMessageBox("录制结束,请到D:\\Record目录下查看");
}


void WorkDlg::OnMoreRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	CreateThread(NULL,0,MoreRecordProc,NULL,0,NULL);
}


void WorkDlg::OnStopMoreRecord()
{
	// TODO: 在此添加控件通知处理程序代码
	MoreRecording = FALSE;
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
