// CDlgFileLoopTest.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileLoopTest.h"
#include "afxdialogex.h"


// CDlgFileLoopTest 对话框

IMPLEMENT_DYNAMIC(CDlgFileLoopTest, CDialogEx)

CDlgFileLoopTest::CDlgFileLoopTest(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_NEW_LOOP_FLAG, pParent)
{

}

CDlgFileLoopTest::~CDlgFileLoopTest()
{
}

void CDlgFileLoopTest::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FileLoopFlag, m_CStatic_FileLoopFlag);
}


BEGIN_MESSAGE_MAP(CDlgFileLoopTest, CDialogEx)

	ON_BN_CLICKED(IDC_BUTTON_FileLoopTest, &CDlgFileLoopTest::OnBnClickedButtonFilelooptest)
END_MESSAGE_MAP()


// CDlgFileLoopTest 消息处理程序





void CDlgFileLoopTest::OnBnClickedButtonFilelooptest()
{
	// TODO: 在此添加控件通知处理程序代码

	if (m_BYTE_TestResult == IDOK)				//1 测试完成
	{
		EndDialog(IDOK);

	}
	else if(m_BYTE_TestResult == IDCANCEL)				//2 取消测试
	{
		EndDialog(IDCANCEL);
		
	}
	else
	{
		EndDialog(IDABORT);
	}
}


BOOL CDlgFileLoopTest::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_BYTE_TestResult == IDOK)
	{
		m_CStatic_FileLoopFlag.SetWindowTextW(_T("正在测试，请等待……！"));

	}
	else if (m_BYTE_TestResult == IDRETRY)
	{
		m_CStatic_FileLoopFlag.SetWindowTextW(_T("正在读取数据，请等待……！"));
	}
	else
	{
	}

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}
