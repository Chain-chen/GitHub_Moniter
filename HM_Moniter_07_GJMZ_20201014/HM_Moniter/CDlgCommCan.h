#pragma once


// CDlgCommCan 对话框

class CDlgCommCan : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgCommCan)

public:
	CDlgCommCan(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgCommCan();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMM_CAN };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();

	UINT8 m_UINT8_isParamChanged;

	CString m_CString_setCanAddress;
	CEdit m_CEdit_setCanAddress;
	UINT8 m_UINT8_setCanAddress;
	CComboBox m_CComboBox_setCanBaudrate;
	afx_msg void OnBnClickedButtonCanCancel();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonCanConfirm();
	UINT8 setCan();
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
