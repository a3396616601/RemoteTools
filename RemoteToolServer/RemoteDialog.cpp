// RemoteDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteToolServer.h"
#include "RemoteDialog.h"
#include "afxdialogex.h"


// RemoteDialog �Ի���

IMPLEMENT_DYNAMIC(RemoteDialog, CDialogEx)

RemoteDialog::RemoteDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_DESKTOP, pParent)
{

}

RemoteDialog::~RemoteDialog()
{
	DeleteCriticalSection(&m_cs);
}

void RemoteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RemoteDialog, CDialogEx)
	ON_COMMAND(ID_ACCELERATOR32775, &RemoteDialog::OnAcceleratorSaveBMP)
END_MESSAGE_MAP()


// RemoteDialog ��Ϣ�������



void RemoteDialog::OnAcceleratorSaveBMP()
{
	// TODO: �ڴ���������������
	EnterCriticalSection(&m_cs);
	m_isSaveBMP = true;
	LeaveCriticalSection(&m_cs);
}


BOOL RemoteDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	m_hAccel = ::LoadAccelerators(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_ACCELERATOR1));
	InitializeCriticalSection(&m_cs);
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}


BOOL RemoteDialog::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (::TranslateAccelerator(GetSafeHwnd(), m_hAccel, pMsg))
		return true;
	return CDialogEx::PreTranslateMessage(pMsg);
}
