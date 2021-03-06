#pragma once

#include "CDlgFileMotorMatch.h"



// CDlgFileNewMotor 对话框

class CDlgFileNewMotor : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileNewMotor)

public:
	CDlgFileNewMotor(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileNewMotor();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_MOTOR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()







public:

	CDlgFileMotorMatch m_CDialog_FileMotorMatch;

	 //类型定义
	typedef	union UN_Param
	{
		DWORD param_int;
		FLOAT param_float;
	}unParam;

	//控件变量定义



	UINT8 m_UINT8_setPolePairs;
	UINT8 m_UINT8_setEncoderType;
	UINT16 m_UINT16_setEncoderResolution;
	//UINT m_UINT_setRatedSpeed;
	unParam m_unParam_setRatedCurrent;
	unParam m_unParam_setStallCurrent;
	INT32 m_INT32_setRatedVelocity;
	//FLOAT m_FLOAT_setRatedVelocity;
	//FLOAT m_FLOAT_setRatedSpeed;
	INT32 m_INT32_setRatedSpeed;



	//串口 相关
	BYTE m_BYTE_DataTxArray[256];
	UINT8 processDataRxRead(UINT16 reg_word);

	//其他
	BYTE m_BYTE_setMotorEnabled;
	BYTE m_BYTE_OpenMode;
	BYTE m_BYTE_SelfMatched;
	UINT8 m_BYTE_setGetStatus;

	UINT8 m_UINT8_isParamChanged;
	UINT8 m_UINT8_isLegalParam;
	UINT8 m_UINT8_isOverLimit;

	//匹配参数
	BYTE m_BYTE_getMatchFlag;
	BYTE m_BYTE_getPhaseOrder;
	BYTE m_BYTE_getHallOrder;
	BYTE m_BYTE_getHallDirection;
	BYTE m_BYTE_getEncoderDirection;
	WORD m_WORD_getDefaultDegree;




	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedButtonSelfmatch();
	afx_msg void OnBnClickedButtonCancel();


	afx_msg void OnTimer(UINT_PTR nIDEvent);



	//UINT8 getRegValue(WORD reg, BYTE reglength);
	//UINT8 setRegValue(WORD reg, BYTE reglength);

	//BYTE HexToArray(BYTE *pArray, UINT data, BYTE length);
	//BYTE ArrayToHex(BYTE *pArray, UINT *data, BYTE length);

protected:
	afx_msg LRESULT OnUserFilenewmotorRx(WPARAM wParam, LPARAM lParam);
public:
	INT m_INT_getMotorMatchFlag;
	afx_msg void OnBnClickedButtonFilemotornextstep();
	virtual void OnCancel();

	CEdit m_CEdit_setPolePairs;
	CComboBox m_CComboBox_setEncorderType;
	CEdit m_CEdit_setEncoderResolution;
	CEdit m_CEdit_setRatedCurrent;
	CEdit m_CEdit_setStallCurrent;
	CEdit m_CEdit_setRatedSpeed;
	CComboBox m_CComboBox_getMotorMatchFlag;

	CString m_CString_setRatedCurrent;
	CString m_CString_setStallCurrent;
	CString m_CString_setRatedSpeed;
	CString m_CString_setPolePairs;
	CString m_CString_setEncoderResolution;
	CString m_CString_setPrevious;

	UINT8 setMotorPoles();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnEnKillfocusEditsetmotorstallcurrent();
	UINT8 isLegalParam();

	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEditsetmotorratedcurrent();
	afx_msg void OnEnKillfocusEditsetpolepairs();
	CComboBox m_CComboBox_setHallOrder;
};


