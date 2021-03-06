
// HM_MoniterDlg.h: 头文件


//#include "CDriver.h"
//#include "CCommunication.h"
#include "CSerialCommDlg.h"
#include "CDlgFileNewMotor.h"
#include "CDlgFileNewDriver.h"
#include "CDlgFileNewPID.h"
#include "CDlgFileNewSave.h"
#include "CDriverParam.h"
#include "CDlgCommCan.h"
#include "CDlgUserDef.h"

//图形控件
#include "CTChart.h"

//#include "CAxis.h"

#pragma once


// CHMMoniterDlg 对话框
class CHMMoniterDlg : public CDialogEx
{
// 构造
public:
	CHMMoniterDlg(CWnd* pParent = nullptr);	// 标准构造函数

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HM_MONITER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:

	//菜单 & 工具栏
	CMenu m_Menu_Main;
	CStatusBar m_StatusBar_Main;
	CRect m_CRect_MainClient;


//	UINT32 indicators[];
	afx_msg void OnMenuFileNew();
	afx_msg void OnMenuCommDisconnect();
	afx_msg void OnMenuMotor();
	afx_msg void OnMenuDriver();
	afx_msg void OnMenuLoop();
	afx_msg void OnMenuCommConnect();
	afx_msg void OnMenuFileOpen();


	//子 对话框
	CSerialCommDlg m_CDialog_SerialComm;			//通讯配置对话框
	CDlgFileNewMotor m_CDialog_FileNewMotor;
	CDlgFileNewDriver m_CDialog_FileNewDriver;
	CDlgFileNewPID m_CDialog_FileNewPID;
	CDlgFileNewSave m_CDialog_FileNewSave;
	CDlgCommCan m_CDialog_CDlgCommCan;
	CDlgUserDef m_CDialog_UserDef;

//	CMSComm m_Mscomm_Main;

	//串口 变量 方法
	HANDLE CommProcessThread;
	static DWORD __stdcall DataProcessThread(LPVOID pParam);
	BYTE m_BYTE_DataTxArray[256];
	//一条完整的数据帧
	BYTE m_BYTE_DataRxArray[1024];
	BYTE m_BYTE_DataRxLength;				//数据帧 长度
	BYTE m_BYTE_MoniterDataRx[256];
	BYTE Comm_DataRxProcess();

	//类型 定义
	typedef	union UN_Param
	{
		UINT32 param_int;
		FLOAT param_float;	
	}unParam;

	//typedef	struct ST_PIDParam
	//{
	//	unParam LoopP;
	//	unParam LoopI;
	//	unParam LoopD;
	//}stPIDParam;


	////变量 定义
	//stPIDParam CurrentPID;
	//stPIDParam VelocityPID;
	//stPIDParam PositionPID;

	//函数 定义
	UINT8 setRegValue(UINT16 reg,UINT8 reglength);
	UINT8 getRegValue(UINT16 reg, UINT8 reglength);

	//运动控制 控件及变量

	CEdit m_CEdit_setCurrent;

	CEdit m_CEdit_setSpeed;					//设定速度
	CString m_CString_setSpeed;
	INT32 m_INT32_setSpeed;
	FLOAT m_FLOAT_setSpeed;

	CString m_CString_setAngle;
	FLOAT m_FLOAT_setAngle;

	INT32 m_INT32_setRunAcceleration;
	CString m_CString_setRunAcceleration;

	BYTE m_BYTE_isMotorEnabled;

	CComboBox m_CComboBox_setWorkMode;
	UINT8 m_UINT8_setWorkMode;

	CComboBox m_CComboBox_setPositionType;
	UINT8 m_UINT8_setPositionType;
	INT32 m_INT32_setPosition;
	INT32 m_INT32_setPositionSpeed;
	unParam m_unParam_setCurrent;
	CString m_CString_setCurrent;

	afx_msg void OnCbnSelchangeCombosetworkmode();
	afx_msg void OnBnClickedButtonsetmotorenable();
	afx_msg void OnBnClickedButtonsetstart();
	afx_msg void OnBnClickedButtonsetstopnormal();
	afx_msg void OnBnClickedButtonsetstopemergency();

	//状态显示 控件 变量 消息
	CString m_CString_getDeviceInfo;
	CString m_CString_getDeviceType;

	UINT8 m_UINT8_getStatusDevice;
	UINT8 m_UINT8_getStatusMotor;
	INT32 m_INT32_getStatusVelocity;
	FLOAT m_FLOAT_getStatusVelocity;
	INT32 m_INT32_getStatusPosition;
	unParam m_unParam_getStatusBusVoltage;
	unParam m_unParam_getStatusPhaseCurrent;
	UINT16 m_UINT16_getStatusError;
	UINT16 m_UINT16_getStatusErrorBackup;

	INT32 m_INT32_setPositionCalibration;

	afx_msg void OnBnClickedCheckgetstatus();



	//波形显示
	UINT8 m_UINT8_setWaveChannel;
	CTChart m_CTChart_Main;
	CSeries m_CSeries_Data1;
	CSeries m_CSeries_Data2;
	CAxes m_CAxes_Axes1;
	CAxis m_CAxis_Axis1;
	CAxisTitle m_CAxisTitle_AxisTitle1;

	CComboBox m_CComboBox_setWaveChannel;

	//环路 测试 相关

	UINT16 m_UINT16_TestDataIndex;
	UINT16 m_UINT16_TestDataLength;



	//CComboBox m_CComboBox_setWaveChannel;
	//CComboBox m_CComboBox_setWaveChannel2;
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	afx_msg void OnBnClickedButtonClearwavedata();

protected:
	afx_msg LRESULT OnUserMoniterRx(WPARAM wParam, LPARAM lParam);
public:
	UINT8 processDataRxRead(UINT16 reg_word);
	UINT8 processDataRxWrite(UINT16 reg_word);
	UINT8 processErrorCode(UINT8 ErrorCode);


	afx_msg void OnCbnSelchangeCombosetwavechannel();

	BYTE isCorrectRegNum(WORD regaddr);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	BYTE ArrayToHex(BYTE * pArray, UINT * pdata, BYTE length);

	//打开 窗口
	UINT8 openDlgSerialComm();
	UINT8 openDlgFileMotor(UINT8 mode);
	UINT8 openDlgFileDriver(UINT8 mode);
	UINT8 openDlgFileLoop(UINT8 mode);
	UINT8 openDlgCommCan();
	UINT8 openDlgUserDef(UINT8 mode);

	UINT8 setCan();
	UINT8 setMotorPoles();
	UINT8 setApplication();
	UINT8 setCurrentLoop();
	UINT8 setVelocityLoop();
	UINT8 setPositionLoop();
	UINT8 setCurrentTest();
	UINT8 setVelocityTest();
	UINT8 getCurrentTestData();
	UINT8 getVelocityTestData();

	UINT8 getPositionTestData();

	//afx_msg void OnEnChangeEditsetspeed();
	afx_msg void OnBnClickedButtonsetmotorreverse();

	UINT16 CommandWord;

	//函数 定义
	UINT8 setWorkMode();
	UINT8 setCurrentMode(INT8 Direction);
	UINT8 setSpeedMode(INT8 Direction);
	UINT8 setPositionMode(INT8 Direction);
	UINT8 setCommand(UINT16 command);
	UINT8 setUserDef();

	CString m_CString_setPosition;
	CEdit m_CEdit_setPosition;
protected:
	afx_msg LRESULT OnUserMotorMatch(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnMenuFileDownload();
	afx_msg void OnMenuFileRestore();
	afx_msg void OnBnClickedChecktoggleunit();
	CButton m_CButton_toggleUnit;
	CStatic m_CStatic_setSpeedUnit;
	CStatic m_CStatic_getStatusVelocityUnit;
	afx_msg void OnBnClickedButtonClearerrorstatus();
protected:
	afx_msg LRESULT OnUserMotorNomatch(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCurrentloopTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserCurrentloopNotest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserVelocityloopTest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserVelocityloopNotest(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnUserPositionloopTest(WPARAM wParam, LPARAM lParam);
public:
	CButton m_CButton_getStatus;
	afx_msg void OnMenuFileSave();
	afx_msg void OnBnClickedButtoncalibrateposition();
	UINT8 setPositionTest();

	//输入检测 变量
	UINT8 m_UINT8_isLegalValue;
	CString m_CString_isLegalMessage;


	afx_msg void OnBnClickedButtonTestwavedisplay();
	afx_msg void OnMenuCommCan();
	CEdit m_CEdit_getStatusJointPosition;
	CEdit m_CEdit_getStatusJointDegree;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	CComboBox m_CComboBox_UsableSerialPort;
	afx_msg void OnBnClickedButtonSeachusablecomm();
	afx_msg void OnBnClickedButtonOpencomm();
	CButton m_CButton_SeachUsableComm;
	CButton m_CButton_OpenComm;
	afx_msg void OnBnClickedButtonsetcanparam();
	CEdit m_CEdit_setRunAcceleration;
	afx_msg void OnBnClickedButtonsavetestdata();
	CEdit m_CEdit_setAngle;
	afx_msg void OnEnKillfocusEditsetangle();
	CEdit m_CEdit_setCanAddress;
	CString m_CString_setCanAddress;
	CComboBox m_CComboBox_setCanBaudrate;
	afx_msg void OnMenuUserDef();
};


