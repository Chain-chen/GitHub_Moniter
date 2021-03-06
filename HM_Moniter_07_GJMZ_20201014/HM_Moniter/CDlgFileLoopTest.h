#pragma once


// CDlgFileLoopTest 对话框

class CDlgFileLoopTest : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileLoopTest)

public:
	CDlgFileLoopTest(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileLoopTest();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_LOOP_FLAG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CStatic m_CStatic_FileLoopFlag;
	BYTE m_BYTE_TestResult;
	afx_msg void OnBnClickedButtonFilelooptest();
	virtual BOOL OnInitDialog();
};
