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
}

void RemoteDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(RemoteDialog, CDialogEx)
END_MESSAGE_MAP()


// RemoteDialog ��Ϣ�������

