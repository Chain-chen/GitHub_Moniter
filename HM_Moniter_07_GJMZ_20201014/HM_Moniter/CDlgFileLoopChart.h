#pragma once


// CDlgFileLoopChart 对话框

class CDlgFileLoopChart : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgFileLoopChart)

public:
	CDlgFileLoopChart(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~CDlgFileLoopChart();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_FILE_LOOP_CHART };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
};
