#pragma once



class CSPortComm
{
public:
	CSPortComm();
	~CSPortComm();
public:

	HWND mainWind;

	BOOL CommOpened;
	HANDLE SPortComm;
	HANDLE CommReceiveThread;
	DCB CommDCB;
	OVERLAPPED OverlappedRead, OverlappedWrite;
	COMSTAT CommStat;
	DWORD CommErrorFlag;

	BYTE CommQuantity;
	BYTE CommArray[256];
	//读取的数据
	BYTE DataReadArray[1024];
	//读取的数据长度
	WORD DataReadIndex;  
	static const BYTE auchCRCHi[256];
	static const BYTE auchCRCLo[256];


	CRITICAL_SECTION g_CriticalSection;

public:
	void SearchComm();
	BOOL OpenComm(UINT CommNumberIndex);
	BOOL CloseComm();
	static DWORD __stdcall ReceiveThread(LPVOID pParam);
	DWORD SendData(BYTE *pToSendChar, BYTE CharLength);

	WORD CRC16(BYTE *pByteArray, BYTE Length);


};

