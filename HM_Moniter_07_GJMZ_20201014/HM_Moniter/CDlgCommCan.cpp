// CDlgCommCan.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgCommCan.h"
#include "afxdialogex.h"

#include "CDriverParam.h"

extern CDriverParam g_CDriverParam_Main;

// CDlgCommCan 对话框

IMPLEMENT_DYNAMIC(CDlgCommCan, CDialogEx)

CDlgCommCan::CDlgCommCan(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_COMM_CAN, pParent)
{

}

CDlgCommCan::~CDlgCommCan()
{
}

void CDlgCommCan::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_setCANAddress, m_CEdit_setCanAddress);
	DDX_Control(pDX, IDC_COMBO_setCANBaudrate, m_CComboBox_setCanBaudrate);
}


BEGIN_MESSAGE_MAP(CDlgCommCan, CDialogEx)
	ON_BN_CLICKED(ID_BUTTON_CAN_CANCEL, &CDlgCommCan::OnBnClickedButtonCanCancel)
	ON_BN_CLICKED(ID_BUTTON_CAN_CONFIRM, &CDlgCommCan::OnBnClickedButtonCanConfirm)
END_MESSAGE_MAP()


// CDlgCommCan 消息处理程序

BOOL CDlgCommCan::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	m_CEdit_setCanAddress.SetLimitText(16);



	m_CString_setCanAddress.Format(_T("d"), g_CDriverParam_Main.m_UINT8_setCanAddress);
	m_CEdit_setCanAddress.SetWindowTextW(m_CString_setCanAddress);

	m_CComboBox_setCanBaudrate.SetCurSel(g_CDriverParam_Main.m_UINT8_setCanBaudrate);

	m_UINT8_isParamChanged = 0;

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}

//按键 取消
void CDlgCommCan::OnBnClickedButtonCanCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	OnCancel();
}

void CDlgCommCan::OnCancel()
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

//按键 确 认
void CDlgCommCan::OnBnClickedButtonCanConfirm()
{
	// TODO: 在此添加控件通知处理程序代码
	if (setCan())
		return;

	EndDialog(IDOK);
}


UINT8 CDlgCommCan::setCan()
{
	CString errorCString;
	UINT8 errorValue;

	//从文本框获取字符串
	m_CEdit_setCanAddress.GetWindowTextW(m_CString_setCanAddress);

	//溢出检查
	errorValue = g_CDriverParam_Main.isLegalString(&m_CString_setCanAddress, &errorCString, 0x0000, 0x00F7);
	if (errorValue == 2)
	{
		AfxMessageBox(_T("设备地址，输入超限\n") + errorCString, MB_ICONERROR);
		return 1;
	}

	g_CDriverParam_Main.m_UINT8_setCanAddress = _ttoi(m_CString_setCanAddress);
	g_CDriverParam_Main.m_UINT8_setCanBaudrate = m_CComboBox_setCanBaudrate.GetCurSel();

	return 0;
}

BOOL CDlgCommCan::OnCommand(WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (HIWORD(wParam) == EN_CHANGE)
		m_UINT8_isParamChanged = 1;


	return CDialogEx::OnCommand(wParam, lParam);
}


BOOL CDlgCommCan::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) return TRUE;
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_SPACE) return TRUE;
	else
		return CDialogEx::PreTranslateMessage(pMsg);
}
