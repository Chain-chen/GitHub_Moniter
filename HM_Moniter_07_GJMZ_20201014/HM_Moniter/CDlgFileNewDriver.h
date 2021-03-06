#pragma once


// CDlgFileNewDriver 对话框

class CDlgFileNewDriver : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileNewDriver)

public:
	CDlgFileNewDriver(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileNewDriver();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_DRIVER };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	UINT8 getRegValue(WORD reg, BYTE reglength);
	UINT8 setRegValue(WORD reg, BYTE reglength);
protected:
	afx_msg LRESULT OnUserFilenewdriverRx(WPARAM wParam, LPARAM lParam);
public:

	//类型定义
	typedef	union UN_Param
	{
		DWORD param_int;
		FLOAT param_float;
	}unParam;

	//控件变量定义
	unParam m_unParam_setAppRatedCurrent;
	unParam m_unParam_setAppStallCurrent;
	INT m_INT_setAppMaxVelocity;
	UINT m_UINT_setAcceleration;
	UINT m_UINT_setDeceleration;
	UINT m_UINT_setStopDeceleration;
	INT m_INT_setAppMaxPosition;
	INT m_INT_setAppMinPosition;
	INT m_INT_setMotionDirection;

	unParam m_unParam_getDriverRatedCurrent;
	unParam m_unParam_getDriverStallCurrent;
	INT m_INT_getDriverMaxVelocity;
	INT m_INT_getDriverMinVelocity;
	unParam m_unParam_getDriverMaxVoltage;
	unParam m_unParam_getDriverMinVoltage;

	//串口

	BYTE m_BYTE_DataTxArray[256];
	UINT8 processDataRxRead(UINT16 reg_word);

	UINT ArrayToHex(BYTE *pArray, BYTE length);
	BYTE HexToArray(BYTE *pArray,UINT data,BYTE length);

	BYTE m_BYTE_OpenMode;
	BYTE m_BYTE_setMotorEnabled;
	UINT8 m_BYTE_setGetStatus;

	UINT8 m_UINT8_isParamChanged;

	afx_msg void OnBnClickedButtonFiledriverpreviousstep();
	afx_msg void OnBnClickedButtonFiledrivernextstep();
	afx_msg void OnBnClickedButtonFiledrivercancel();
	virtual void OnCancel();
	
	CComboBox m_CComboBox_setMotionDirection;

	CEdit m_CEdit_setAppRatedCurrent;
	CEdit m_CEdit_setAppStallCurrent;
	CEdit m_CEdit_setAppMaxVelocity;
	CEdit m_CEdit_setAppMaxSpeed;
	CEdit m_CEdit_setAcceleration;
	CEdit m_CEdit_setDeceleration;
	CEdit m_CEdit_setStopDeceleration;
	CEdit m_CEdit_setAppMaxPosition;
	CEdit m_CEdit_setAppMinPosition;

	CEdit m_CEdit_getDriverMinVoltage;
	CEdit m_CEdit_getDriverMaxVoltage;
	CEdit m_CEdit_getDriverRatedCurrent;
	CEdit m_CEdit_getDriverStallCurrent;
	CEdit m_CEdit_getDriverMinVelocity;
	CEdit m_CEdit_getDriverMaxVelocity;

	CString m_CString_setAppRatedCurrent;
	CString m_CString_setAppStallCurrent;
	CString m_CString_setAppMaxVelocity;
	CString m_CString_setAppMaxSpeed;
	CString m_CString_setAcceleration;
	CString m_CString_setDeceleration;
	CString m_CString_setStopDeceleration;
	CString m_CString_setAppMaxPosition;
	CString m_CString_setAppMinPosition;
	CString m_CString_setInitialPosition;
	CString m_CString_setAppMaxAngle;
	CString m_CString_setAppMinAngle;

	CString m_CString_getDriverMinVoltage;
	CString m_CString_getDriverMaxVoltage;
	CString m_CString_getDriverRatedCurrent;
	CString m_CString_getDriverStallCurrent;
	CString m_CString_getDriverMinVelocity;
	CString m_CString_getDriverMaxVelocity;

	FLOAT m_FLOAT_setAppMaxSpeed;


	UINT8 setApplication();
	UINT8 getApplication();


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	CEdit m_CEdit_setInitialPosition;
	CButton m_CButton_setInitialPosition;
};
