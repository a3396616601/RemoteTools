// RemoteProcess.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteToolServer.h"
#include "RemoteProcess.h"
#include "afxdialogex.h"

static std::vector<CString> split(std::wstring str, std::wstring pattern)
{
	std::wstring::size_type pos;
	std::vector<CString> result;
	size_t size = str.size();

	for (size_t i = 0; i < size; i++)
	{
		pos = str.find(pattern, i);
		if (pos < size)
		{
			CString s;
			s = str.substr(i, pos - i).c_str();
			result.push_back(s);
			i = pos + pattern.size() - 1;
		}
	}
	if (result.back() == "")
		result.erase(result.end() - 1);
	return result;
}
// RemoteProcess �Ի���

IMPLEMENT_DYNAMIC(RemoteProcess, CDialogEx)

RemoteProcess::RemoteProcess(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_PROCESS, pParent),
	selectPID(L"")
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
	ON_NOTIFY(NM_RCLICK, IDC_LIST_PROCESS, &RemoteProcess::OnRclickListProcess)
	ON_COMMAND(ID_2_TERMINATE, &RemoteProcess::OnTerminateProcess)
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
BOOL RemoteProcess::UpdateListCtrl(std::wstring data)
{
	std::vector<CString> vecListData = split(data, L"\n");
	assert(vecListData.size() % 4 == 0);
	DWORD dwRow = 0;
	for (size_t i = 0; i < vecListData.size(); i += 4)
	{
		m_listProcess.InsertItem(dwRow, vecListData[i]);
		m_listProcess.SetItemText(dwRow, 1, vecListData[i + 1]);
		m_listProcess.SetItemText(dwRow, 2, vecListData[i + 2]);
		m_listProcess.SetItemText(dwRow, 3, vecListData[i + 3]);
		dwRow++;
	}
	return TRUE;
}


void RemoteProcess::OnRclickListProcess(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POINT pt;
	GetCursorPos(&pt);

	if (pNMItemActivate->iItem >= 0 && pNMItemActivate->iItem <= m_listProcess.GetItemCount())
	{
		selectPID = m_listProcess.GetItemText(pNMItemActivate->iItem, 1);
		CMenu menu;
		menu.LoadMenu(IDR_MENU2);
		CMenu *pSubMenu = NULL;
		pSubMenu = menu.GetSubMenu(0);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON,
			pt.x, pt.y, this);
	}
	*pResult = 0;
}


void RemoteProcess::OnTerminateProcess()
{
	// TODO: �ڴ���������������
	if (selectPID != L"")
	{
		m_socketClient->Send(Socket::MessageType::ProcessTerminateRequest, selectPID.GetBuffer());
		selectPID = "";
	}
}
