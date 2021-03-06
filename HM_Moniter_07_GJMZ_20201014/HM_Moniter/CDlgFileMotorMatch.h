#pragma once


// CDlgFileMotorMatch 对话框

class CDlgFileMotorMatch : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileMotorMatch)

public:
	CDlgFileMotorMatch(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileMotorMatch();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_MOTOR_FLAG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:


	BYTE m_BYTE_MatchResult;



	CStatic m_CStatic_FileMotorMatch;
	afx_msg void OnBnClickedButtonFilelooptest();
	CButton m_CButton_FileMotorMatch;
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonFilemotormatch();
};
