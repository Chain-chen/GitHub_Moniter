// CDlgUserDef.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgUserDef.h"
#include "afxdialogex.h"
#include "CDriverParam.h"

extern CDriverParam g_CDriverParam_Main;
// CDlgUserDef 对话框

IMPLEMENT_DYNAMIC(CDlgUserDef, CDialog)

CDlgUserDef::CDlgUserDef(CWnd* pParent /*=nullptr*/)
	: CDialog(IDD_DIALOG_USER_DEF, pParent)
{

}

CDlgUserDef::~CDlgUserDef()
{
}

void CDlgUserDef::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_USER_CHECK0, m_CButton_UserCheck0);
	DDX_Control(pDX, IDC_USER_CHECK1, m_CButton_UserCheck1);
	DDX_Control(pDX, IDC_USER_CHECK2, m_CButton_UserCheck2);
	DDX_Control(pDX, IDC_USER_CHECK3, m_CButton_UserCheck3);
	DDX_Control(pDX, IDC_USER_CHECK4, m_CButton_UserCheck4);
	DDX_Control(pDX, IDC_USER_CHECK5, m_CButton_UserCheck5);
	DDX_Control(pDX, IDC_USER_CHECK6, m_CButton_UserCheck6);
	DDX_Control(pDX, IDC_USER_CHECK7, m_CButton_UserCheck7);
	DDX_Control(pDX, IDC_USER_CHECK8, m_CButton_UserCheck8);
	DDX_Control(pDX, IDC_USER_CHECK9, m_CButton_UserCheck9);
	DDX_Control(pDX, IDC_USER_CHECK10, m_CButton_UserCheck10);
	DDX_Control(pDX, IDC_USER_CHECK11, m_CButton_UserCheck11);
	DDX_Control(pDX, IDC_USER_CHECK12, m_CButton_UserCheck12);
	DDX_Control(pDX, IDC_USER_CHECK13, m_CButton_UserCheck13);
	DDX_Control(pDX, IDC_USER_CHECK14, m_CButton_UserCheck14);
	DDX_Control(pDX, IDC_USER_CHECK15, m_CButton_UserCheck15);
	DDX_Control(pDX, IDC_USER_CHECK16, m_CButton_UserCheck16);
	DDX_Control(pDX, IDC_USER_CHECK17, m_CButton_UserCheck17);
	DDX_Control(pDX, IDC_USER_CHECK18, m_CButton_UserCheck18);
	DDX_Control(pDX, IDC_USER_CHECK19, m_CButton_UserCheck19);
	DDX_Control(pDX, IDC_USER_CHECK20, m_CButton_UserCheck20);
	DDX_Control(pDX, IDC_USER_CHECK21, m_CButton_UserCheck21);
	DDX_Control(pDX, IDC_USER_CHECK22, m_CButton_UserCheck22);
	DDX_Control(pDX, IDC_USER_CHECK23, m_CButton_UserCheck23);
	DDX_Control(pDX, IDC_USER_CHECK24, m_CButton_UserCheck24);
	DDX_Control(pDX, IDC_USER_CHECK25, m_CButton_UserCheck25);
	DDX_Control(pDX, IDC_USER_CHECK26, m_CButton_UserCheck26);
	DDX_Control(pDX, IDC_USER_CHECK27, m_CButton_UserCheck27);
	DDX_Control(pDX, IDC_USER_CHECK28, m_CButton_UserCheck28);
	DDX_Control(pDX, IDC_USER_CHECK29, m_CButton_UserCheck29);
	DDX_Control(pDX, IDC_USER_CHECK30, m_CButton_UserCheck30);
	DDX_Control(pDX, IDC_USER_CHECK31, m_CButton_UserCheck31);
	DDX_Control(pDX, IDC_USER_EDIT1, m_CEdit_UserEdit1);
	DDX_Control(pDX, IDC_USER_EDIT2, m_CEdit_UserEdit2);
	DDX_Control(pDX, IDC_USER_EDIT3, m_CEdit_UserEdit3);
	DDX_Control(pDX, IDC_USER_EDIT4, m_CEdit_UserEdit4);
	DDX_Control(pDX, IDC_USER_EDIT5, m_CEdit_UserEdit5);
	DDX_Control(pDX, IDC_USER_EDIT6, m_CEdit_UserEdit6);
	DDX_Control(pDX, IDC_USER_EDIT7, m_CEdit_UserEdit7);
	DDX_Control(pDX, IDC_USER_EDIT8, m_CEdit_UserEdit8);
	DDX_Control(pDX, IDC_USER_EDIT9, m_CEdit_UserEdit9);
	DDX_Control(pDX, IDC_USER_EDIT10, m_CEdit_UserEdit10);
	DDX_Control(pDX, IDC_USER_EDIT11, m_CEdit_UserEdit11);
	DDX_Control(pDX, IDC_USER_EDIT12, m_CEdit_UserEdit12);
	DDX_Control(pDX, IDC_USER_EDIT13, m_CEdit_UserEdit13);
	DDX_Control(pDX, IDC_USER_EDIT14, m_CEdit_UserEdit14);
	DDX_Control(pDX, IDC_USER_EDIT15, m_CEdit_UserEdit15);
	DDX_Control(pDX, IDC_USER_EDIT16, m_CEdit_UserEdit16);
	DDX_Control(pDX, IDC_USER_EDIT17, m_CEdit_UserEdit17);
	DDX_Control(pDX, IDC_USER_EDIT18, m_CEdit_UserEdit18);
	DDX_Control(pDX, IDC_USER_EDIT19, m_CEdit_UserEdit19);
	DDX_Control(pDX, IDC_USER_EDIT20, m_CEdit_UserEdit20);
	DDX_Control(pDX, IDC_USER_EDIT21, m_CEdit_UserEdit21);
	DDX_Control(pDX, IDC_USER_EDIT22, m_CEdit_UserEdit22);
	DDX_Control(pDX, IDC_USER_EDIT23, m_CEdit_UserEdit23);
	DDX_Control(pDX, IDC_USER_EDIT24, m_CEdit_UserEdit24);
}


BEGIN_MESSAGE_MAP(CDlgUserDef, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_USER_OK, &CDlgUserDef::OnBnClickedButtonUserOk)
END_MESSAGE_MAP()


// CDlgUserDef 消息处理程序


BOOL CDlgUserDef::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化
	if (m_UINT8_OpenMode)
	{
		SetWindowText(_T("自定义参数-修改"));
		GetDlgItem(IDC_BUTTON_USER_OK)->SetWindowTextW(_T("保 存"));
	}
	m_CButton_UserCheck0.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[0]);
	m_CButton_UserCheck1.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[1]);
	m_CButton_UserCheck2.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[2]);
	m_CButton_UserCheck3.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[3]);
	m_CButton_UserCheck4.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[4]);
	m_CButton_UserCheck5.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[5]);
	m_CButton_UserCheck6.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[6]);
	m_CButton_UserCheck7.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[7]);
	m_CButton_UserCheck8.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[8]);
	m_CButton_UserCheck9.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[9]);
	m_CButton_UserCheck10.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[10]);
	m_CButton_UserCheck11.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[11]);
	m_CButton_UserCheck12.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[12]);
	m_CButton_UserCheck13.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[13]);
	m_CButton_UserCheck14.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[14]);
	m_CButton_UserCheck15.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[15]);
	m_CButton_UserCheck16.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[16]);
	m_CButton_UserCheck17.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[17]);
	m_CButton_UserCheck18.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[18]);
	m_CButton_UserCheck19.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[19]);
	m_CButton_UserCheck20.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[20]);
	m_CButton_UserCheck21.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[21]);
	m_CButton_UserCheck22.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[22]);
	m_CButton_UserCheck23.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[23]);
	m_CButton_UserCheck24.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[24]);
	m_CButton_UserCheck25.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[25]);
	m_CButton_UserCheck26.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[26]);
	m_CButton_UserCheck27.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[27]);
	m_CButton_UserCheck28.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[28]);
	m_CButton_UserCheck29.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[29]);
	m_CButton_UserCheck30.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[30]);
	m_CButton_UserCheck31.SetCheck(g_CDriverParam_Main.m_UINT8_UserDef[31]);


	m_CString_UserEdit[0].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[0]);
	m_CEdit_UserEdit1.SetWindowTextW(m_CString_UserEdit[0]);

	m_CString_UserEdit[1].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[1]);
	m_CEdit_UserEdit2.SetWindowTextW(m_CString_UserEdit[1]);

	m_CString_UserEdit[2].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[2]);
	m_CEdit_UserEdit3.SetWindowTextW(m_CString_UserEdit[2]);

	m_CString_UserEdit[3].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[3]);
	m_CEdit_UserEdit4.SetWindowTextW(m_CString_UserEdit[3]);

	m_CString_UserEdit[4].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[4]);
	m_CEdit_UserEdit5.SetWindowTextW(m_CString_UserEdit[4]);

	m_CString_UserEdit[5].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[5]);
	m_CEdit_UserEdit6.SetWindowTextW(m_CString_UserEdit[5]);

	m_CString_UserEdit[6].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[6]);
	m_CEdit_UserEdit7.SetWindowTextW(m_CString_UserEdit[6]);

	m_CString_UserEdit[7].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[7]);
	m_CEdit_UserEdit8.SetWindowTextW(m_CString_UserEdit[7]);

	m_CString_UserEdit[8].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[8]);
	m_CEdit_UserEdit9.SetWindowTextW(m_CString_UserEdit[8]);

	m_CString_UserEdit[9].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[9]);
	m_CEdit_UserEdit10.SetWindowTextW(m_CString_UserEdit[9]);

	m_CString_UserEdit[10].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[10]);
	m_CEdit_UserEdit11.SetWindowTextW(m_CString_UserEdit[10]);

	m_CString_UserEdit[11].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[11]);
	m_CEdit_UserEdit12.SetWindowTextW(m_CString_UserEdit[11]);

	m_CString_UserEdit[12].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[12]);
	m_CEdit_UserEdit13.SetWindowTextW(m_CString_UserEdit[12]);

	m_CString_UserEdit[13].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[13]);
	m_CEdit_UserEdit14.SetWindowTextW(m_CString_UserEdit[13]);

	m_CString_UserEdit[14].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[14]);
	m_CEdit_UserEdit15.SetWindowTextW(m_CString_UserEdit[14]);

	m_CString_UserEdit[15].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[15]);
	m_CEdit_UserEdit16.SetWindowTextW(m_CString_UserEdit[15]);

	m_CString_UserEdit[16].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[16]);
	m_CEdit_UserEdit17.SetWindowTextW(m_CString_UserEdit[16]);

	m_CString_UserEdit[17].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[17]);
	m_CEdit_UserEdit18.SetWindowTextW(m_CString_UserEdit[17]);

	m_CString_UserEdit[18].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[18]);
	m_CEdit_UserEdit19.SetWindowTextW(m_CString_UserEdit[18]);

	m_CString_UserEdit[19].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[19]);
	m_CEdit_UserEdit20.SetWindowTextW(m_CString_UserEdit[19]);

	m_CString_UserEdit[20].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[20]);
	m_CEdit_UserEdit21.SetWindowTextW(m_CString_UserEdit[20]);

	m_CString_UserEdit[21].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[21]);
	m_CEdit_UserEdit22.SetWindowTextW(m_CString_UserEdit[21]);

	m_CString_UserEdit[22].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[22]);
	m_CEdit_UserEdit23.SetWindowTextW(m_CString_UserEdit[22]);

	m_CString_UserEdit[23].Format(_T("%d"), g_CDriverParam_Main.m_INT32_UserDef[23]);
	m_CEdit_UserEdit24.SetWindowTextW(m_CString_UserEdit[23]);


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 异常: OCX 属性页应返回 FALSE
}


void CDlgUserDef::OnBnClickedButtonUserOk()
{
	// TODO: 在此添加控件通知处理程序代码

	if (setUserDef())
		return;
	EndDialog(IDOK);
}


UINT8 CDlgUserDef::setUserDef()
{
	g_CDriverParam_Main.m_UINT8_UserDef[0] = m_CButton_UserCheck0.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[1] = m_CButton_UserCheck1.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[2] = m_CButton_UserCheck2.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[3] = m_CButton_UserCheck3.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[4] = m_CButton_UserCheck4.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[5] = m_CButton_UserCheck5.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[6] = m_CButton_UserCheck6.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[7] = m_CButton_UserCheck7.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[8] = m_CButton_UserCheck8.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[9] = m_CButton_UserCheck9.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[10] = m_CButton_UserCheck10.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[11] = m_CButton_UserCheck11.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[12] = m_CButton_UserCheck12.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[13] = m_CButton_UserCheck13.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[14] = m_CButton_UserCheck14.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[15] = m_CButton_UserCheck15.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[16] = m_CButton_UserCheck16.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[17] = m_CButton_UserCheck17.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[18] = m_CButton_UserCheck18.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[19] = m_CButton_UserCheck19.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[20] = m_CButton_UserCheck20.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[21] = m_CButton_UserCheck21.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[22] = m_CButton_UserCheck22.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[23] = m_CButton_UserCheck23.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[24] = m_CButton_UserCheck24.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[25] = m_CButton_UserCheck25.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[26] = m_CButton_UserCheck26.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[27] = m_CButton_UserCheck27.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[28] = m_CButton_UserCheck28.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[29] = m_CButton_UserCheck29.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[30] = m_CButton_UserCheck30.GetCheck();
	g_CDriverParam_Main.m_UINT8_UserDef[31] = m_CButton_UserCheck31.GetCheck();

	m_CEdit_UserEdit1.GetWindowTextW(m_CString_UserEdit[0]);
	m_CEdit_UserEdit2.GetWindowTextW(m_CString_UserEdit[1]);
	m_CEdit_UserEdit3.GetWindowTextW(m_CString_UserEdit[2]);
	m_CEdit_UserEdit4.GetWindowTextW(m_CString_UserEdit[3]);
	m_CEdit_UserEdit5.GetWindowTextW(m_CString_UserEdit[4]);
	m_CEdit_UserEdit6.GetWindowTextW(m_CString_UserEdit[5]);
	m_CEdit_UserEdit7.GetWindowTextW(m_CString_UserEdit[6]);
	m_CEdit_UserEdit8.GetWindowTextW(m_CString_UserEdit[7]);
	m_CEdit_UserEdit9.GetWindowTextW(m_CString_UserEdit[8]);
	m_CEdit_UserEdit10.GetWindowTextW(m_CString_UserEdit[9]);
	m_CEdit_UserEdit11.GetWindowTextW(m_CString_UserEdit[10]);
	m_CEdit_UserEdit12.GetWindowTextW(m_CString_UserEdit[11]);
	m_CEdit_UserEdit13.GetWindowTextW(m_CString_UserEdit[12]);
	m_CEdit_UserEdit14.GetWindowTextW(m_CString_UserEdit[13]);
	m_CEdit_UserEdit15.GetWindowTextW(m_CString_UserEdit[14]);
	m_CEdit_UserEdit16.GetWindowTextW(m_CString_UserEdit[15]);
	m_CEdit_UserEdit17.GetWindowTextW(m_CString_UserEdit[16]);
	m_CEdit_UserEdit18.GetWindowTextW(m_CString_UserEdit[17]);
	m_CEdit_UserEdit19.GetWindowTextW(m_CString_UserEdit[18]);
	m_CEdit_UserEdit20.GetWindowTextW(m_CString_UserEdit[19]);
	m_CEdit_UserEdit21.GetWindowTextW(m_CString_UserEdit[20]);
	m_CEdit_UserEdit22.GetWindowTextW(m_CString_UserEdit[21]);
	m_CEdit_UserEdit23.GetWindowTextW(m_CString_UserEdit[22]);
	m_CEdit_UserEdit24.GetWindowTextW(m_CString_UserEdit[23]);

	g_CDriverParam_Main.m_INT32_UserDef[0] = _ttoi(m_CString_UserEdit[0]);
	g_CDriverParam_Main.m_INT32_UserDef[1] = _ttoi(m_CString_UserEdit[1]);
	g_CDriverParam_Main.m_INT32_UserDef[2] = _ttoi(m_CString_UserEdit[2]);
	g_CDriverParam_Main.m_INT32_UserDef[3] = _ttoi(m_CString_UserEdit[3]);
	g_CDriverParam_Main.m_INT32_UserDef[4] = _ttoi(m_CString_UserEdit[4]);
	g_CDriverParam_Main.m_INT32_UserDef[5] = _ttoi(m_CString_UserEdit[5]);
	g_CDriverParam_Main.m_INT32_UserDef[6] = _ttoi(m_CString_UserEdit[6]);
	g_CDriverParam_Main.m_INT32_UserDef[7] = _ttoi(m_CString_UserEdit[7]);
	g_CDriverParam_Main.m_INT32_UserDef[8] = _ttoi(m_CString_UserEdit[8]);
	g_CDriverParam_Main.m_INT32_UserDef[9] = _ttoi(m_CString_UserEdit[9]);
	g_CDriverParam_Main.m_INT32_UserDef[10] = _ttoi(m_CString_UserEdit[10]);
	g_CDriverParam_Main.m_INT32_UserDef[11] = _ttoi(m_CString_UserEdit[11]);
	g_CDriverParam_Main.m_INT32_UserDef[12] = _ttoi(m_CString_UserEdit[12]);
	g_CDriverParam_Main.m_INT32_UserDef[13] = _ttoi(m_CString_UserEdit[13]);
	g_CDriverParam_Main.m_INT32_UserDef[14] = _ttoi(m_CString_UserEdit[14]);
	g_CDriverParam_Main.m_INT32_UserDef[15] = _ttoi(m_CString_UserEdit[15]);
	g_CDriverParam_Main.m_INT32_UserDef[16] = _ttoi(m_CString_UserEdit[16]);
	g_CDriverParam_Main.m_INT32_UserDef[17] = _ttoi(m_CString_UserEdit[17]);
	g_CDriverParam_Main.m_INT32_UserDef[18] = _ttoi(m_CString_UserEdit[18]);
	g_CDriverParam_Main.m_INT32_UserDef[19] = _ttoi(m_CString_UserEdit[19]);
	g_CDriverParam_Main.m_INT32_UserDef[20] = _ttoi(m_CString_UserEdit[20]);
	g_CDriverParam_Main.m_INT32_UserDef[21] = _ttoi(m_CString_UserEdit[21]);
	g_CDriverParam_Main.m_INT32_UserDef[22] = _ttoi(m_CString_UserEdit[22]);
	g_CDriverParam_Main.m_INT32_UserDef[23] = _ttoi(m_CString_UserEdit[23]);

	return 0;
}