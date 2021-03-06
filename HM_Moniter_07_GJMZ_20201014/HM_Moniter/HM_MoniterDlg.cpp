
// HM_MoniterDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "HM_MoniterDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CSPortComm g_CSPortComm_Main;
HANDLE g_HANDLE_CommResponce;
BYTE g_BYTE_CommFrameReceived = 0;
WORD g_BYTE_CommResponceStatus = 0;
BYTE g_BYTE_NewMotorDataRxArray[256];
BYTE g_BYTE_NewDriverDataRxArray[256];
BYTE g_BYTE_NewPIDDataRxArray[256];
BYTE g_BYTE_NewSaveDataRxArray[256];
CDriverParam g_CDriverParam_Main;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP();
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


// CHMMoniterDlg 对话框



CHMMoniterDlg::CHMMoniterDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HM_MONITER_DIALOG, pParent)
	, m_CString_getDeviceInfo(_T(""))
	, m_CString_getDeviceType(_T(""))
	, m_UINT8_getStatusDevice(0)
	, m_UINT8_getStatusMotor(0)
	, m_INT32_getStatusVelocity(0)
	, m_INT32_getStatusPosition(0)
	, m_UINT8_setWaveChannel(0)
	, m_BYTE_isMotorEnabled(0)
//	, m_INT_setSpeed(0)
	//	, m_INT_setMaxSpeed(0)
	, m_UINT8_setWorkMode(0)
	, m_UINT8_setPositionType(0)
	, m_INT32_setPosition(0)
	, CommandWord(0)
	, m_CString_setSpeed(_T(""))
{
}

void CHMMoniterDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//运动控制
	//	DDX_CBIndex(pDX, IDC_COMBO_setWorkMode, m_INT_setWorkMode);
	//	DDX_CBIndex(pDX, IDC_COMBO_setPositionType, m_INT_setPositionType);
	//	DDX_Text(pDX, IDC_EDIT_setSpeed, m_CString_setSpeed);
	//	DDX_Text(pDX, IDC_EDIT_setPosition, m_INT_setPosition);
	//	DDX_Text(pDX, IDC_EDIT_setCurrent, m_unParam_setCurrent.param_float);

	//报文显示

	//状态显示

	//DDX_Text(pDX, IDC_EDIT_DeviceInfo, m_CString_getDeviceInfo);
	//DDX_Text(pDX, IDC_EDIT_DeviceType, m_CString_getDeviceType);


	//	DDX_CBIndex(pDX, IDC_COMBO_getStatusDevice, m_INT_getStatusDevice);
	//	DDX_CBIndex(pDX, IDC_COMBO_getStatusMotor, m_INT_getStatusMotor);
	//	DDX_Text(pDX, IDC_EDIT_getStatusBusVoltage, m_unParam_getStatusBusVoltage.param_float);
	//	DDX_Text(pDX, IDC_EDIT_getStatusPhaseCurrent, m_unParam_getStatusPhaseCurrent.param_float);
	//	DDX_Text(pDX, IDC_EDIT_getStatusVelocity, m_INT_getStatusVelocity);
	//	DDX_Text(pDX, IDC_EDIT_getStatusPosition, m_INT_getStatusPosition);

	//波形显示
	DDX_Control(pDX, IDC_TCHART_Main, m_CTChart_Main);
	DDX_Control(pDX, IDC_COMBO_setWaveChannel, m_CComboBox_setWaveChannel);
	//	DDX_CBIndex(pDX, IDC_COMBO_setWaveChannel, m_INT_setWaveChannel);
	DDX_Control(pDX, IDC_EDIT_setSpeed, m_CEdit_setSpeed);
	//	DDX_Text(pDX, IDC_EDIT_setSpeed, m_CString_setSpeed);
	DDX_Control(pDX, IDC_COMBO_setWorkMode, m_CComboBox_setWorkMode);
	DDX_Control(pDX, IDC_EDIT_setCurrent, m_CEdit_setCurrent);
	DDX_Control(pDX, IDC_COMBO_setPositionType, m_CComboBox_setPositionType);
	DDX_Control(pDX, IDC_EDIT_setPosition, m_CEdit_setPosition);
	//	DDX_Control(pDX, IDC_STATIC_setSpeedUnit, m_CStatic_setSpeedUnit);
	DDX_Control(pDX, IDC_STATIC_getStatusVelocityUnit, m_CStatic_getStatusVelocityUnit);
	DDX_Control(pDX, IDC_CHECK_getStatus, m_CButton_getStatus);
	DDX_Control(pDX, IDC_EDIT_getStatusJointPosition, m_CEdit_getStatusJointPosition);
	DDX_Control(pDX, IDC_EDIT_getStatusJointDegree, m_CEdit_getStatusJointDegree);
	DDX_Control(pDX, IDC_COMBO_CommNumber, m_CComboBox_UsableSerialPort);
	DDX_Control(pDX, IDC_BUTTON_SeachUsableComm, m_CButton_SeachUsableComm);
	DDX_Control(pDX, IDC_BUTTON_OpenComm, m_CButton_OpenComm);
	DDX_Control(pDX, IDC_EDIT_setRunAcceleration, m_CEdit_setRunAcceleration);
	DDX_Control(pDX, IDC_EDIT_setAngle, m_CEdit_setAngle);
	DDX_Control(pDX, IDC_EDIT_setCANAddress, m_CEdit_setCanAddress);
	DDX_Control(pDX, IDC_COMBO_setCANBaudrate, m_CComboBox_setCanBaudrate);
}

BEGIN_MESSAGE_MAP(CHMMoniterDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_TIMER()

	ON_BN_CLICKED(IDC_BUTTON_ClearWaveData, &CHMMoniterDlg::OnBnClickedButtonClearwavedata)
	ON_COMMAND(ID_MENU_Comm_Connect, &CHMMoniterDlg::OnMenuCommConnect)
	ON_COMMAND(ID_MENU_File_New, &CHMMoniterDlg::OnMenuFileNew)
	ON_CBN_SELCHANGE(IDC_COMBO_setWorkMode, &CHMMoniterDlg::OnCbnSelchangeCombosetworkmode)
	ON_BN_CLICKED(IDC_CHECK_getStatus, &CHMMoniterDlg::OnBnClickedCheckgetstatus)
	ON_BN_CLICKED(IDC_BUTTON_setMotorEnable, &CHMMoniterDlg::OnBnClickedButtonsetmotorenable)
	ON_BN_CLICKED(IDC_BUTTON_setStart, &CHMMoniterDlg::OnBnClickedButtonsetstart)
	ON_BN_CLICKED(IDC_BUTTON_setStopNormal, &CHMMoniterDlg::OnBnClickedButtonsetstopnormal)
	ON_BN_CLICKED(IDC_BUTTON_setStopEmergency, &CHMMoniterDlg::OnBnClickedButtonsetstopemergency)

	ON_COMMAND(ID_MENU_Comm_Disconnect, &CHMMoniterDlg::OnMenuCommDisconnect)
	ON_MESSAGE(WM_USER_MONITER_RX, &CHMMoniterDlg::OnUserMoniterRx)
	ON_CBN_SELCHANGE(IDC_COMBO_setWaveChannel, &CHMMoniterDlg::OnCbnSelchangeCombosetwavechannel)
	ON_COMMAND(ID_MENU_File_Open, &CHMMoniterDlg::OnMenuFileOpen)
	ON_COMMAND(ID_MENU_Motor, &CHMMoniterDlg::OnMenuMotor)
	ON_COMMAND(ID_MENU_Driver, &CHMMoniterDlg::OnMenuDriver)
	ON_COMMAND(ID_MENU_Loop, &CHMMoniterDlg::OnMenuLoop)
//	ON_BN_CLICKED(IDC_CHECK_toggleVelocityUnit, &CHMMoniterDlg::OnBnClickedChecktogglevelocityunit)
	ON_BN_CLICKED(IDC_BUTTON_setMotorReverse, &CHMMoniterDlg::OnBnClickedButtonsetmotorreverse)
	ON_MESSAGE(WM_USER_MOTOR_MATCH, &CHMMoniterDlg::OnUserMotorMatch)
	ON_COMMAND(ID_MENU_File_DownLoad, &CHMMoniterDlg::OnMenuFileDownload)
	ON_COMMAND(ID_MENU_File_Restore, &CHMMoniterDlg::OnMenuFileRestore)
	ON_BN_CLICKED(IDC_CHECK_toggleUnit, &CHMMoniterDlg::OnBnClickedChecktoggleunit)
	ON_BN_CLICKED(IDC_BUTTON_ClearErrorStatus, &CHMMoniterDlg::OnBnClickedButtonClearerrorstatus)
	ON_MESSAGE(WM_USER_MOTOR_NOMATCH, &CHMMoniterDlg::OnUserMotorNomatch)
	ON_MESSAGE(WM_USER_CURRENTLOOP_TEST, &CHMMoniterDlg::OnUserCurrentloopTest)
	ON_MESSAGE(WM_USER_CURRENTLOOP_NOTEST, &CHMMoniterDlg::OnUserCurrentloopNotest)
	ON_MESSAGE(WM_USER_VELOCITYLOOP_TEST, &CHMMoniterDlg::OnUserVelocityloopTest)
	ON_MESSAGE(WM_USER_VELOCITYLOOP_NOTEST, &CHMMoniterDlg::OnUserVelocityloopNotest)
	ON_COMMAND(ID_MENU_File_Save, &CHMMoniterDlg::OnMenuFileSave)
	ON_BN_CLICKED(IDC_BUTTON_calibratePosition, &CHMMoniterDlg::OnBnClickedButtoncalibrateposition)
	ON_MESSAGE(WM_USER_POSITIONLOOP_TEST, &CHMMoniterDlg::OnUserPositionloopTest)
	ON_BN_CLICKED(IDC_BUTTON_TestWaveDisplay, &CHMMoniterDlg::OnBnClickedButtonTestwavedisplay)
	ON_COMMAND(ID_MENU_COMM_CAN, &CHMMoniterDlg::OnMenuCommCan)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_BUTTON_SeachUsableComm, &CHMMoniterDlg::OnBnClickedButtonSeachusablecomm)
	ON_BN_CLICKED(IDC_BUTTON_OpenComm, &CHMMoniterDlg::OnBnClickedButtonOpencomm)
	ON_BN_CLICKED(IDC_BUTTON_setCanParam, &CHMMoniterDlg::OnBnClickedButtonsetcanparam)
	ON_BN_CLICKED(IDC_BUTTON_saveTestData, &CHMMoniterDlg::OnBnClickedButtonsavetestdata)
	ON_EN_KILLFOCUS(IDC_EDIT_setAngle, &CHMMoniterDlg::OnEnKillfocusEditsetangle)
	ON_COMMAND(32824, &CHMMoniterDlg::OnMenuUserDef)
END_MESSAGE_MAP()


// CHMMoniterDlg 消息处理程序

BOOL CHMMoniterDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON_HM);

	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	//m_ComboBox_Bautrate.ResetContent();

	m_Menu_Main.LoadMenu(IDR_MENU_Main);
	SetMenu(&m_Menu_Main);

	UINT32 indicators[] = { IDS_DEVICE_TYPE,IDS_DEVICE_INFO,IDS_DEVICE_STATUS,IDS_MOTOR_STATUS,IDS_ERROR_CODE,IDS_COMPANY_INFO };
	m_StatusBar_Main.Create(this);
	m_StatusBar_Main.SetIndicators(indicators, 6);
	GetClientRect(&m_CRect_MainClient);
	//伸缩窗格以填满空间，第一栏显示IDS_STRING_MESSAGE内容，即“状态栏” ，0表示默认长度 
	m_StatusBar_Main.SetPaneInfo(0, IDS_DEVICE_TYPE, SBPS_POPOUT, 80);
	//窗格突出显示，第二栏长度为320单位，显示IDS_STRING_VERSION字符串  
	m_StatusBar_Main.SetPaneInfo(1, IDS_DEVICE_INFO, SBPS_POPOUT, 160);
	m_StatusBar_Main.SetPaneInfo(2, IDS_DEVICE_STATUS, SBPS_POPOUT, 100);
	m_StatusBar_Main.SetPaneInfo(3, IDS_MOTOR_STATUS, SBPS_POPOUT, 100);
	m_StatusBar_Main.SetPaneInfo(4, IDS_ERROR_CODE, SBPS_POPOUT, 100);
	m_StatusBar_Main.SetPaneInfo(5, IDS_COMPANY_INFO, SBPS_STRETCH, 100);

	m_StatusBar_Main.MoveWindow(0, m_CRect_MainClient.bottom - 20, m_CRect_MainClient.right, 20);

	//RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, IDS_DEVICE_TYPE);

//	m_StatusBar_Main.SetPaneInfo(0,)

//	m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Connect, MF_BYCOMMAND | MF_ENABLED);
	////	m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Connect, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

//	m_CButton_OpenComm.EnableWindow(FALSE);


	m_CEdit_setCurrent.SetLimitText(16);
	m_CEdit_setSpeed.SetLimitText(16);
	m_CEdit_setPosition.SetLimitText(16);
	//控件禁用
	m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Disconnect, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

	m_CEdit_setCurrent.EnableWindow(FALSE);
	m_CEdit_setSpeed.EnableWindow(FALSE);
	m_CEdit_setPosition.EnableWindow(FALSE);
	m_CComboBox_setPositionType.EnableWindow(FALSE);
	m_CEdit_setRunAcceleration.EnableWindow(FALSE);
//	CRect rc;
//	GetClientRect(rc);
//	m_CTChart_2.Create(NULL, WS_CHILD | WS_VISIBLE, rc, this, 1000);
//	m_CTChart_Chart1.AddSeries(0);
//	m_CTChart_Chart1.AddSeries(0);
	//m_CSeries_Data1 = (CSeries)m_CTChart_Chart1.Series(0);
	//m_CSeries_Data2 = (CSeries)m_CTChart_Chart1.Series(1);
	//m_CAxes_Axes1 = m_CTChart_Chart1.get_Axis();
	//m_CAxis_Axis1 = m_CAxes_Axes1.get_Bottom();
	//m_CAxisTitle_AxisTitle1 = m_CAxis_Axis1.get_Title();
	//m_CAxisTitle_AxisTitle1.put_Caption(_T("时间"));
	m_CComboBox_setWaveChannel.SetCurSel(0);  //当前选项
	((CEnvironment)m_CTChart_Main.get_Environment()).put_MouseWheelScroll(FALSE);
	m_CSeries_Data1 = (CSeries)m_CTChart_Main.Series(0);
//	m_CSeries_Data2 = (CSeries)m_CTChart_Main.Series(1);
	//必须要引入CSeries类  
	//m_CSeries_Data1.AddXY(10,20,NULL,RGB(255,255,255));//添加一些数据   
	//m_CSeries_Data1.Add(11,NULL,RGB(255,0,0));
	//m_CSeries_Data1.Add(12,NULL,RGB(255,0,0));
	//m_CSeries_Data1.Add(13,NULL,RGB(255,0,0));
	//m_CSeries_Data2.Add(8, NULL, RGB(0, 0, 255));
	//m_CSeries_Data2.Add(9, NULL, RGB(0, 0, 255));
	//m_CSeries_Data2.Add(10, NULL, RGB(0, 0, 255));
//	m_Series_Data1.AddXY(0, 15, NULL, RGB(255,0, 0));
	//初始化完成后 读取一次数据
	g_HANDLE_CommResponce = CreateEvent(NULL, FALSE, FALSE, NULL);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CHMMoniterDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		// DoModal 可以用来显示一个模态对话框
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

//如果向对话框添加最小化按钮，则需要下面的代码来绘制该图标
//对于使用文档/视图模型的 MFC 应用程序，这将由框架自动完成

void CHMMoniterDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CHMMoniterDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

//菜单 文件 新建
void CHMMoniterDlg::OnMenuFileNew()
{
	UINT8 result = 0;
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}

	if (m_CButton_getStatus.GetCheck())				
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}


FILEMOTOR:
	result = openDlgFileMotor(0);	//0 新建 1 打开
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
	else if (result == IDCANCEL)
		return;
	else
	{
	}

FILEDRIVER:

	result = openDlgFileDriver(0);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
	else if (result == IDCANCEL)
		return;
	else if (result == IDABORT)
		goto FILEMOTOR;
	else {}

FILELOOP:
	result = openDlgFileLoop(0);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
	else if (result == IDCANCEL)
		return;
	else if (result == IDABORT)
		goto FILEDRIVER;
	else {}

	m_CDialog_FileNewSave.m_BYTE_OpenMode = 0;		//新建 0
	result = m_CDialog_FileNewSave.DoModal();
	if (result == IDABORT)
		goto FILELOOP;
}
//菜单 文件 打开
void CHMMoniterDlg::OnMenuFileOpen()
{
	// TODO: 在此添加命令处理程序代码

	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}
	//m_CButton_getStatus
	if (m_CButton_getStatus.GetCheck())
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}
	
	m_CDialog_FileNewSave.m_BYTE_OpenMode = 1;		//打开 1
	m_CDialog_FileNewSave.DoModal();
}
//菜单 文件 保存
void CHMMoniterDlg::OnMenuFileSave()
{
	// TODO: 在此添加命令处理程序代码
	UINT8 result = 0;
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}

	result = getRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
	if (result)
	{
		processErrorCode(result);
	}
	result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
	if (result)
	{
		processErrorCode(result);
	}

	result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
	if (result)
	{
		processErrorCode(result);
	}
	result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
	if (result)
	{
		processErrorCode(result);
	}
	result = getRegValue(REG_CURRENTLOOP_P, REG_CURRENTLOOP_NUM);
	if (result)
	{
		processErrorCode(result);
	}
	result = getRegValue(REG_VELOCITYLOOP_P, REG_VELOCITYLOOP_NUM);
	if (result)
	{
		processErrorCode(result);
	}
	result = getRegValue(REG_POSITIONLOOP_P, REG_POSITIONLOOP_NUM);
	if (result)
	{
		processErrorCode(result);
	}

	m_CDialog_FileNewSave.m_BYTE_OpenMode = 2;		//打开 2
	m_CDialog_FileNewSave.DoModal();
}
//菜单 文件 下载
void CHMMoniterDlg::OnMenuFileDownload()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}

	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_CPARAM_SAVE);   //？
	if (setRegValue(REG_COMMAND, 0x02) == 1)       
	{ 
		AfxMessageBox(_T("设备无响应"));
		return;
	}
	Sleep(500);
	AfxMessageBox(_T("参数下载完成！"));
}
//菜单 文件 恢复
void CHMMoniterDlg::OnMenuFileRestore()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}

	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_DPARAM_SAVE);
	if (setRegValue(REG_COMMAND, 0x02) == 1)
	{
		AfxMessageBox(_T("设备无响应"));
		return;
	}

	Sleep(500);
	AfxMessageBox(_T("参数恢复完成！"));
}
//菜单 电机
void CHMMoniterDlg::OnMenuMotor()
{
	// TODO: 在此添加命令处理程序代码

	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	//0新建 1打开
	UINT8 result = openDlgFileMotor(1);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
}
//菜单 驱动器
void CHMMoniterDlg::OnMenuDriver()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	UINT8 result = openDlgFileDriver(1);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
/*


	if (m_BYTE_isMotorEnabled)
	{
		m_CDialog_FileNewDriver.m_BYTE_setMotorEnabled = 1;
	}
	else
	{
		m_CDialog_FileNewDriver.m_BYTE_setMotorEnabled = 0;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		m_CDialog_FileNewDriver.m_BYTE_setGetStatus = 1;
	}
	else
	{
		m_CDialog_FileNewDriver.m_BYTE_setGetStatus = 0;
	}
	   	  
	m_CDialog_FileNewDriver.m_BYTE_OpenMode = 1;
	m_CDialog_FileNewDriver.DoModal();
*/
}
//菜单 环路
void CHMMoniterDlg::OnMenuLoop()
{
	// TODO: 在此添加命令处理程序代码

	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	UINT8 result = openDlgFileLoop(1);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
/*
	if (m_BYTE_isMotorEnabled)
	{
		m_CDialog_FileNewPID.m_BYTE_setMotorEnabled = 1;
	}
	else
	{
		m_CDialog_FileNewPID.m_BYTE_setMotorEnabled = 0;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		m_CDialog_FileNewPID.m_BYTE_setGetStatus = 1;
	}
	else
	{
		m_CDialog_FileNewPID.m_BYTE_setGetStatus = 0;
	}

	m_CDialog_FileNewPID.m_BYTE_OpenMode = 1;
	m_CDialog_FileNewPID.DoModal();
*/
}
//菜单 通信 连接
void CHMMoniterDlg::OnMenuCommConnect()
{
	// TODO: 在此添加命令处理程序代码
	//m_CDialog_SerialComm.Create(IDD_SERIALPORT, this);
	//m_CDialog_SerialComm.ShowWindow(SW_SHOW);
	UINT8 result = openDlgSerialComm();

	if (result >= 0xF0)
	{
		g_CSPortComm_Main.CommOpened = FALSE;
		g_CSPortComm_Main.CloseComm();

		processErrorCode(result);
	}
	else if (result == IDOK)
	{
		m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Connect, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Disconnect, MF_BYCOMMAND | MF_ENABLED);
	}
	else;
}
//菜单 通信 断开
void CHMMoniterDlg::OnMenuCommDisconnect()
{
	// TODO: 在此添加命令处理程序代码
	g_CSPortComm_Main.CommOpened = FALSE;

	//if (m_CButton_getStatus.GetCheck())
	//{		
	//	KillTimer(TIMER_ID_STATUS);
	//	KillTimer(TIMER_ID_REAL);
	//	m_CButton_getStatus.SetCheck(FALSE);
	//}
	DWORD aa = WaitForSingleObject(CommProcessThread, 1000);


	if (aa != WAIT_OBJECT_0) 
	{
		AfxMessageBox(_T("关闭失败"));
	}
	else
	{
		if (g_CSPortComm_Main.CloseComm() == FALSE) 
		{
			AfxMessageBox(_T("关闭失败"));
		}

		else
		{
			m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Connect, MF_BYCOMMAND | MF_ENABLED);
			////	m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Connect, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
			m_Menu_Main.EnableMenuItem(ID_MENU_Comm_Disconnect, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);

			AfxMessageBox(_T("已关闭"));
		}
	}
}
//菜单 通信 CAN
void CHMMoniterDlg::OnMenuCommCan()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	UINT8 result = openDlgCommCan();
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
}
//菜单 用户自定义参数
void CHMMoniterDlg::OnMenuUserDef()
{
	// TODO: 在此添加命令处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;

	}
	UINT8 result = openDlgUserDef(1);
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
}

//按键 使能
void CHMMoniterDlg::OnBnClickedButtonsetmotorenable()
{
	// TODO: 在此添加控件通知处理程序代码
	UINT8 result = 0;
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (!m_BYTE_isMotorEnabled)
	{
		if (!g_CDriverParam_Main.m_UINT8_getMatchFlag)
		{
			AfxMessageBox(_T("电机未匹配"));
			return;
		}

		//工作模式
		result = setWorkMode();
		if (result)
		{
			processErrorCode(result);
			return;
		}

		CommandWord = COMMAND_MOTOR_ENABLE;
		result = setCommand(COMMAND_MOTOR_ENABLE);
		if (result)
			processErrorCode(result);
	}
	else
	{
		CommandWord = COMMAND_MOTOR_DISABLE;
		result = setCommand(COMMAND_MOTOR_DISABLE);
		if (result)
			processErrorCode(result);
	}
}
//按键 更改工作模式
void CHMMoniterDlg::OnCbnSelchangeCombosetworkmode()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	UINT8 result = 0;
	m_UINT8_setWorkMode = m_CComboBox_setWorkMode.GetCurSel();

	if (!m_BYTE_isMotorEnabled)
	{
		switch (m_UINT8_setWorkMode)
		{
		case 0:
			result = getRegValue(REG_RUN_CURRENT, 4);     //电流模式
			if (result)
			{
				processErrorCode(result);
				return;
			}

			m_CEdit_setCurrent.EnableWindow(TRUE);
			m_CEdit_setSpeed.EnableWindow(FALSE);
			m_CEdit_setPosition.EnableWindow(FALSE);
			m_CEdit_setRunAcceleration.EnableWindow(FALSE);
			m_CComboBox_setPositionType.EnableWindow(FALSE);
			m_CEdit_setAngle.EnableWindow(FALSE);

			break;

		case 1:
			result = getRegValue(REG_RUN_VELOCITY, 8);      //速度模式
			if (result)
			{
				processErrorCode(result);
				return;
			}

			m_CEdit_setCurrent.EnableWindow(FALSE);
			m_CEdit_setSpeed.EnableWindow(TRUE);
			m_CEdit_setPosition.EnableWindow(FALSE);
			m_CEdit_setRunAcceleration.EnableWindow(TRUE);
			m_CComboBox_setPositionType.EnableWindow(FALSE);
			m_CEdit_setAngle.EnableWindow(FALSE);

			break;

		case 2:
			result = getRegValue(REG_RUN_POSITION, 13);    //位置模式
			if (result)
			{
				processErrorCode(result);
				return;
			}
			m_CEdit_setCurrent.EnableWindow(FALSE);
			m_CEdit_setSpeed.EnableWindow(TRUE);
			m_CEdit_setPosition.EnableWindow(FALSE);
			m_CEdit_setRunAcceleration.EnableWindow(TRUE);
			m_CComboBox_setPositionType.EnableWindow(TRUE);
			m_CEdit_setAngle.EnableWindow(TRUE);


			break;

		default:
			AfxMessageBox(_T("工作模式无效"));
			break;
		}
	}
}
//按键 启动
void CHMMoniterDlg::OnBnClickedButtonsetstart()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	if (!m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机未使能"));
		return;
	}

	m_UINT8_setWorkMode = m_CComboBox_setWorkMode.GetCurSel();

	//设置各模式的参数
	UINT8 result = 0;
	switch (m_UINT8_setWorkMode)
	{
	case 0:
		result = setCurrentMode(1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	case 1:
		result = setSpeedMode(1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	case 2:
		result = setPositionMode(1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	default:
		AfxMessageBox(_T("工作模式无效"));
		break;
	}

	CommandWord = COMMAND_MOTOR_RUN;
	result = setCommand(COMMAND_MOTOR_RUN);
	if (result)
		processErrorCode(result);
}
//按键 正常停止
void CHMMoniterDlg::OnBnClickedButtonsetstopnormal()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	if (!m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机未使能"));
		return;
	}
	UINT8 result = 0;
	if (g_CSPortComm_Main.CommOpened)
	{
		CommandWord = COMMAND_MOTOR_STOP;
		result = setCommand(COMMAND_MOTOR_STOP);
		if (result)
			processErrorCode(result);
	}
}
//按键 紧急停止
void CHMMoniterDlg::OnBnClickedButtonsetstopemergency()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	if (!m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机未使能"));
		return;
	}
	UINT8 result = 0;
	if (g_CSPortComm_Main.CommOpened)
	{
		CommandWord = COMMAND_MOTOR_ESTOP;
		result = setCommand(COMMAND_MOTOR_ESTOP);
		if (result)
			processErrorCode(result);
	}
}
//按键 反向启动
void CHMMoniterDlg::OnBnClickedButtonsetmotorreverse()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	if (!m_BYTE_isMotorEnabled)
	{
		AfxMessageBox(_T("电机未使能"));
		return;
	}
	m_UINT8_setWorkMode = m_CComboBox_setWorkMode.GetCurSel();

	//设置各模式的参数
	UINT8 result = 0;
	switch (m_UINT8_setWorkMode)
	{
	case 0:
		result = setCurrentMode(-1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	case 1:
		result = setSpeedMode(-1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	case 2:
		result = setPositionMode(-1);
		if (result)
		{
			processErrorCode(result);
			return;
		}
		break;
	default:
		AfxMessageBox(_T("工作模式无效"));
		break;
	}

	CommandWord = COMMAND_MOTOR_RUN;
	result = setCommand(COMMAND_MOTOR_RUN);
	if (result)
		processErrorCode(result);
}
//按键 切换单位
void CHMMoniterDlg::OnBnClickedChecktoggleunit()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (!g_CSPortComm_Main.CommOpened)
	//{
	//	AfxMessageBox(_T("串口未打开"));
	//	return;
	//}
	if (m_CButton_toggleUnit.GetCheck())
	{
		m_CButton_toggleUnit.SetWindowTextW(_T("R/min"));
		m_CStatic_getStatusVelocityUnit.SetWindowTextW(_T("R/min"));

		m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
		m_INT32_setSpeed = _ttoi(m_CString_setSpeed);

		//切换成 R/m单位
		m_FLOAT_setSpeed = m_INT32_setSpeed * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
		if (m_FLOAT_setSpeed >= 0)
		{
			m_FLOAT_setSpeed = m_FLOAT_setSpeed + 0.5F;
		}
		else
			m_FLOAT_setSpeed = m_FLOAT_setSpeed - 0.5F;

		m_CString_setSpeed.Format(_T("%d"), (INT32)m_FLOAT_setSpeed);
		m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);
	}

	else
	{
		m_CButton_toggleUnit.SetWindowTextW(_T("P/s"));
		m_CStatic_getStatusVelocityUnit.SetWindowTextW(_T("P/s"));

		m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
		m_FLOAT_setSpeed = (FLOAT)_ttof(m_CString_setSpeed);

		//切换成 P/s单位
		m_FLOAT_setSpeed = m_FLOAT_setSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
		if (m_FLOAT_setSpeed >= 0)
		{
			m_FLOAT_setSpeed = m_FLOAT_setSpeed + 0.5F;
		}
		else
			m_FLOAT_setSpeed = m_FLOAT_setSpeed - 0.5F;

		m_INT32_setSpeed = (INT32)m_FLOAT_setSpeed;

		m_CString_setSpeed.Format(_T("%d"), m_INT32_setSpeed);
		m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);
	}
}


//按键 清除 错误
void CHMMoniterDlg::OnBnClickedButtonClearerrorstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		//m_CButton_getStatus.SetCheck(FALSE);
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	   	 
	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH, 0);

	UINT8 result = setRegValue(REG_DEVICE_ERRORCODE, REG_DEVICE_ERRORCODE_NUM);
	if (result)
	{
		processErrorCode(result);
		return;
	}
}
//按键 状态 更新
void CHMMoniterDlg::OnBnClickedCheckgetstatus()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		m_CButton_getStatus.SetCheck(FALSE);
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (m_CButton_getStatus.GetCheck())
	{
		//m_Menu_Main.EnableMenuItem(ID_MENU_Motor, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//m_Menu_Main.EnableMenuItem(ID_MENU_Driver, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		//m_Menu_Main.EnableMenuItem(ID_MENU_Loop, MF_BYCOMMAND | MF_DISABLED | MF_GRAYED);
		g_CDriverParam_Main.m_UINT8_getStatusFlag = 1;

		KillTimer(TIMER_ID_STATUS);
		SetTimer(TIMER_ID_REAL, 50, NULL);		//启动定时器

	}
	else
	{
		g_CDriverParam_Main.m_UINT8_getStatusFlag = 0;

		KillTimer(TIMER_ID_REAL);
		SetTimer(TIMER_ID_STATUS, 500, NULL);		//启动定时器
	}
}
//按键 波形通道选择
void CHMMoniterDlg::OnCbnSelchangeCombosetwavechannel()
{
	// TODO: 在此添加控件通知处理程序代码
	m_UINT8_setWaveChannel = m_CComboBox_setWaveChannel.GetCurSel();
}
//按键 清除波形
void CHMMoniterDlg::OnBnClickedButtonClearwavedata()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CSeries_Data1.Clear();
	//	m_CSeries_Data2.Clear();
}


//串口 数据处理 线程
DWORD CHMMoniterDlg::DataProcessThread(LPVOID pParam)
{
	CHMMoniterDlg *pObj = (CHMMoniterDlg *)pParam;
	while (g_CSPortComm_Main.CommOpened) 
	{
		//if (g_BYTE_CommFrameReceived = 1)
		//{
		//	g_BYTE_CommFrameReceived = 0;
		//g_BYTE_RunIndex = 0xF1;
		for (WORD tempIndex = 0; tempIndex < g_CSPortComm_Main.DataReadIndex; tempIndex++) 
		{				
			if ((g_CSPortComm_Main.DataReadArray[tempIndex] == 0x5A) && (g_CSPortComm_Main.DataReadArray[tempIndex + 1] == 0xA5))
			{
				if (g_CSPortComm_Main.DataReadArray[tempIndex + 2] + 5 <= g_CSPortComm_Main.DataReadIndex - tempIndex)
				{
					pObj->m_BYTE_DataRxLength = g_CSPortComm_Main.DataReadArray[tempIndex + 2] + 5;
					memcpy(pObj->m_BYTE_DataRxArray, g_CSPortComm_Main.DataReadArray + tempIndex, sizeof(BYTE)*pObj->m_BYTE_DataRxLength);
					EnterCriticalSection(&g_CSPortComm_Main.g_CriticalSection);		//进入临界区
					memcpy(g_CSPortComm_Main.DataReadArray, g_CSPortComm_Main.DataReadArray + pObj->m_BYTE_DataRxLength + tempIndex, sizeof(BYTE)*g_CSPortComm_Main.DataReadIndex); //剩余数据向前覆盖
					g_CSPortComm_Main.DataReadIndex -= pObj->m_BYTE_DataRxLength + tempIndex;
					LeaveCriticalSection(&g_CSPortComm_Main.g_CriticalSection);		//离开临界区
																					//数据处理						
					pObj->Comm_DataRxProcess();
					memset(pObj->m_BYTE_DataRxArray, 0x00, pObj->m_BYTE_DataRxLength);

				}

			}
		}
		Sleep(1);
	}
	return 0;
}
//串口 数据 处理
BYTE CHMMoniterDlg::Comm_DataRxProcess()   
{
	WORD RegAddr = 0;
	BYTE RegLength = 0;
	CString strtemp=_T("");
	BYTE i=0 ;
	BYTE Error = 0;
	CString temp= _T("");

	g_CDriverParam_Main.m_UINT8_RegisterErrorCode = m_BYTE_DataRxArray[3] & 0xF0;

	BYTE FunctionCode = m_BYTE_DataRxArray[3] & 0x07;

	if (m_BYTE_DataRxArray[2] != (m_BYTE_DataRxLength - 5))				//数据域长度不正确
		return 1; 
	
	if (g_CDriverParam_Main.m_UINT8_RegisterErrorCode)
	{
		SetEvent(g_HANDLE_CommResponce);

		//processErrorCode(ErrorCode);
		return 1;
	}

	//设备地址 1											
	if ((m_BYTE_DataRxArray[3] & 0x08) == 0x00)
	{
		RegAddr = m_BYTE_DataRxArray[4] << 8 | m_BYTE_DataRxArray[5];
		RegLength = m_BYTE_DataRxArray[6];
		
		switch (RegAddr)
		{
			//通讯参数
			case REG_CAN:
				if (!isCorrectRegNum(REG_CAN))
					return 5;
				if (FunctionCode == 0x01) 
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_setCanBaudrate);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_setCanAddress);
				}
				SetEvent(g_HANDLE_CommResponce);
				break;		
			
			//设备信息
			case REG_DEVICE_INFO:
				if (!isCorrectRegNum(REG_DEVICE_INFO)) 
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.m_CString_getDeviceInfo = "";
					g_CDriverParam_Main.m_CString_getDeviceType = "";

					for (i = REG_INFIX_LENGTH; m_BYTE_DataRxArray[i] != '?'; i++)
					{
						strtemp.Format(_T("%c"), m_BYTE_DataRxArray[i]);
						g_CDriverParam_Main.m_CString_getDeviceInfo += strtemp;
					}
					for (i = 16 + REG_INFIX_LENGTH; m_BYTE_DataRxArray[i] != '?'; i++)
					{
						strtemp.Format(_T("%c"), m_BYTE_DataRxArray[i]);
						g_CDriverParam_Main.m_CString_getDeviceType += strtemp;
					}
				}
				//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
				break;
			//设备默认
			case REG_DEVICE_STATIC:
				if (!isCorrectRegNum(REG_DEVICE_STATIC))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_unParam_getDriverMinVoltage.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_getDriverMaxVoltage.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 12, &g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 16, &g_CDriverParam_Main.m_INT32_getDriverMinVelocity);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 20, &g_CDriverParam_Main.m_INT32_getDriverMaxVelocity);
					g_CDriverParam_Main.m_FLOAT_getDriverMinSpeed = g_CDriverParam_Main.m_INT32_getDriverMinVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
					g_CDriverParam_Main.m_FLOAT_getDriverMaxSpeed = g_CDriverParam_Main.m_INT32_getDriverMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
				}
				//memcpy(g_BYTE_NewDriverDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
				break;
			//设备状态
			case REG_DEVICE_STATUS:
				if (!isCorrectRegNum(REG_DEVICE_STATUS))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_getStatusDevice);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &g_CDriverParam_Main.m_UINT8_getStatusMotor);
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &g_CDriverParam_Main.m_UINT16_getStatusError);
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_UINT16_getStatusErrorBackup);

					if (g_CDriverParam_Main.m_UINT8_getStatusFlag)			//实时显示 勾选
					{
						g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 6, &g_CDriverParam_Main.m_INT32_getStatusVelocity);
						g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 10, &g_CDriverParam_Main.m_INT32_getStatusPosition);
						g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 14, &g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_int);
						g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 18, &g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_int);

						if (g_CDriverParam_Main.m_CString_getDeviceType == "ACS306")
							g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 22, &g_CDriverParam_Main.m_UINT32_getStatusJointPosition);	//关节模组驱动器 多读一个 关节位置寄存器
					}
				}
				//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//设备实时
			//case REG_DEVICE_REAL:
			//	if (!isCorrectRegNum(REG_DEVICE_REAL))
			//		return 5;
			//	if (FunctionCode == 0x01)
			//	{
			//		g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_INT32_getStatusVelocity);
			//		g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_INT32_getStatusPosition);
			//		g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_int);
			//		g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 12, &g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_int);
			//		//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
			//	}
			//	SetEvent(g_HANDLE_CommResponce);
			//	break;
			//设备故障

			case REG_DEVICE_REAL + 4:
				if (FunctionCode == 0x02)
				{
				}
				SetEvent(g_HANDLE_CommResponce);
				break;

			case REG_DEVICE_ERRORCODE:
				if (!isCorrectRegNum(REG_DEVICE_ERRORCODE))
					return 5;
				if (FunctionCode == 0x02)
				{
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//电机参数
			case REG_MOTOR_POLES:
				if (!isCorrectRegNum(REG_MOTOR_POLES))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_setPolePairs);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &g_CDriverParam_Main.m_UINT8_setEncoderType);
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &g_CDriverParam_Main.m_UINT16_setEncoderResolution);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 12, &g_CDriverParam_Main.m_INT32_setRatedSpeed);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 16, &g_CDriverParam_Main.m_UINT8_setHallOrder);

					//if (g_CDriverParam_Main.m_UINT8_setEncoderType == 0)
					//{
					//	g_CDriverParam_Main.m_UINT32_setPulsePerRevolution = g_CDriverParam_Main.m_UINT8_setPolePairs * 6;
					//
					//	//g_CDriverParam_Main.m_FLOAT_setSpeedUnitScale = 10.0F / g_CDriverParam_Main.m_UINT8_setPolePairs;
					//}
					//else
					//{
					//	g_CDriverParam_Main.m_UINT32_setPulsePerRevolution = g_CDriverParam_Main.m_UINT16_setEncoderResolution * 4;
					//	//g_CDriverParam_Main.m_FLOAT_setSpeedUnitScale = 15.0F / g_CDriverParam_Main.m_UINT16_setEncoderResolution;
					//}

					//g_CDriverParam_Main.m_FLOAT_setRatedSpeed = g_CDriverParam_Main.m_INT32_setRatedVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//应用参数
			case REG_APPLICATION:
				if (!isCorrectRegNum(REG_APPLICATION))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_setMotionDirection, 0x00);
					g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_setInitialPosition, 0x01);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 9, &g_CDriverParam_Main.m_INT32_setAppMaxSpeed);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 13, &g_CDriverParam_Main.m_INT32_setAcceleration);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 17, &g_CDriverParam_Main.m_INT32_setDeceleration);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 21, &g_CDriverParam_Main.m_INT32_setStopDeceleration);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 25, &g_CDriverParam_Main.m_INT32_setAppMaxPosition);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 29, &g_CDriverParam_Main.m_INT32_setAppMinPosition);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 33, &g_CDriverParam_Main.m_INT32_setInitialPosition);
					
					g_CDriverParam_Main.m_FLOAT_setAppMaxAngle = g_CDriverParam_Main.m_INT32_setAppMaxPosition /* 365.0f / 8080000*/;
					g_CDriverParam_Main.m_FLOAT_setAppMinAngle = g_CDriverParam_Main.m_INT32_setAppMinPosition /* 365.0f / 8080000*/;

					//g_CDriverParam_Main.m_FLOAT_setAppMaxSpeed = g_CDriverParam_Main.m_INT32_setAppMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
					g_CDriverParam_Main.m_FLOAT_setAppMaxSpeed = g_CDriverParam_Main.m_INT32_setAppMaxVelocity;
				}
				//memcpy(g_BYTE_NewDriverDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//电流环参数
			case REG_CURRENTLOOP_P:
				if (!isCorrectRegNum(REG_CURRENTLOOP_P))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//速度环参数
			case REG_VELOCITYLOOP_P:
				if (!isCorrectRegNum(REG_VELOCITYLOOP_P))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//位置环参数
			case REG_POSITIONLOOP_P:
				if (!isCorrectRegNum(REG_POSITIONLOOP_P))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);
				};
				SetEvent(g_HANDLE_CommResponce);
			break;
			//电流环 测试参数
			case REG_CURRENTLOOP_TEST:
				if (!isCorrectRegNum(REG_CURRENTLOOP_TEST))
					return 5;
				//memcpy(g_BYTE_NewPIDDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//电流环 测试标志
			case REG_CURRENTLOOP_TEST_FLAG:
				if (!isCorrectRegNum(REG_CURRENTLOOP_TEST_FLAG))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_getCurrentTestResult);
				}
				//memcpy(g_BYTE_NewPIDDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//电流环 测试数据长度
			case REG_CURRENTLOOP_TEST_LENGTH:
				if (!isCorrectRegNum(REG_CURRENTLOOP_TEST_LENGTH))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT16_getCurrentTestLength);
				}
				//memcpy(g_BYTE_NewPIDDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//速度环 测试标志
			case REG_VELOCITYLOOP_TEST_FLAG:
				if (!isCorrectRegNum(REG_VELOCITYLOOP_TEST_FLAG))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_getVelocityTestResult);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//速度环 测试参数
			case REG_VELOCITYLOOP_TEST_VELOCITY:
				if (!isCorrectRegNum(REG_VELOCITYLOOP_TEST_VELOCITY))
					return 5;
				//memcpy(g_BYTE_NewPIDDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//速度环 测试数据长度
			case REG_VELOCITYLOOP_TEST_LENGTH:
				if (!isCorrectRegNum(REG_VELOCITYLOOP_TEST_LENGTH))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT16_getVelocityTestLength);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;

			//位置环 测试标志
			case REG_POSITIONLOOP_TEST_FLAG:
				if (!isCorrectRegNum(REG_POSITIONLOOP_TEST_FLAG))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_getPositionTestResult);
				}
				SetEvent(g_HANDLE_CommResponce);
				break;
			//位置环 测试参数
			case REG_POSITIONLOOP_TEST:
				if (!isCorrectRegNum(REG_POSITIONLOOP_TEST))
					return 5;
				//memcpy(g_BYTE_NewPIDDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
				break;
			//位置环 测试数据长度
			case REG_POSITIONLOOP_TEST_LENGTH:
				if (!isCorrectRegNum(REG_POSITIONLOOP_TEST_LENGTH))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT16_getPositionTestLength);
				}
				SetEvent(g_HANDLE_CommResponce);
				break;


			//电机匹配 标志
			case REG_MATCH_FLAG:
				if (!isCorrectRegNum(REG_MATCH_FLAG))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &g_CDriverParam_Main.m_UINT8_getMatchFlag);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &g_CDriverParam_Main.m_UINT8_getPhaseOrder);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, &g_CDriverParam_Main.m_UINT8_getHallOrder);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 3, &g_CDriverParam_Main.m_INT8_getHallDirection);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_INT8_getEncoderDirection);
					g_CDriverParam_Main.ArrayToHex16(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &g_CDriverParam_Main.m_INT16_getDefaultDegree);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;

			//工作模式
			case REG_RUN_MODE:
				if (!isCorrectRegNum(REG_RUN_MODE))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &m_UINT8_setWorkMode);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &m_unParam_setCurrent.param_int);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &m_INT32_setSpeed);
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 9, &m_UINT8_setPositionType);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 10, &m_INT32_setPosition);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 14, &m_INT32_setPositionSpeed);
				}
				SetEvent(g_HANDLE_CommResponce);
			break;
			//工作模式 电流
			case REG_RUN_CURRENT:
				if (!isCorrectRegNum(REG_RUN_CURRENT))
					return 5;		
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &m_unParam_setCurrent.param_int);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
				}	
				//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//工作模式 速度
			case REG_RUN_VELOCITY:
				if (!isCorrectRegNum(REG_RUN_VELOCITY))
					return 5;
				//			g_BYTE_CommResponceStatus = REG_RUN_VELOCITY;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &m_INT32_setSpeed);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
				}
				//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;
			//工作模式 位置
			case REG_RUN_POSITION:
				if (!isCorrectRegNum(REG_RUN_POSITION))
					return 5;
				if (FunctionCode == 0x01)
				{
					g_CDriverParam_Main.ArrayToHex8(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, &m_UINT8_setPositionType);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, &m_INT32_setPosition);
					g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 5, &m_INT32_setPositionSpeed);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4, &g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
					//g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 8, &g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
				}


				//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			break;

			case REG_USER_DEF:
				if (!isCorrectRegNum(REG_USER_DEF))
					return 5;
				if (FunctionCode == 0x01)
				{
					for (UINT8 i = 0; i < 8; i++)
					{
						g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_UserDef + i, i);
						g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_UserDef + i + 8, i);
						g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT8_UserDef + i + 16, i);
						g_CDriverParam_Main.ArrayToBit(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_UINT8_UserDef + i + 24, i);
					}
					for (UINT8 i = 0; i < 24; i++)
					{
						g_CDriverParam_Main.ArrayToHex32(m_BYTE_DataRxArray + REG_INFIX_LENGTH + 4 + 4 * i, g_CDriverParam_Main.m_INT32_UserDef + i);
					}
				}
				SetEvent(g_HANDLE_CommResponce);
				break;
		//命令
		case REG_COMMAND:
			if (!isCorrectRegNum(REG_COMMAND))
				return 5;
			//memcpy(m_BYTE_MoniterDataRx, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
			//memcpy(g_BYTE_NewMotorDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
			//memcpy(g_BYTE_NewSaveDataRxArray, m_BYTE_DataRxArray, sizeof(BYTE)*m_BYTE_DataRxLength);
			SetEvent(g_HANDLE_CommResponce);				//寄存器数量 错误
		break;

		//环路 测试数据
		default:
			if ((RegAddr >= REG_LOOP_TESTDATA) && (RegAddr <= REG_LOOP_TESTDATA + 0x2000))
			{
				//if (!isCorrectRegNum(REG_LOOP_TESTDATA))
				//	return 5;
				if (m_UINT16_TestDataLength > 128)
				{
					memcpy(g_CDriverParam_Main.m_UINT8_pTestData + m_UINT16_TestDataIndex , m_BYTE_DataRxArray + REG_INFIX_LENGTH, sizeof(UINT8) * 128);					
					//memcpy(m_BYTE_pTestData + m_BYTE_TestDataIndex, g_CDriverParam_Main.m_UINT8_getLoopTestData, sizeof(BYTE) * 128);
					m_UINT16_TestDataIndex += 128;
					m_UINT16_TestDataLength -= 128;
				}
				else
				{
					memcpy(g_CDriverParam_Main.m_UINT8_pTestData + m_UINT16_TestDataIndex, m_BYTE_DataRxArray + REG_INFIX_LENGTH, sizeof(UINT8) * (UINT8)m_UINT16_TestDataLength);
					//memcpy(m_BYTE_pTestData + m_BYTE_TestDataIndex, g_CDriverParam_Main.m_UINT8_getLoopTestData, sizeof(BYTE) * (g_CDriverParam_Main.m_UINT16_getCurrentTestLength - (RegAddr - REG_LOOP_TESTDATA));
					m_UINT16_TestDataIndex += (UINT8)m_UINT16_TestDataLength;
					m_UINT16_TestDataLength -= (UINT8)m_UINT16_TestDataLength;
				}
				//memcpy(g_CDriverParam_Main.m_UINT8_getLoopTestData, m_BYTE_DataRxArray + REG_INFIX_LENGTH, sizeof(UINT8)*m_BYTE_DataRxLength);
				SetEvent(g_HANDLE_CommResponce);
			}
			else
			{
				return 4;
			}
			break;											//寄存器地址不支持
		}
	}
	//设备地址 2
	else if((m_BYTE_DataRxArray[3] & 0x08) == 0x01)
	{}
	//设备地址 错误
	else 
		return 2;
	return 0;
}
//帧 消息处理
LRESULT CHMMoniterDlg::OnUserMoniterRx(WPARAM wParam, LPARAM lParam)
{
	//processDataRx(wParam);
	memset(m_BYTE_MoniterDataRx, 0x00, sizeof(m_BYTE_MoniterDataRx));
	return 0;
}

//数据 读取处理 负责界面显示及对象内变量设置
//0 处理正常 1 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::processDataRxRead(UINT16 reg_word)
{
	// TODO: 在此处添加实现代码.
//	BYTE FunctionCode = m_BYTE_MoniterDataRx[3] & 0x07;
	CString strtemp;

	if (g_CDriverParam_Main.m_UINT8_RegisterErrorCode)
	{
		return 0xF0 + (g_CDriverParam_Main.m_UINT8_RegisterErrorCode >> 4);
	}	
	switch (reg_word)
	{
		case REG_DEVICE_STATUS:
			//显示设备状态
			switch (g_CDriverParam_Main.m_UINT8_getStatusDevice)
			{
			case 0:
			case 1:
			case 2:
			case 3:
			case 4:
			case 5:
			case 6:
			case 7:
			case 8:
				strtemp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_getStatusDevice);
				m_StatusBar_Main.SetPaneText(2, strtemp);
			default:
				break;
			}
			//显示电机状态
			switch (g_CDriverParam_Main.m_UINT8_getStatusMotor)
			{
			case 0:

				m_StatusBar_Main.SetPaneText(3, _T("电机禁用"));
				m_CComboBox_setWorkMode.EnableWindow(TRUE);
				m_BYTE_isMotorEnabled = 0;
				break;
			case 1:
			case 2:
				m_StatusBar_Main.SetPaneText(3, _T("电机使能"));
				m_CComboBox_setWorkMode.EnableWindow(FALSE);
				m_BYTE_isMotorEnabled = 1;
				break;
			default:
				break;  
			}

			//显示错误状态
			strtemp.Format(_T("%d"), g_CDriverParam_Main.m_UINT16_getStatusError);
			m_StatusBar_Main.SetPaneText(4, strtemp);

			//显示实时参数
			if (g_CDriverParam_Main.m_UINT8_getStatusFlag)
			{
				//参数显示
				//if (m_CButton_toggleUnit.GetCheck())
				//{

				//g_CDriverParam_Main.m_FLOAT_getStatusVelocity = g_CDriverParam_Main.m_INT32_getStatusVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
				//if (g_CDriverParam_Main.m_FLOAT_getStatusVelocity >= 0)
				//	g_CDriverParam_Main.m_FLOAT_getStatusVelocity += 0.5F;
				//else
				//	g_CDriverParam_Main.m_FLOAT_getStatusVelocity -= 0.5F;

				//g_CDriverParam_Main.m_INT32_getStatusVelocity
				strtemp.Format(_T("%d"), (INT32)g_CDriverParam_Main.m_INT32_getStatusVelocity);
				//}
				//else
				//{
				//	strtemp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_getStatusVelocity);
				//}
				GetDlgItem(IDC_EDIT_getStatusVelocity)->SetWindowTextW(strtemp);
				strtemp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_getStatusPosition);
				GetDlgItem(IDC_EDIT_getStatusPosition)->SetWindowTextW(strtemp);
				strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_float);
				GetDlgItem(IDC_EDIT_getStatusPhaseCurrent)->SetWindowTextW(strtemp);
				strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_float);
				GetDlgItem(IDC_EDIT_getStatusBusVoltage)->SetWindowTextW(strtemp);
				strtemp.Format(_T("%d"), g_CDriverParam_Main.m_UINT32_getStatusJointPosition);
				m_CEdit_getStatusJointPosition.SetWindowTextW(strtemp);
				strtemp.Format(_T("%f"), g_CDriverParam_Main.m_UINT32_getStatusJointPosition * 360 / 131072.0);
				m_CEdit_getStatusJointDegree.SetWindowTextW(strtemp);

				//波形显示
				switch (m_UINT8_setWaveChannel)
				{
				case 0:
					break;
				case 1:
					m_CSeries_Data1.Add(g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_float, NULL, RGB(255, 0, 0));
					break;
				case 2:
					m_CSeries_Data1.Add(g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_float, NULL, RGB(255, 0, 0));
					break;
				case 3:
					if (m_CButton_toggleUnit.GetCheck())
					{
						m_CSeries_Data1.Add(g_CDriverParam_Main.m_FLOAT_getStatusVelocity, NULL, RGB(255, 0, 0));
					}
					else
					{
						m_CSeries_Data1.Add(g_CDriverParam_Main.m_INT32_getStatusVelocity, NULL, RGB(255, 0, 0));
					}
					break;
				case 4:
					m_CSeries_Data1.Add(g_CDriverParam_Main.m_INT32_getStatusPosition, NULL, RGB(255, 0, 0));
					break;
				case 5:
					m_CSeries_Data1.Add(g_CDriverParam_Main.m_UINT32_getStatusJointPosition, NULL, RGB(255, 0, 0));
					break;
				default:
					AfxMessageBox(_T("设置无效"));
					break;
				}
			}
		break;
		case REG_DEVICE_REAL:
			//实时显示
			//if (m_CButton_toggleUnit.GetCheck())
			//{
			//	g_CDriverParam_Main.m_FLOAT_getStatusVelocity = g_CDriverParam_Main.m_INT32_getStatusVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;

			//	if (g_CDriverParam_Main.m_FLOAT_getStatusVelocity >= 0)

			//		g_CDriverParam_Main.m_FLOAT_getStatusVelocity += 0.5F;
			//	else
			//		g_CDriverParam_Main.m_FLOAT_getStatusVelocity -= 0.5F;

			//	strtemp.Format(_T("%d"), (INT32)g_CDriverParam_Main.m_FLOAT_getStatusVelocity);
			//}
			//else
			//{
			//	strtemp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_getStatusVelocity);
			//}
			//GetDlgItem(IDC_EDIT_getStatusVelocity)->SetWindowTextW(strtemp);
			//strtemp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_getStatusPosition);
			//GetDlgItem(IDC_EDIT_getStatusPosition)->SetWindowTextW(strtemp);
			//strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_float);
			//GetDlgItem(IDC_EDIT_getStatusPhaseCurrent)->SetWindowTextW(strtemp);
			//strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_float);
			//GetDlgItem(IDC_EDIT_getStatusBusVoltage)->SetWindowTextW(strtemp);
			////波形显示
			//switch (m_UINT8_setWaveChannel)
			//{
			//case 0:
			//	break;
			//case 1:
			//	m_CSeries_Data1.Add(g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_float, NULL, RGB(255, 0, 0));
			//	break;
			//case 2:
			//	m_CSeries_Data1.Add(g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_float, NULL, RGB(255, 0, 0));
			//	break;
			//case 3:
			//	if (m_CButton_toggleUnit.GetCheck())
			//	{
			//		m_CSeries_Data1.Add(g_CDriverParam_Main.m_FLOAT_getStatusVelocity, NULL, RGB(255, 0, 0));
			//	}
			//	else
			//	{
			//		m_CSeries_Data1.Add(g_CDriverParam_Main.m_INT32_getStatusVelocity, NULL, RGB(255, 0, 0));
			//	}
			//	break;
			//case 4:
			//	m_CSeries_Data1.Add(g_CDriverParam_Main.m_INT32_getStatusPosition, NULL, RGB(255, 0, 0));
			//	break;
			//default:
			//	AfxMessageBox(_T("设置无效"));
			//	break;
			//}
		break;
		case REG_DEVICE_INFO:

			m_StatusBar_Main.SetPaneText(0, g_CDriverParam_Main.m_CString_getDeviceType);
			m_StatusBar_Main.SetPaneText(1, g_CDriverParam_Main.m_CString_getDeviceInfo);

		break;
		case REG_RUN_MODE:

			m_CComboBox_setWorkMode.SetCurSel(m_UINT8_setWorkMode);
			switch (m_UINT8_setWorkMode)
			{
			//电流模式
			case 0:
				GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(FALSE);
				GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(TRUE);
				//GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(FALSE);

				m_CString_setCurrent.Format(_T("%g"), m_unParam_setCurrent.param_float);
				m_CEdit_setCurrent.SetWindowTextW(m_CString_setCurrent);

				break;
			//速度模式
			case 1:
				GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(FALSE);
				GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(FALSE);
				GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(FALSE);
				//GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(TRUE);

				m_CString_setSpeed.Format(_T("%d"), m_INT32_setSpeed);
				m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);

				break;
			//位置模式
			case 2:
				GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(TRUE);
				GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(TRUE);
				GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(FALSE);
				//GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(FALSE);

				m_CComboBox_setPositionType.SetCurSel(m_UINT8_setPositionType);
				m_CString_setPosition.Format(_T("%d"), m_INT32_setPosition);
				m_CEdit_setPosition.SetWindowTextW(m_CString_setPosition);
				m_CString_setSpeed.Format(_T("%d"), m_INT32_setPositionSpeed);
				m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);

				break;
			default:
				AfxMessageBox(_T("工作模式无效"));
				break;
			}
		break;

		case REG_RUN_CURRENT:

			m_CString_setCurrent.Format(_T("%g"), m_unParam_setCurrent.param_float);
			m_CEdit_setCurrent.SetWindowTextW(m_CString_setCurrent);

			break;

		case REG_RUN_VELOCITY:
			//if (m_CButton_toggleUnit.GetCheck())
			//{
			m_FLOAT_setSpeed = m_INT32_setSpeed * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;

			if (m_FLOAT_setSpeed >= 0)

				m_FLOAT_setSpeed += 0.5F;
			else
				m_FLOAT_setSpeed -= 0.5F;

			m_CString_setSpeed.Format(_T("%d"), (INT32)m_FLOAT_setSpeed);
			//}
			//else
			//{
			//	m_CString_setSpeed.Format(_T("%d"), m_INT32_setSpeed);
			//}
			m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);
			//strtemp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_getStatusPosition);
			//GetDlgItem(IDC_EDIT_getStatusPosition)->SetWindowTextW(strtemp);
			strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusPhaseCurrent.param_float);
			GetDlgItem(IDC_EDIT_getStatusPhaseCurrent)->SetWindowTextW(strtemp);
			strtemp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getStatusBusVoltage.param_float);
			GetDlgItem(IDC_EDIT_getStatusBusVoltage)->SetWindowTextW(strtemp);

			break;

		case REG_RUN_POSITION:
			m_CComboBox_setPositionType.SetCurSel(m_UINT8_setPositionType);
			m_CString_setPosition.Format(_T("%d"), m_INT32_setPosition);
			m_CEdit_setPosition.SetWindowTextW(m_CString_setPosition);
			m_CString_setSpeed.Format(_T("%d"), m_INT32_setPositionSpeed);
			m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);
			break;
	case REG_MATCH_FLAG:
		if (g_CDriverParam_Main.m_UINT8_getMatchFlag == 0x01)
		{
			//KillTimer(TIMER_ID_MATCHFLAG);
			if (m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_hWnd != NULL)
			{
				//KillTimer(TIMER_ID_MATCHFLAG);
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDOK;			//1
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配完成，请关闭……！"));
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
			}
		}
		else if (g_CDriverParam_Main.m_UINT8_getMatchFlag == 0x02)
		{
			//KillTimer(TIMER_ID_MATCHFLAG);

			if (m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_hWnd != NULL)
			{
				//KillTimer(TIMER_ID_MATCHFLAG);
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDABORT;		//3
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配失败，请关闭……！"));
				m_CDialog_FileNewMotor.m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
			}
		}
		break;
	case REG_CURRENTLOOP_TEST_FLAG:
		//if (g_CDriverParam_Main.m_UINT8_getCurrentTestResult == 03)
		//{
		//	KillTimer(TIMER_ID_CURRENTTEST);
		//	m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，正在读取数据……"));		
		//}
		break;
	case REG_COMMAND:
/*			if (CommandWord == COMMAND_MOTOR_ENABLE)
			{
				GetDlgItem(IDC_BUTTON_setMotorEnable)->SetWindowTextW(_T("禁 用"));
				m_CComboBox_setWorkMode.EnableWindow(FALSE);
				m_BYTE_isMotorEnabled = 1;
			}
			else if (CommandWord == COMMAND_MOTOR_DISABLE)
			{
				GetDlgItem(IDC_BUTTON_setMotorEnable)->SetWindowTextW(_T("使 能"));
				m_CComboBox_setWorkMode.EnableWindow(TRUE);
				m_BYTE_isMotorEnabled = 0;

			}
			else if (CommandWord == COMMAND_MOTOR_RUN)
			{
			}
			else
			{
			}	*/	
		break;
	case REG_CAN:
		strtemp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setCanAddress);
		m_CComboBox_setCanBaudrate.SetCurSel(g_CDriverParam_Main.m_UINT8_setCanBaudrate);
		m_CEdit_setCanAddress.SetWindowTextW(strtemp);
		break;
	default:
		break;
	}

	return 0;
}

//数据 写入处理 负责界面显示及对象内变量设置
//0 处理正常 1 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::processDataRxWrite(UINT16 reg_word)
{
	
	if (g_CDriverParam_Main.m_UINT8_RegisterErrorCode)
	{
		return 0xF0 + (g_CDriverParam_Main.m_UINT8_RegisterErrorCode >> 4);
	}
	
	switch (reg_word)
	{
	case REG_DEVICE_STATUS:
		break;
	case REG_DEVICE_ERRORCODE:
		g_CDriverParam_Main.m_UINT16_getStatusError = 0;
		GetDlgItem(IDC_EDIT_getStatusError)->SetWindowTextW(_T("无错误"));
		break;
	case REG_DEVICE_INFO:
		break;
	case REG_RUN_MODE:
		//g_CDriverParam_Main.ArrayToHex8(m_BYTE_MoniterDataRx + REG_INFIX_LENGTH + 0, &m_UINT8_setWorkMode);
		//g_CDriverParam_Main.ArrayToHex32(m_BYTE_MoniterDataRx + REG_INFIX_LENGTH + 1, &m_unParam_setCurrent.param_int);
		//g_CDriverParam_Main.ArrayToHex32(m_BYTE_MoniterDataRx + REG_INFIX_LENGTH + 5, &m_INT32_setSpeed);
		//g_CDriverParam_Main.ArrayToHex8(m_BYTE_MoniterDataRx + REG_INFIX_LENGTH + 9, &m_UINT8_setPositionType);
		//g_CDriverParam_Main.ArrayToHex32(m_BYTE_MoniterDataRx + REG_INFIX_LENGTH + 10, &m_INT32_setPosition);

		//m_CComboBox_setWorkMode.SetCurSel(m_UINT8_setWorkMode);

		//switch (m_UINT8_setWorkMode)
		//{
		//	//电流模式
		//case 0:
		//	GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(FALSE);
		//	break;
		//	//速度模式
		//case 1:
		//	GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(TRUE);
		//	break;
		//	//位置模式
		//case 2:
		//	GetDlgItem(IDC_EDIT_setSpeed)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_setPosition)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_COMBO_setPositionType)->EnableWindow(TRUE);
		//	GetDlgItem(IDC_EDIT_setCurrent)->EnableWindow(FALSE);
		//	GetDlgItem(IDC_BUTTON_setMotorReverse)->EnableWindow(FALSE);
		//	break;
		//default:
		//	AfxMessageBox(_T("工作模式无效"));
		//	break;
		//}

		//m_CString_setCurrent.Format(_T("%g"), m_unParam_setCurrent.param_float);
		//m_CEdit_setCurrent.SetWindowTextW(m_CString_setCurrent);

		//m_CString_setSpeed.Format(_T("%d"), m_INT32_setSpeed);
		//m_CEdit_setSpeed.SetWindowTextW(m_CString_setSpeed);

		//m_CComboBox_setPositionType.SetCurSel(m_UINT8_setPositionType);

		//m_CString_setPosition.Format(_T("%d"), m_INT32_setPosition);
		//m_CEdit_setPosition.SetWindowTextW(m_CString_setPosition);

		break;

	case REG_CURRENTLOOP_P:
		break;
	case REG_CURRENTLOOP_TEST:
		break;
	case REG_CURRENTLOOP_TEST_FLAG:
		break;
	case REG_COMMAND:
		if (CommandWord == COMMAND_MOTOR_ENABLE)
		{
			GetDlgItem(IDC_BUTTON_setMotorEnable)->SetWindowTextW(_T("禁 用"));
			m_CComboBox_setWorkMode.EnableWindow(FALSE);
			m_BYTE_isMotorEnabled = 1;
		}
		else if (CommandWord == COMMAND_MOTOR_DISABLE)
		{
			GetDlgItem(IDC_BUTTON_setMotorEnable)->SetWindowTextW(_T("使 能"));
			m_CComboBox_setWorkMode.EnableWindow(TRUE);
			m_BYTE_isMotorEnabled = 0;
		}
		else
		{
		}
		break;
	default:
		break;
	}
	return 0;
}

//函数 处理错误代码的现实
//参数1 无响应 参数F0 + ErrorCode 访问寄存器出错
UINT8 CHMMoniterDlg::processErrorCode(UINT8 ErrorCode)
{
	// TODO: 在此处添加实现代码.

	switch (ErrorCode)
	{
	case ERRORCODE_NORESPONCE:
		AfxMessageBox(_T("设备无响应"));
		break;
	case ERRORCODE_CRC:
		AfxMessageBox(_T("错误的CRC校验"));
		break;
	case ERRORCODE_FUNCTION:
		AfxMessageBox(_T("错误的功能码"));
		break;
	case ERRORCODE_ADDRESS:
		AfxMessageBox(_T("错误的参数地址"));
		break;
	case ERRORCODE_COMMAND:
		AfxMessageBox(_T("错误的命令"));
		break;
	case ERRORCODE_COMMANDVALUE:
		AfxMessageBox(_T("错误的命令值"));
		break;
	case ERRORCODE_INVALIDE:
		AfxMessageBox(_T("无法处理命令"));
		break;
	case ERRORCODE_MATCH:
		AfxMessageBox(_T("电机未匹配"));
		break;
	case ERRORCODE_READONLY:
		AfxMessageBox(_T("参数只读"));
		break;
	case ERRORCODE_ENABLE:
		AfxMessageBox(_T("电机未使能"));
		break;
	case ERRORCODE_OVERFLOW:
		AfxMessageBox(_T("参数溢出"));
		break;
	default:
		if (ErrorCode > 0xF0)
			AfxMessageBox(_T("未知错误"));
		break;
	}
	return 0;
}

// 函数 写入 寄存器 返回 无响应 或 processDataRxWrite的处理结果
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setRegValue(UINT16 reg,UINT8 reglength)
{
	m_BYTE_DataTxArray[0] = 0x5A;
	m_BYTE_DataTxArray[1] = 0xA5;

	m_BYTE_DataTxArray[2] = 0x04 + reglength;

	m_BYTE_DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x02 & 0x07);
	m_BYTE_DataTxArray[4] = reg >> 8;
	m_BYTE_DataTxArray[5] = reg & 0xFF;
	m_BYTE_DataTxArray[6] = reglength;

	WORD CRCValue = g_CSPortComm_Main.CRC16(m_BYTE_DataTxArray, reglength+ REG_INFIX_LENGTH);
	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH] = (CRCValue & 0xFF);
	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH+1] = (CRCValue >> 8);

	g_CSPortComm_Main.SendData(m_BYTE_DataTxArray, reglength + REG_INFIX_LENGTH + 2);

	DWORD ErrorFlag = WaitForSingleObject(g_HANDLE_CommResponce, 10000);
	if (ErrorFlag != WAIT_OBJECT_0)
	{
		return ERRORCODE_NORESPONCE;
	}
	else
	{
		return processDataRxWrite(reg);
		//SendMessageW(WM_USER_MONITER_RX, COMMAND_MOTOR_ENABLE, 0);
	}
	return 0;
}

// 函数 读取 寄存器 返回 无响应 或 processDataRxWrite的处理结果 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::getRegValue(UINT16 reg, UINT8 reglength)
{
	m_BYTE_DataTxArray[0] = 0x5A;
	m_BYTE_DataTxArray[1] = 0xA5;

	m_BYTE_DataTxArray[2] = 0x04;

	m_BYTE_DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x01 & 0x07);
	m_BYTE_DataTxArray[4] = reg >> 8;       //取高八位
	m_BYTE_DataTxArray[5] = reg & 0xFF;    //取低八位
	m_BYTE_DataTxArray[6] = reglength;

	WORD CRCValue = g_CSPortComm_Main.CRC16(m_BYTE_DataTxArray, 0 + REG_INFIX_LENGTH);
	m_BYTE_DataTxArray[REG_INFIX_LENGTH + 0] = (CRCValue & 0xFF);    //低字节在前
	m_BYTE_DataTxArray[REG_INFIX_LENGTH + 0 + 1] = (CRCValue >> 8);  //高字节在后

	g_CSPortComm_Main.SendData(m_BYTE_DataTxArray, REG_INFIX_LENGTH + 0 + 2);

	DWORD ErrorFlag = WaitForSingleObject(g_HANDLE_CommResponce, 10000);
	if (ErrorFlag != WAIT_OBJECT_0)
	{
		return ERRORCODE_NORESPONCE;
	}
	else
	{
		return processDataRxRead(reg);
	//	SendMessageW(WM_USER_MONITER_RX, 0, 0);
	}

	return 0;
}

BYTE CHMMoniterDlg::isCorrectRegNum(WORD regaddr)
{
	// TODO: 在此处添加实现代码.
	return 1;
}

BOOL CHMMoniterDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}

//函数 打开 连接 子对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::openDlgSerialComm()
{
	UINT8 result = 0;

	// TODO: 在此处添加实现代码.
	result = m_CDialog_SerialComm.DoModal();
	if(result == IDOK)
	{

		CommProcessThread = CreateThread(NULL, 0, DataProcessThread, this, 0, NULL);		//开启接收线程

		//读取 设备信息
		result = getRegValue(REG_DEVICE_INFO, 34);
		if (result)
			return result;	

		//读取 设备状态
		g_CDriverParam_Main.m_UINT8_getStatusFlag = 0;

		if (g_CDriverParam_Main.m_CString_getDeviceType == "ACS306")
			result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM + 4);				//关节模组驱动器 多读一个 关节位置寄存器
		else
			result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);

		if (result)
			return result;

		//读取 通讯参数
		result = getRegValue(REG_CAN, REG_CAN_NUM);
		if (result)
			return result;

		//读取 设备运行模式
		result = getRegValue(REG_RUN_MODE, 18);
		if (result)
			return result;

		//读取 电机默认参数
		result = getRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
		if (result)
			return result;

		//读取 匹配参数
		result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
		if (result)
			return result;

		////读取 应用参数
		result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
		if (result)
			return result;

		result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
		if (result)
			return result;

		////读取 电流环
		//if (getRegValue(REG_CURRENTLOOP_P, 12) == 1)
		//{
		//	return 1;
		//}
		////读取 速度环
		//if (getRegValue(REG_VELOCITYLOOP_P, 12) == 1)
		//{
		//	return 1;
		//}
		////读取 位置环
		//if (getRegValue(REG_POSITIONLOOP_P, 12) == 1)
		//{
		//	return 1;
		//}

		SetDlgItemTextW(IDC_EDIT_setPositionCalibration, _T("0"));
		SetTimer(TIMER_ID_STATUS, 500, NULL);

		return IDOK;
	}

	return IDCANCEL;
}
//函数 打开 电机对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::openDlgFileMotor(UINT8 mode)
{
	//1:通讯错误 2:Ok 3:Cancel 4:上一步
	UINT8 result = 0;
	result = getRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
	if (result)
	{
		return result;
	}
	result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
	if (result)
	{
		return result;
	}
	
	if (mode)
	{
		m_CDialog_FileNewMotor.m_BYTE_setMotorEnabled = m_BYTE_isMotorEnabled;
		m_CDialog_FileNewMotor.m_BYTE_setGetStatus = m_CButton_getStatus.GetCheck();

		m_CDialog_FileNewMotor.m_BYTE_OpenMode = 1;
		result = m_CDialog_FileNewMotor.DoModal();
		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDOK)
		{
			result = setMotorPoles();
			if(result)
				return result;
			return IDOK;
		}
	}
	else
	{
		m_CDialog_FileNewMotor.m_BYTE_OpenMode = 0;
		result = m_CDialog_FileNewMotor.DoModal();

		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDABORT)
			return IDABORT;
		if (result == IDOK)
		{
			result = setMotorPoles();
			if (result)
				return result;
			return IDOK;
		}
	}
	return 0;
}
//函数 打开 驱动器对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::openDlgFileDriver(UINT8 mode)
{
	//1:通讯错误 2:Ok 3:Cancel 4:上一步
	UINT8 result = 0;
	result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
	if(result)
	{
		return result;
	}
	result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
	if (result)
	{
		return result;
	}

	//1 打开 0 新建
	if (mode)
	{

		m_CDialog_FileNewDriver.m_BYTE_setMotorEnabled = m_BYTE_isMotorEnabled;
		m_CDialog_FileNewDriver.m_BYTE_setGetStatus = m_CButton_getStatus.GetCheck();


		m_CDialog_FileNewDriver.m_BYTE_OpenMode = 1;
		result = m_CDialog_FileNewDriver.DoModal();
		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDOK)
		{
			result = setApplication();
			if (result)
				return result;
			return IDOK;
		}
	}

	else
	{
		m_CDialog_FileNewDriver.m_BYTE_OpenMode = 0;
		result = m_CDialog_FileNewDriver.DoModal();

		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDABORT)
			return IDABORT;
		if (result == IDOK)
		{
			result = setApplication();
			if (result)
				return result;
			return IDOK;
		}
	}
	return 0;
}
//函数 打开 环路对话框 //0 无效 1确认2取消3上一步 F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::openDlgFileLoop(UINT8 mode)
{
	//1:通讯错误 2:Ok 3:Cancel 4:上一步
	UINT8 result = 0;
	result = getRegValue(REG_CURRENTLOOP_P, REG_CURRENTLOOP_NUM);
	if(result)
	{
		return result;
	}
	result = getRegValue(REG_VELOCITYLOOP_P, REG_VELOCITYLOOP_NUM);
	if(result)
	{
		return result;
	}
	result = getRegValue(REG_POSITIONLOOP_P, REG_POSITIONLOOP_NUM);
	if (result)
	{
		return result;
	}

	if (mode)
	{

		m_CDialog_FileNewPID.m_BYTE_setMotorEnabled = m_BYTE_isMotorEnabled;
		m_CDialog_FileNewPID.m_BYTE_setGetStatus = m_CButton_getStatus.GetCheck();


		m_CDialog_FileNewPID.m_BYTE_OpenMode = 1;
		result = m_CDialog_FileNewPID.DoModal();
		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDOK)
		{			
			result = setCurrentLoop();
			if (result)
				return result;
			result = setVelocityLoop();
			if (result)
				return result;
			result = setPositionLoop();
			if (result)
				return result;
			return IDOK;
		}
	}

	else
	{
		m_CDialog_FileNewPID.m_BYTE_OpenMode = 0;
		result = m_CDialog_FileNewPID.DoModal();

		if (result == IDCANCEL)
			return IDCANCEL;
		if (result == IDABORT)
			return IDABORT;
		if (result == IDOK)
		{
			result = setCurrentLoop();
			if (result)
				return result;
			result = setVelocityLoop();
			if (result)
				return result;
			result = setPositionLoop();
			if (result)
				return result;
			return IDOK;
		}
	}
	return 0;
}
//函数 打开 CAN对话框 //0 无效 1确认2取消  F0 无响应 F0 + ErrCode 访问寄存器错误
UINT8 CHMMoniterDlg::openDlgCommCan()
{
	UINT8 result = 0;
	result = getRegValue(REG_CAN,REG_CAN_NUM);
	if (result)
	{
		return result;
	}

	result = m_CDialog_CDlgCommCan.DoModal();

	if (result == IDCANCEL)
		return IDCANCEL;
	if (result == IDOK)
	{
		result = setCan();
		if (result)
			return result;
	}
	return 0;
}
UINT8 CHMMoniterDlg::openDlgUserDef(UINT8 mode)
{
	UINT8 result = 0;
	result = getRegValue(REG_USER_DEF, REG_USER_DEF_NUM);
	if (result)
	{
		//return result;
	}
	m_CDialog_UserDef.m_UINT8_OpenMode = 1;
	result = m_CDialog_UserDef.DoModal();

	if (result == IDCANCEL)
		return IDCANCEL;
	if (result == IDOK)
	{
		result = setUserDef();
		if (result)
			return result;
	}
	return 0;
}

//函数 写入CAN通讯参数 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setCan()
{
	m_CEdit_setCanAddress.GetWindowTextW(m_CString_setCanAddress);
	g_CDriverParam_Main.m_UINT8_setCanAddress = _ttoi(m_CString_setCanAddress);
	g_CDriverParam_Main.m_UINT8_setCanBaudrate = m_CComboBox_setCanBaudrate.GetCurSel();

	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setCanBaudrate);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_setCanAddress);
	UINT8 result = setRegValue(REG_CAN, REG_CAN_NUM);
	return result;
	
}
//函数 写入电机参数 RAM //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setMotorPoles()
{
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setPolePairs);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_setEncoderType);
	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 12, g_CDriverParam_Main.m_INT32_setRatedSpeed);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 16, g_CDriverParam_Main.m_UINT8_setHallOrder);
	
	UINT8 result = setRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
	return result;
}
//函数 写入应用参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setApplication()
{
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, (g_CDriverParam_Main.m_UINT8_setMotionDirection << 0x00) | (g_CDriverParam_Main.m_UINT8_setInitialPosition << 0x01));
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT32_setAppMaxSpeed);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_INT32_setAcceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_INT32_setDeceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_INT32_setStopDeceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT32_setAppMinPosition);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 33, g_CDriverParam_Main.m_INT32_setInitialPosition);

	UINT8 result = setRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
	return result;
}
//函数 写入电流环参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setCurrentLoop()
{
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);

	UINT8 result = setRegValue(REG_CURRENTLOOP_P, REG_CURRENTLOOP_NUM);
	return result;
}
//函数 写入速度环参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setVelocityLoop()
{
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);

	UINT8 result = setRegValue(REG_VELOCITYLOOP_P, REG_VELOCITYLOOP_NUM);
	return result;
}
//函数 写入速度环参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setPositionLoop()
{
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);

	UINT8 result = setRegValue(REG_POSITIONLOOP_P, REG_POSITIONLOOP_NUM);
	return result;
}
//函数 写入电流环测试参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setCurrentTest()
{
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setCurrentTestType);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setCurrentTestAmplitude.param_int);
	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_UINT16_setCurrentTestFrequency);

	UINT8 result = setRegValue(REG_CURRENTLOOP_TEST, REG_CURRENTLOOP_TEST_NUM);
	return result;
}
//函数 写入速度环测试参数 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setVelocityTest()
{
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_INT32_setVelocityTestVelocity);
	
	UINT8 result = setRegValue(REG_VELOCITYLOOP_TEST_VELOCITY, 4);
	return result;
}
//函数 读取电流环测试数据 RAM
//0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::getCurrentTestData()
{
	UINT8 result = getRegValue(REG_CURRENTLOOP_TEST_LENGTH, 2);
	if(result)
		return result;

	m_UINT16_TestDataIndex = 0;
	m_UINT16_TestDataLength = g_CDriverParam_Main.m_UINT16_getCurrentTestLength;

	while (m_UINT16_TestDataLength)
	{
		if (m_UINT16_TestDataLength > 128)
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, 128);
			if (result)
				return result;
		}
		else
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, (UINT8)m_UINT16_TestDataLength);
			if (result)
				return result;
		}
	}
	return 0;
}

//0 处理正常 F0 无响应 F0 - FA 访问寄存器错误
UINT8 CHMMoniterDlg::getVelocityTestData()
{
	UINT8 result = getRegValue(REG_VELOCITYLOOP_TEST_LENGTH, 2);
	if (result)
		return result;

	m_UINT16_TestDataIndex = 0;
	m_UINT16_TestDataLength = g_CDriverParam_Main.m_UINT16_getVelocityTestLength;

	while (m_UINT16_TestDataLength)
	{
		if (m_UINT16_TestDataLength > 128)
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, 128);
			if (result)
				return result;
		}
		else
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, (UINT8)m_UINT16_TestDataLength);
			if (result)
				return result;
		}
	}
	return 0;
}

UINT8 CHMMoniterDlg::getPositionTestData()
{
	UINT8 result = getRegValue(REG_POSITIONLOOP_TEST_LENGTH, 2);
	if (result)
		return result;

	m_UINT16_TestDataIndex = 0;
	m_UINT16_TestDataLength = g_CDriverParam_Main.m_UINT16_getPositionTestLength;

	while (m_UINT16_TestDataLength)
	{
		if (m_UINT16_TestDataLength > 128)
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, 128);
			if (result)
				return result;
		}
		else
		{
			result = getRegValue(REG_LOOP_TESTDATA + m_UINT16_TestDataIndex, (UINT8)m_UINT16_TestDataLength);
			if (result)
				return result;
		}
	}
	return 0;
}

//函数 设定模式 返回 setRegValue的处理结果 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setWorkMode()
{	
	m_UINT8_setWorkMode = m_CComboBox_setWorkMode.GetCurSel();

	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_UINT8_setWorkMode);
	
	UINT8 result = setRegValue(REG_RUN_MODE, 1);
	return result;
}
//函数 设定电流模式 返回 setRegValue的处理结果 //0 0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setCurrentMode(INT8 Direction)
{
	// TODO: 在此处添加实现代码.
	//memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));

	m_CEdit_setCurrent.GetWindowTextW(m_CString_setCurrent);

	//判断电流是否超限
	m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setCurrent, &m_CString_isLegalMessage,
		-g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float);
	if (m_UINT8_isLegalValue == 2)
	{
		AfxMessageBox(_T("设定电流，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
		return 1;
	}

	m_unParam_setCurrent.param_float = FLOAT(_ttof(m_CString_setCurrent)) * Direction;

	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH, m_unParam_setCurrent.param_int);

	UINT8 result = setRegValue(REG_RUN_CURRENT, 4);
	return result;
}
//函数 设定速度模式 返回 setRegValue的处理结果 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setSpeedMode(INT8 Direction)
{
	// TODO: 在此处添加实现代码.
	//memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));

	m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
	m_CEdit_setRunAcceleration.GetWindowTextW(m_CString_setRunAcceleration);

//	if (m_CButton_toggleUnit.GetCheck())
//	{
		//设定速度 非法检查
	m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setSpeed, &m_CString_isLegalMessage, -g_CDriverParam_Main.m_INT32_setAppMaxSpeed, g_CDriverParam_Main.m_INT32_setAppMaxSpeed);

		if (m_UINT8_isLegalValue == 2)
		{
			AfxMessageBox(_T("设定速度，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
			return 1;
		}
//	}
	//else
	//{
	//	m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setSpeed, &m_CString_isLegalMessage,-g_CDriverParam_Main.m_INT32_setAppMaxVelocity,	g_CDriverParam_Main.m_INT32_setAppMaxVelocity);

	//	if (m_UINT8_isLegalValue == 2)
	//	{
	//		AfxMessageBox(_T("设定速度，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
	//		return 1;
	//	}
	//}

//	if (m_CButton_toggleUnit.GetCheck())
//	{
		m_FLOAT_setSpeed = (FLOAT)_ttof(m_CString_setSpeed) * Direction;
		m_INT32_setRunAcceleration = _ttoi(m_CString_setRunAcceleration);

		//g_CDriverParam_Main.toggleSpeedToVelocityUnit(&m_FLOAT_setSpeed, &m_INT32_setSpeed);

		m_INT32_setSpeed = (INT32)m_FLOAT_setSpeed;
		//m_FLOAT_setSpeed = m_FLOAT_setSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
		//if (m_FLOAT_setSpeed >= 0)
		//{
		//	m_FLOAT_setSpeed = m_FLOAT_setSpeed + 0.5F;
		//}
		//else
		//	m_FLOAT_setSpeed = m_FLOAT_setSpeed - 0.5F;

		//m_INT32_setSpeed = (INT32)m_FLOAT_setSpeed;
//	}
	//else
	//{
	//	m_INT32_setSpeed = _ttoi(m_CString_setSpeed);
	//}

	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_INT32_setSpeed);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_INT32_setRunAcceleration);

	UINT8 result = setRegValue(REG_RUN_VELOCITY, 8);
	return result;
}
//函数 设定位置模式 返回 setRegValue的处理结果 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setPositionMode(INT8 Direction)
{
	m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
	m_CEdit_setAngle.GetWindowTextW(m_CString_setAngle);
	m_CEdit_setPosition.GetWindowTextW(m_CString_setPosition);
	m_CEdit_setRunAcceleration.GetWindowTextW(m_CString_setRunAcceleration);

	//设定速度 非法检查
//	if (m_CButton_toggleUnit.GetCheck())
//	{
		m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setSpeed, &m_CString_isLegalMessage, -g_CDriverParam_Main.m_INT32_setAppMaxSpeed, g_CDriverParam_Main.m_INT32_setAppMaxSpeed/*
			-g_CDriverParam_Main.m_INT32_setAppMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution,
			g_CDriverParam_Main.m_INT32_setAppMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution*/);
		if (m_UINT8_isLegalValue == 2)
		{
			AfxMessageBox(_T("设定速度，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
			return 1;
		}
//	}
	//else
	//{
	//	m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setSpeed, &m_CString_isLegalMessage, -g_CDriverParam_Main.m_INT32_setAppMaxVelocity, g_CDriverParam_Main.m_INT32_setAppMaxVelocity);

	//	if (m_UINT8_isLegalValue == 2)
	//	{
	//		AfxMessageBox(_T("设定速度，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
	//		return 1;
	//	}
	//}
	//设定位置 非法检查
		m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&m_CString_setAngle, &m_CString_isLegalMessage, -g_CDriverParam_Main.m_FLOAT_setAppMinAngle, g_CDriverParam_Main.m_FLOAT_setAppMaxAngle);
	if (m_UINT8_isLegalValue == 2)
	{
		AfxMessageBox(_T("设定位置，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
		return 1;
	}

	//if (m_CButton_toggleUnit.GetCheck())
	//{
	//	m_FLOAT_setSpeed = (FLOAT)_ttof(m_CString_setSpeed);

	//	g_CDriverParam_Main.toggleSpeedToVelocityUnit(&m_FLOAT_setSpeed, &m_INT32_setPositionSpeed);
	//	m_FLOAT_setSpeed = m_FLOAT_setSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
	//	if (m_FLOAT_setSpeed >= 0)
	//	{
	//		m_FLOAT_setSpeed = m_FLOAT_setSpeed + 0.5F;
	//	}
	//	else
	//		m_FLOAT_setSpeed = m_FLOAT_setSpeed - 0.5F;

	//	m_INT32_setSpeed = (INT32)m_FLOAT_setSpeed;
	//}
	//else
	//{
	//	m_INT32_setPositionSpeed = _ttoi(m_CString_setSpeed);
	//}

	m_INT32_setPositionSpeed = _ttoi(m_CString_setSpeed);
	m_UINT8_setPositionType = m_CComboBox_setPositionType.GetCurSel();
	m_INT32_setPosition = _ttoi(m_CString_setPosition) * Direction;
	m_INT32_setRunAcceleration = _ttoi(m_CString_setRunAcceleration);
	
	
	//if (m_CButton_toggleUnit.GetCheck())
	//{
	//	m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
	//	m_FLOAT_setSpeed = (FLOAT)_ttof(m_CString_setSpeed);

	//	m_FLOAT_setSpeed = m_FLOAT_setSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
	//	if (m_FLOAT_setSpeed >= 0)
	//	{
	//		m_FLOAT_setSpeed = m_FLOAT_setSpeed + 0.5F;
	//	}
	//	else
	//		m_FLOAT_setSpeed = m_FLOAT_setSpeed - 0.5F;

	//	m_INT32_setPositionSpeed = (INT32)m_FLOAT_setSpeed;
	//}

	//else
	//{
	//	m_CEdit_setSpeed.GetWindowTextW(m_CString_setSpeed);
	//	m_INT32_setPositionSpeed = _ttoi(m_CString_setSpeed);
	//}
	
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_UINT8_setPositionType);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, m_INT32_setPosition);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, m_INT32_setPositionSpeed);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, m_INT32_setRunAcceleration);

	UINT8 result = setRegValue(REG_RUN_POSITION, 13);
	return result;
}
//函数 设定命令 返回 setRegValue的处理结果 //0 处理正常 F0 无响应 F0-FA 访问寄存器错误
UINT8 CHMMoniterDlg::setCommand(UINT16 command)
{
	//memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));

	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH, command);
	UINT8 result = setRegValue(REG_COMMAND, 2);
	return result;
}
//函数 设定自定义参数
UINT8 CHMMoniterDlg::setUserDef()
{
	for (UINT8 i = 0; i < 8; i++)
	{
		g_CDriverParam_Main.BitToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_UserDef + i, i);
		g_CDriverParam_Main.BitToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_UserDef + 8 + i, i);
		g_CDriverParam_Main.BitToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT8_UserDef + 16 + i, i);
		g_CDriverParam_Main.BitToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_UINT8_UserDef + 24 + i, i);
	}

	for (UINT8 i = 0; i < 24; i++)
	{
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4 + 4 * i, g_CDriverParam_Main.m_INT32_UserDef[i]);
	}

	UINT8 result = setRegValue(REG_USER_DEF, REG_USER_DEF_NUM);
	return result;
}
//

//消息 开始 电机匹配
afx_msg LRESULT CHMMoniterDlg::OnUserMotorMatch(WPARAM wParam, LPARAM lParam)
{
	//
		//g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setPolePairs);
		//g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_setEncoderType);
		//g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
		//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
		//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
		//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 12, g_CDriverParam_Main.m_INT32_setRatedSpeed);
	UINT8 result = 0;

	result = setMotorPoles();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setCommand(COMMAND_MOTOR_MATCH);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	SetTimer(TIMER_ID_MATCHFLAG, 500, NULL);

	return 0;
}
//消息 取消 电机匹配
afx_msg LRESULT CHMMoniterDlg::OnUserMotorNomatch(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = setCommand(COMMAND_MOTOR_NOMATCH);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}
	return 0;
}
//消息 开始 电流环测试
afx_msg LRESULT CHMMoniterDlg::OnUserCurrentloopTest(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = 0;
	result = setCurrentLoop();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setCurrentTest();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setCommand(COMMAND_CURRENT_TEST_BG);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	//AfxMessageBox(_T("开始测试"));

	SetTimer(TIMER_ID_CURRENTTEST, 500, NULL);

	//电流环测试的结果 在 定时器的消息响应函数里面
	return 0;
}
//消息 取消 电流环测试
afx_msg LRESULT CHMMoniterDlg::OnUserCurrentloopNotest(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = setCommand(COMMAND_CURRENT_TEST_ST);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}
	return 0;
}
//消息 开始 速度环测试
afx_msg LRESULT CHMMoniterDlg::OnUserVelocityloopTest(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = 0;
	result = setVelocityLoop();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setVelocityTest();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setCommand(COMMAND_VELOCITY_TEST_BG);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}
	//AfxMessageBox(_T("开始测试"));

	SetTimer(TIMER_ID_VELOCITYTEST, 500, NULL);

	//电流环测试的结果 在 定时器的消息响应函数里面
	return 0;
}
//消息 取消 速度环测试
afx_msg LRESULT CHMMoniterDlg::OnUserVelocityloopNotest(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = setCommand(COMMAND_VELOCITY_TEST_ST);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	return 0;

}
//消息 开始 位置环测试
afx_msg LRESULT CHMMoniterDlg::OnUserPositionloopTest(WPARAM wParam, LPARAM lParam)
{
	UINT8 result = 0;
	result = setPositionLoop();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setPositionTest();
	if (result)
	{
		processErrorCode(result);
		return 1;
	}

	result = setCommand(COMMAND_POSITION_TEST_BG);
	if (result)
	{
		processErrorCode(result);
		return 1;
	}
	//AfxMessageBox(_T("开始测试"));

	SetTimer(TIMER_ID_POSITIONTEST, 500, NULL);

	//电流环测试的结果 在 定时器的消息响应函数里面
	return 0;
}

//消息 定时器 状态更新 处理
void CHMMoniterDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UINT8 result = 0;

	if (g_CSPortComm_Main.CommOpened)
	{
		switch (nIDEvent)
		{
		case TIMER_ID_STATUS:

			result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_STATUS_NUM);
			if (result)
			{
				KillTimer(TIMER_ID_STATUS);
				AfxMessageBox(_T("设备无响应，请重新连接……"));
				//processErrorCode(result);
			}
			break;

		case TIMER_ID_REAL:
			if (g_CDriverParam_Main.m_CString_getDeviceType == "ACS306")
				result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM + 4);				//关节模组驱动器 多读一个 关节位置寄存器
			else
				result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);
			if (result)
			{
				KillTimer(TIMER_ID_REAL);
				m_CButton_getStatus.SetCheck(FALSE);
				processErrorCode(result);
			}
			break;

		case TIMER_ID_MATCHFLAG:

			result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
			if (result)
			{
				KillTimer(TIMER_ID_MATCHFLAG);
				processErrorCode(result);
			}
			else
			{
				if (g_CDriverParam_Main.m_UINT8_getMatchFlag)
					KillTimer(TIMER_ID_MATCHFLAG);
			}

			break;

		case TIMER_ID_CURRENTTEST:

			result = getRegValue(REG_CURRENTLOOP_TEST_FLAG, 1);
			if (result)
			{
				KillTimer(TIMER_ID_CURRENTTEST);
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));

				processErrorCode(result);
			}

			else
			{
				if (g_CDriverParam_Main.m_UINT8_getCurrentTestResult == 03)
				{
					KillTimer(TIMER_ID_CURRENTTEST);
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，正在读取数据……"));

					result = getCurrentTestData();
					if(result)
					{
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDABORT);
					}
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDOK);
				}
			}
			break;
		case TIMER_ID_VELOCITYTEST:

			result = getRegValue(REG_VELOCITYLOOP_TEST_FLAG, 1);
			if(result)
			{
				KillTimer(TIMER_ID_VELOCITYTEST);
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));
				processErrorCode(result);
			}
			else
			{
				if (g_CDriverParam_Main.m_UINT8_getVelocityTestResult == 03)
				{
					KillTimer(TIMER_ID_VELOCITYTEST);
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，正在读取数据……"));

					result = getVelocityTestData();
					if (result)
					{
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDABORT);
					}
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDOK);
				}
			}
			break;

		case TIMER_ID_POSITIONTEST:

			result = getRegValue(REG_POSITIONLOOP_TEST_FLAG, 1);
			if (result)
			{
				KillTimer(TIMER_ID_POSITIONTEST);
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
				m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));
				processErrorCode(result);
			}
			else
			{
				if (g_CDriverParam_Main.m_UINT8_getPositionTestResult == 03)
				{
					KillTimer(TIMER_ID_POSITIONTEST);
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，正在读取数据……"));

					result = getPositionTestData();
					if (result)
					{
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试出错，请关闭……"));
						m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDABORT);
					}
					m_CDialog_FileNewPID.m_CDialog_FileLoopFlag.EndDialog(IDOK);
				}
			}
			break;

		default:
			break;
		}
	}
	CDialogEx::OnTimer(nIDEvent);
}
//按键 绝对位置校准
void CHMMoniterDlg::OnBnClickedButtoncalibrateposition()
{
	// TODO: 在此添加控件通知处理程序代码

	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}

	if (g_CDriverParam_Main.m_UINT8_getStatusDevice != 1)
	{
		AfxMessageBox(_T("设备非空闲状态"));
		return;
	}

	CString temp;
	GetDlgItemTextW(IDC_EDIT_setPositionCalibration, temp);

	m_UINT8_isLegalValue = g_CDriverParam_Main.isLegalString(&temp, &m_CString_isLegalMessage, g_CDriverParam_Main.m_INT32_setAppMinPosition, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	if (m_UINT8_isLegalValue == 2)
	{
		AfxMessageBox(_T("位置校准，输入超限\n") + m_CString_isLegalMessage, MB_ICONERROR);
		return ;
	}
	
	m_INT32_setPositionCalibration = _ttoi(temp);

	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH, m_INT32_setPositionCalibration);
	UINT8 result = setRegValue(REG_DEVICE_REAL + 4, 4);
	if (result)
		processErrorCode(result);

	if (g_CDriverParam_Main.m_CString_getDeviceType == "ACS306")
		result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM + 4);				//关节模组驱动器 多读一个 关节位置寄存器
	else
		result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);
	if (result)
		processErrorCode(result);
}

UINT8 CHMMoniterDlg::setPositionTest()
{
	// TODO: 在此处添加实现代码.
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_INT32_setPositionTestAcceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_INT32_setPositionTestDeceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_INT32_setPositionTestSpeed);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 12, g_CDriverParam_Main.m_INT32_setPositionTestPosition);

	UINT8 result = setRegValue(REG_POSITIONLOOP_TEST, 16);
	return result;
}

void CHMMoniterDlg::OnBnClickedButtonTestwavedisplay()
{

}

void CHMMoniterDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	GetClientRect(&m_CRect_MainClient);
	m_StatusBar_Main.MoveWindow(0, m_CRect_MainClient.bottom - 30, m_CRect_MainClient.right, 30);


}

//按键 搜索串口
void CHMMoniterDlg::OnBnClickedButtonSeachusablecomm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened) {

		CString str;

		m_CComboBox_UsableSerialPort.ResetContent();

		g_CSPortComm_Main.SearchComm();

		if (!g_CSPortComm_Main.CommQuantity)
		{
			m_CComboBox_UsableSerialPort.AddString(_T("无"));
		}
		else {

			for (UINT i = 0; i < g_CSPortComm_Main.CommQuantity; i++) {
				str.Format(_T("COM%d"), g_CSPortComm_Main.CommArray[i]);
				m_CComboBox_UsableSerialPort.AddString(str);
			}
		}
		m_CComboBox_UsableSerialPort.SetCurSel(0);
	}
}

//按键 打开关闭串口
void CHMMoniterDlg::OnBnClickedButtonOpencomm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened) 
	{

		g_CSPortComm_Main.CommDCB.BaudRate = 115200;
		g_CSPortComm_Main.CommDCB.ByteSize = 8;
		g_CSPortComm_Main.CommDCB.Parity = 0;
		g_CSPortComm_Main.CommDCB.StopBits = 0;

		if (!g_CSPortComm_Main.OpenComm(m_CComboBox_UsableSerialPort.GetCurSel()))
		{
			AfxMessageBox(_T("打开失败！"));
			return;
		}
		g_CSPortComm_Main.CommOpened = TRUE;
		//	m_CSPortComm_Main = g_CSPortComm_Main;
		m_CButton_SeachUsableComm.EnableWindow(FALSE);
		m_CButton_OpenComm.SetWindowTextW(_T("关 闭"));
		//CString str;
		//str.Format(_T("COM%d,%d,%d,%d,%d"), g_CSPortComm_Main.CommArray[m_CComboBox_CommNumber.GetCurSel()], g_CSPortComm_Main.CommDCB.BaudRate, g_CSPortComm_Main.CommDCB.ByteSize, g_CSPortComm_Main.CommDCB.Parity, g_CSPortComm_Main.CommDCB.StopBits);
		//AfxMessageBox(str);

		CommProcessThread = CreateThread(NULL, 0, DataProcessThread, this, 0, NULL);		//开启接收线程
		//m_CSPortComm_Main = g_CSPortComm_Main;

		//Sleep(1000);

		//读取 设备信息
		 UINT8 result = getRegValue(REG_DEVICE_INFO, 34);
		 if (result)
		 {
			 processErrorCode(result);
			 return;
		 }


		//读取 设备状态
		g_CDriverParam_Main.m_UINT8_getStatusFlag = 0;

		if (g_CDriverParam_Main.m_CString_getDeviceType == "ACS306")
			result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM + 4);				//关节模组驱动器 多读一个 关节位置寄存器
		else
			result = getRegValue(REG_DEVICE_STATUS, REG_DEVICE_REAL_NUM + REG_DEVICE_STATUS_NUM);

		if (result)
		{
			processErrorCode(result);
			return;
		}
		
		result = getRegValue(REG_CAN, REG_CAN_NUM);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		//读取 设备运行模式
		result = getRegValue(REG_RUN_MODE, 18);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		//读取 电机默认参数
		result = getRegValue(REG_MOTOR_POLES, REG_MOTOR_POLES_NUM);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		//读取 匹配参数
		result = getRegValue(REG_MATCH_FLAG, REG_MATCH_FLAG_NUM);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		////读取 应用参数
		result = getRegValue(REG_APPLICATION, REG_APPLICATION_NUM);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		result = getRegValue(REG_DEVICE_STATIC, REG_DEVICE_STATIC_NUM);
		if (result)
		{
			processErrorCode(result);
			return;
		}

		SetDlgItemTextW(IDC_EDIT_setPositionCalibration, _T("0"));
		SetTimer(TIMER_ID_STATUS, 500, NULL);
	}
	else
	{
		g_CSPortComm_Main.CommOpened = FALSE;

		//if (m_CButton_getStatus.GetCheck())
		//{		
		//	KillTimer(TIMER_ID_STATUS);
		//	KillTimer(TIMER_ID_REAL);
		//	m_CButton_getStatus.SetCheck(FALSE);
		//}
		DWORD aa = WaitForSingleObject(CommProcessThread, 1000);


		if (aa != WAIT_OBJECT_0)
		{
			AfxMessageBox(_T("关闭失败"));
			return;
		}
		else
		{
			if (g_CSPortComm_Main.CloseComm() == FALSE)
			{
				AfxMessageBox(_T("关闭失败"));
				return;
			}
			else
			{
				m_CButton_SeachUsableComm.EnableWindow(TRUE);
				m_CButton_OpenComm.SetWindowTextW(_T("打 开"));
			}
		}
	}
}

//按键 设置CAN参数
void CHMMoniterDlg::OnBnClickedButtonsetcanparam()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened)
	{
		AfxMessageBox(_T("串口未打开"));
		return;
	}
	UINT8 result = setCan();
	if (result >= 0xF0)
	{
		processErrorCode(result);
		return;
	}
}

void CHMMoniterDlg::OnBnClickedButtonsavetestdata()
{
	// TODO: 在此添加控件通知处理程序代码

	CTime m_time = CTime::GetCurrentTime();
	CString m_strDateTime = m_time.Format(_T("%Y%m%d%H%M"));
	//m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	//m_strDate = m_time.Format(_T("%x"));          //格式化日期  
	//m_strTime = m_time.Format(_T("%X"));          //格式化时间  
	//m_strDateTime = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));   //格式化日期时间

	CString filename = _T(".\\TestData" + m_strDateTime + ".txt");//在本例中路径为:E:\work\BruceXu\CodeRelevant\mytestcode\读写txt\ReadWriteTxt\ReadWriteTxt\Test.txt
	CString strWriteData;
	strWriteData.Format(_T("111111\n"));
	CStdioFile csFile;
	CFileException cfException;
	if (csFile.Open(filename, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite, &cfException))//以txt方式读取|若没有文件则创建该文件
	{
		csFile.SeekToEnd();
		csFile.WriteString(strWriteData);
	}
	csFile.Close();

}

void CHMMoniterDlg::OnEnKillfocusEditsetangle()
{
	// TODO: 在此添加控件通知处理程序代码
	m_CEdit_setAngle.GetWindowTextW(m_CString_setAngle);
	m_FLOAT_setAngle = _ttof(m_CString_setAngle);
	m_INT32_setPosition = m_FLOAT_setAngle * 22444.0f;
	m_CString_setPosition.Format(_T("%d"), m_INT32_setPosition);
	m_CEdit_setPosition.SetWindowTextW(m_CString_setPosition);		
}