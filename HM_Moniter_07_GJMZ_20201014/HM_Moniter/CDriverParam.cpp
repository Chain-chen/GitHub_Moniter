#include "stdafx.h"
#include "CDriverParam.h"


CDriverParam::CDriverParam() :
	m_UINT32_setPulsePerRevolution(1)
{
}


CDriverParam::~CDriverParam()
{
}



UINT8 CDriverParam::Hex32ToArray(UINT8 * pArray, UINT32 data)
{
	// TODO: 在此处添加实现代码.

	pArray[0] = (data >> 24) & 0xFF;
	pArray[1] = (data >> 16) & 0xFF;
	pArray[2] = (data >> 8) & 0xFF;
	pArray[3] = (data >> 0) & 0xFF;

	return 0;
}
UINT8 CDriverParam::Hex16ToArray(UINT8 * pArray, UINT16 data)
{
	// TODO: 在此处添加实现代码.

	pArray[0] = (data >> 8) & 0xFF;
	pArray[1] = (data >> 0) & 0xFF;

	return 0;
}
UINT8 CDriverParam::Hex8ToArray(UINT8 * pArray, UINT8 data)
{
	// TODO: 在此处添加实现代码.

	pArray[0] = (data >> 0) & 0xFF;

	return 0;
}
UINT8 CDriverParam::BitToArray(UINT8* pArray,UINT8* data,UINT8 position)
{
	if (*data)
		*pArray |= 0x01 << position;
	else
		*pArray &= ~(0x01 << position);
	return 0;
}
UINT8 CDriverParam::ArrayToHex32(UINT8 * pArray, UINT32 * pdata)
{
	
	*pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
	return 0;
}
UINT8 CDriverParam::ArrayToHex32(UINT8 * pArray, INT32 * pdata)
{

	*pdata = (pArray[0] << 24) | (pArray[1] << 16) | (pArray[2] << 8) | (pArray[3] << 0);
	return 0;
}
UINT8 CDriverParam::ArrayToHex16(UINT8 * pArray, UINT16 * pdata)
{
	*pdata = (pArray[0] << 8) | (pArray[1] << 0);
	return 0;
}
UINT8 CDriverParam::ArrayToHex16(UINT8 * pArray, INT16 * pdata)
{
	*pdata = (pArray[0] << 8) | (pArray[1] << 0);
	return 0;
}
UINT8 CDriverParam::ArrayToHex8(UINT8 * pArray, UINT8 * pdata)
{
	*pdata = (pArray[0] << 0);
	return 0;
}
UINT8 CDriverParam::ArrayToHex8(UINT8 * pArray, INT8 * pdata)
{
	*pdata = (pArray[0] << 0);
	return 0;
}

UINT8 CDriverParam::ArrayToBit(UINT8* pArray, UINT8* pdata, UINT8 position)
{
	*pdata = (pArray[0] & (0x01<<position))>>position;
	return 0;
}

UINT8 CDriverParam::isLegalString(CEdit* cedit, LPCTCH schar, DOUBLE min, DOUBLE max, DOUBLE cur)
{
	//cstring[0]
	cedit->GetWindowTextW(m_CString_isLegalString);							//获取文本框的字符串
	m_DOUBLE_isLegalParam = _ttof(m_CString_isLegalString);					//从字符串获取临时值
	//m_CString_isLegalStringPrevious.Format(_T("%g"), m_FLOAT_isLegalParam);	//从临时值生成临时字符串
	//if (m_CString_isLegalString != m_CString_isLegalStringPrevious)			//比较字符串和临时字符串
	//{
	//	AfxMessageBox((CString)schar + _T("，输入非法"), MB_ICONSTOP);
	//	return 1;
	//}

	if ((m_DOUBLE_isLegalParam > max) || (m_DOUBLE_isLegalParam < min))
	{
		//m_CString_isLegalStringTemp.Format(_T("，允许范围 %d 到 %d"), (INT32)min, (INT32)max);
		AfxMessageBox((CString)schar + m_CString_isLegalStringTemp, MB_ICONSTOP);
		return 2;
	}
	return 0;
}
UINT8 CDriverParam::isLegalString(/*CEdit* cedit,*/ CString* cstring, CString* errcstring, DOUBLE min, DOUBLE max)
{
	//cstring[0]
	m_DOUBLE_isLegalParam = _ttof(*cstring);										//从字符串获取临时值
	//m_CString_isLegalStringPrevious.Format(_T("%g"), m_FLOAT_isLegalParam);	//从临时值生成临时字符串
	//if (m_CString_isLegalString != m_CString_isLegalStringPrevious)			//比较字符串和临时字符串
	//{
	//	AfxMessageBox((CString)schar + _T("，输入非法"), MB_ICONSTOP);
	//	return 1;
	//}

	if ((m_DOUBLE_isLegalParam > max) || (m_DOUBLE_isLegalParam < min))
	{
		errcstring->Format(_T("允许范围 %.16g 到 %.16g"), min, max);
		//AfxMessageBox((CString)schar + m_CString_isLegalStringTemp, MB_ICONSTOP);
		return 2;
	}
	return 0;
}


//转每分 切换为 脉冲每秒
UINT8 CDriverParam::toggleSpeedToVelocityUnit(FLOAT * speed, INT32 * velocity)
{
	m_FLOAT_toggleUnitTemp = *speed / 60.0F * m_UINT32_setPulsePerRevolution;
	if (m_FLOAT_toggleUnitTemp >= 0)
	{
		*velocity = (INT32)(m_FLOAT_toggleUnitTemp + 0.5F);
	}
	else
		*velocity = (INT32)(m_FLOAT_toggleUnitTemp - 0.5F);	
	

	//if (m_FLOAT_toggleUnitTemp >= 0)
	//{
	//	*speed = (INT32)(m_FLOAT_toggleUnitTemp + 0.5F);
	//}
	//else
	//	*speed = (INT32)(m_FLOAT_toggleUnitTemp - 0.5F);

	return 0;
}
//脉冲每秒 切换为 转每分
UINT8 CDriverParam::toggleVelocityToSpeedUnit(INT32 * velocity, FLOAT * speed)
{
	*speed = *velocity * 60.0F / m_UINT32_setPulsePerRevolution;

	return 0;
}