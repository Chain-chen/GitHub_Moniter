// CDlgFileNewPID.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileNewPID.h"
#include "afxdialogex.h"
#include "CSPortComm.h"
#include "CDriverParam.h"

extern CSPortComm g_CSPortComm_Main;
extern HANDLE g_HANDLE_CommResponce;
extern BYTE g_BYTE_NewPIDDataRxArray[256];
extern CDriverParam g_CDriverParam_Main;


// CDlgFileNewPID 对话框

IMPLEMENT_DYNAMIC(CDlgFileNewPID, CDialogEx)

CDlgFileNewPID::CDlgFileNewPID(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_LOOP, pParent)
	//, m_INT_setCurrentTestType(0)
	//, m_unParam_setCurrentTestAmplitude({0})
	//, m_UINT_setCurrentTestFrequency(0)
	//, m_INT_setVelocityTestVelocity(0)
{
}

CDlgFileNewPID::~CDlgFileNewPID()
{
}

void CDlgFileNewPID::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Text(pDX, IDC_EDIT_setCurrentLoopP, m_unParam_setCurrentLoopP.param_float);
	//DDX_Text(pDX, IDC_EDIT_setCurrentLoopI, m_unParam_setCurrentLoopI.param_float);
	//DDX_Text(pDX, IDC_EDIT_setCurrentLoopD, m_unParam_setCurrentLoopD.param_float);
	//DDX_Text(pDX, IDC_EDIT_setVelocityLoopP, m_unParam_setVelocityLoopP.param_float);
	//DDX_Text(pDX, IDC_EDIT_setVelocityLoopI, m_unParam_setVelocityLoopI.param_float);
	//DDX_Text(pDX, IDC_EDIT_setVelocityLoopD, m_unParam_setVelocityLoopD.param_float);
	//DDX_Text(pDX, IDC_EDIT_setPositionLoopP, m_unParam_setPositionLoopP.param_float);
	//DDX_Text(pDX, IDC_EDIT_setPositionLoopI, m_unParam_setPositionLoopI.param_float);
	//DDX_Text(pDX, IDC_EDIT_setPositionLoopD, m_unParam_setPositionLoopD.param_float);

	//DDX_CBIndex(pDX, IDC_COMBO_setCurrentTestType, m_INT_setCurrentTestType);
	//DDX_Text(pDX, IDC_EDIT_setCurrentTestAmplitude, m_unParam_setCurrentTestAmplitude.param_float);
	//DDX_Text(pDX, IDC_EDIT_setCurrentTestFrequency, m_UINT_setCurrentTestFrequency);
	//DDX_Text(pDX, IDC_EDIT_setVelocityTestVelocity, m_INT_setVelocityTestVelocity);
	DDX_Control(pDX, IDC_TCHART_LOOP, m_CTChart_Loop);
	DDX_Control(pDX, IDC_TCHART_LOOP2, m_CTChart_Loop1);

	DDX_Control(pDX, IDC_EDIT_setCurrentLoopP, m_CEdit_setCurrentLoopP);
	DDX_Control(pDX, IDC_EDIT_setCurrentLoopI, m_CEdit_setCurrentLoopI);
	DDX_Control(pDX, IDC_EDIT_setCurrentLoopD, m_CEdit_setCurrentLoopD);
	DDX_Control(pDX, IDC_EDIT_setVelocityLoopP, m_CEdit_setVelocityLoopP);
	DDX_Control(pDX, IDC_EDIT_setVelocityLoopI, m_CEdit_setVelocityLoopI);
	DDX_Control(pDX, IDC_EDIT_setVelocityLoopD, m_CEdit_setVelocityLoopD);
	DDX_Control(pDX, IDC_EDIT_setPositionLoopP, m_CEdit_setPositionLoopP);
	DDX_Control(pDX, IDC_EDIT_setPositionLoopI, m_CEdit_setPositionLoopI);
	DDX_Control(pDX, IDC_EDIT_setPositionLoopD, m_CEdit_setPositionLoopD);
	DDX_Control(pDX, IDC_COMBO_setCurrentTestType, m_CComboBox_setCurrentTestType);
	DDX_Control(pDX, IDC_EDIT_setCurrentTestAmplitude, m_CEdit_setCurrentTestAmplitude);
	DDX_Control(pDX, IDC_EDIT_setCurrentTestFrequency, m_CEdit_setCurrentTestFrequency);
	DDX_Control(pDX, IDC_EDIT_setVelocityTestVelocity, m_CEdit_setVelocityTestVelocity);
	//DDX_Control(pDX, IDC_CHECK_toggleVelocityTestVelocityUnit, m_CButton_toggleVelocityTestVelocityUnit);
	//DDX_Control(pDX, IDC_STATIC_setVelocityTestVelocityUnit, m_CStatic_setVelocityTestVelocityUnit);
	DDX_Control(pDX, IDC_EDIT_setPositionTestSpeed, m_CEdit_setPositionTestSpeed);
	DDX_Control(pDX, IDC_EDIT_setPositionTestPosition, m_CEdit_setPositionTestPosition);
	DDX_Control(pDX, IDC_EDIT_setPositionTestAcceleration, m_CEdit_setPositionTestAcceleration);
	DDX_Control(pDX, IDC_EDIT_setPositionTestDeceleration, m_CEdit_setPositionTestDeceleration);
	DDX_Control(pDX, IDC_STATIC_setPositionTestVelocityUnit, m_CStatic_setPositionTestVelocityUnit);
}


BEGIN_MESSAGE_MAP(CDlgFileNewPID, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_Cancel, &CDlgFileNewPID::OnBnClickedButtonCancel)
	ON_MESSAGE(WM_USER_FILENEWPID_RX, &CDlgFileNewPID::OnUserFilenewpidRx)
	ON_CBN_SELCHANGE(IDC_COMBO_setCurrentTestType, &CDlgFileNewPID::OnCbnSelchangeCombosetcurrenttesttype)
	ON_BN_CLICKED(IDC_BUTTON_setCurrentTest, &CDlgFileNewPID::OnBnClickedButtonsetcurrenttest)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_FileLoopPreviousStep, &CDlgFileNewPID::OnBnClickedButtonFilelooppreviousstep)
	ON_BN_CLICKED(IDC_BUTTON_FileLoopNextStep, &CDlgFileNewPID::OnBnClickedButtonFileloopnextstep)
	ON_BN_CLICKED(IDC_BUTTON_setVelocityTest, &CDlgFileNewPID::OnBnClickedButtonsetvelocitytest)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CHECK_toggleVelocityTestVelocityUnit, &CDlgFileNewPID::OnBnClickedChecktogglevelocitytestvelocityunit)
	ON_BN_CLICKED(IDC_BUTTON_setPositionTest, &CDlgFileNewPID::OnBnClickedButtonsetpositiontest)
	ON_BN_CLICKED(IDC_BUTTON_saveTestData, &CDlgFileNewPID::OnBnClickedButtonsavetestdata)
	ON_BN_CLICKED(IDC_BUTTON_saveTestData2, &CDlgFileNewPID::OnBnClickedButtonsavetestdata2)
	ON_BN_CLICKED(IDC_BUTTON_saveTestData3, &CDlgFileNewPID::OnBnClickedButtonsavetestdata3)
END_MESSAGE_MAP()

// CDlgFileNewPID 消息处理程序
BOOL CDlgFileNewPID::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	setTChartPosition();


	// TODO:  在此添加额外的初始化
	if (m_BYTE_OpenMode)
	{
		SetWindowText(_T("环路参数-修改"));
		GetDlgItem(IDC_BUTTON_FileLoopNextStep)->SetWindowTextW(_T("保 存"));
		GetDlgItem(IDC_BUTTON_FileLoopPreviousStep)->ShowWindow(FALSE);
	}

	if (m_BYTE_setMotorEnabled)
	{
		GetDlgItem(IDC_BUTTON_FileLoopNextStep)->EnableWindow(FALSE);
	}

	if (m_BYTE_setGetStatus)
	{
		GetDlgItem(IDC_BUTTON_FileLoopNextStep)->EnableWindow(FALSE);
	}

	GetDlgItem(IDC_EDIT_setCurrentTestFrequency)->EnableWindow(FALSE);

	m_CEdit_setCurrentLoopP.SetLimitText(16);
	m_CEdit_setCurrentLoopI.SetLimitText(16);
	m_CEdit_setCurrentLoopD.SetLimitText(16);
	m_CEdit_setVelocityLoopP.SetLimitText(16);
	m_CEdit_setVelocityLoopI.SetLimitText(16);
	m_CEdit_setVelocityLoopD.SetLimitText(16);
	m_CEdit_setPositionLoopP.SetLimitText(16);
	m_CEdit_setPositionLoopI.SetLimitText(16);
	m_CEdit_setPositionLoopD.SetLimitText(16);

	m_CEdit_setCurrentTestAmplitude.SetLimitText(16);
	m_CEdit_setCurrentTestFrequency.SetLimitText(16);

	m_CEdit_setVelocityTestVelocity.SetLimitText(16);

	m_CEdit_setPositionTestSpeed.SetLimitText(16);
	m_CEdit_setPositionTestPosition.SetLimitText(16);
	m_CEdit_setPositionTestAcceleration.SetLimitText(16);
	m_CEdit_setPositionTestDeceleration.SetLimitText(16);


	getCurrentLoop();
	getVelocityLoop();
	getPositionLoop();
	
//	m_CTChart_Loop.AddSeries(0);
//	m_CTChart_Loop.AddSeries(0);

	((CEnvironment)m_CTChart_Loop.get_Environment()).put_MouseWheelScroll(FALSE);
	((CEnvironment)m_CTChart_Loop1.get_Environment()).put_MouseWheelScroll(FALSE);


	m_CSeries_Data1 = m_CTChart_Loop.Series(0);
	m_CSeries_Data2 = m_CTChart_Loop.Series(1);
	m_CSeries_Data12 = m_CTChart_Loop.Series(2);

	m_CSeries_Data11 = m_CTChart_Loop1.Series(0);




	m_UINT8_isParamChanged = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//按键 保存or下一步
void CDlgFileNewPID::OnBnClickedButtonFileloopnextstep()
{
	// TODO: 在此添加控件通知处理程序代码
	
	if (setCurrentLoop())
		return;
	
	if (setVelocityLoop())
		return;

	if (setPositionLoop())
		return;

	EndDialog(IDOK);
}

//按键 取消
void CDlgFileNewPID::OnBnClickedButtonCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

afx_msg LRESULT CDlgFileNewPID::OnUserFilenewpidRx(WPARAM wParam, LPARAM lParam)
{
	//processDataRx();
	
	
	
	return 0;
}

BYTE CDlgFileNewPID::processDataRxRead(UINT16 regword)
{
	// TODO: 在此处添加实现代码.

	switch (regword)
	{
	case REG_CURRENTLOOP_P:



		//ArrayToHex(g_BYTE_NewPIDDataRxArray + REG_INFIX_LENGTH, &m_unParam_setCurrentLoopP.param_int, 4);
		//ArrayToHex(g_BYTE_NewPIDDataRxArray + REG_INFIX_LENGTH + 4, &m_unParam_setCurrentLoopI.param_int, 4);
		//ArrayToHex(g_BYTE_NewPIDDataRxArray + REG_INFIX_LENGTH + 8, &m_unParam_setCurrentLoopD.param_int, 4);

		break;

	case REG_VELOCITYLOOP_P:

		
		break;
	case REG_POSITIONLOOP_P:



	   break;
	case REG_CURRENTLOOP_TEST_FLAG:

		//if (ErrorCode)
		//{
		//	KillTimer(1);
		//	processErrorCode(ErrorCode);
		//	return 1;
		//}
		if (g_CDriverParam_Main.m_UINT8_getCurrentTestResult == 0x03)
		{
			KillTimer(1);
			m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDOK;
			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，请关闭!"));
			m_CDialog_FileLoopFlag.EndDialog(IDOK);
		}
		else if (g_CDriverParam_Main.m_UINT8_getCurrentTestResult == 0x06)
		{
			KillTimer(1);
			m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试失败，请关闭!"));
		}
		else
		{
		}
	
		break;
	case REG_CURRENTLOOP_TEST_LENGTH:
		//if (ErrorCode)
		//{
		//	processErrorCode(ErrorCode);
		//	return 1;
		//}

		m_WORD_LoopTestLength = g_CDriverParam_Main.m_UINT16_getCurrentTestLength;
		//ArrayToHex(g_BYTE_NewPIDDataRxArray + REG_INFIX_LENGTH, (DWORD*)&m_WORD_LoopTestLength, 2);

		break;

	case REG_VELOCITYLOOP_TEST_FLAG:
		//if (ErrorCode)
		//{
		//	KillTimer(2);
		//	processErrorCode(ErrorCode);
		//	return 1;
		//}

		if (g_CDriverParam_Main.m_UINT8_getVelocityTestResult == 0x03)
		{
			KillTimer(2);
			m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDOK;
			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试完成，请关闭!"));
			m_CDialog_FileLoopFlag.EndDialog(IDOK);
		}
		else if (g_CDriverParam_Main.m_UINT8_getVelocityTestResult == 0x06)
		{
			KillTimer(2);
			m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("测试失败，请关闭!"));
		}
		else
		{
		}

		break;
	case REG_VELOCITYLOOP_TEST_LENGTH:
		//if (ErrorCode)
		//{
		//	processErrorCode(ErrorCode);
		//	return 1;
		//}
		m_WORD_LoopTestLength = g_CDriverParam_Main.m_UINT16_getVelocityTestLength;

		break;

	default:

		if ((regword >= REG_LOOP_TESTDATA) && (regword <= REG_LOOP_TESTDATA + 0x2000))
		{
			//if (ErrorCode)
			//{
			//	processErrorCode(ErrorCode);
			//	return 1;
			//}

			if (m_WORD_LoopTestLength > 128)
			{
				memcpy(m_BYTE_pTestData + m_BYTE_TestDataIndex, g_CDriverParam_Main.m_UINT8_getLoopTestData, sizeof(BYTE) * 128);
				m_BYTE_TestDataIndex += 128;
				m_WORD_LoopTestLength -= 128;
			}
			else
			{
				memcpy(m_BYTE_pTestData + m_BYTE_TestDataIndex, g_CDriverParam_Main.m_UINT8_getLoopTestData, sizeof(BYTE) * m_WORD_LoopTestLength);
				m_BYTE_TestDataIndex += m_WORD_LoopTestLength;
				m_WORD_LoopTestLength -= m_WORD_LoopTestLength;
			}
		}
		else
		{
		}

	break;
	}
	//processErrorCode(ErrorCode);
	return 0;
}

//按键 电流环 测试 更改类型
void CDlgFileNewPID::OnCbnSelchangeCombosetcurrenttesttype()
{
	// TODO: 在此添加控件通知处理程序代码
	m_UINT8_setCurrentTestType = m_CComboBox_setCurrentTestType.GetCurSel();

	if (m_UINT8_setCurrentTestType)
	{
		GetDlgItem(IDC_EDIT_setCurrentTestFrequency)->EnableWindow(TRUE);
	}
	else
	{
		GetDlgItem(IDC_EDIT_setCurrentTestFrequency)->EnableWindow(FALSE);
	}
}



void CDlgFileNewPID::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	//switch (nIDEvent)
	//{
	//case 1:

	//	if (getRegValue(REG_CURRENTLOOP_TEST_FLAG, 1) == 1)
	//	{
	//		KillTimer(1);
	//		AfxMessageBox(_T("设备无响应"));
	//		m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
	//		m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("通讯错误，请关闭!"));

	//	}
	//	break;
	//case 2:

	//	if (getRegValue(REG_VELOCITYLOOP_TEST_FLAG, 1) == 1)
	//	{
	//		KillTimer(2);
	//		AfxMessageBox(_T("设备无响应"));
	//		m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDABORT;
	//		m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("通讯错误，请关闭!"));

	//	}

	//	break;
	//default:
	//	break;
	//}

	CDialogEx::OnTimer(nIDEvent);
}



void CDlgFileNewPID::OnBnClickedButtonFilelooppreviousstep()
{
	// TODO: 在此添加控件通知处理程序代码
	EndDialog(IDABORT);

}


void CDlgFileNewPID::OnCancel()
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



//按键 电流环 测试
void CDlgFileNewPID::OnBnClickedButtonsetcurrenttest()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_CDriverParam_Main.m_UINT8_getMatchFlag != 1)
	{
		AfxMessageBox(_T("电机未匹配"));
		return;
	}
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

	//写入 环路参数
	if (setCurrentLoop())
		return;
	if (setCurrentLoopTest())
		return;

	CWnd * pwnd = this->GetParent();
	pwnd->SendMessage(WM_USER_CURRENTLOOP_TEST, 0, 0);

	m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDCANCEL;
	//m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("正在测试，请等待……!"));
	UINT8 TestResult = m_CDialog_FileLoopFlag.DoModal();
	if (IDCANCEL == TestResult)
	{
		pwnd->SendMessage(WM_USER_CURRENTLOOP_NOTEST, 0, 0);
	}
	//测试完成
	else if (IDOK == TestResult)
	{
		//读取 测试数据
		//m_BYTE_TestDataIndex = 0;
		//m_BYTE_pTestData = new BYTE[m_WORD_LoopTestLength];
		//m_WORD_ChartDataLenth = m_WORD_LoopTestLength;
		//显示波形
		m_CSeries_Data1.Clear();
		m_CSeries_Data2.Clear();
		m_CSeries_Data11.Clear();
		m_CSeries_Data12.Clear();

		for (UINT16 i = 0; i < (g_CDriverParam_Main.m_UINT16_getCurrentTestLength >> 3); i++)
		{
			unParam temp1, temp2;
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 3), &temp1.param_int);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 3) + 4, &temp2.param_int);
			m_CSeries_Data1.Add(temp1.param_float, NULL, RGB(255, 0, 0));
			m_CSeries_Data2.Add(temp2.param_float, NULL, RGB(0, 255, 0));
		}
		//m_CDialog_FileLoopChart.DoModal();
	}
	else
	{
	}
}
//按键 速度环测试
void CDlgFileNewPID::OnBnClickedButtonsetvelocitytest()
{
	// TODO: 在此添加控件通知处理程序代码
		//写入 PID参数

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

	//写入 速度环
	if (setVelocityLoop())
		return;
	if (setVelocityLoopTest())
		return;

	CWnd * pwnd = this->GetParent();
	pwnd->SendMessage(WM_USER_VELOCITYLOOP_TEST, 0, 0);

	////SendMessageW(REG_COMMAND, COMMAND_MOTOR_MATCH, 0);
	//SetTimer(2, 500, NULL);
	m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDCANCEL;
	//					m_CDialog_FileLoopFlag.SetWindowTextW(_T("速度环-测试"));
				//			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("正在测试，请等待……!"));
	UINT8 TestResult = m_CDialog_FileLoopFlag.DoModal();
	if (IDCANCEL == TestResult)
	{
		//CWnd * pwnd = this->GetParent();
		pwnd->SendMessage(WM_USER_VELOCITYLOOP_NOTEST, 0, 0);
		//KillTimer(2);

		//g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_VELOCITY_TEST_ST);

		//if (setRegValue(REG_COMMAND, 0x02) == 1)
		//{
		//	AfxMessageBox(_T("设备无响应"));
		//	return;
		//}

	}
	//测试完成
	else if (IDOK == TestResult)
	{
		//if(getRegValue(REG_VELOCITYLOOP_TEST_LENGTH, 2)==1)
		//{
		//	AfxMessageBox(_T("设备无响应"));
		//	return;
		//}
		//	//读取测试数据
		//m_BYTE_TestDataIndex = 0;
		//m_BYTE_pTestData = new BYTE[m_WORD_LoopTestLength];
		//m_WORD_ChartDataLenth = m_WORD_LoopTestLength;
		//while (m_WORD_LoopTestLength)
		//{
		//	if (m_WORD_LoopTestLength > 128)
		//	{
		//		if (getRegValue(REG_LOOP_TESTDATA + m_BYTE_TestDataIndex, 128) == 1)
		//		{
		//			AfxMessageBox(_T("设备无响应"));
		//			return;
		//		}
		//	}
		//	else
		//	{
		//		if (getRegValue(REG_LOOP_TESTDATA + m_BYTE_TestDataIndex, (BYTE)m_WORD_LoopTestLength) == 1)
		//		{
		//			AfxMessageBox(_T("设备无响应"));
		//			return;
		//		}
		//显示波形
		m_CSeries_Data1.Clear();
		m_CSeries_Data2.Clear();
		m_CSeries_Data11.Clear();
		m_CSeries_Data12.Clear();

		for (WORD i = 0; i < (g_CDriverParam_Main.m_UINT16_getVelocityTestLength >> 4); i++)
		{
			INT temp_velocityrated, temp_velocityactual;
			UINT temp_focindex;
			unParam temp_currentactual;

			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4), &temp_velocityrated);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 4, &temp_velocityactual);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 8, &temp_focindex);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 12, &temp_currentactual.param_int);

			m_CSeries_Data1.AddXY(temp_focindex, temp_velocityrated, NULL, RGB(255, 0, 0));
			m_CSeries_Data2.AddXY(temp_focindex, temp_velocityactual, NULL, RGB(0, 255, 0));

			m_CSeries_Data11.AddXY(temp_focindex, temp_currentactual.param_float, NULL, RGB(255, 0, 0));
			//m_CSeries_Data12.AddXY(temp_focindex, temp_velocityactual, NULL, RGB(255, 0, 0));
		}
		//m_CDialog_FileLoopChart.DoModal();					
	}
	else
	{
	}
}
//按键 位置环测试
void CDlgFileNewPID::OnBnClickedButtonsetpositiontest()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
		//写入 PID参数

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

	//写入 位置环
	if (setPositionLoop())
		return;
	if (setPositionLoopTest())
		return;

	CWnd * pwnd = this->GetParent();
	pwnd->SendMessage(WM_USER_POSITIONLOOP_TEST, 0, 0);



	////写入 环路参数
	//if (setVelocityLoop() == 1)
	//{
	//	AfxMessageBox(_T("设备无响应"));
	//	return;
	//}

	//写入 测试参数

	//this->SendMessageW(WM_USER_FILENEWDRIVER_RX, REG_APPLICATION, 0);

	//启动 测试

	//g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_VELOCITY_TEST_BG);

	//if (setRegValue(REG_COMMAND, 0x02) == 1)
	//{
	//	AfxMessageBox(_T("设备无响应"));
	//	return;
	//}

	////SendMessageW(REG_COMMAND, COMMAND_MOTOR_MATCH, 0);
	//SetTimer(2, 500, NULL);
	m_CDialog_FileLoopFlag.m_BYTE_TestResult = IDCANCEL;
	//					m_CDialog_FileLoopFlag.SetWindowTextW(_T("速度环-测试"));
				//			m_CDialog_FileLoopFlag.m_CStatic_FileLoopFlag.SetWindowTextW(_T("正在测试，请等待……!"));
	UINT8 TestResult = m_CDialog_FileLoopFlag.DoModal();

	if (IDCANCEL == TestResult)
	{
		//CWnd * pwnd = this->GetParent();
		pwnd->SendMessage(WM_USER_POSITIONLOOP_TEST, 0, 0);


		//KillTimer(2);

		//g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_VELOCITY_TEST_ST);

		//if (setRegValue(REG_COMMAND, 0x02) == 1)
		//{
		//	AfxMessageBox(_T("设备无响应"));
		//	return;
		//}

	}
	//测试完成
	else if (IDOK == TestResult)
	{
		//if(getRegValue(REG_VELOCITYLOOP_TEST_LENGTH, 2)==1)
		//{
		//	AfxMessageBox(_T("设备无响应"));
		//	return;
		//}
		//	//读取测试数据
		//m_BYTE_TestDataIndex = 0;
		//m_BYTE_pTestData = new BYTE[m_WORD_LoopTestLength];
		//m_WORD_ChartDataLenth = m_WORD_LoopTestLength;
		//while (m_WORD_LoopTestLength)
		//{
		//	if (m_WORD_LoopTestLength > 128)
		//	{
		//		if (getRegValue(REG_LOOP_TESTDATA + m_BYTE_TestDataIndex, 128) == 1)
		//		{
		//			AfxMessageBox(_T("设备无响应"));
		//			return;
		//		}
		//	}
		//	else
		//	{
		//		if (getRegValue(REG_LOOP_TESTDATA + m_BYTE_TestDataIndex, (BYTE)m_WORD_LoopTestLength) == 1)
		//		{
		//			AfxMessageBox(_T("设备无响应"));
		//			return;
		//		}
		//显示波形
		m_CSeries_Data1.Clear();
		m_CSeries_Data2.Clear();
		m_CSeries_Data11.Clear();		
		//m_CTChart_Loop.AddSeries(0);
		//CSeries m_CSeries_Data3 = m_CTChart_Loop.Series(2);
		m_CSeries_Data12.Clear();
		for (WORD i = 0; i < (g_CDriverParam_Main.m_UINT16_getPositionTestLength >> 4); i++)
		{
			INT32 temp_positionrated, temp_positionactual;
			UINT32 temp_focindex;
			INT32 temp_velocityactual;

			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4), &temp_positionrated);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 4, &temp_positionactual);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 8, &temp_focindex);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 12, &temp_velocityactual);

			m_CSeries_Data1.AddXY(temp_focindex, temp_positionrated, NULL, RGB(255, 0, 0));
			m_CSeries_Data2.AddXY(temp_focindex, temp_positionactual, NULL, RGB(0, 255, 0));
			m_CSeries_Data12.AddXY(temp_focindex, temp_positionactual - temp_positionrated, NULL, RGB(0, 0, 255));
			m_CSeries_Data11.AddXY(temp_focindex, temp_velocityactual, NULL, RGB(255, 0, 0));
			//m_CSeries_Data12.AddXY(temp_focindex, temp_velocityactual, NULL, RGB(255, 0, 0));
		}
		//m_CDialog_FileLoopChart.DoModal();					
	}
	else
	{
	}
}

BYTE CDlgFileNewPID::setTChartPosition()
{
	// TODO: 在此处添加实现代码.

	CWnd *pCWndFileLoopChartDisplay = GetDlgItem(IDC_STATIC_FileLoopChartDisplay);

	if (pCWndFileLoopChartDisplay)
	{

		GetDlgItem(IDC_STATIC_FileLoopChartDisplay)->GetWindowRect(m_CRect_StaticChartDisplay);

		ScreenToClient(m_CRect_StaticChartDisplay);

		m_CTChart_Loop.GetWindowRect(m_CRect_Chart);
		m_CTChart_Loop1.GetWindowRect(m_CRect_Chart1);


		m_CRect_Chart.left = m_CRect_StaticChartDisplay.left + 10;
		m_CRect_Chart.top = m_CRect_StaticChartDisplay.top + 15;
		m_CRect_Chart.right = m_CRect_StaticChartDisplay.right - 10;
		m_CRect_Chart.bottom = (m_CRect_StaticChartDisplay.top + m_CRect_StaticChartDisplay.bottom) / 2 + 10;

		m_CRect_Chart1.left = m_CRect_StaticChartDisplay.left + 10;
		m_CRect_Chart1.top = (m_CRect_StaticChartDisplay.top + m_CRect_StaticChartDisplay.bottom) / 2 + 10;
		m_CRect_Chart1.right = m_CRect_StaticChartDisplay.right - 10;
		m_CRect_Chart1.bottom = m_CRect_StaticChartDisplay.bottom - 10;

		m_CTChart_Loop.MoveWindow(m_CRect_Chart);
		m_CTChart_Loop1.MoveWindow(m_CRect_Chart1);
	}

	return 0;
}
void CDlgFileNewPID::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	setTChartPosition();

}


//写入电流环PID值
UINT8 CDlgFileNewPID::setCurrentLoop()
{
	m_CEdit_setCurrentLoopP.GetWindowTextW(m_CString_setCurrentLoopP);
	m_CEdit_setCurrentLoopI.GetWindowTextW(m_CString_setCurrentLoopI);
	m_CEdit_setCurrentLoopD.GetWindowTextW(m_CString_setCurrentLoopD);

	m_FLOAT_setCurrentLoopP = (FLOAT)(_ttof(m_CString_setCurrentLoopP));
	m_FLOAT_setCurrentLoopI = (FLOAT)(_ttof(m_CString_setCurrentLoopI));
	m_FLOAT_setCurrentLoopD = (FLOAT)(_ttof(m_CString_setCurrentLoopD));

	if (m_FLOAT_setCurrentLoopP < 0) 
	{
		AfxMessageBox(_T("电流环P值，输入超限\n允许范围 大于0"));
		return 1;
	}
	if (m_FLOAT_setCurrentLoopI < 0)
	{
		AfxMessageBox(_T("电流环I值，输入超限\n允许范围 大于0"));
		return 1;

	}
	if (m_FLOAT_setCurrentLoopD < 0)
	{
		AfxMessageBox(_T("电流环D值，输入超限\n允许范围 大于0"));
		return 1;
	}

	g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_float = m_FLOAT_setCurrentLoopP;
	g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_float = m_FLOAT_setCurrentLoopI;
	g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_float = m_FLOAT_setCurrentLoopD;

	return 0;
}
//写入电流环测试值
UINT8 CDlgFileNewPID::setCurrentLoopTest()
{	
	CString errorCString;
	UINT8 errorValue;

	m_CEdit_setCurrentTestAmplitude.GetWindowTextW(m_CString_setCurrentTestAmplitude);
	m_CEdit_setCurrentTestFrequency.GetWindowTextW(m_CString_setCurrentTestFrequency);

	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setCurrentTestAmplitude, &errorCString, 0x0000,g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("电流幅值，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}

	g_CDriverParam_Main.m_UINT8_setCurrentTestType = m_CComboBox_setCurrentTestType.GetCurSel();
	g_CDriverParam_Main.m_unParam_setCurrentTestAmplitude.param_float = (FLOAT)(_ttof(m_CString_setCurrentTestAmplitude));
	g_CDriverParam_Main.m_UINT16_setCurrentTestFrequency = _ttoi(m_CString_setCurrentTestFrequency);

	return 0;
}

//写入速度环PID值
UINT8 CDlgFileNewPID::setVelocityLoop()
{
	m_CEdit_setVelocityLoopP.GetWindowTextW(m_CString_setVelocityLoopP);
	m_CEdit_setVelocityLoopI.GetWindowTextW(m_CString_setVelocityLoopI);
	m_CEdit_setVelocityLoopD.GetWindowTextW(m_CString_setVelocityLoopD);

	m_FLOAT_setVelocityLoopP = (FLOAT)(_ttof(m_CString_setVelocityLoopP));
	m_FLOAT_setVelocityLoopI = (FLOAT)(_ttof(m_CString_setVelocityLoopI));
	m_FLOAT_setVelocityLoopD = (FLOAT)(_ttof(m_CString_setVelocityLoopD));

	if (m_FLOAT_setVelocityLoopP < 0)
	{
		AfxMessageBox(_T("速度环P值，输入超限\n允许范围 大于0"));
		return 1;
	}
	if (m_FLOAT_setVelocityLoopI < 0)
	{
		AfxMessageBox(_T("速度环I值，输入超限\n允许范围 大于0"));
		return 1;

	}
	if (m_FLOAT_setVelocityLoopD < 0)
	{
		AfxMessageBox(_T("速度环D值，输入超限\n允许范围 大于0"));
		return 1;
	}

	g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_float = m_FLOAT_setVelocityLoopP;
	g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_float = m_FLOAT_setVelocityLoopI;
	g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_float = m_FLOAT_setVelocityLoopD;

	return 0;
}
//写入速度环测试值
UINT8 CDlgFileNewPID::setVelocityLoopTest()
{	
	//写入 速度环 测试
	CString errorCString;
	UINT8 errorValue;

	m_CEdit_setVelocityTestVelocity.GetWindowTextW(m_CString_setVelocityTestVelocity);

	//设定速度 非法检查
	//if (m_CButton_toggleVelocityTestVelocityUnit.GetCheck())
	//{
	//	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setVelocityTestVelocity, &errorCString, 0x00, 
	//		g_CDriverParam_Main.m_INT32_setAppMaxVelocity * 60.0 / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution);
	//	if (errorValue == 2)
	//	{
	//		AfxMessageBox(_T("设定速度，输入超限\n") + errorCString, MB_ICONERROR);
	//		return 1;
	//	}
	//}
	//else
	//{
		errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setVelocityTestVelocity, &errorCString, 0x00, g_CDriverParam_Main.m_INT32_setAppMaxSpeed);
		if (errorValue == 2)
		{
			AfxMessageBox(_T("设定速度，输入超限\n") + errorCString, MB_ICONERROR);
			return 1;
		}
	//}

	//if (m_CButton_toggleVelocityTestVelocityUnit.GetCheck())
	//{
	//	m_FLOAT_setVelocityTestSpeed = (FLOAT)_ttof(m_CString_setVelocityTestVelocity);
	//	m_FLOAT_setVelocityTestVelocity = m_FLOAT_setVelocityTestSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
	//	if (m_FLOAT_setVelocityTestVelocity >= 0)
	//	{
	//		m_INT32_setVelocityTestVelocity = (INT32)(m_FLOAT_setVelocityTestVelocity + 0.5F);
	//	}
	//	else
	//		m_INT32_setVelocityTestVelocity = (INT32)(m_FLOAT_setVelocityTestVelocity - 0.5F);

	//}
	//else
	//{
		m_INT32_setVelocityTestVelocity = _ttoi(m_CString_setVelocityTestVelocity);
	//}

	g_CDriverParam_Main.m_INT32_setVelocityTestVelocity = m_INT32_setVelocityTestVelocity;

	return 0;
}

//写入位置环PID值
UINT8 CDlgFileNewPID::setPositionLoop()
{
	m_CEdit_setPositionLoopP.GetWindowTextW(m_CString_setPositionLoopP);
	m_CEdit_setPositionLoopI.GetWindowTextW(m_CString_setPositionLoopI);
	m_CEdit_setPositionLoopD.GetWindowTextW(m_CString_setPositionLoopD);

	m_FLOAT_setPositionLoopP = (FLOAT)(_ttof(m_CString_setPositionLoopP));
	m_FLOAT_setPositionLoopI = (FLOAT)(_ttof(m_CString_setPositionLoopI));
	m_FLOAT_setPositionLoopD = (FLOAT)(_ttof(m_CString_setPositionLoopD));

	if (m_FLOAT_setPositionLoopP < 0)
	{
		AfxMessageBox(_T("位置环P值，输入超限\n允许范围 大于0"));
		return 1;
	}
	if (m_FLOAT_setPositionLoopI < 0)
	{
		AfxMessageBox(_T("位置环I值，输入超限\n允许范围 大于0"));
		return 1;

	}
	if (m_FLOAT_setPositionLoopD < 0)
	{
		AfxMessageBox(_T("位置环D值，输入超限\n允许范围 大于0"));
		return 1;
	}

	g_CDriverParam_Main.m_unParam_setPositionLoopP.param_float = m_FLOAT_setPositionLoopP;
	g_CDriverParam_Main.m_unParam_setPositionLoopI.param_float = m_FLOAT_setPositionLoopI;
	g_CDriverParam_Main.m_unParam_setPositionLoopD.param_float = m_FLOAT_setPositionLoopD;

	return 0;
}
//写入位置环测试值
UINT8 CDlgFileNewPID::setPositionLoopTest()
{
	CString errorCString;
	UINT8 errorValue;
	
	m_CEdit_setPositionTestSpeed.GetWindowTextW(m_CString_setPositionTestSpeed);
	m_CEdit_setPositionTestPosition.GetWindowTextW(m_CString_setPositionTestPosition);
	//m_INT32_setPositionTestPosition = _ttoi(m_CString_setPositionTestPosition);
	m_CEdit_setPositionTestAcceleration.GetWindowTextW(m_CString_setPositionTestAcceleration);
	//m_INT32_setPositionTestAcceleration = _ttoi(m_CString_setPositionTestAcceleration);
	m_CEdit_setPositionTestDeceleration.GetWindowTextW(m_CString_setPositionTestDeceleration);
	//m_INT32_setPositionTestDeceleration = _ttoi(m_CString_setPositionTestDeceleration);

	//加速度 非法检查
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setPositionTestAcceleration, &errorCString, 0x00, g_CDriverParam_Main.m_INT32_setAcceleration);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("加速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	//减速度 非法检查
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setPositionTestDeceleration, &errorCString, 0x00, g_CDriverParam_Main.m_INT32_setDeceleration);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("减速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	//设定速度 非法检查

	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setPositionTestSpeed, &errorCString, 0x00, g_CDriverParam_Main.m_INT32_setAppMaxSpeed);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("最大速度，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	//设定位置 非法检查
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setPositionTestPosition, &errorCString, 
		-g_CDriverParam_Main.m_INT32_setAppMinPosition, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("目标位置，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}
	if (m_CButton_toggleVelocityTestVelocityUnit.GetCheck())
	{
		m_FLOAT_setPositionTestSpeed = (FLOAT)_ttof(m_CString_setPositionTestSpeed);
		m_FLOAT_setPositionTestVelocity = m_FLOAT_setPositionTestSpeed / 60.0F * g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
		if (m_FLOAT_setPositionTestVelocity >= 0)
		{
			//g_CDriverParam_Main.m_FLOAT_setVelocityTestVelocity = g_CDriverParam_Main.m_FLOAT_setVelocityTestVelocity + 0.5F;
			m_INT32_setPositionTestVelocity = (INT32)(m_FLOAT_setPositionTestVelocity + 0.5F);
		}
		else
			//g_CDriverParam_Main.m_FLOAT_setVelocityTestVelocity = g_CDriverParam_Main.m_FLOAT_setVelocityTestVelocity - 0.5F;

			m_INT32_setPositionTestVelocity = (INT32)(m_FLOAT_setPositionTestVelocity - 0.5F);
	}
	else
	{
		m_INT32_setPositionTestVelocity = _ttoi(m_CString_setPositionTestSpeed);
	}



	//写入 全局变量
	g_CDriverParam_Main.m_INT32_setPositionTestSpeed = m_INT32_setPositionTestVelocity;
	g_CDriverParam_Main.m_FLOAT_setPositionTestAngle = _ttof(m_CString_setPositionTestPosition);// m_INT32_setPositionTestPosition;
	g_CDriverParam_Main.m_INT32_setPositionTestPosition = g_CDriverParam_Main.m_FLOAT_setPositionTestAngle * 22444.0f;
	g_CDriverParam_Main.m_INT32_setPositionTestAcceleration = _ttoi(m_CString_setPositionTestAcceleration);// m_INT32_setPositionTestAcceleration;
	g_CDriverParam_Main.m_INT32_setPositionTestDeceleration = _ttoi(m_CString_setPositionTestDeceleration);// m_INT32_setPositionTestDeceleration;

	return 0;
}

//读取 电流环PID和测试值
UINT8 CDlgFileNewPID::getCurrentLoop()
{
	m_CString_setCurrentLoopP.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_float);
	m_CString_setCurrentLoopI.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_float);
	m_CString_setCurrentLoopD.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_float);

	m_CEdit_setCurrentLoopP.SetWindowTextW(m_CString_setCurrentLoopP);
	m_CEdit_setCurrentLoopI.SetWindowTextW(m_CString_setCurrentLoopI);
	m_CEdit_setCurrentLoopD.SetWindowTextW(m_CString_setCurrentLoopD);

	m_CComboBox_setCurrentTestType.SetCurSel(0);
	m_CEdit_setCurrentTestAmplitude.SetWindowTextW(_T("1"));

	return 0;
}
//读取 速度环PID和测试值
UINT8 CDlgFileNewPID::getVelocityLoop()
{

	m_CString_setVelocityLoopP.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_float);
	m_CString_setVelocityLoopI.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_float);
	m_CString_setVelocityLoopD.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_float);

	m_CEdit_setVelocityLoopP.SetWindowTextW(m_CString_setVelocityLoopP);
	m_CEdit_setVelocityLoopI.SetWindowTextW(m_CString_setVelocityLoopI);
	m_CEdit_setVelocityLoopD.SetWindowTextW(m_CString_setVelocityLoopD);

	m_CEdit_setVelocityTestVelocity.SetWindowTextW(_T("1000"));

	return 0;
}
//读取 位置环PID和测试值
UINT8 CDlgFileNewPID::getPositionLoop()
{
	m_CString_setPositionLoopP.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopP.param_float);
	m_CString_setPositionLoopI.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopI.param_float);
	m_CString_setPositionLoopD.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopD.param_float);

	m_CEdit_setPositionLoopP.SetWindowTextW(m_CString_setPositionLoopP);
	m_CEdit_setPositionLoopI.SetWindowTextW(m_CString_setPositionLoopI);
	m_CEdit_setPositionLoopD.SetWindowTextW(m_CString_setPositionLoopD);

	m_CEdit_setPositionTestAcceleration.SetWindowTextW(_T("100"));
	m_CEdit_setPositionTestDeceleration.SetWindowTextW(_T("100"));
	m_CEdit_setPositionTestSpeed.SetWindowTextW(_T("1000"));
	m_CEdit_setPositionTestPosition.SetWindowTextW(_T("10000"));
	
	return 0;
}


BOOL CDlgFileNewPID::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;
	else
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CDlgFileNewPID::OnBnClickedChecktogglevelocitytestvelocityunit()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_CButton_toggleVelocityTestVelocityUnit.GetCheck())
	{
		m_CStatic_setVelocityTestVelocityUnit.SetWindowTextW(_T("R/m"));
		m_CStatic_setPositionTestVelocityUnit.SetWindowTextW(_T("R/m"));

		m_CEdit_setVelocityTestVelocity.GetWindowTextW(m_CString_setVelocityTestVelocity);
		m_CEdit_setPositionTestSpeed.GetWindowTextW(m_CString_setPositionTestSpeed);

		m_INT32_setVelocityTestVelocity = _ttoi(m_CString_setVelocityTestVelocity);
		m_INT32_setPositionTestVelocity = _ttoi(m_CString_setPositionTestSpeed);


		//切换成 R/m单位
		g_CDriverParam_Main.toggleVelocityToSpeedUnit(&m_INT32_setVelocityTestVelocity, &m_FLOAT_setVelocityTestSpeed);
		g_CDriverParam_Main.toggleVelocityToSpeedUnit(&m_INT32_setPositionTestVelocity, &m_FLOAT_setPositionTestSpeed);

		m_CString_setVelocityTestVelocity.Format(_T("%g"), m_FLOAT_setVelocityTestSpeed);
		m_CEdit_setVelocityTestVelocity.SetWindowTextW(m_CString_setVelocityTestVelocity);
		m_CString_setPositionTestSpeed.Format(_T("%g"), m_FLOAT_setPositionTestSpeed);
		m_CEdit_setPositionTestSpeed.SetWindowTextW(m_CString_setPositionTestSpeed);
	}
	else
	{
		//m_CButton_toggleUnit.SetWindowTextW(_T("P/s"));
		m_CStatic_setVelocityTestVelocityUnit.SetWindowTextW(_T("P/s"));
		m_CStatic_setPositionTestVelocityUnit.SetWindowTextW(_T("P/s"));

		m_CEdit_setVelocityTestVelocity.GetWindowTextW(m_CString_setVelocityTestVelocity);
		m_CEdit_setPositionTestSpeed.GetWindowTextW(m_CString_setPositionTestSpeed);

		m_FLOAT_setVelocityTestSpeed = (FLOAT)_ttof(m_CString_setVelocityTestVelocity);
		m_FLOAT_setPositionTestSpeed = (FLOAT)_ttof(m_CString_setPositionTestSpeed);

		//切换成 P/s单位
		g_CDriverParam_Main.toggleSpeedToVelocityUnit(&m_FLOAT_setVelocityTestSpeed, &m_INT32_setVelocityTestVelocity);
		g_CDriverParam_Main.toggleSpeedToVelocityUnit(&m_FLOAT_setPositionTestSpeed, &m_INT32_setPositionTestVelocity);


		m_CString_setVelocityTestVelocity.Format(_T("%d"), m_INT32_setVelocityTestVelocity);
		m_CEdit_setVelocityTestVelocity.SetWindowTextW(m_CString_setVelocityTestVelocity);
		m_CString_setPositionTestSpeed.Format(_T("%d"), m_INT32_setPositionTestVelocity);
		m_CEdit_setPositionTestSpeed.SetWindowTextW(m_CString_setPositionTestSpeed);


	}

}





BOOL CDlgFileNewPID::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类

	if (HIWORD(wParam) == EN_CHANGE)
		m_UINT8_isParamChanged = 1;


	return CDialogEx::OnCommand(wParam, lParam);
}


void CDlgFileNewPID::OnBnClickedButtonsavetestdata()
{
	// TODO: 在此添加控件通知处理程序代码

	CTime m_time = CTime::GetCurrentTime();
	CString m_strDateTime = m_time.Format(_T("%Y%m%d%H%M"));
	//m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	//m_strDate = m_time.Format(_T("%x"));          //格式化日期  
	//m_strTime = m_time.Format(_T("%X"));          //格式化时间  
	//m_strDateTime = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));   //格式化日期时间

	CString filename = _T(".\\CurrentTestData" + m_strDateTime + ".txt");//在本例中路径为:E:\work\BruceXu\CodeRelevant\mytestcode\读写txt\ReadWriteTxt\ReadWriteTxt\Test.txt
	CString strWriteData1,strWriteData2;
	unParam WriteData1, WriteData2;

	CStdioFile csFile;
	CFileException cfException;
	if (csFile.Open(filename, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, &cfException))//以txt方式读取|若没有文件则创建该文件|文件打开时不清除
	{
		for (WORD i = 0; i < (g_CDriverParam_Main.m_UINT16_getCurrentTestLength >> 3); i++)
		{
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 3), &WriteData1.param_int);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 3) + 4, &WriteData2.param_int);

			strWriteData1.Format(_T("%f\t%f\n"), WriteData1.param_float, WriteData2.param_float);

			csFile.SeekToEnd();
			csFile.WriteString(strWriteData1);
		}
	}
	csFile.Close();
}


void CDlgFileNewPID::OnBnClickedButtonsavetestdata2()
{
	// TODO: 在此添加控件通知处理程序代码
	CTime m_time = CTime::GetCurrentTime();
	CString m_strDateTime = m_time.Format(_T("%Y%m%d%H%M"));
	//m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	//m_strDate = m_time.Format(_T("%x"));          //格式化日期  
	//m_strTime = m_time.Format(_T("%X"));          //格式化时间  
	//m_strDateTime = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));   //格式化日期时间

	CString filename = _T(".\\VelocityTestData" + m_strDateTime + ".txt");//在本例中路径为:E:\work\BruceXu\CodeRelevant\mytestcode\读写txt\ReadWriteTxt\ReadWriteTxt\Test.txt
	CString strWriteData1, strWriteData2;

	INT32 WriteData1, WriteData2;
	UINT WriteData3;
	unParam WriteData4;


	CStdioFile csFile;
	CFileException cfException;
	if (csFile.Open(filename, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, &cfException))//以txt方式读取|若没有文件则创建该文件|文件打开时不清除
	{
		for (WORD i = 0; i < (g_CDriverParam_Main.m_UINT16_getVelocityTestLength >> 4); i++)
		{
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4), &WriteData1);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 4, &WriteData2);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 8, &WriteData3);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 12, &WriteData4.param_int);


			strWriteData1.Format(_T("%d\t%d\t%d\t%f\n"), WriteData1, WriteData2, WriteData3, WriteData4.param_float);

			csFile.SeekToEnd();
			csFile.WriteString(strWriteData1);
		}
	}
	csFile.Close();
}


void CDlgFileNewPID::OnBnClickedButtonsavetestdata3()
{
	// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
	CTime m_time = CTime::GetCurrentTime();
	CString m_strDateTime = m_time.Format(_T("%Y%m%d%H%M"));
	//m_time = CTime::GetCurrentTime();             //获取当前时间日期  
	//m_strDate = m_time.Format(_T("%x"));          //格式化日期  
	//m_strTime = m_time.Format(_T("%X"));          //格式化时间  
	//m_strDateTime = m_time.Format(_T("%Y-%m-%d %H:%M:%S %A"));   //格式化日期时间

	CString filename = _T(".\\PositionTestData" + m_strDateTime + ".txt");//在本例中路径为:E:\work\BruceXu\CodeRelevant\mytestcode\读写txt\ReadWriteTxt\ReadWriteTxt\Test.txt
	CString strWriteData1, strWriteData2;
	INT32 WriteData1, WriteData2, WriteData4;
	UINT32 WriteData3;

	CStdioFile csFile;
	CFileException cfException;
	if (csFile.Open(filename, CFile::typeText | CFile::modeCreate | CFile::modeReadWrite | CFile::modeNoTruncate, &cfException))//以txt方式读取|若没有文件则创建该文件|文件打开时不清除
	{
		for (WORD i = 0; i < (g_CDriverParam_Main.m_UINT16_getPositionTestLength >> 4); i++)
		{
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4), &WriteData1);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 4, &WriteData2);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 8, &WriteData3);
			g_CDriverParam_Main.ArrayToHex32(g_CDriverParam_Main.m_UINT8_pTestData + (i << 4) + 12, &WriteData4);


			strWriteData1.Format(_T("%d\t%d\t%d\t%d\n"), WriteData1, WriteData2, WriteData3, WriteData4);

			csFile.SeekToEnd();
			csFile.WriteString(strWriteData1);
		}
	}
	csFile.Close();
}
