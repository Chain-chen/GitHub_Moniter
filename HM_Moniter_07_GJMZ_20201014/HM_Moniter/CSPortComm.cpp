#include "stdafx.h"
#include "HM_Moniter.h"
#include "CSPortComm.h"

extern BYTE g_BYTE_CommFrameReceived;

extern BYTE g_BYTE_RunIndex;

CSPortComm::CSPortComm()
{
	DataReadIndex = 0;
}


CSPortComm::~CSPortComm()
{
}

void CSPortComm::SearchComm()
{
	HANDLE hCom;
	CString str;
	CommQuantity = 0;
	//	pCbx->SetCurSel(0);
	for (int i = 0; i < 256; i++)
	{
		str.Format(_T("\\\\.\\COM%d"), i + 1);
		hCom = CreateFile(str, 0, 0, 0, OPEN_EXISTING,//�����Ѿ����ڡ����豸���Ҫ��
							FILE_ATTRIBUTE_NORMAL,   //Ĭ������
							0);
		if (INVALID_HANDLE_VALUE != hCom)
		{
			CloseHandle(hCom);
			CommArray[CommQuantity++] = i + 1;
		}
	}
}

BOOL CSPortComm::OpenComm(UINT CommNumberIndex)
{
	//	HANDLE g_SerialPort_hCom;
	CString str;
	UINT CommNumber = CommArray[CommNumberIndex];
	//	g_CommQuantity = 0;
	//	m_ComboBox_CommNumber.ResetContent();
	//	pCbx->SetCurSel(0);
	//	for (int i = 0; i < 256; i++)
	//	{
	str.Format(_T("\\\\.\\COM%d"), CommNumber);
	SPortComm = CreateFile(str,GENERIC_READ | GENERIC_WRITE, 0, 0, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 0);
	if (INVALID_HANDLE_VALUE != SPortComm)
	{
		SetupComm(SPortComm, 1024, 1024); //���뻺����������������Ĵ�С����1024
		//	BOOL aaa = SetupComm(SPortComm, 1024, 1024); //���뻺����������������Ĵ�С����1024

		COMMTIMEOUTS TimeOuts;
		//�趨����ʱ
		//�ڶ�һ�����뻺���������ݺ���������������أ��������Ƿ������Ҫ����ַ�
		TimeOuts.ReadIntervalTimeout = MAXDWORD;
		TimeOuts.ReadTotalTimeoutMultiplier = 0;
		TimeOuts.ReadTotalTimeoutConstant = 0;
		//�趨д��ʱ
		TimeOuts.WriteTotalTimeoutMultiplier = 0;
		TimeOuts.WriteTotalTimeoutConstant = 0;
		SetCommTimeouts(SPortComm, &TimeOuts);				//���ó�ʱ

		//GetCommState(SPortComm, &CommDCB);
		//CommDCB.BaudRate = 9600; //������Ϊ9600
		//CommDCB.ByteSize = 8; //ÿ���ֽ���8λ
		//CommDCB.Parity = NOPARITY; //����żУ��λ
		//CommDCB.StopBits = ONE5STOPBITS;
		//	dcb.StopBits = TWOSTOPBITS; //����ֹͣλ
		SetCommState(SPortComm, &CommDCB);

		PurgeComm(SPortComm, PURGE_TXCLEAR | PURGE_RXCLEAR);

		SetCommMask(SPortComm, EV_RXCHAR);	//���ý����¼�
		InitializeCriticalSection(&g_CriticalSection);		//��ʼ���ٽ���
		mainWind = AfxGetApp()->GetMainWnd()->m_hWnd;
		CommReceiveThread = CreateThread(NULL, 0, ReceiveThread, this, 0, NULL);//���������߳�
		return TRUE;
	}
	else return FALSE;
}

BOOL CSPortComm::CloseComm()
{
	DWORD aa = WaitForSingleObject(CommReceiveThread, 1000);
	if (aa == WAIT_TIMEOUT)
	{
		return FALSE;
	}
	DeleteCriticalSection(&g_CriticalSection);	//ɾ���ٽ���
	CloseHandle(SPortComm);						//�رմ��ھ��
	return TRUE;
}



DWORD CSPortComm::ReceiveThread(LPVOID pParam)
{
	CSPortComm *pObj = (CSPortComm *)pParam;
	DWORD /*dwMask,*/ dwTrans = 0, dwError = 0, dwBytesRead = 0;
	BOOL bReadStat;

	BYTE tempDataArray[1024] = {0};

	memset(&pObj->OverlappedRead,0, sizeof(OVERLAPPED));
	pObj->OverlappedRead.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	while (pObj->CommOpened)
	{

		//if (!WaitCommEvent(pObj->SPortComm, &dwMask, &pObj->OverlappedRead))
		//{
		//	//			if (( dwError=GetLastError()) == ERROR_IO_PENDING)
		//	//			{
		//	dwError = WaitForSingleObject(pObj->OverlappedRead.hEvent, 1);			//�ȴ�1ms

		//	if (dwError != WAIT_OBJECT_0)
		//		continue;

		//	//if (dwError == WAIT_FAILED)
		//	//	dwError = GetLastError();
		//}

		//if (dwMask == EV_RXCHAR)
		//{
			//					ClearCommError(g_Handle_SerialPort, &dwErrorFlags, &ComStat);
			//					dwBytesRead = min(dwBytesRead, (DWORD)ComStat.cbInQue);
	//	g_BYTE_RunIndex = 0xF3;
	
		dwBytesRead = 1024;

		while (dwBytesRead > 0) 
		{
			ClearCommError(pObj->SPortComm, &pObj->CommErrorFlag, &pObj->CommStat);
			dwBytesRead = min(1024, pObj->CommStat.cbInQue);

			if (dwBytesRead > 0)
			{
				bReadStat = ReadFile(pObj->SPortComm,	//�ļ��ľ��
										tempDataArray,  //���ڱ���������ݵ�һ��������
										dwBytesRead,    //Ҫ������ֽ���
										&dwBytesRead,   //ָ��ʵ�ʶ�ȡ�ֽ�����ָ��
										&pObj->OverlappedRead);//���ļ���ʱָ����FILE_FLAG_OVERLAPPED����ô���룬�������������һ������Ľṹ��
																//�ýṹ������һ���첽��ȡ����������Ӧ�����������ΪNULL

				if (!bReadStat)
				{
					if (GetLastError() == ERROR_IO_PENDING)
						WaitForSingleObject(pObj->OverlappedRead.hEvent, 1000);
				}

				if (pObj->DataReadIndex + dwBytesRead > 1024)
				{
					AfxMessageBox(_T("�����������"), MB_ICONERROR);
					return 0;
				}

				else
				{
					EnterCriticalSection(&pObj->g_CriticalSection);
					memcpy(pObj->DataReadArray + pObj->DataReadIndex, tempDataArray, sizeof(BYTE)*dwBytesRead);
					pObj->DataReadIndex += (BYTE)dwBytesRead;
					LeaveCriticalSection(&pObj->g_CriticalSection);
					memset(tempDataArray, 0, sizeof(tempDataArray));
				}
			}
		}
		Sleep(1);
	}
	return 0;
}



DWORD CSPortComm::SendData(BYTE *pToSendChar, BYTE CharLength)
{
	//	CByteArray m_11;
	memset(&OverlappedWrite, 0, sizeof(OVERLAPPED));
//	OverlappedRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	BYTE * lpOutBuffer = pToSendChar;
	DWORD dwBytesWrite = CharLength;
	BOOL bWriteStat;

//	ResetEvent(m_Handle_ReceiveEvent);

	ClearCommError(SPortComm, &CommErrorFlag, &CommStat);    //������ڴ�����ȡ�������ڵ�״̬
	//������д��һ���ļ�����I / O�豸�����óɹ�,���ط�0��ʧ�ܷ���0
	bWriteStat = WriteFile(SPortComm,    //�ļ����
						   lpOutBuffer,   //���ݻ�����ָ��
						   dwBytesWrite,   //��Ҫд���ֽ���
						   &dwBytesWrite, //���ڱ���ʵ��д���ֽ����Ĵ洢�����ָ��
						   &OverlappedWrite);//OVERLAPPED�ṹ��ָ��

	if (!bWriteStat)
	{
		if (GetLastError() == ERROR_IO_PENDING)
		{
			WaitForSingleObject(OverlappedWrite.hEvent, 1000);
			//GetOverlappedResult(SPortComm, &OverlappedWrite, &dwBytesWrite, TRUE);
			return (WORD)dwBytesWrite;
		}
		return 0;
	}
	return (WORD)dwBytesWrite;
}

WORD CSPortComm::CRC16(BYTE * pByteArray, BYTE Length)
{
	BYTE uchCRCHi = 0xFF; /* CRC �ĸ��ֽڳ�ʼ��*/
	BYTE uchCRCLo = 0xFF; /* CRC �ĵ��ֽڳ�ʼ��*/
	BYTE uIndex; /* CRC ��ѯ������*/
	BYTE i = 0;
	while (Length--) /* ����������Ļ�����*/
	{
		uIndex = uchCRCLo ^ (pByteArray[i++]);// uchCRCLo ^ (*(pByteArray++)); /* ����CRC */
		uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
		uchCRCHi = auchCRCLo[uIndex];
	}
	return (uchCRCHi << 8 | uchCRCLo);

}



const BYTE CSPortComm::auchCRCHi[256] = {
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
	0x41, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
	0xC1, 0x81, 0x40,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81,
	0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1,
	0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
	0x40, 0x01, 0xC0,
	0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
	0x80, 0x41, 0x01,
	0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
	0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
	0x00, 0xC1, 0x81,
	0x40
};
/* ��λ�ֽڵ�CRC ֵ*/
const BYTE CSPortComm::auchCRCLo[256] = {
	0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7,
	0x05, 0xC5, 0xC4,
	0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB,
	0x0B, 0xC9, 0x09,
	0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE,
	0xDF, 0x1F, 0xDD,
	0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2,
	0x12, 0x13, 0xD3,
	0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32,
	0x36, 0xF6, 0xF7,
	0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E,
	0xFE, 0xFA, 0x3A,
	0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B,
	0x2A, 0xEA, 0xEE,
	0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27,
	0xE7, 0xE6, 0x26,
	0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1,
	0x63, 0xA3, 0xA2,
	0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD,
	0x6D, 0xAF, 0x6F,
	0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8,
	0xB9, 0x79, 0xBB,
	0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4,
	0x74, 0x75, 0xB5,
	0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0,
	0x50, 0x90, 0x91,
	0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94,
	0x54, 0x9C, 0x5C,
	0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59,
	0x58, 0x98, 0x88,
	0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D,
	0x4D, 0x4C, 0x8C,
	0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83,
	0x41, 0x81, 0x80,
	0x40
};
