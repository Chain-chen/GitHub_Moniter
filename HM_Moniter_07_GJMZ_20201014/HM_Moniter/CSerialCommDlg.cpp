// CSerialCommDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CSerialCommDlg.h"
#include "afxdialogex.h"

extern CSPortComm g_CSPortComm_Main;

// CSerialCommDlg 对话框

IMPLEMENT_DYNAMIC(CSerialCommDlg, CDialogEx)

CSerialCommDlg::CSerialCommDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_SERIALPORT, pParent)
{

}

CSerialCommDlg::~CSerialCommDlg()
{
}

void CSerialCommDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_CommNumber, m_CComboBox_CommNumber);
	DDX_Control(pDX, IDC_COMBO_Baudrate, m_CComboBox_Baudrate);
	DDX_Control(pDX, IDC_COMBO_ParityBit, m_CComboBox_ParityBit);
	DDX_Control(pDX, IDC_COMBO_StopBit, m_CComboBox_StopBit);
	DDX_Control(pDX, IDC_BUTTON_SeachUsableComm, m_CButton_SearchComm);
	DDX_Control(pDX, IDC_BUTTON_OpenComm, m_CButton_OpenComm);
}


BEGIN_MESSAGE_MAP(CSerialCommDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SeachUsableComm, &CSerialCommDlg::OnBnClickedButtonSeachusablecomm)
	ON_BN_CLICKED(IDC_BUTTON_OpenComm, &CSerialCommDlg::OnBnClickedButtonOpencomm)
END_MESSAGE_MAP()


// CSerialCommDlg 消息处理程序


BOOL CSerialCommDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (!g_CSPortComm_Main.CommOpened) {

		CString str;

		m_CComboBox_CommNumber.ResetContent();

		g_CSPortComm_Main.SearchComm();

		if (!g_CSPortComm_Main.CommQuantity)
		{
			m_CComboBox_CommNumber.AddString(_T("无"));
		}
		else {

			for (UINT i = 0; i < g_CSPortComm_Main.CommQuantity; i++) {
				str.Format(_T("COM%d"), g_CSPortComm_Main.CommArray[i]);
				m_CComboBox_CommNumber.AddString(str);
			}
		}
		m_CComboBox_CommNumber.SetCurSel(0);
		m_CComboBox_Baudrate.SetCurSel(5);
		m_CComboBox_ParityBit.SetCurSel(0);
		m_CComboBox_StopBit.SetCurSel(0);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CSerialCommDlg::OnBnClickedButtonSeachusablecomm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened) {

		CString str;

		m_CComboBox_CommNumber.ResetContent();

		g_CSPortComm_Main.SearchComm();

		if (!g_CSPortComm_Main.CommQuantity)
		{
			m_CComboBox_CommNumber.AddString(_T("无"));
		}
		else {

			for (UINT i = 0; i < g_CSPortComm_Main.CommQuantity; i++) {
				str.Format(_T("COM%d"), g_CSPortComm_Main.CommArray[i]);
				m_CComboBox_CommNumber.AddString(str);
			}
		}
		m_CComboBox_CommNumber.SetCurSel(0);
		m_CComboBox_Baudrate.SetCurSel(0);
		m_CComboBox_ParityBit.SetCurSel(0);
		m_CComboBox_StopBit.SetCurSel(0);
	}


}


void CSerialCommDlg::OnBnClickedButtonOpencomm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!g_CSPortComm_Main.CommOpened) {

		UINT baudrate, paritybit, stopbit;
		switch (m_CComboBox_Baudrate.GetCurSel()) {
		case 0:baudrate = 9600; break;
		case 1:baudrate = 14400; break;
		case 2:baudrate = 19600; break;
		case 3:baudrate = 38400; break;
		case 4:baudrate = 57600; break;
		case 5:baudrate = 115200; break;
		default:baudrate = 115200; break;
		}
		paritybit = m_CComboBox_ParityBit.GetCurSel();
		stopbit = m_CComboBox_StopBit.GetCurSel();

		g_CSPortComm_Main.CommDCB.BaudRate = baudrate;
		g_CSPortComm_Main.CommDCB.ByteSize = 8;
		g_CSPortComm_Main.CommDCB.Parity = paritybit;
		g_CSPortComm_Main.CommDCB.StopBits = stopbit;


		g_CSPortComm_Main.CommOpened = TRUE;

		g_CSPortComm_Main.OpenComm(m_CComboBox_CommNumber.GetCurSel());



		//	m_CSPortComm_Main = g_CSPortComm_Main;

		m_CButton_SearchComm.EnableWindow(FALSE);

		m_CComboBox_CommNumber.EnableWindow(FALSE);
		m_CComboBox_Baudrate.EnableWindow(FALSE);
		m_CComboBox_ParityBit.EnableWindow(FALSE);
		m_CComboBox_StopBit.EnableWindow(FALSE);

		m_CButton_OpenComm.SetWindowTextW(_T("关 闭"));

		//CString str;
		//str.Format(_T("COM%d,%d,%d,%d,%d"), g_CSPortComm_Main.CommArray[m_CComboBox_CommNumber.GetCurSel()], g_CSPortComm_Main.CommDCB.BaudRate, g_CSPortComm_Main.CommDCB.ByteSize, g_CSPortComm_Main.CommDCB.Parity, g_CSPortComm_Main.CommDCB.StopBits);
		//AfxMessageBox(str);

		//m_CSPortComm_Main = g_CSPortComm_Main;

		OnOK();

	}

	else 
	{
		g_CSPortComm_Main.CloseComm();

		g_CSPortComm_Main.CommOpened = FALSE;

		m_CButton_SearchComm.EnableWindow(TRUE);

		m_CComboBox_CommNumber.EnableWindow(TRUE);
		m_CComboBox_Baudrate.EnableWindow(TRUE);
		m_CComboBox_ParityBit.EnableWindow(TRUE);
		m_CComboBox_StopBit.EnableWindow(TRUE);

		m_CButton_OpenComm.SetWindowTextW(_T("打 开"));
	}
}
