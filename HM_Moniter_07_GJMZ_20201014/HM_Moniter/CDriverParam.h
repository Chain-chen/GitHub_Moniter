#pragma once
class CDriverParam
{
public:
	CDriverParam();
	~CDriverParam();

	//类型定义
	typedef	union UN_Param
	{
		UINT32 param_int;
		FLOAT param_float;
	}unParam;


	UINT8 m_UINT8_RegisterErrorCode;

	//参数变量定义

	//通讯参数
	UINT8 m_UINT8_setCanBaudrate;
	UINT8 m_UINT8_setCanAddress;

	//电机参数
	UINT8 m_UINT8_setPolePairs;
	UINT8 m_UINT8_setEncoderType;
	UINT16 m_UINT16_setEncoderResolution;
	unParam m_unParam_setRatedCurrent;
	unParam m_unParam_setStallCurrent;
	UINT8 m_UINT8_setHallOrder;
	INT32 m_INT32_setRatedSpeed;				//转每分
	FLOAT m_FLOAT_setRatedSpeed;
	//INT32 m_INT32_setRatedVelocity;				//脉冲每秒
	//FLOAT m_FLOAT_setRatedVelocity;				//临时				
//	FLOAT m_FLOAT_setSpeedUnitScale;			// = R/m  /  p/s
	UINT32 m_UINT32_setPulsePerRevolution;		//每圈 脉冲数   P/s = P/r * r/m  / 60

	//匹配参数
	UINT8 m_UINT8_getMatchFlag;
	UINT8 m_UINT8_getPhaseOrder;
	UINT8 m_UINT8_getHallOrder;
	INT8 m_INT8_getHallDirection;
	INT8 m_INT8_getEncoderDirection;
	INT16 m_INT16_getDefaultDegree;

	//应用参数
	UINT8 m_UINT8_setMotionDirection;
	UINT8 m_UINT8_setInitialPosition;
	unParam m_unParam_setAppRatedCurrent;
	unParam m_unParam_setAppStallCurrent;
	INT32 m_INT32_setAppMaxVelocity;	//脉冲 每秒
	FLOAT m_FLOAT_setAppMaxSpeed;		//转每分
	INT32 m_INT32_setAppMaxSpeed;		//转每分 整型
	INT32 m_INT32_setAcceleration;
	INT32 m_INT32_setDeceleration;
	INT32 m_INT32_setStopDeceleration;
	INT32 m_INT32_setAppMaxPosition;
	INT32 m_INT32_setAppMinPosition;
	INT32 m_INT32_setInitialPosition;
	FLOAT m_FLOAT_setAppMaxAngle;
	FLOAT m_FLOAT_setAppMinAngle;


	//PID参数
	unParam m_unParam_setCurrentLoopP;
	unParam m_unParam_setCurrentLoopI;
	unParam m_unParam_setCurrentLoopD;
	unParam m_unParam_setVelocityLoopP;
	unParam m_unParam_setVelocityLoopI;
	unParam m_unParam_setVelocityLoopD;
	unParam m_unParam_setPositionLoopP;
	unParam m_unParam_setPositionLoopI;
	unParam m_unParam_setPositionLoopD;

	//PID测试

	UINT8 m_UINT8_setCurrentTestType;
	unParam m_unParam_setCurrentTestAmplitude;
	UINT16 m_UINT16_setCurrentTestFrequency;

	UINT8 m_UINT8_getCurrentTestResult;
	UINT16 m_UINT16_getCurrentTestLength;

	INT32 m_INT32_setVelocityTestVelocity;
	FLOAT m_FLOAT_setVelocityTestVelocity;
	UINT8 m_UINT8_getVelocityTestResult;
	UINT16 m_UINT16_getVelocityTestLength;

	INT32 m_INT32_setPositionTestSpeed;
	INT32 m_INT32_setPositionTestPosition;
	FLOAT m_FLOAT_setPositionTestAngle;
	INT32 m_INT32_setPositionTestAcceleration;
	INT32 m_INT32_setPositionTestDeceleration;
	UINT8 m_UINT8_getPositionTestResult;
	UINT16 m_UINT16_getPositionTestLength;


	UINT8 m_UINT8_getLoopTestData[256];
	UINT8 m_UINT8_pTestData[0x2000];


	//固有参数
	unParam m_unParam_getDriverMinVoltage;
	unParam m_unParam_getDriverMaxVoltage;
	unParam m_unParam_getDriverRatedCurrent;
	unParam m_unParam_getDriverStallCurrent;
	INT32 m_INT32_getDriverMinVelocity;
	INT32 m_INT32_getDriverMaxVelocity;
	FLOAT m_FLOAT_getDriverMaxSpeed;
	FLOAT m_FLOAT_getDriverMinSpeed;


	//设备信息
	CString m_CString_getDeviceInfo;
	CString m_CString_getDeviceType;



	UINT8 m_UINT8_getStatusFlag;
	UINT8 m_UINT8_getStatusDevice;
	UINT8 m_UINT8_getStatusMotor;
	INT32 m_INT32_getStatusVelocity;
	FLOAT m_FLOAT_getStatusVelocity;
	INT32 m_INT32_getStatusPosition;
	unParam m_unParam_getStatusPhaseCurrent;
	unParam m_unParam_getStatusBusVoltage;
	UINT16 m_UINT16_getStatusError;
	UINT16 m_UINT16_getStatusErrorBackup;

	INT32 m_UINT32_getStatusJointPosition;



	DOUBLE m_DOUBLE_isLegalParam;

	UINT8 Hex32ToArray(UINT8 * pArray, UINT32 data);
	UINT8 Hex16ToArray(UINT8 * pArray, UINT16 data);
	UINT8 Hex8ToArray(UINT8 * pArray, UINT8 data);
	UINT8 BitToArray(UINT8* pArray, UINT8* data, UINT8 position);
	UINT8 ArrayToHex32(UINT8 * pArray, UINT32 * pdata);
	UINT8 ArrayToHex32(UINT8 * pArray, INT32 * pdata);
	UINT8 ArrayToHex16(UINT8 * pArray, UINT16 * pdata);
	UINT8 ArrayToHex16(UINT8 * pArray, INT16 * pdata);
	UINT8 ArrayToHex8(UINT8 * pArray, UINT8 * pdata);
	UINT8 ArrayToHex8(UINT8 * pArray, INT8 * pdata);
	UINT8 ArrayToBit(UINT8* pArray, UINT8* pdata, UINT8 position);


	UINT8 isLegalString(CEdit * cedit, LPCTCH schar, DOUBLE min, DOUBLE max, DOUBLE cur);

	UINT8 isLegalString(/*CEdit * cedit,*/ CString* cstring, CString* errcstring, DOUBLE min, DOUBLE max);

	CString m_CString_isLegalString;
	CString m_CString_isLegalStringPrevious;
	CString m_CString_isLegalStringTemp;
	CString m_CString_isOverLimit;

//	UINT8 CDriverParam::isLegalString(CString* cstring);

	UINT8 toggleSpeedToVelocityUnit(FLOAT* speed, INT32* velocity);
	UINT8 toggleVelocityToSpeedUnit(INT32 * velocity, FLOAT * speed);
	FLOAT m_FLOAT_toggleUnitTemp;

	//自定义参数
	UINT8 m_UINT8_UserDef[32];
	INT32 m_INT32_UserDef[24];








};

