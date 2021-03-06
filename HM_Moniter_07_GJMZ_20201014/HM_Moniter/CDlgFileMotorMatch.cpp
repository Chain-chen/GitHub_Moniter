// CDlgFileMotorMatch.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileMotorMatch.h"
#include "afxdialogex.h"


// CDlgFileMotorMatch 对话框

IMPLEMENT_DYNAMIC(CDlgFileMotorMatch, CDialogEx)

CDlgFileMotorMatch::CDlgFileMotorMatch(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_MOTOR_FLAG, pParent)
{

}

CDlgFileMotorMatch::~CDlgFileMotorMatch()
{
}

void CDlgFileMotorMatch::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FileMotorMatch, m_CStatic_FileMotorMatch);
	DDX_Control(pDX, IDC_BUTTON_FileMotorMatch, m_CButton_FileMotorMatch);
}


BEGIN_MESSAGE_MAP(CDlgFileMotorMatch, CDialogEx)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_FileMotorMatch, &CDlgFileMotorMatch::OnBnClickedButtonFilemotormatch)
END_MESSAGE_MAP()






void CDlgFileMotorMatch::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if (m_BYTE_MatchResult == IDOK)				//1 匹配完成
	{
		EndDialog(IDOK);
	}
	else if (m_BYTE_MatchResult == IDCANCEL)			//2 取消匹配
	{
		EndDialog(IDCANCEL);
	}
	else if (m_BYTE_MatchResult == IDABORT)
	{
		EndDialog(IDABORT);								//3 匹配失败
	}
	else
	{
		EndDialog(IDRETRY);
	}
//	CDialogEx::OnClose();
}

// CDlgFileMotorMatch 消息处理程序
void CDlgFileMotorMatch::OnBnClickedButtonFilemotormatch()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_BYTE_MatchResult == IDOK)				//1 匹配完成
	{
		EndDialog(IDOK);
	}
	else if (m_BYTE_MatchResult == IDCANCEL)			//2 取消匹配
	{
		EndDialog(IDCANCEL);
	}
	else if (m_BYTE_MatchResult == IDABORT)
	{
		EndDialog(IDABORT);								//3 匹配失败
	}
	else
	{
		EndDialog(IDRETRY);
	}	
}
