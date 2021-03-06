#pragma once

#include "CTChart.h"

#include "CDlgFileLoopTest.h"
#include "CDlgFileLoopChart.h"


// CDlgFileNewPID 对话框

class CDlgFileNewPID : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileNewPID)

public:
	CDlgFileNewPID(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileNewPID();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_NEW_LOOP };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnUserFilenewpidRx(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
public:

	CDlgFileLoopTest m_CDialog_FileLoopFlag;
	CDlgFileLoopChart m_CDialog_FileLoopChart;


	//类型 定义
	typedef	union UN_Param
	{
		UINT32 param_int;
		FLOAT param_float;
	}unParam;

	afx_msg void OnBnClickedButtonCancel();

	//UINT8 getRegValue(WORD reg, BYTE reglength);
	//UINT8 setRegValue(WORD reg, BYTE reglength);
	BYTE m_BYTE_DataTxArray[256];
	BYTE m_BYTE_OpenMode;				//0：新建 1：打开
	UINT8 m_UINT8_isParamChanged;
	BYTE m_BYTE_setMotorEnabled;
	UINT8 m_BYTE_setGetStatus;

	INT32 m_INT32_setVelocityTestVelocity;
	FLOAT m_FLOAT_setVelocityTestVelocity;
	FLOAT m_FLOAT_setVelocityTestSpeed;



	UINT8 processDataRxRead(UINT16 regword);

	//unParam m_unParam_setCurrentLoopP;
	//unParam m_unParam_setCurrentLoopI;
	//unParam m_unParam_setCurrentLoopD;
	//unParam m_unParam_setVelocityLoopP;
	//unParam m_unParam_setVelocityLoopI;
	//unParam m_unParam_setVelocityLoopD;
	//unParam m_unParam_setPositionLoopP;
	//unParam m_unParam_setPositionLoopI;
	//unParam m_unParam_setPositionLoopD;
	//电流环 变量、按键

	FLOAT m_FLOAT_setCurrentLoopP;
	FLOAT m_FLOAT_setCurrentLoopI;
	FLOAT m_FLOAT_setCurrentLoopD;
	FLOAT m_FLOAT_setVelocityLoopP;
	FLOAT m_FLOAT_setVelocityLoopI;
	FLOAT m_FLOAT_setVelocityLoopD;
	FLOAT m_FLOAT_setPositionLoopP;
	FLOAT m_FLOAT_setPositionLoopI;
	FLOAT m_FLOAT_setPositionLoopD;

	UINT8 m_UINT8_setCurrentTestType;
	unParam m_unParam_setCurrentTestAmplitude;
	UINT16 m_UINT16_setCurrentTestFrequency;
	afx_msg void OnCbnSelchangeCombosetcurrenttesttype();
	afx_msg void OnBnClickedButtonsetcurrenttest();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	BYTE processErrorCode(BYTE ErroCode);

	//波形显示
	CRect m_CRect_StaticChartDisplay;
	CRect m_CRect_Chart;
	CRect m_CRect_Chart1;
	BYTE setTChartPosition();

	WORD m_WORD_LoopTestLength;

	BYTE * m_BYTE_pTestData;
	UINT m_BYTE_TestDataIndex;
//	CByteArray m_CByteArray_TestDataGiven;
//	CByteArray m_CByteArray_TestDataActual;

	WORD m_WORD_ChartDataLenth;
	CTChart m_CTChart_Loop;
	CTChart m_CTChart_Loop1;
	CSeries m_CSeries_Data1;
	CSeries m_CSeries_Data2;
	CSeries m_CSeries_Data11;
	CSeries m_CSeries_Data12;

	afx_msg void OnBnClickedButtonFilelooppreviousstep();
	virtual void OnCancel();
	afx_msg void OnBnClickedButtonFileloopnextstep();
	afx_msg void OnBnClickedButtonsetvelocitytest();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	
	CEdit m_CEdit_setCurrentLoopP;
	CEdit m_CEdit_setCurrentLoopI;
	CEdit m_CEdit_setCurrentLoopD;
	CEdit m_CEdit_setVelocityLoopP;
	CEdit m_CEdit_setVelocityLoopI;
	CEdit m_CEdit_setVelocityLoopD;
	CEdit m_CEdit_setPositionLoopP;
	CEdit m_CEdit_setPositionLoopI;
	CEdit m_CEdit_setPositionLoopD;

	CString m_CString_setCurrentLoopP;
	CString m_CString_setCurrentLoopI;
	CString m_CString_setCurrentLoopD;
	CString m_CString_setVelocityLoopP;
	CString m_CString_setVelocityLoopI;
	CString m_CString_setVelocityLoopD;
	CString m_CString_setPositionLoopP;
	CString m_CString_setPositionLoopI;
	CString m_CString_setPositionLoopD;

	CComboBox m_CComboBox_setCurrentTestType;
	CEdit m_CEdit_setCurrentTestAmplitude;
	CEdit m_CEdit_setCurrentTestFrequency;
	CEdit m_CEdit_setVelocityTestVelocity;

	CString m_CString_setCurrentTestAmplitude;
	CString m_CString_setCurrentTestFrequency;
	CString m_CString_setVelocityTestVelocity;


	UINT8 setCurrentLoop();
	UINT8 setCurrentLoopTest();
	UINT8 setVelocityLoop();
	UINT8 setVelocityLoopTest();
	UINT8 setPositionLoop();
	UINT8 setPositionLoopTest();
	UINT8 getCurrentLoop();
	UINT8 getVelocityLoop();
	UINT8 getPositionLoop();


	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_CButton_toggleVelocityTestVelocityUnit;
	afx_msg void OnBnClickedChecktogglevelocitytestvelocityunit();
	CStatic m_CStatic_setVelocityTestVelocityUnit;
	afx_msg void OnBnClickedButtonsetpositiontest();
	CEdit m_CEdit_setPositionTestSpeed;
	CEdit m_CEdit_setPositionTestPosition;
	CEdit m_CEdit_setPositionTestAcceleration;
	CEdit m_CEdit_setPositionTestDeceleration;
	CString m_CString_setPositionTestSpeed;
	CString m_CString_setPositionTestPosition;
	CString m_CString_setPositionTestAcceleration;
	CString m_CString_setPositionTestDeceleration;
	INT32 m_INT32_setPositionTestVelocity;
	FLOAT m_FLOAT_setPositionTestVelocity;
	FLOAT m_FLOAT_setPositionTestSpeed;
	INT32 m_INT32_setPositionTestPosition;
	INT32 m_INT32_setPositionTestAcceleration;
	INT32 m_INT32_setPositionTestDeceleration;

	CStatic m_CStatic_setPositionTestVelocityUnit;
	virtual BOOL OnCommand(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedButtonsavetestdata();
	afx_msg void OnBnClickedButtonsavetestdata2();
	afx_msg void OnBnClickedButtonsavetestdata3();
};
