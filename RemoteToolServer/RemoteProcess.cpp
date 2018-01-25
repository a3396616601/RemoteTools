// RemoteProcess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteToolServer.h"
#include "RemoteProcess.h"
#include "afxdialogex.h"


// RemoteProcess �Ի���

IMPLEMENT_DYNAMIC(RemoteProcess, CDialogEx)

RemoteProcess::RemoteProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent)
{

}

RemoteProcess::~RemoteProcess()
{
}

void RemoteProcess::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_PROCESS, m_listProcess);
}


BEGIN_MESSAGE_MAP(RemoteProcess, CDialogEx)
END_MESSAGE_MAP()


// RemoteProcess ��Ϣ�������


BOOL RemoteProcess::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_listProcess.ModifyStyle(LVS_ICON, LVS_REPORT | LVS_SINGLESEL);
	m_listProcess.SetExtendedStyle(m_listProcess.GetExtendedStyle() | LVS_EX_DOUBLEBUFFER | LVS_EX_GRIDLINES | LVS_EX_FULLROWSELECT);
	m_listProcess.InsertColumn(0, _T("����"), LVCFMT_LEFT, 200);
	m_listProcess.InsertColumn(1, _T("����ID"), LVCFMT_LEFT, 50);
	m_listProcess.InsertColumn(2, _T("������ID"), LVCFMT_LEFT, 50);
	m_listProcess.InsertColumn(3, _T("ӳ��·��"), LVCFMT_LEFT, 400);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}
