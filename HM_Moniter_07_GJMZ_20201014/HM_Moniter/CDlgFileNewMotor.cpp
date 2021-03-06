// CDlgFileNewMotor.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileNewMotor.h"
#include "afxdialogex.h"
#include "CSPortComm.h"
#include "CDriverParam.h"

extern CDriverParam g_CDriverParam_Main;

extern CSPortComm g_CSPortComm_Main;
extern WORD g_BYTE_CommResponceStatus;
extern BYTE g_BYTE_NewMotorDataRxArray[256];
extern HANDLE g_HANDLE_CommResponce;

// CDlgFileNewMotor 对话框

IMPLEMENT_DYNAMIC(CDlgFileNewMotor, CDialogEx)

CDlgFileNewMotor::CDlgFileNewMotor(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_MOTOR, pParent)
{
	m_CString_setRatedCurrent = _T("");
	m_CString_setStallCurrent = _T("");
	m_CString_setRatedSpeed = _T("");
	m_CString_setPolePairs = _T("");
	m_CString_setEncoderResolution = _T("");
}

CDlgFileNewMotor::~CDlgFileNewMotor()
{
}

void CDlgFileNewMotor::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_EDIT_setMotorRatedCurrent, m_CEdit_setRatedCurrent);
	DDX_Control(pDX, IDC_EDIT_setMotorStallCurrent, m_CEdit_setStallCurrent);
	DDX_Control(pDX, IDC_EDIT_setMotorRatedSpeed, m_CEdit_setRatedSpeed);
	DDX_Control(pDX, IDC_EDIT_setPolePairs, m_CEdit_setPolePairs);
	DDX_Control(pDX, IDC_COMBO_setEncorderType, m_CComboBox_setEncorderType);
	DDX_Control(pDX, IDC_EDIT_setEncorderResolution, m_CEdit_setEncoderResolution);
	DDX_Control(pDX, IDC_COMBO_getMotorMatchFlag, m_CComboBox_getMotorMatchFlag);
	DDX_Control(pDX, IDC_COMBO_setHallOrder, m_CComboBox_setHallOrder);
}


BEGIN_MESSAGE_MAP(CDlgFileNewMotor, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SelfMatch, &CDlgFileNewMotor::OnBnClickedButtonSelfmatch)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, &CDlgFileNewMotor::OnBnClickedButtonCancel)
//	ON_WM_TIMER()
	ON_MESSAGE(WM_USER_FILENEWMOTOR_RX, &CDlgFileNewMotor::OnUserFilenewmotorRx)
	ON_BN_CLICKED(IDC_BUTTON_FileMotorNextStep, &CDlgFileNewMotor::OnBnClickedButtonFilemotornextstep)

END_MESSAGE_MAP()


// CDlgFileNewMotor 消息处理程序

//消息 初始化
BOOL CDlgFileNewMotor::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
//	g_BYTE_CommResponceStatus = 0;
	if (m_BYTE_OpenMode)
	{
		SetWindowText(_T("电机参数 - 修改"));
		GetDlgItem(IDC_BUTTON_FileMotorNextStep)->SetWindowTextW(_T("保 存"));
	}
	if (m_BYTE_setMotorEnabled)
	{
		GetDlgItem(IDC_BUTTON_FileMotorNextStep)->EnableWindow(FALSE);
	}

	if (m_BYTE_setGetStatus)
	{
		GetDlgItem(IDC_BUTTON_FileMotorNextStep)->EnableWindow(FALSE);
	}

	m_CEdit_setPolePairs.SetLimitText(16);
	m_CEdit_setEncoderResolution.SetLimitText(16);
	m_CEdit_setRatedCurrent.SetLimitText(16);
	m_CEdit_setStallCurrent.SetLimitText(16);
	m_CEdit_setRatedSpeed.SetLimitText(16);



	//从全局变量显示电机各参数
	m_CString_setPolePairs.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setPolePairs);
	m_CEdit_setPolePairs.SetWindowTextW(m_CString_setPolePairs);
	m_CComboBox_setEncorderType.SetCurSel(g_CDriverParam_Main.m_UINT8_setEncoderType);
	m_CString_setEncoderResolution.Format(_T("%d"), g_CDriverParam_Main.m_UINT16_setEncoderResolution);
	m_CEdit_setEncoderResolution.SetWindowTextW(m_CString_setEncoderResolution);
	m_CString_setRatedCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float);
	m_CEdit_setRatedCurrent.SetWindowTextW(m_CString_setRatedCurrent);
	m_CString_setStallCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setStallCurrent.param_float);
	m_CEdit_setStallCurrent.SetWindowTextW(m_CString_setStallCurrent);

	//if (m_FLOAT_setRatedVelocity >= 0)
	//{
	//	m_FLOAT_setRatedSpeed = m_FLOAT_setRatedVelocity + 0.5F;
	//}
	//else
	//	m_FLOAT_setRatedSpeed = m_FLOAT_setRatedVelocity - 0.5F;
	m_CString_setRatedSpeed.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setRatedSpeed);
	m_CEdit_setRatedSpeed.SetWindowTextW(m_CString_setRatedSpeed);

	m_CComboBox_setHallOrder.SetCurSel(g_CDriverParam_Main.m_UINT8_setHallOrder - 1);
	m_CComboBox_getMotorMatchFlag.SetCurSel(g_CDriverParam_Main.m_UINT8_getMatchFlag);
	//if (getRegValue(REG_MOTOR_POLES, 16) == 1)
	//{
	//	AfxMessageBox(_T("设备无响应"));
	//	return TRUE;
	//}
	//if (getRegValue(REG_MATCH_FLAG, 7) == 1)
	//{
	//	AfxMessageBox(_T("设备无响应"));
	//	return TRUE;
	//}
	m_UINT8_isParamChanged = 0;
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//数据处理
//BYTE CDlgFileNewMotor::processDataRx(WORD wParam)
//{
//	BYTE FunctionCode = g_BYTE_NewMotorDataRxArray[3] & 0x07;
//	WORD RegAddr = g_BYTE_NewMotorDataRxArray[4] << 8 | g_BYTE_NewMotorDataRxArray[5];
//	switch (RegAddr)
//	{
//	case REG_MOTOR_POLES:
//
//		if (FunctionCode == 0x01)
//		{
//			//m_BYTE_setPolePairs = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH];
//			//m_INT_setEncoderType = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 1];
//			//m_UINT_setEncoderResolution = (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 2] << 8) | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 3] << 0);
//
//			m_unParam_setRatedCurrent.param_int = (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 4] << 24) | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 5] << 16)
//				| (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 6] << 8) | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 7] << 0);
//			m_unParam_setStallCurrent.param_int = (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 8] << 24) | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 9] << 16)
//				| (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 10] << 8) | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 11] << 0);
//			
//			//ArrayToHex(g_BYTE_NewMotorDataRxArray + 7 + 12, &m_UINT_setRatedVelocity, 4);
//
//			//if (m_INT_setEncoderType == 0)
//			//{
//			//	m_UINT_setRatedSpeed = m_UINT_setRatedVelocity / m_BYTE_setPolePairs * 10;
//			//	//m_UINT_setRatedVelocity = m_UINT_setRatedSpeed * m_BYTE_setPolePairs / 10;
//
//			//}
//			//else
//			//{
//			//	m_UINT_setRatedSpeed = m_UINT_setRatedVelocity / m_UINT_setEncoderResolution * 15;
//
//
//			//	//m_UINT_setRatedVelocity = m_UINT_setEncoderResolution * m_UINT_setRatedSpeed / 15;
//
//			//}						
//		}
//		else if(FunctionCode == 0x02)
//		{
//
//
//		}
//		break;
//
//	case REG_MATCH_FLAG:
//		if (FunctionCode == 0x01)
//		{
//			
//			
//			m_BYTE_getMatchFlag = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH];
//			m_BYTE_getPhaseOrder = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 1];
//			m_BYTE_getHallOrder = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 2];
//			m_BYTE_getHallDirection = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 3];
//			m_BYTE_getEncoderDirection = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 4];
//			m_WORD_getDefaultDegree = (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 5]) << 8 | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 6]);
//			
//			if (m_BYTE_getMatchFlag == 0x01) {
//				KillTimer(2);
//				m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDOK;			//1
//
//				if (m_CDialog_FileMotorMatch.m_hWnd != NULL) 
//				{
//					m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配完成，请关闭……！"));
//
//					m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
//				}
//				//m_BYTE_SelfMatched = 0;
//				//GetDlgItem(IDC_BUTTON_SelfMatch)->SetWindowTextW(_T("开始匹配"));
//				m_INT_getMotorMatchFlag = 1;
//				//AfxMessageBox(_T("匹配完成"));
//
//			}
//			else if(m_BYTE_getMatchFlag == 0x02)
//			{	
//				KillTimer(2);
//				m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDABORT;		//3
//				//m_BYTE_SelfMatched = 0;
//				if (m_CDialog_FileMotorMatch.m_hWnd != NULL)
//				{
//					m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配失败，请关闭……！"));
//					m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
//
//				}
//				m_INT_getMotorMatchFlag = 0;
//			}
//			else
//			{
//			}		
//		}
//
//		else if (FunctionCode == 0x02)
//		{
//		}
//		else
//		{
//		}
//
//		break;	
//	}
//	UpdateData(FALSE);
//	return 0;
//}

//数据处理
UINT8 CDlgFileNewMotor::processDataRxRead(UINT16 regword)
{
	switch (regword)
	{
	case REG_MOTOR_POLES:

		//m_CString_setRatedCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float);
		//m_CEdit_setRatedCurrent.SetWindowTextW(m_CString_setRatedCurrent);

		//m_CString_setStallCurrent.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setStallCurrent.param_float);
		//m_CEdit_setStallCurrent.SetWindowTextW(m_CString_setStallCurrent);

		//g_CDriverParam_Main.m_FLOAT_setRateSpeed = g_CDriverParam_Main.m_INT32_setRatedSpeed * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
		//if (g_CDriverParam_Main.m_FLOAT_setRateSpeed >= 0)
		//{
		//	g_CDriverParam_Main.m_FLOAT_setRateSpeed = g_CDriverParam_Main.m_FLOAT_setRateSpeed + 0.5F;
		//}
		//else
		//	g_CDriverParam_Main.m_FLOAT_setRateSpeed = g_CDriverParam_Main.m_FLOAT_setRateSpeed - 0.5F;

		//m_CString_setRatedSpeed.Format(_T("%d"), (INT32)g_CDriverParam_Main.m_FLOAT_setRateSpeed);
		//m_CEdit_setRatedSpeed.SetWindowTextW(m_CString_setRatedSpeed);

		//m_CString_setPolePairs.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setPolePairs);
		//m_CEdit_setPolePairs.SetWindowTextW(m_CString_setPolePairs);

		//m_CComboBox_setEncorderType.SetCurSel(g_CDriverParam_Main.m_UINT8_setEncoderType);

		//m_CString_setEncoderResolution.Format(_T("%d"), g_CDriverParam_Main.m_UINT16_setEncoderResolution);
		//m_CEdit_setEncoderResolution.SetWindowTextW(m_CString_setEncoderResolution);
		//
		//break;

	case REG_MATCH_FLAG:			
			
		//m_CComboBox_getMotorMatchFlag.SetCurSel(g_CDriverParam_Main.m_UINT8_getMatchFlag);
			//m_BYTE_getMatchFlag = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH];
			//m_BYTE_getPhaseOrder = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 1];
			//m_BYTE_getHallOrder = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 2];
			//m_BYTE_getHallDirection = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 3];
			//m_BYTE_getEncoderDirection = g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 4];
			//m_WORD_getDefaultDegree = (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 5]) << 8 | (g_BYTE_NewMotorDataRxArray[REG_INFIX_LENGTH + 6]);
			
		if (g_CDriverParam_Main.m_UINT8_getMatchFlag == 0x01) {
			KillTimer(2);
			m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDOK;			//1

			if (m_CDialog_FileMotorMatch.m_hWnd != NULL) 
			{
				m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配完成，请关闭……！"));

				m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
			}
			//m_BYTE_SelfMatched = 0;
			//GetDlgItem(IDC_BUTTON_SelfMatch)->SetWindowTextW(_T("开始匹配"));
			//m_INT_getMotorMatchFlag = 1;
			//AfxMessageBox(_T("匹配完成"));

		}
		else if(g_CDriverParam_Main.m_UINT8_getMatchFlag == 0x02)
		{	
			KillTimer(2);
			m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDABORT;		//3
			//m_BYTE_SelfMatched = 0;
			if (m_CDialog_FileMotorMatch.m_hWnd != NULL)
			{
				m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("匹配失败，请关闭……！"));
				m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));

			}
			//m_INT_getMotorMatchFlag = 0;
		}
		else
		{
		}

		break;
	}
	return 0;
}

//UINT8 CDlgFileNewMotor::getRegValue(WORD reg, BYTE reglength)
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

//UINT8 CDlgFileNewMotor::setRegValue(WORD reg, BYTE reglength)
//{
//	m_BYTE_DataTxArray[0] = 0x5A;
//	m_BYTE_DataTxArray[1] = 0xA5;
//
//	m_BYTE_DataTxArray[2] = 0x04 + reglength;
//
//	m_BYTE_DataTxArray[3] = (0x00 & 0x80) + (0x00 & 0x40) + 0x02;
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
//}

//BYTE CDlgFileNewMotor::HexToArray(BYTE * pArray, UINT data, BYTE length)
//{
//	if (length == 4)
//	{
//		pArray[0] = (data >> 24) & 0xFF;
//		pArray[1] = (data >> 16) & 0xFF;
//		pArray[2] = (data >> 8) & 0xFF;
//		pArray[3] = (data >> 0) & 0xFF;
//		return 1;
//	}
//	else if (length == 2)
//	{
//		pArray[0] = (data >> 8) & 0xFF;
//		pArray[1] = (data >> 0) & 0xFF;
//
//		return 1;
//	}
//
//	else if (length == 1)
//	{
//		pArray[0] = data;
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//
//	return 0;
//}
//
//
//BYTE CDlgFileNewMotor::ArrayToHex(BYTE * pArray, UINT * pdata, BYTE length)
//{
//	if (length == 4)
//	{
//		*pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
//		return 1;
//	}
//	else if (length == 2)
//	{
//		*pdata = (pArray[0] << 8) | (pArray[1] << 0);
//
//		return 1;
//	}
//	else if (length == 1)
//	{
//		*pdata = (pArray[0] << 0);
//		return 1;
//	}
//	else
//	{
//		return 0;
//	}
//	return 0;
//}

//按键 匹配
void CDlgFileNewMotor::OnBnClickedButtonSelfmatch()
{


	if (m_BYTE_setMotorEnabled)
	{
		AfxMessageBox(_T("电机已使能"));
		return;
	}

	if (m_BYTE_setGetStatus)
	{
		AfxMessageBox(_T("设备正忙"));
		return;
	}


	if (setMotorPoles())
		return;


	CWnd * pwnd = this->GetParent();
	pwnd->SendMessage(WM_USER_MOTOR_MATCH, 0, 0);



//	// TODO: 在此添加控件通知处理程序代码
//	if (m_BYTE_setMotorEnabled)
//	{
//		AfxMessageBox(_T("电机已使能"));
//		return;
//
//	}
//	if (m_BYTE_setGetStatus)
//	{
//		AfxMessageBox(_T("设备正忙"));
//		return;
//	}
//
//	if (setMotorPoles() == 1)
//	{
//		return;
//	}
//
////	memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));
//
//	g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_MOTOR_MATCH);
//	
//	if (setRegValue(REG_COMMAND, 0x02) == 1)
//	{
//		AfxMessageBox(_T("设备无响应"));
//		return;
//	}
//
//	SetTimer(2, 500, NULL);
//	m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDCANCEL;			//2
//	BYTE MatchResult = m_CDialog_FileMotorMatch.DoModal();
//	if (IDCANCEL == MatchResult)									//2 取消
//	{
//		KillTimer(2);
//		
//		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_MOTOR_NOTMATCH);
//
//		if (setRegValue(REG_COMMAND, 0x02) == 1)
//		{
//			AfxMessageBox(_T("设备无响应"));
//			return;
//		}
//
//	}
//	else if (IDOK == MatchResult)								//1 匹配完成
//	{
//	}
//	else
//	{
//	}
	m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDCANCEL;

	//	if (IDCANCEL == MatchResult)									//2 取消
//	{
//		KillTimer(2);
//		
//		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_MOTOR_NOTMATCH);
//
//		if (setRegValue(REG_COMMAND, 0x02) == 1)
//		{
//			AfxMessageBox(_T("设备无响应"));
//			return;
//		}
//



	UINT8 result = m_CDialog_FileMotorMatch.DoModal();
	if (IDCANCEL == result)						//2 取消
	{
//		KillTimer(2);
//		
//		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_MOTOR_NOTMATCH);
//
//		if (setRegValue(REG_COMMAND, 0x02) == 1)
//		{
//			AfxMessageBox(_T("设备无响应"));
//			return;
		pwnd->SendMessage(WM_USER_MOTOR_NOMATCH, 0, 0);
	}

	else if (result == IDOK)
	{
		m_CComboBox_getMotorMatchFlag.SetCurSel(1);
	}
	else 
		m_CComboBox_getMotorMatchFlag.SetCurSel(0);


}



//按键 取消
void CDlgFileNewMotor::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

//按键 下一步OR保存
void CDlgFileNewMotor::OnBnClickedButtonFilemotornextstep()
{
	// TODO: 在此添加控件通知处理程序代码
	//if (m_INT_setEncoderType == 0)
	//{
	//	m_FLOAT_VeloctiyUnitScale = 10.0f / m_BYTE_setPolePairs;			//	p/s = 10/pole * r/m
	//}
	//else
	//{
	//	m_FLOAT_VeloctiyUnitScale = 15.0f / m_UINT_setEncoderResolution;	//	p/s = 15.0/reso * r/m
	//}


	if (setMotorPoles())
		return;

		//memset(m_BYTE_DataTxArray, 0, sizeof(m_BYTE_DataTxArray));
	   	 
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_BYTE_getMatchFlag, 1);
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, m_BYTE_getPhaseOrder, 1);
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, m_BYTE_getHallOrder, 1);
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, m_BYTE_getHallDirection, 1);
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_BYTE_getEncoderDirection, 1);
	//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, m_WORD_getDefaultDegree, 2);

	//if (setRegValue(REG_MATCH_FLAG, 7) == 1)
	//	return;

	//g_CDriverParam_Main.m_BYTE_getMatchFlag = m_BYTE_getMatchFlag;
	//g_CDriverParam_Main.m_BYTE_getPhaseOrder = m_BYTE_getPhaseOrder;
	//g_CDriverParam_Main.m_BYTE_getHallOrder = m_BYTE_getHallOrder;
	//g_CDriverParam_Main.m_BYTE_getHallDirection = m_BYTE_getHallDirection;
	//g_CDriverParam_Main.m_BYTE_getEncoderDirection = m_BYTE_getEncoderDirection;
	//g_CDriverParam_Main.m_WORD_getDefaultDegree = m_WORD_getDefaultDegree;

	EndDialog(IDOK);
}

//void CDlgFileNewMotor::OnTimer(UINT_PTR nIDEvent)
//{
//	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	switch (nIDEvent) 
//	{
//
//	case 1:
////		g_BYTE_CommResponceStatus = 0xFFFF; KillTimer(1); break;
//	case 2:
//
//		if(getRegValue(REG_MATCH_FLAG, 0x07)==1)
//		{
//			KillTimer(2);
//			m_CDialog_FileMotorMatch.m_BYTE_MatchResult = IDRETRY;
//			m_CDialog_FileMotorMatch.m_CStatic_FileMotorMatch.SetWindowTextW(_T("设备无响应，请关闭……！"));
//			m_CDialog_FileMotorMatch.m_CButton_FileMotorMatch.SetWindowTextW(_T("关 闭"));
//			//AfxMessageBox(_T("设备无响应"));
//		}
//		break;
//
//	default:	break;
//	}
//
//	CDialogEx::OnTimer(nIDEvent);
//}


afx_msg LRESULT CDlgFileNewMotor::OnUserFilenewmotorRx(WPARAM wParam, LPARAM lParam)
{
	//processDataRx(wParam);

	memset(g_BYTE_NewMotorDataRxArray, 0x00, sizeof(g_BYTE_NewMotorDataRxArray));
	
	return 0;
}



void CDlgFileNewMotor::OnCancel()
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

UINT8 CDlgFileNewMotor::setMotorPoles()
{
	//从文本框获取字符串
	CString errorCString;
	UINT8 errorValue;
	m_CEdit_setPolePairs.GetWindowTextW(m_CString_setPolePairs);
	m_CEdit_setEncoderResolution.GetWindowTextW(m_CString_setEncoderResolution);
	m_CEdit_setRatedCurrent.GetWindowTextW(m_CString_setRatedCurrent);
	m_CEdit_setStallCurrent.GetWindowTextW(m_CString_setStallCurrent);
	m_CEdit_setRatedSpeed.GetWindowTextW(m_CString_setRatedSpeed);

	//非法及溢出检查 极对数 编码器线 额定电流 堵转电流
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setPolePairs, &errorCString, 0x00, 0xFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("极对数，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setEncoderResolution, &errorCString, 0x0000, 0xFFFF);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("编码器线，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	//errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setRatedCurrent, &errorCString, 0x0000, g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_float);
	//if (errorValue == 2)
	//{
	//	AfxMessageBox(_T("额定电流，输入超限\n") + errorCString, MB_ICONERROR);
	//	return 1;
	//}
	//errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setStallCurrent, &errorCString, 0x0000, g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_float);
	//if (errorValue == 2)
	//{
	//	AfxMessageBox(_T("堵转电流，输入超限\n") + errorCString, MB_ICONERROR);
	//	return 1;
	//}
	
	//从字符串获取电机参数值
	m_UINT8_setPolePairs = _ttoi(m_CString_setPolePairs);
	m_UINT8_setEncoderType = m_CComboBox_setEncorderType.GetCurSel();
	m_UINT16_setEncoderResolution = _ttoi(m_CString_setEncoderResolution);
	m_unParam_setRatedCurrent.param_float = (FLOAT)_ttof(m_CString_setRatedCurrent);
	m_unParam_setStallCurrent.param_float = (FLOAT)_ttof(m_CString_setStallCurrent);
	m_INT32_setRatedSpeed = _ttoi(m_CString_setRatedSpeed);

	//数据干涉检查 额定电流 堵转电流
	if (m_unParam_setStallCurrent.param_float < m_unParam_setRatedCurrent.param_float)
	{
		AfxMessageBox(_T("额定电流，输入超限\n允许范围 0 到 堵转电流"), MB_ICONERROR);
		return 1;
	}

	//根据电机计算 脉冲数每圈
	if (m_UINT8_setEncoderType == 0)
	{
		g_CDriverParam_Main.m_UINT32_setPulsePerRevolution = m_UINT8_setPolePairs * 6;
	}
	else
	{
		g_CDriverParam_Main.m_UINT32_setPulsePerRevolution = m_UINT16_setEncoderResolution * 4;
	}
	//非法及溢出检查 检查机械转速
	//errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setRatedSpeed, &errorCString, 0x00, g_CDriverParam_Main.m_INT32_getDriverMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution);
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setRatedSpeed, &errorCString, 0x00, g_CDriverParam_Main.m_FLOAT_getDriverMaxSpeed);

	if (errorValue == 2)
	{
		AfxMessageBox(_T("机械转速，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}

	//保存参数到全局
	g_CDriverParam_Main.m_UINT8_setPolePairs = m_UINT8_setPolePairs;
	g_CDriverParam_Main.m_UINT8_setEncoderType = m_UINT8_setEncoderType;
	g_CDriverParam_Main.m_UINT16_setEncoderResolution = m_UINT16_setEncoderResolution;
	g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int = m_unParam_setRatedCurrent.param_int;
	g_CDriverParam_Main.m_unParam_setStallCurrent.param_int = m_unParam_setStallCurrent.param_int;
	g_CDriverParam_Main.m_UINT8_setHallOrder = m_CComboBox_setHallOrder.GetCurSel() + 0x01;
	g_CDriverParam_Main.m_INT32_setRatedSpeed = m_INT32_setRatedSpeed;


	//m_FLOAT_setRatedSpeed = m_FLOAT_setRatedSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
	//if (m_FLOAT_setRatedSpeed >= 0)
	//{
	//	//g_CDriverParam_Main.m_FLOAT_setRateSpeed = g_CDriverParam_Main.m_FLOAT_setRateSpeed + 0.5F;
	//	g_CDriverParam_Main.m_INT32_setRatedVelocity = (INT32)(m_FLOAT_setRatedSpeed + 0.5F);
	//}
	//else
	//{	// g_CDriverParam_Main.m_FLOAT_setRateSpeed = g_CDriverParam_Main.m_FLOAT_setRateSpeed - 0.5F;
	//	g_CDriverParam_Main.m_INT32_setRatedVelocity = (INT32)(m_FLOAT_setRatedSpeed - 0.5F);
	//}
	   	  
	//errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setRatedCurrent, &errorCString, 0x00, 0xFF);
	//if (errorValue == 2)
	//	AfxMessageBox(_T("极对数，输入溢出\n") + errorCString, MB_ICONERROR);
	//errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setStallCurrent, &errorCString, 0x00, 0xFF);
	//if (errorValue == 2)
	//	AfxMessageBox(_T("极对数，输入溢出\n") + errorCString, MB_ICONERROR);


	//g_CDriverParam_Main.m_INT32_setRatedSpeed = (INT32)g_CDriverParam_Main.m_FLOAT_setRateSpeed;

	//判断变量是否超限	
	//m_UINT8_isOverLimit = g_CDriverParam_Main.isOverLimit(_T("额定电流"),g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float, 0.0F, g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_float);
	//if (m_UINT8_isOverLimit)
	//	return 1;
	//m_UINT8_isOverLimit = g_CDriverParam_Main.isOverLimit(_T("堵转电流"), g_CDriverParam_Main.m_unParam_setStallCurrent.param_float, 0.0F, g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_float);
	//if (m_UINT8_isOverLimit)
	//	return 1;
	//m_UINT8_isOverLimit = g_CDriverParam_Main.isOverLimit(_T("机械转速"), g_CDriverParam_Main.m_INT32_setRatedVelocity, 0, (INT32)(g_CDriverParam_Main.m_INT32_getDriverMaxVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution));
	//if (m_UINT8_isOverLimit)
	//	return 1;
	//m_UINT8_isOverLimit = g_CDriverParam_Main.isOverLimit(_T("极对数"), g_CDriverParam_Main.m_UINT8_setPolePairs, (UINT8)0, (UINT8)255);
	//if (m_UINT8_isOverLimit)
	//	return 1;
	//m_UINT8_isOverLimit = g_CDriverParam_Main.isOverLimit(_T("编码器线"), g_CDriverParam_Main.m_UINT16_setEncoderResolution, (UINT16)0, (UINT16)65535);
	//if (m_UINT8_isOverLimit)
	//	return 1;
	return 0;
}


//虚函数 消息解析
BOOL CDlgFileNewMotor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}


//void CDlgFileNewMotor::OnEnKillfocusEditsetmotorstallcurrent()
//{
//	// TODO: 在此添加控件通知处理程序代码
//
//	m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setStallCurrent, NULL, 0.0F, 20.0F, 8.0F);
//
//	if (m_UINT8_isLegalParam == 1)
//	{
//		AfxMessageBox(_T("输入非法"));
////		m_CEdit_setStallCurrent.SetFocus();
//	}
//	if (m_UINT8_isLegalParam == 2)
//	{
//		AfxMessageBox(_T("输入超限"));
//		m_CEdit_setStallCurrent.SetFocus();
//	}
//}


UINT8 CDlgFileNewMotor::isLegalParam()
{
	// TODO: 在此处添加实现代码.

	m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setRatedCurrent, _T("额定电流"), 0.0, g_CDriverParam_Main.m_unParam_getDriverRatedCurrent.param_float, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float);
	if ((m_UINT8_isLegalParam == 1) || (m_UINT8_isLegalParam == 2))
		return 1;
	if (m_UINT8_isLegalParam == 3)
		m_UINT8_isParamChanged = 1;

	m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setStallCurrent, _T("堵转电流"), 0.0F, g_CDriverParam_Main.m_unParam_getDriverStallCurrent.param_float, g_CDriverParam_Main.m_unParam_setStallCurrent.param_float);
	if ((m_UINT8_isLegalParam == 1) || (m_UINT8_isLegalParam == 2))
		return 1;
	if (m_UINT8_isLegalParam == 3)
		m_UINT8_isParamChanged = 1;

	//m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setRatedCurrent, _T("额定转速"), 0.0F, g_CDriverParam_Main.m_INT32_getDriverMaxVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution, g_CDriverParam_Main.m_FLOAT_setRateSpeed);
	//if ((m_UINT8_isLegalParam == 1) || (m_UINT8_isLegalParam == 2))
	//	return 1;
	//if (m_UINT8_isLegalParam == 3)
	//	m_UINT8_isParamChanged = 1;

	m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setPolePairs, _T("极对数"), 0.0F, 255.0F, g_CDriverParam_Main.m_UINT8_setPolePairs);
	if ((m_UINT8_isLegalParam == 1) || (m_UINT8_isLegalParam == 2))
		return 1;
	if (m_UINT8_isLegalParam == 3)
		m_UINT8_isParamChanged = 1;
	
	m_UINT8_isLegalParam = g_CDriverParam_Main.isLegalString(&m_CEdit_setEncoderResolution, _T("编码器线"), 0.0F,65535.0F, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
	if ((m_UINT8_isLegalParam == 1) || (m_UINT8_isLegalParam == 2))
		return 1;
	if (m_UINT8_isLegalParam == 3)
		m_UINT8_isParamChanged = 1;


	return 0;
}



BOOL CDlgFileNewMotor::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (HIWORD(wParam) == EN_CHANGE)
		m_UINT8_isParamChanged = 1;

	return CDialogEx::OnCommand(wParam, lParam);
}

