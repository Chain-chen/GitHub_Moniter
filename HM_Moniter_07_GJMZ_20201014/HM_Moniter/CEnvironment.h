// �����Ϳ������á�����ࡱ�����ļ�������ɵ� IDispatch ��װ����

// CEnvironment ��װ����

class CEnvironment : public COleDispatchDriver
{
public:
	CEnvironment() {} // ���� COleDispatchDriver Ĭ�Ϲ��캯��
	CEnvironment(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CEnvironment(const CEnvironment& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// ����
public:

	// ����
public:


	// IEnvironment ����
public:
	BOOL get_NoPromptDBConnect()
	{
		BOOL result;
		InvokeHelper(0x2f, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_NoPromptDBConnect(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x2f, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_IEPrintWithPage()
	{
		BOOL result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_IEPrintWithPage(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	void InternalRepaint()
	{
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, nullptr, nullptr);
	}
	BOOL get_MouseWheelScroll()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_MouseWheelScroll(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString get_DecimalSeparator()
	{
		CString result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, nullptr);
		return result;
	}
	BOOL get_TeeClipWhenPrinting()
	{
		BOOL result;
		InvokeHelper(0xc9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_TeeClipWhenPrinting(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0xc9, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	BOOL get_TeeClipWhenMetafiling()
	{
		BOOL result;
		InvokeHelper(0xca, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_TeeClipWhenMetafiling(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0xca, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}
	CString DecodeUTF8String(LPCTSTR aString)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR;
		InvokeHelper(0xcb, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, aString);
		return result;
	}
	CString CodeUTF8String(LPCTSTR aString, BOOL addPrefix)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR VTS_BOOL;
		InvokeHelper(0xcc, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, aString, addPrefix);
		return result;
	}
	BOOL get_ChartVisible()
	{
		BOOL result;
		InvokeHelper(0xcd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, nullptr);
		return result;
	}
	void put_ChartVisible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL;
		InvokeHelper(0xcd, DISPATCH_PROPERTYPUT, VT_EMPTY, nullptr, parms, newValue);
	}

	// IEnvironment ����
public:

};
