// CDlgFileNewSave.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileNewSave.h"
#include "afxdialogex.h"
#include "CSPortComm.h"
#include "CDriverParam.h"

extern CDriverParam g_CDriverParam_Main;
extern CSPortComm g_CSPortComm_Main;
extern HANDLE g_HANDLE_CommResponce;
extern BYTE g_BYTE_NewSaveDataRxArray[256];
// CDlgFileNewSave 对话框

IMPLEMENT_DYNAMIC(CDlgFileNewSave, CDialogEx)

CDlgFileNewSave::CDlgFileNewSave(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_SAVE, pParent)
{

	m_BYTE_OpenMode = 0;
}

CDlgFileNewSave::~CDlgFileNewSave()
{
}

void CDlgFileNewSave::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//	DDX_Control(pDX, IDC_LIST_ParamList, m_CListCtrl_ParamList);
	DDX_Control(pDX, IDC_LIST_PARAM, m_CListBox_ParamList);
}


BEGIN_MESSAGE_MAP(CDlgFileNewSave, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_DownloadFile, &CDlgFileNewSave::OnBnClickedButtonDownloadfile)
	ON_MESSAGE(WM_USER_FILESAVE_RX, &CDlgFileNewSave::OnUserFilesaveRx)
	ON_BN_CLICKED(IDC_BUTTON_SaveFile, &CDlgFileNewSave::OnBnClickedButtonSavefile)
	ON_BN_CLICKED(IDC_BUTTON_OpenFile, &CDlgFileNewSave::OnBnClickedButtonOpenfile)
	ON_BN_CLICKED(IDC_BUTTON_FileSaveOK, &CDlgFileNewSave::OnBnClickedButtonFilesaveok)
	ON_BN_CLICKED(IDC_BUTTON_FileSaveCancel, &CDlgFileNewSave::OnBnClickedButtonFilesavecancel)
END_MESSAGE_MAP()


// CDlgFileNewSave 消息处理程序


BOOL CDlgFileNewSave::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	//0 新建 1打开 2保存
	if (m_BYTE_OpenMode == 0)
	{
		setParamListDisplay();
		
		//SetWindowText(_T("文件-打开"));
		//GetDlgItem(IDC_BUTTON_SaveFile)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_OpenFile)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_FileSaveCancel)->EnableWindow(FALSE);

		//GetDlgItem(IDC_BUTTON_FileSaveCancel)->SetWindowTextW(_T("取 消"));

	}
	else if (m_BYTE_OpenMode == 1)
	{
		//setParamListDisplay();

		SetWindowText(_T("文件-打开"));
		GetDlgItem(IDC_BUTTON_SaveFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_OpenFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_FileSaveCancel)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_FileSaveCancel)->SetWindowTextW(_T("取 消"));
	}

	else if (m_BYTE_OpenMode == 2)
	{
		SetWindowText(_T("文件-保存"));
		GetDlgItem(IDC_BUTTON_SaveFile)->EnableWindow(TRUE);
		//GetDlgItem(IDC_BUTTON_OpenFile)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_FileSaveCancel)->EnableWindow(FALSE);
		//GetDlgItem(IDC_BUTTON_FileSaveCancel)->SetWindowTextW(_T("取 消"));
		setParamListDisplay();
	}

	else
		;

	// TODO:  在此添加额外的初始化
	//m_CListCtrl_ParamList.InsertColumn(0, _T("参数项"), LVCFMT_LEFT, 80);
	//m_CListCtrl_ParamList.InsertColumn(1, _T("参数值"), LVCFMT_LEFT, 100);

	//m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT_setRatedVelocity);
	//m_CListBox_ParamList.AddString(_T("额定转速：") + m_CString_Temp);

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


UINT8 CDlgFileNewSave::setRegValue(WORD reg, BYTE reglength)
{
	// TODO: 在此处添加实现代码.
	m_BYTE_DataTxArray[0] = 0x5A;
	m_BYTE_DataTxArray[1] = 0xA5;

	m_BYTE_DataTxArray[2] = 0x04 + reglength;

	m_BYTE_DataTxArray[3] = (0x00 & 0xF0) + (0x00 & 0x08) + 0x02;
	m_BYTE_DataTxArray[4] = reg >> 8;
	m_BYTE_DataTxArray[5] = reg & 0xFF;
	m_BYTE_DataTxArray[6] = reglength;

	WORD CRCValue = g_CSPortComm_Main.CRC16(m_BYTE_DataTxArray, reglength + REG_INFIX_LENGTH);
	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH] = (CRCValue & 0xFF);
	m_BYTE_DataTxArray[reglength + REG_INFIX_LENGTH + 1] = (CRCValue >> 8);

	g_CSPortComm_Main.SendData(m_BYTE_DataTxArray, reglength + REG_INFIX_LENGTH + 2);

	DWORD ErrorFlag = WaitForSingleObject(g_HANDLE_CommResponce, 1000);
	if (ErrorFlag != WAIT_OBJECT_0)
	{
		return 1;
	}
	else
	{
		//processDataRx(reg);
		//SendMessageW(WM_USER_MONITER_RX, COMMAND_MOTOR_ENABLE, 0);
	}
	return 0;
}


LRESULT CDlgFileNewSave::OnUserFilesaveRx(WPARAM wParam, LPARAM lParam)
{		
	return 0;
}


int CDlgFileNewSave::processDataRx()
{
	// TODO: 在此处添加实现代码.
	BYTE FunctionCode = g_BYTE_NewSaveDataRxArray[3] & 0x07;

	switch ((g_BYTE_NewSaveDataRxArray[4] << 8) | (g_BYTE_NewSaveDataRxArray[5]))
	{
	case REG_COMMAND:

		if (FunctionCode == 0x01)
		{

		}
		else if (FunctionCode == 0x02)
		{


		}
		break;

	default:
		
		
		
		break;
	}
	return 0;
}

//按键 保存到FLASH
void CDlgFileNewSave::OnBnClickedButtonDownloadfile()
{
	// TODO: 在此添加控件通知处理程序代码
	if (g_CSPortComm_Main.CommOpened) 
	{
		UINT8 result = 0;
		
		//0 新建 1打开 2保存
		if (m_BYTE_OpenMode == 1)
		{
			//下载到RAM 电机参数
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setPolePairs);
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_setEncoderType);
			g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 12, g_CDriverParam_Main.m_INT32_setRatedSpeed);

			if (setRegValue(REG_MOTOR_POLES, 16) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}

			//下载到RAM 匹配参数
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_getMatchFlag);
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_getPhaseOrder);
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT8_getHallOrder);
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_INT8_getHallDirection);
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_INT8_getEncoderDirection);
			g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_INT16_getDefaultDegree);
					   
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_BYTE_getMatchFlag, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_BYTE_getPhaseOrder, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_BYTE_getHallOrder, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_BYTE_getHallDirection, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_BYTE_getEncoderDirection, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_WORD_getDefaultDegree, 2);

			if (setRegValue(REG_MATCH_FLAG, 7) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}

			//下载到RAM 应用参数
			g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, (g_CDriverParam_Main.m_UINT8_setMotionDirection << 0x00));
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_INT32_setAcceleration);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_INT32_setDeceleration);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_INT32_setStopDeceleration);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT32_setAppMinPosition);


			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_INT_setMotionDirection, 1);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT_setAppMaxVelocity, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_UINT_setAcceleration, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_UINT_setDeceleration, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_UINT_setStopDeceleration, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT_setAppMaxPosition, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT_setAppMinPosition, 4);

			if (setRegValue(REG_APPLICATION, 33) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}
			//this->SendMessageW(WM_USER_FILENEWDRIVER_RX, REG_APPLICATION, 0);

			//下载到RAM 电流环参数
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);

			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setCurrentLoopP.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setCurrentLoopI.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setCurrentLoopD.param_int, 4);

			if (setRegValue(REG_CURRENTLOOP_P, 12) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}

			//下载到RAM 速度环参数

			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);

			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setVelocityLoopP.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setVelocityLoopI.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setVelocityLoopD.param_int, 4);

			if (setRegValue(REG_VELOCITYLOOP_P, 12) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}

			//下载到RAM 位置环参数

			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
			g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);

			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setPositionLoopP.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setPositionLoopI.param_int, 4);
			//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setPositionLoopD.param_int, 4);

			if (setRegValue(REG_POSITIONLOOP_P, 12) == 1)
			{
				AfxMessageBox(_T("设备无响应"));
				return;
			}

		}

		//下载到FLASH
			
		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, COMMAND_CPARAM_SAVE);
		if (setRegValue(REG_COMMAND, 0x02) == 1)
		{
			AfxMessageBox(_T("设备无响应"));
			return;
		}
			
		Sleep(500);
		AfxMessageBox(_T("参数下载完成！"));
	}
}

//按键 保存到文件
void CDlgFileNewSave::OnBnClickedButtonSavefile()
{
	// TODO: 在此添加控件通知处理程序代码

	//CFileDialog m_CFileDialog_Main(BOOL bOpenFileDialog,
	//	LPCTSTR lpszDefExt = NULL,
	//	LPCTSTR lpszFileName = NULL,
	//	DWORD dwFlags = OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
	//	LPCTSTR lpszFilter = NULL,
	//	CWnd* pParentWnd = NULL
	//);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer, g_CDriverParam_Main.m_UINT8_setPolePairs);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 1, g_CDriverParam_Main.m_UINT8_setEncoderType);
	g_CDriverParam_Main.Hex16ToArray(m_BYTE_ParamDataBuffer + 2, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 4, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 8, g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 12, g_CDriverParam_Main.m_INT32_setRatedSpeed);


	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 16, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 20, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 24, g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 28, g_CDriverParam_Main.m_INT32_setAcceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 32, g_CDriverParam_Main.m_INT32_setDeceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 36, g_CDriverParam_Main.m_INT32_setStopDeceleration);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 40, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 44, g_CDriverParam_Main.m_INT32_setAppMinPosition);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 48, (g_CDriverParam_Main.m_UINT8_setMotionDirection << 0x00));

	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 49, g_CDriverParam_Main.m_UINT8_getMatchFlag);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 50, g_CDriverParam_Main.m_UINT8_getPhaseOrder);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 51, g_CDriverParam_Main.m_UINT8_getHallOrder);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 52, g_CDriverParam_Main.m_INT8_getHallDirection);
	g_CDriverParam_Main.Hex8ToArray(m_BYTE_ParamDataBuffer + 53, g_CDriverParam_Main.m_INT8_getEncoderDirection);
	g_CDriverParam_Main.Hex16ToArray(m_BYTE_ParamDataBuffer + 54, g_CDriverParam_Main.m_INT16_getDefaultDegree);

	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 56, g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 60, g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 64, g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 68, g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 72, g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 76, g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 80, g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 84, g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 88, g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);

	g_CDriverParam_Main.Hex32ToArray(m_BYTE_ParamDataBuffer + 92, g_CDriverParam_Main.m_UINT32_setPulsePerRevolution);


	CFileDialog m_CFileDialog_Main(FALSE, _T(".dat"), _T("Application1"), OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, _T("DAT文件(*.dat)|*.dat||"), this);
	if (m_CFileDialog_Main.DoModal() == IDOK) 
	{
		CFile m_CFile_Main(m_CFileDialog_Main.GetPathName(), CFile::modeCreate | CFile::modeWrite | CFile::typeBinary);
		
		//
		m_CFile_Main.Write(m_BYTE_ParamDataBuffer, 96);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setEncoderType, 1);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_UINT_setEncoderResolution, 2);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_unParam_setStallCurrent.param_int, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_UINT_setRatedVelocity, 4);

		//m_CFile_Main.Write(&g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setAppMaxVelocity, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_UINT_setAcceleration, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_UINT_setDeceleration, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_UINT_setStopDeceleration, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setAppMaxPosition, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setAppMinPosition, 4);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setMotionDirection, 1);
		//m_CFile_Main.Write(&g_CDriverParam_Main.m_INT_setAppMaxVelocity, 1);

		m_CFile_Main.Flush();
		m_CFile_Main.Close();	
	}
}

//按键 打开文件
void CDlgFileNewSave::OnBnClickedButtonOpenfile()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog m_CFileDialog_Main(TRUE, _T(".dat"), _T(""), OFN_HIDEREADONLY, _T("DAT文件(*.dat)|*.dat||"), this);

	if (m_CFileDialog_Main.DoModal() == IDOK)
	{
		AfxMessageBox(m_CFileDialog_Main.GetPathName());

		CFile m_CFile_Main(m_CFileDialog_Main.GetPathName(), CFile::modeRead | CFile::typeBinary);
		m_CFile_Main.Read(m_BYTE_ParamDataBuffer, (UINT32)(m_CFile_Main.GetLength()));

		m_CFile_Main.Close();

		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 0, &g_CDriverParam_Main.m_UINT8_setPolePairs);
		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 1, &g_CDriverParam_Main.m_UINT8_setEncoderType);
		g_CDriverParam_Main.ArrayToHex16(m_BYTE_ParamDataBuffer + 2, &g_CDriverParam_Main.m_UINT16_setEncoderResolution);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 4, &g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 8, &g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 12, &g_CDriverParam_Main.m_INT32_setRatedSpeed);

		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 16, &g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 20, &g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 24, &g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 28, &g_CDriverParam_Main.m_INT32_setAcceleration);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 32, &g_CDriverParam_Main.m_INT32_setDeceleration);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 36, &g_CDriverParam_Main.m_INT32_setStopDeceleration);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 40, &g_CDriverParam_Main.m_INT32_setAppMaxPosition);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 44, &g_CDriverParam_Main.m_INT32_setAppMinPosition);
		g_CDriverParam_Main.ArrayToBit(m_BYTE_ParamDataBuffer + 48, &g_CDriverParam_Main.m_UINT8_setMotionDirection, 0x00);

		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 49, &g_CDriverParam_Main.m_UINT8_getMatchFlag);
		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 50, &g_CDriverParam_Main.m_UINT8_getPhaseOrder);
		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 51, &g_CDriverParam_Main.m_UINT8_getHallOrder);
		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 52, &g_CDriverParam_Main.m_INT8_getHallDirection);
		g_CDriverParam_Main.ArrayToHex8(m_BYTE_ParamDataBuffer + 53, &g_CDriverParam_Main.m_INT8_getEncoderDirection);
		g_CDriverParam_Main.ArrayToHex16(m_BYTE_ParamDataBuffer + 54, &g_CDriverParam_Main.m_INT16_getDefaultDegree);

		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 56, &g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 60, &g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 64, &g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 68, &g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 72, &g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 76, &g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 80, &g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 84, &g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 88, &g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);

		g_CDriverParam_Main.ArrayToHex32(m_BYTE_ParamDataBuffer + 92, &g_CDriverParam_Main.m_UINT32_setPulsePerRevolution);

		setParamListDisplay();

		GetDlgItem(IDC_BUTTON_DownloadFile)->EnableWindow(TRUE);
		GetDlgItem(IDC_BUTTON_SaveFile)->EnableWindow(TRUE);
	}
}

//按键 完成
void CDlgFileNewSave::OnBnClickedButtonFilesaveok()
{
	// TODO: 在此添加控件通知处理程序代码
/*
	if (m_BYTE_OpenMode)
	{

		//下载到RAM 电机参数
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setPolePairs);
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_setEncoderType);
		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT16_setEncoderResolution);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setRatedCurrent.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setStallCurrent.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 12, g_CDriverParam_Main.m_INT32_setRatedSpeed);


		if (setRegValue(REG_MOTOR_POLES, 16) == 1)
		{
			AfxMessageBox(_T("设备无响应"));
			return;
		}


		//下载到RAM 匹配参数

		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_getMatchFlag);
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_UINT8_getPhaseOrder);
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_UINT8_getHallOrder);
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_INT8_getHallDirection);
		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_INT8_getEncoderDirection);
		g_CDriverParam_Main.Hex16ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_INT16_getDefaultDegree);



		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_BYTE_getMatchFlag, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_BYTE_getPhaseOrder, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 2, g_CDriverParam_Main.m_BYTE_getHallOrder, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 3, g_CDriverParam_Main.m_BYTE_getHallDirection, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_BYTE_getEncoderDirection, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_WORD_getDefaultDegree, 2);

		if (setRegValue(REG_MATCH_FLAG, 7) == 1)
		{
			AfxMessageBox(_T("设备无响应"));
			return;
		}


		//下载到RAM 应用参数

		g_CDriverParam_Main.Hex8ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_UINT8_setMotionDirection);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_INT32_setAcceleration);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_INT32_setDeceleration);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_INT32_setStopDeceleration);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT32_setAppMaxPosition);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT32_setAppMinPosition);


		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_INT_setMotionDirection, 1);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 1, g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 5, g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 9, g_CDriverParam_Main.m_INT_setAppMaxVelocity, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 13, g_CDriverParam_Main.m_UINT_setAcceleration, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 17, g_CDriverParam_Main.m_UINT_setDeceleration, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 21, g_CDriverParam_Main.m_UINT_setStopDeceleration, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 25, g_CDriverParam_Main.m_INT_setAppMaxPosition, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 29, g_CDriverParam_Main.m_INT_setAppMinPosition, 4);

		if (setRegValue(REG_APPLICATION, 33) == 1)
		{
			AfxMessageBox(_T("设备无响应"));
		}
		//this->SendMessageW(WM_USER_FILENEWDRIVER_RX, REG_APPLICATION, 0);

		//下载到RAM 电流环参数


		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_int);

		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setCurrentLoopP.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setCurrentLoopI.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setCurrentLoopD.param_int, 4);

		if (setRegValue(REG_CURRENTLOOP_P, 12) == 1)
		{
			return;
		}

		//下载到RAM 速度环参数

		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_int);

		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setVelocityLoopP.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setVelocityLoopI.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setVelocityLoopD.param_int, 4);

		if (setRegValue(REG_VELOCITYLOOP_P, 12) == 1)
		{
			return;
		}

		//下载到RAM 位置环参数

		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, g_CDriverParam_Main.m_unParam_setPositionLoopP.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, g_CDriverParam_Main.m_unParam_setPositionLoopI.param_int);
		g_CDriverParam_Main.Hex32ToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, g_CDriverParam_Main.m_unParam_setPositionLoopD.param_int);

		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 0, m_unParam_setPositionLoopP.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 4, m_unParam_setPositionLoopI.param_int, 4);
		//HexToArray(m_BYTE_DataTxArray + REG_INFIX_LENGTH + 8, m_unParam_setPositionLoopD.param_int, 4);

		if (setRegValue(REG_POSITIONLOOP_P, 12) == 1)
		{
			return;
		}

	}
*/
	EndDialog(IDOK);
}

//按键 取消
void CDlgFileNewSave::OnBnClickedButtonFilesavecancel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BYTE_OpenMode == 0)
		EndDialog(IDABORT);
	else
		EndDialog(IDCANCEL);
}

UINT8 CDlgFileNewSave::setParamListDisplay()
{
	m_CListBox_ParamList.ResetContent();  //清空列表

	m_CListBox_ParamList.AddString(_T("电机参数"));

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setPolePairs);
	m_CListBox_ParamList.AddString(_T("极对数：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setEncoderType);
	m_CListBox_ParamList.AddString(_T("反馈类型：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT16_setEncoderResolution);
	m_CListBox_ParamList.AddString(_T("编码器线数：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setRatedCurrent.param_float);
	m_CListBox_ParamList.AddString(_T("额定电流：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setStallCurrent.param_float);
	m_CListBox_ParamList.AddString(_T("堵转电流：") + m_CString_Temp);

	//m_FLOAT_setRatedVelocity = g_CDriverParam_Main.m_INT32_setRatedVelocity * 60.0F / g_CDriverParam_Main.m_UINT32_setPulsePerRevolution;
	//if (m_FLOAT_setRatedVelocity >= 0)
	//{
	//	m_FLOAT_setRatedSpeed = m_FLOAT_setRatedVelocity + 0.5F;
	//}
	//else
//		g_CDriverParam_Main.m_FLOAT_setRatedSpeed = g_CDriverParam_Main.m_FLOAT_setRateSpeed - 0.5F;

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_INT32_setRatedSpeed);
	m_CListBox_ParamList.AddString(_T("额定转速：") + m_CString_Temp);

	m_CListBox_ParamList.AddString(_T(""));
	m_CListBox_ParamList.AddString(_T("匹配参数"));

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_getMatchFlag);
	m_CListBox_ParamList.AddString(_T("匹配标志：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_getPhaseOrder);
	m_CListBox_ParamList.AddString(_T("相线线序：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_getHallOrder);
	m_CListBox_ParamList.AddString(_T("霍尔线序：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT8_getHallDirection);
	m_CListBox_ParamList.AddString(_T("霍尔方向：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT8_getEncoderDirection);
	m_CListBox_ParamList.AddString(_T("编码器方向：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT16_getDefaultDegree);
	m_CListBox_ParamList.AddString(_T("初始电角度：") + m_CString_Temp);

	m_CListBox_ParamList.AddString(_T(""));
	m_CListBox_ParamList.AddString(_T("应用参数"));

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setAppRatedCurrent.param_float);
	m_CListBox_ParamList.AddString(_T("应用额定电流：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setAppStallCurrent.param_float);
	m_CListBox_ParamList.AddString(_T("应用堵转电流：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAppMaxVelocity);
	m_CListBox_ParamList.AddString(_T("应用最大转速：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAcceleration);
	m_CListBox_ParamList.AddString(_T("加速度：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setDeceleration);
	m_CListBox_ParamList.AddString(_T("减速度：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setStopDeceleration);
	m_CListBox_ParamList.AddString(_T("停止减速度：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAppMaxPosition);
	m_CListBox_ParamList.AddString(_T("应用最大位置：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_INT32_setAppMinPosition);
	m_CListBox_ParamList.AddString(_T("应用最小位置：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%d"), g_CDriverParam_Main.m_UINT8_setMotionDirection);
	m_CListBox_ParamList.AddString(_T("运动方向：") + m_CString_Temp);

	m_CListBox_ParamList.AddString(_T(""));
	m_CListBox_ParamList.AddString(_T("环路参数"));

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopP.param_float);
	m_CListBox_ParamList.AddString(_T("电流环P：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopI.param_float);
	m_CListBox_ParamList.AddString(_T("电流环I：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setCurrentLoopD.param_float);
	m_CListBox_ParamList.AddString(_T("电流环D：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopP.param_float);
	m_CListBox_ParamList.AddString(_T("速度环P：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopI.param_float);
	m_CListBox_ParamList.AddString(_T("速度环I：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setVelocityLoopD.param_float);
	m_CListBox_ParamList.AddString(_T("速度环D：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopP.param_float);
	m_CListBox_ParamList.AddString(_T("位置环P：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopI.param_float);
	m_CListBox_ParamList.AddString(_T("位置环I：") + m_CString_Temp);

	m_CString_Temp.Format(_T("%g"), g_CDriverParam_Main.m_unParam_setPositionLoopD.param_float);
	m_CListBox_ParamList.AddString(_T("位置环D：") + m_CString_Temp);	
	
	return 0;
}


BOOL CDlgFileNewSave::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}
