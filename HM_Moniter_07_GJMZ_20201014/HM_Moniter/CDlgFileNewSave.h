#pragma once


// CDlgFileNewSave 对话框

class CDlgFileNewSave : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileNewSave)

public:
	CDlgFileNewSave(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileNewSave();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_SAVE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	FLOAT m_FLOAT_setRatedVelocity;
	FLOAT m_FLOAT_setRatedSpeed;

	BYTE m_BYTE_DataTxArray[256];
	BYTE m_BYTE_ParamDataBuffer[256];

	CListBox m_CListBox_ParamList;
	CString m_CString_Temp;
	afx_msg void OnBnClickedButtonDownloadfile();
	UINT8 setRegValue(WORD reg, BYTE reglength);

protected:
	afx_msg LRESULT OnUserFilesaveRx(WPARAM wParam, LPARAM lParam);
public:
	int processDataRx();
	afx_msg void OnBnClickedButtonSavefile();
	afx_msg void OnBnClickedButtonOpenfile();
	afx_msg void OnBnClickedButtonFilesaveok();
	afx_msg void OnBnClickedButtonFilesavecancel();
	BYTE m_BYTE_OpenMode;

	UINT8 setParamListDisplay();



	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
