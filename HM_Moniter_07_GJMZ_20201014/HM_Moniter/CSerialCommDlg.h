#pragma once

#include "CSPortComm.h"

// CSerialCommDlg 对话框

class CSerialCommDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSerialCommDlg)

public:
	CSerialCommDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CSerialCommDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SERIALPORT };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	CSPortComm m_CSPortComm_Main;







	CComboBox m_CComboBox_CommNumber;
	CComboBox m_CComboBox_Baudrate;
	CComboBox m_CComboBox_ParityBit;
	CComboBox m_CComboBox_StopBit;
	afx_msg void OnBnClickedButtonSeachusablecomm();
	afx_msg void OnBnClickedButtonOpencomm();
	CButton m_CButton_SearchComm;
	CButton m_CButton_OpenComm;
};
