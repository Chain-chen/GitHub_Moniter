// CDlgFileNewDriver.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileNewDriver.h"
#include "afxdialogex.h"
#include "CSPortComm.h"
#include "CDriverParam.h"

extern CDriverParam g_CDriverParam_Main;

extern CSPortComm g_CSPortComm_Main;
extern HANDLE g_HANDLE_CommResponce;
extern BYTE g_BYTE_NewDriverDataRxArray[256];

// CDlgFileNewDriver 对话框

IMPLEMENT_DYNAMIC(CDlgFileNewDriver, CDialogEx)

CDlgFileNewDriver::CDlgFileNewDriver(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_DRIVER, pParent)
	//, m_unParam_setAppRatedCurrent({ 0 })
	//, m_unParam_setAppStallCurrent({ 0 })
	//, m_INT_setAppMaxVelocity(0)
	//, m_UINT_setAcceleration(0)
	//, m_UINT_setDeceleration(0)
	//, m_UINT_setStopDeceleration(0)
	//, m_INT_setAppMaxPosition(0)
	//, m_INT_setAppMinPosition(0)
	//, m_INT_setMotionDirection(0)

	//, m_unParam_getDriverRatedCurrent({ 0 })
	//, m_unParam_getDriverStallCurrent({ 0 })
	//, m_INT_getDriverMaxVelocity(0)
	//, m_INT_getDriverMinVelocity(0)
	//, m_unParam_getDriverMaxVoltage({ 0 })
	//, m_unParam_getDriverMinVoltage({ 0 })

{
}

CDlgFileNewDriver::~CDlgFileNewDriver()
{
}

void CDlgFileNewDriver::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	//DDX_Text(pDX, IDC_EDIT_setAppRatedCurrent, m_unParam_setAppRatedCurrent.param_float);
	//DDX_Text(pDX, IDC_EDIT_setAppStallCurrent, m_unParam_setAppStallCurrent.param_float);
	//DDX_Text(pDX, IDC_EDIT_setAppMaxVelocity, m_INT_setAppMaxVelocity);
	//DDX_Text(pDX, IDC_EDIT_setAcceleration, m_UINT_setAcceleration);
	//DDX_Text(pDX, IDC_EDIT_setDeceleration, m_UINT_setDeceleration);
	//DDX_Text(pDX, IDC_EDIT_setStopDeceleration, m_UINT_setStopDeceleration);
	//DDX_Text(pDX, IDC_EDIT_setAppMaxPosition, m_INT_setAppMaxPosition);
	//DDX_Text(pDX, IDC_EDIT_setAppMinPosition, m_INT_setAppMinPosition);
	//DDX_CBIndex(pDX, IDC_COMBO_setMotionDirection, m_INT_setMotionDirection);

	//DDX_Text(pDX, IDC_EDIT_getDriverRatedCurrent, m_unParam_getDriverRatedCurrent.param_float);
	//DDX_Text(pDX, IDC_EDIT_getDriverStallCurrent, m_unParam_getDriverStallCurrent.param_float);
	//DDX_Text(pDX, IDC_EDIT_getDriverMaxVelocity, m_INT_getDriverMaxVelocity);
	//DDX_Text(pDX, IDC_EDIT_getDriverMinVelocity, m_INT_getDriverMinVelocity);
	//DDX_Text(pDX, IDC_EDIT_getDriverMaxVoltage, m_unParam_getDriverMaxVoltage.param_float);
	//DDX_Text(pDX, IDC_EDIT_getDriverMinVoltage, m_unParam_getDriverMinVoltage.param_float);
	DDX_Control(pDX, IDC_EDIT_setAppRatedCurrent, m_CEdit_setAppRatedCurrent);
	DDX_Control(pDX, IDC_EDIT_setAppStallCurrent, m_CEdit_setAppStallCurrent);
	//DDX_Control(pDX, IDC_EDIT_setAppMaxVelocity, m_CEdit_setAppMaxVelocity);
	DDX_Control(pDX, IDC_EDIT_setAppMaxVelocity, m_CEdit_setAppMaxSpeed);
	DDX_Control(pDX, IDC_EDIT_setAcceleration, m_CEdit_setAcceleration);
	DDX_Control(pDX, IDC_EDIT_setDeceleration, m_CEdit_setDeceleration);
	DDX_Control(pDX, IDC_EDIT_setStopDeceleration, m_CEdit_setStopDeceleration);
	DDX_Control(pDX, IDC_EDIT_setAppMaxPosition, m_CEdit_setAppMaxPosition);
	DDX_Control(pDX, IDC_EDIT_setAppMinPosition, m_CEdit_setAppMinPosition);
	DDX_Control(pDX, IDC_EDIT_getDriverRatedCurrent, m_CEdit_getDriverRatedCurrent);
	DDX_Control(pDX, IDC_EDIT_getDriverStallCurrent, m_CEdit_getDriverStallCurrent);
	DDX_Control(pDX, IDC_EDIT_getDriverMaxVelocity, m_CEdit_getDriverMaxVelocity);
	DDX_Control(pDX, IDC_EDIT_getDriverMinVelocity, m_CEdit_getDriverMinVelocity);
	DDX_Control(pDX, IDC_EDIT_getDriverMaxVoltage, m_CEdit_getDriverMaxVoltage);
	DDX_Control(pDX, IDC_EDIT_getDriverMinVoltage, m_CEdit_getDriverMinVoltage);
	DDX_Control(pDX, IDC_COMBO_setMotionDirection, m_CComboBox_setMotionDirection);
	DDX_Control(pDX, IDC_EDIT_setInitialPosition, m_CEdit_setInitialPosition);
	DDX_Control(pDX, IDC_CHECK_setInitialPosition, m_CButton_setInitialPosition);
}


BEGIN_MESSAGE_MAP(CDlgFileNewDriver, CDialogEx)
	ON_MESSAGE(WM_USER_FILENEWDRIVER_RX, &CDlgFileNewDriver::OnUserFilenewdriverRx)

	ON_BN_CLICKED(IDC_BUTTON_FileDriverPreviousStep, &CDlgFileNewDriver::OnBnClickedButtonFiledriverpreviousstep)
	ON_BN_CLICKED(IDC_BUTTON_FileDriverNextStep, &CDlgFileNewDriver::OnBnClickedButtonFiledrivernextstep)
	ON_BN_CLICKED(IDC_BUTTON_FileDriverCancel, &CDlgFileNewDriver::OnBnClickedButtonFiledrivercancel)
END_MESSAGE_MAP()


// CDlgFileNewDriver 消息处理程序


BOOL CDlgFileNewDriver::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化

	if (m_BYTE_OpenMode)
	{
		SetWindowText(_T("应用参数-修改"));
		GetDlgItem(IDC_BUTTON_FileDriverNextStep)->SetWindowTextW(_T("保 存"));
		GetDlgItem(IDC_BUTTON_FileDriverPreviousStep)->ShowWindow(FALSE);
	}
	if (m_BYTE_setMotorEnabled)
	{
		GetDlgItem(IDC_BUTTON_FileDriverNextStep)->EnableWindow(FALSE);
	}

	if (m_BYTE_setGetStatus)
	{
		GetDlgItem(IDC_BUTTON_FileDriverNextStep)->EnableWindow(FALSE);
	}

	m_CEdit_setAppRatedCurrent.SetLimitText(16);
	m_CEdit_setAppStallCurrent.SetLimitText(16);
	//m_CEdit_setAppMaxVelocity.SetLimitText(16); 
	m_CEdit_setAppMaxSpeed.SetLimitText(16);
	m_CEdit_setAcceleration.SetLimitText(16);
	m_CEdit_setDeceleration.SetLimitText(16);
	m_CEdit_setStopDeceleration.SetLimitText(16);
	m_CEdit_setAppMaxPosition.SetLimitText(16);
	m_CEdit_setAppMinPosition.SetLimitText(16);

	m_CEdit_getDriverMinVoltage.SetLimitText(16);
	m_CEdit_getDriverMaxVoltage.SetLimitText(16);
	m_CEdit_getDriverRatedCurrent.SetLimitText(16);
	m_CEdit_getDriverStallCurrent.SetLimitText(16);
	m_CEdit_getDriverMinVelocity.SetLimitText(16);
	m_CEdit_getDriverMaxVelocity.SetLimitText(16);

	getApplication();

	m_UINT8_isParamChanged = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


//UINT8 CDlgFileNewDriver::getRegValue(WORD reg,BYTE reglength)
//{
//	// TODO: 在此处添加实现代码.
//	m_BYTE_DataTxArray[0] = 0x5A;
//	m_BYTE_DataTxArray[1] = 0xA5;
//
//	m_BYTE_DataTxArray[2] = 0x04;
//
//	m_BYTE_DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x01 & 0x07);
//	m_BYTE_DataTxArray[4] = reg >> 8;
//	m_BYTE_DataTxArray[5] = reg & 0xFF;
//	m_BYTE_DataTxArray[6] = reglength;
//
//	WORD CRCValue = g_CSPortComm_Main.CRC16(m_BYTE_DataTxArray, 0 + REG_INFIX_LENGTH);
//	m_BYTE_DataTxArray[REG_INFIX_LENGTH + 0] = (CRCValue & 0xFF);
//	m_BYTE_DataTxArray[REG_INFIX_LENGTH + 0 + 1] = (CRCValue >> 8);
//
//	g_CSPortComm_Main.SendData(m_BYTE_DataTxArray, REG_INFIX_LENGTH + 0 + 2);
//
//	DWORD ErrorFlag = WaitForSingleObject(g_HANDLE_CommResponce, 1000);
//	if (ErrorFlag != WAIT_OBJECT_0)
//	{
//		return 1;
//	}
//	else
//	{
//		processDataRxRead(reg);
//		//	SendMessageW(WM_USER_MONITER_RX, 0, 0);
//	}
//
//	return 0;
//}
//
//UINT8 CDlgFileNewDriver::setRegValue(WORD reg, BYTE reglength)
//{
//	// TODO: 在此处添加实现代码.
//	m_BYTE_DataTxArray[0] = 0x5A;
//	m_BYTE_DataTxArray[1] = 0xA5;
//
//	m_BYTE_DataTxArray[2] = 0x04 + reglength;
//
//	m_BYTE_DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + (0x02 & 0x07);
//	m_BYTE_DataTxArray[4] = reg >> 8;
//	m_BYTE_DataTxArray[5] = reg & 0xFF;
//	m_BYTE_DataTxArray[6] = reglength;
//
//	WORD CRCValue = g_CSPortComm_Main.CRC16(m_BYTE_DataTxArray, reglength + REG_INFIX_LENGTH);
//	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH] = (CRCValue & 0xFF);
//	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH + 1] = (CRCValue >> 8);
//
//	g_CSPortComm_Main.SendData(m_BYTE_DataTxArray, reglength + REG_INFIX_LENGTH + 2);
//
//	DWORD ErrorFlag = WaitForSingleObject(g_HANDLE_CommResponce, 1000);
//	if (ErrorFlag != WAIT_OBJECT_0)
//	{
//		return 1;
//	}
//	else
//	{
//		//processDataRx(reg);
//		//SendMessageW(WM_USER_MONITER_RX, COMMAND_MOTOR_ENABLE, 0);
//	}
//	return 0;
//
//}


afx_msg LRESULT CDlgFileNewDriver::OnUserFilenewdriverRx(WPARAM wParam, LPARAM lParam)
{
	//processDataRx(wParam);
	
	return 0;
}


UINT8 CDlgFileNewDriver::processDataRxRead(UINT16 reg_word)
{
	// TODO: 在此处添加实现代码.

	switch (reg_word)
	{
	case REG_APPLICATION:

		//m_INT_setMotionDirection = g_BYTE_NewDriverDataRxArray[REG_INFIX_LENGTH];
		//m_unParam_setAppRatedCurrent.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 1, 4);
		//m_unParam_setAppStallCurrent.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 5, 4);
		//m_INT_setAppMaxVelocity = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 9, 4);
		//m_UINT_setAcceleration = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 13, 4);
		//m_UINT_setDeceleration = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 17, 4);
		//m_UINT_setStopDeceleration = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 21, 4);
		//m_INT_setAppMaxPosition = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 25, 4);
		//m_INT_setAppMinPosition = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 29, 4);
		
		break;

	case REG_DEVICE_STATIC:




		//m_unParam_getDriverMinVoltage.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH, 4);
		//m_unParam_getDriverMaxVoltage.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 4, 4);
		//m_unParam_getDriverRatedCurrent.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 8, 4);
		//m_unParam_getDriverStallCurrent.param_int = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 12, 4);
		//m_INT_getDriverMinVelocity = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 16, 4);
		//m_INT_getDriverMaxVelocity = ArrayToHex(g_BYTE_NewDriverDataRxArray + REG_INFIX_LENGTH + 20, 4);
			   
		break;

	default:
		break;
	}

	return 0;
}

void CDlgFileNewDriver::OnBnClickedButtonFiledriverpreviousstep()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDABORT);
}

//按键 下一步 OR 保存到RAM
void CDlgFileNewDriver::OnBnClickedButtonFiledrivernextstep()
{
	// TODO: 在此添加控件通知处理程序代码
	if (setApplication())
		return;

	//g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int = m_unParam_setAppRatedCurrent.param_int;
	//g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int = m_unParam_setAppStallCurrent.param_int;
	//g_CDriverParam_Main.m_INT_setAppMaxVelocity = m_INT_setAppMaxVelocity;
	//g_CDriverParam_Main.m_UINT_setAcceleration = m_UINT_setAcceleration;
	//g_CDriverParam_Main.m_UINT_setDeceleration = m_UINT_setDeceleration;
	//g_CDriverParam_Main.m_UINT_setStopDeceleration = m_UINT_setStopDeceleration;
	//g_CDriverParam_Main.m_INT_setAppMaxPosition = m_INT_setAppMaxPosition;
	//g_CDriverParam_Main.m_INT_setAppMinPosition = m_INT_setAppMinPosition;
	//g_CDriverParam_Main.m_INT_setMotionDirection = m_INT_setMotionDirection;

	EndDialog(IDOK);
}


void CDlgFileNewDriver::OnBnClickedButtonFiledrivercancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}


void CDlgFileNewDriver::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_UINT8_isParamChanged)
		//CDialogEx::OnCancel();
	{
		BYTE result = AfxMessageBox(_T("参数未保存，确定取消？"), MB_ICONQUESTION | MB_OKCANCEL, 0);
		if (IDOK == result)
		{
			CDialogEx::OnCancel();
		}
	}
	else
		CDialogEx::OnCancel();
	//EndDialog(IDCANCEL);
	//return;
}

UINT8 CDlgFileNewDriver::setApplication()
{
	CString errorCString;
	UINT8 errorValue;

	//从文本框获取字符串
	m_CEdit_setAppRatedCurrent.GetWindowTextW(m_CString_setAppRatedCurrent);
	m_CEdit_setAppStallCurrent.GetWindowTextW(m_CString_setAppStallCurrent);
	m_CEdit_setAppMaxSpeed.GetWindowTextW(m_CString_setAppMaxSpeed);
	//m_CEdit_setAppMaxVelocity.GetWindowTextW(m_CString_setAppMaxVelocity);
	m_CEdit_setAcceleration.GetWindowTextW(m_CString_setAcceleration);
	m_CEdit_setDeceleration.GetWindowTextW(m_CString_setDeceleration);
	m_CEdit_setStopDeceleration.GetWindowTextW(m_CString_setStopDeceleration);
	m_CEdit_setAppMaxPosition.GetWindowTextW(m_CString_setAppMaxAngle);
	m_CEdit_setAppMinPosition.GetWindowTextW(m_CString_setAppMinAngle);
	m_CEdit_setInitialPosition.GetWindowTextW(m_CString_setInitialPosition);

	//非法及溢出检查
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAppRatedCurrent, &errorCString, 0x0000,
		min(g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_float, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float));
	if (errorValue == 2)
	{
		AfxMessageBox(_T("额定电流，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAppStallCurrent, &errorCString, 0x0000,
		min(g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_float, g_CDriverParam_Main.m_unParam_setStallCurrent.param_float));
	if (errorValue == 2)
	{
		AfxMessageBox(_T("堵转电流，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAppMaxSpeed, &errorCString, 0x0000, g_CDriverParam_Main.m_INT32_setRatedSpeed /*g_CDriverParam_Main.m_INT32_setRatedVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution*/);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("最大速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAcceleration, &errorCString, 0x0000, 0x7FFFFFFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("加速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setDeceleration, &errorCString, 0x0000, 0x7FFFFFFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("减速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setStopDeceleration, &errorCString, 0x0000, 0x7FFFFFFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("停止减速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAppMaxAngle, &errorCString, 0x0000, 0x0001FFFF /* 360.0 / 8080000*/);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("最大位置，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setAppMinAngle, &errorCString, 0x0000, _ttof(m_CString_setAppMaxAngle));
	if (errorValue == 2)
	{
		AfxMessageBox(_T("最小位置，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setInitialPosition, &errorCString, (INT32)0x80000000, 0x7FFFFFFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("初始位置，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}


	if (m_CComboBox_setMotionDirection.GetCurSel() == 0)
		g_CDriverParam_Main.m_UINT8_setMotionDirection = 0x00;
	else
		g_CDriverParam_Main.m_UINT8_setMotionDirection = 0x01;

	if (m_CButton_setInitialPosition.GetCheck())
		g_CDriverParam_Main.m_UINT8_setInitialPosition = 0x01;
	else
		g_CDriverParam_Main.m_UINT8_setInitialPosition = 0x00;


	g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float = (FLOAT)(_ttof(m_CString_setAppRatedCurrent));
	g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_float = (FLOAT)(_ttof(m_CString_setAppStallCurrent));
	m_FLOAT_setAppMaxSpeed = _ttof(m_CString_setAppMaxSpeed);
	if (m_FLOAT_setAppMaxSpeed > 0)
		m_FLOAT_setAppMaxSpeed += 0.5f;
	else
		m_FLOAT_setAppMaxSpeed -= 0.5f;
	//g_CDriverParam_Main.m_INT32_setAppMaxVelocity = (INT32)(m_FLOAT_setAppMaxSpeed / 60.0 * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution);
	g_CDriverParam_Main.m_INT32_setAppMaxSpeed = (INT32)m_FLOAT_setAppMaxSpeed;
	g_CDriverParam_Main.m_INT32_setAcceleration = _ttoi(m_CString_setAcceleration);
	g_CDriverParam_Main.m_INT32_setDeceleration = _ttoi(m_CString_setDeceleration);
	g_CDriverParam_Main.m_INT32_setStopDeceleration = _ttoi(m_CString_setStopDeceleration);
	g_CDriverParam_Main.m_FLOAT_setAppMaxAngle = _ttof(m_CString_setAppMaxAngle);
	g_CDriverParam_Main.m_FLOAT_setAppMinAngle = _ttof(m_CString_setAppMinAngle);
	g_CDriverParam_Main.m_INT32_setInitialPosition = _ttoi(m_CString_setInitialPosition);

	g_CDriverParam_Main.m_INT32_setAppMaxPosition = g_CDriverParam_Main.m_FLOAT_setAppMaxAngle /*/ 360.0 * 8080000*/;
	g_CDriverParam_Main.m_INT32_setAppMinPosition = g_CDriverParam_Main.m_FLOAT_setAppMinAngle /*/ 360.0 * 8080000*/;
	//g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setMotionDirection);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_INT32_setAcceleration);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_INT32_setDeceleration);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_INT32_setStopDeceleration);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	//g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT32_setAppMinPosition);

	////memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));
	////UpdateData(TRUE);
	////m_BYTE_DataTxArray[REG_INFIX_LENGTH] = m_INT_setMotionDirection;
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, m_unParam_setAppRatedCurrent.param_int, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, m_unParam_setAppStallCurrent.param_int, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, m_INT_setAppMaxVelocity, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, m_UINT_setAcceleration, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, m_UINT_setDeceleration, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, m_UINT_setStopDeceleration, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, m_INT_setAppMaxPosition, 4);
	////HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, m_INT_setAppMinPosition, 4);

	//if (setRegValue(REG_APPLICATION, 33) == 1)
	//{
	//	return 1;
	//}

	return 0;
}

UINT8 CDlgFileNewDriver::getApplication()
{
	//显示 驱动器参数

	m_CComboBox_setMotionDirection.SetCurSel(g_CDriverParam_Main.m_UINT8_setMotionDirection);

	m_CString_setAppRatedCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float);
	m_CEdit_setAppRatedCurrent.SetWindowTextW(m_CString_setAppRatedCurrent);

	m_CString_setAppStallCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_float);
	m_CEdit_setAppStallCurrent.SetWindowTextW(m_CString_setAppStallCurrent);

	m_CString_setAppMaxSpeed.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAppMaxSpeed);
	//m_CEdit_setAppMaxVelocity.SetWindowTextW(m_CString_setAppMaxVelocity);
	m_CEdit_setAppMaxSpeed.SetWindowTextW(m_CString_setAppMaxSpeed);

	m_CString_setAcceleration.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAcceleration);
	m_CEdit_setAcceleration.SetWindowTextW(m_CString_setAcceleration);

	m_CString_setDeceleration.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setDeceleration);
	m_CEdit_setDeceleration.SetWindowTextW(m_CString_setDeceleration);

	m_CString_setStopDeceleration.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setStopDeceleration);
	m_CEdit_setStopDeceleration.SetWindowTextW(m_CString_setStopDeceleration);

	m_CString_setAppMaxAngle.Format(_T("%g"), g_CDriverParam_Main.m_FLOAT_setAppMaxAngle);
	m_CEdit_setAppMaxPosition.SetWindowTextW(m_CString_setAppMaxAngle);

	m_CString_setAppMinAngle.Format(_T("%g"), g_CDriverParam_Main.m_FLOAT_setAppMinAngle);
	m_CEdit_setAppMinPosition.SetWindowTextW(m_CString_setAppMinAngle);

	m_CString_setInitialPosition.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setInitialPosition);
	m_CEdit_setInitialPosition.SetWindowTextW(m_CString_setInitialPosition);

	//显示 静态参数
	m_CString_getDriverMinVoltage.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getDriverMinVoltage.param_float);
	m_CEdit_getDriverMinVoltage.SetWindowTextW(m_CString_getDriverMinVoltage);

	m_CString_getDriverMaxVoltage.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getDriverMaxVoltage.param_float);
	m_CEdit_getDriverMaxVoltage.SetWindowTextW(m_CString_getDriverMaxVoltage);

	m_CString_getDriverRatedCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_float);
	m_CEdit_getDriverRatedCurrent.SetWindowTextW(m_CString_getDriverRatedCurrent);

	m_CString_getDriverStallCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_float);
	m_CEdit_getDriverStallCurrent.SetWindowTextW(m_CString_getDriverStallCurrent);

	m_CString_getDriverMinVelocity.Format(_T("%g"), g_CDriverParam_Main.m_FLOAT_getDriverMinSpeed);
	m_CEdit_getDriverMinVelocity.SetWindowTextW(m_CString_getDriverMinVelocity);

	m_CString_getDriverMaxVelocity.Format(_T("%g"), g_CDriverParam_Main.m_FLOAT_getDriverMaxSpeed);
	m_CEdit_getDriverMaxVelocity.SetWindowTextW(m_CString_getDriverMaxVelocity);

	return 0;
}




BOOL CDlgFileNewDriver::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}


BOOL CDlgFileNewDriver::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (HIWORD(wParam) == EN_CHANGE)
		m_UINT8_isParamChanged = 1;


	return CDialogEx::OnCommand(wParam, lParam);
}
