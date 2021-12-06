#pragma once


// CDlgUserDef 对话框

class CDlgUserDef : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserDef)

public:
	CDlgUserDef(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgUserDef();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_USER_DEF };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CButton m_CButton_UserCheck0;
	CButton m_CButton_UserCheck1;
	CButton m_CButton_UserCheck2;
	CButton m_CButton_UserCheck3;
	CButton m_CButton_UserCheck4;
	CButton m_CButton_UserCheck5;
	CButton m_CButton_UserCheck6;
	CButton m_CButton_UserCheck7;
	CButton m_CButton_UserCheck8;
	CButton m_CButton_UserCheck9;
	CButton m_CButton_UserCheck10;
	CButton m_CButton_UserCheck11;
	CButton m_CButton_UserCheck12;
	CButton m_CButton_UserCheck13;
	CButton m_CButton_UserCheck14;
	CButton m_CButton_UserCheck15;
	CButton m_CButton_UserCheck16;
	CButton m_CButton_UserCheck17;
	CButton m_CButton_UserCheck18;
	CButton m_CButton_UserCheck19;
	CButton m_CButton_UserCheck20;
	CButton m_CButton_UserCheck21;
	CButton m_CButton_UserCheck22;
	CButton m_CButton_UserCheck23;
	CButton m_CButton_UserCheck24;
	CButton m_CButton_UserCheck25;
	CButton m_CButton_UserCheck26;
	CButton m_CButton_UserCheck27;
	CButton m_CButton_UserCheck28;
	CButton m_CButton_UserCheck29;
	CButton m_CButton_UserCheck30;
	CButton m_CButton_UserCheck31;
	CEdit m_CEdit_UserEdit1;
	CEdit m_CEdit_UserEdit2;
	CEdit m_CEdit_UserEdit3;
	CEdit m_CEdit_UserEdit4;
	CEdit m_CEdit_UserEdit5;
	CEdit m_CEdit_UserEdit6;
	CEdit m_CEdit_UserEdit7;
	CEdit m_CEdit_UserEdit8;
	CEdit m_CEdit_UserEdit9;
	CEdit m_CEdit_UserEdit10;
	CEdit m_CEdit_UserEdit11;
	CEdit m_CEdit_UserEdit12;
	CEdit m_CEdit_UserEdit13;
	CEdit m_CEdit_UserEdit14;
	CEdit m_CEdit_UserEdit15;
	CEdit m_CEdit_UserEdit16;
	CEdit m_CEdit_UserEdit17;
	CEdit m_CEdit_UserEdit18;
	CEdit m_CEdit_UserEdit19;
	CEdit m_CEdit_UserEdit20;
	CEdit m_CEdit_UserEdit21;
	CEdit m_CEdit_UserEdit22;
	CEdit m_CEdit_UserEdit23;
	CEdit m_CEdit_UserEdit24;
	virtual BOOL OnInitDialog();

	UINT8 m_UINT8_OpenMode;
	CString m_CString_UserEdit[24];
	afx_msg void OnBnClickedButtonUserOk();
	UINT8 setUserDef();
};
