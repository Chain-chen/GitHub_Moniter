// CDlgFileLoopChart.cpp: 实现文件
//

#include "stdafx.h"
#include "HM_Moniter.h"
#include "CDlgFileLoopChart.h"
#include "afxdialogex.h"


// CDlgFileLoopChart 对话框

IMPLEMENT_DYNAMIC(CDlgFileLoopChart, CDialogEx)

CDlgFileLoopChart::CDlgFileLoopChart(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_FILE_LOOP_CHART, pParent)
{

}

CDlgFileLoopChart::~CDlgFileLoopChart()
{
}

void CDlgFileLoopChart::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgFileLoopChart, CDialogEx)
END_MESSAGE_MAP()


// CDlgFileLoopChart 消息处理程序
