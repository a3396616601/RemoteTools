// RemoteFileDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "RemoteToolServer.h"
#include "RemoteFileDialog.h"
#include "afxdialogex.h"
int GetIconIndex(CString strFilePath)
{
	//��ȡ�ļ����Ը���·��
	DWORD dwAttribute = GetFileAttributes(strFilePath);

	SHFILEINFO sfi = { 0 };
	SHGetFileInfo(strFilePath,//�ļ�·��
		dwAttribute,//�ļ�����
		&sfi,//��������ṹ��
		sizeof(SHFILEINFO),//�ṹ��Size
						   //��ȡ�ļ���ͼ�������
		SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);
	return sfi.iIcon;

}
CString GetFileTypeName(CString strFilePath)
{
	//��ȡ�ļ����Ը���·��
	DWORD dwAttribute = GetFileAttributes(strFilePath);

	SHFILEINFO sfi = { 0 };
	SHGetFileInfo(strFilePath,//�ļ�·��
		dwAttribute,//�ļ�����
		&sfi,//��������ṹ��
		sizeof(SHFILEINFO),//�ṹ��Size
						   //��ȡ�ļ���ͼ�������
		SHGFI_TYPENAME);
	return sfi.szTypeName;

}

// RemoteFileDialog �Ի���
//�ַ����ָ��
std::vector<CString> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<CString> result;
	str += pattern;//��չ�ַ����Է������
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

std::vector<CString> split(std::wstring str, std::wstring pattern)
{
	std::wstring::size_type pos;
	std::vector<CString> result;
	str += pattern;//��չ�ַ����Է������
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

IMPLEMENT_DYNAMIC(RemoteFileDialog, CDialogEx)

RemoteFileDialog::RemoteFileDialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG_FILE, pParent)
	, m_strDisplayTypeRemote(_T(""))
	, m_strDirPathRemote(_T(""))
	, m_strDisplayTypeLocal(_T(""))
	, m_strDirPathLocal(_T(""))
{

}

RemoteFileDialog::~RemoteFileDialog()
{
}

void RemoteFileDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_DISPLAYTYPE, m_comboDisplayTypeRemote);
	DDX_CBString(pDX, IDC_COMBO_DISPLAYTYPE, m_strDisplayTypeRemote);
	DDX_Control(pDX, IDC_EDIT_DIRPATH, m_editDirPathRemote);
	DDX_Text(pDX, IDC_EDIT_DIRPATH, m_strDirPathRemote);
	DDX_Control(pDX, IDC_LIST_FILELIST, m_listFileRemote);
	DDX_Control(pDX, IDC_COMBO_DISPLAYTYPE2, m_comboDisplayTypeLocal);
	DDX_CBString(pDX, IDC_COMBO_DISPLAYTYPE2, m_strDisplayTypeLocal);
	DDX_Control(pDX, IDC_EDIT_DIRPATH2, m_editDirPathLocal);
	DDX_Text(pDX, IDC_EDIT_DIRPATH2, m_strDirPathLocal);
	DDX_Control(pDX, IDC_LIST_FILELIST2, m_listFileLocal);
}


BEGIN_MESSAGE_MAP(RemoteFileDialog, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAYTYPE, &RemoteFileDialog::OnSelchangeRemoteComboDisplaytype)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILELIST, &RemoteFileDialog::OnDblclkRemoteListFilelist)
	ON_CBN_SELCHANGE(IDC_COMBO_DISPLAYTYPE2, &RemoteFileDialog::OnSelchangeLocalComboDisplaytype)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_FILELIST2, &RemoteFileDialog::OnDblclkLocalListFilelist)
	ON_BN_CLICKED(IDC_BUTTON_DOWNLOAD, &RemoteFileDialog::OnBnClickedButtonDownload)
	ON_BN_CLICKED(IDC_BUTTON_UPLOAD, &RemoteFileDialog::OnBnClickedButtonUpload)
END_MESSAGE_MAP()


// RemoteFileDialog ��Ϣ�������


void RemoteFileDialog::OnSelchangeRemoteComboDisplaytype()
{
	SHFILEINFO sfi;
	HIMAGELIST hIconLst;

	switch (m_comboDisplayTypeRemote.GetCurSel())
	{
	case 0:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileRemote.ModifyStyle(LVS_REPORT | LVS_ICON, LVS_SMALLICON, TRUE);
		m_listFileRemote.SetImageList(m_pImageList, LVSIL_SMALL);
		//FileFindInit();
		break;
	case 1:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_ICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileRemote.ModifyStyle(LVS_REPORT | LVS_SMALLICON, LVS_ICON, TRUE);
		m_listFileRemote.SetImageList(m_pImageList, LVSIL_NORMAL);
		//FileFindInit();
		break;
	case 2:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileRemote.ModifyStyle(LVS_ICON | LVS_SMALLICON, LVS_REPORT, TRUE);
		m_listFileRemote.SetImageList(m_pImageList, LVSIL_SMALL);
		m_listFileRemote.SetColumnWidth(0, 300);
		//FileFindInit();
		break;
	}
}


void RemoteFileDialog::OnDblclkRemoteListFilelist(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	if (pNMItemActivate->iItem >= 0)
	{
		if (m_vecFilenameRemote[pNMItemActivate->iItem] == _T(".."))
			m_vecPathRemote.pop_back();
		else
			m_vecPathRemote.push_back(m_vecFilenameRemote[pNMItemActivate->iItem]);
		// TODO ���� m_vecPath ��������
		CString str;
		for (size_t i = 0; i < m_vecPathRemote.size(); i++)
		{
			str += m_vecPathRemote[i] + '?';
		}
		m_socketClient->Send(Socket::MessageType::FileQueryRequset, str.GetBuffer());
	}
	*pResult = 0;
}

void RemoteFileDialog::OnSelchangeLocalComboDisplaytype()
{
	SHFILEINFO sfi;
	HIMAGELIST hIconLst;

	switch (m_comboDisplayTypeLocal.GetCurSel())
	{
	case 0:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileLocal.ModifyStyle(LVS_REPORT | LVS_ICON, LVS_SMALLICON, TRUE);
		m_listFileLocal.SetImageList(m_pImageList, LVSIL_SMALL);
		//FileFindInit();
		break;
	case 1:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_ICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileLocal.ModifyStyle(LVS_REPORT | LVS_SMALLICON, LVS_ICON, TRUE);
		m_listFileLocal.SetImageList(m_pImageList, LVSIL_NORMAL);
		//FileFindInit();
		break;
	case 2:
		hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileLocal.ModifyStyle(LVS_ICON | LVS_SMALLICON, LVS_REPORT, TRUE);
		m_listFileLocal.SetImageList(m_pImageList, LVSIL_SMALL);
		m_listFileLocal.SetColumnWidth(0, 300);
		//FileFindInit();
		break;
	}
}

void RemoteFileDialog::OnDblclkLocalListFilelist(NMHDR* pNMHDR, LRESULT* pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (pNMItemActivate->iItem >= 0)
	{
		if (m_vecFilenameLocal[pNMItemActivate->iItem] == _T(".."))
			m_vecPathLocal.pop_back();
		else
			m_vecPathLocal.push_back(m_vecFilenameLocal[pNMItemActivate->iItem]);
		FindLocalFile();
		FillLocalFileList();


	}
	*pResult = 0;
}


BOOL RemoteFileDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	//remote����
	{
		m_comboDisplayTypeRemote.AddString(_T("Сͼ��"));
		m_comboDisplayTypeRemote.AddString(_T("��ͼ��"));
		m_comboDisplayTypeRemote.AddString(_T("��ϸ��Ϣ"));
		m_comboDisplayTypeRemote.SetCurSel(0);

		//��ȡ��ǰList�ķ��״̬
		DWORD dwExtStyle = m_listFileRemote.GetExtendedStyle();
		//����ѡ��ʱΪ����ѡ��
		dwExtStyle |= LVS_EX_FULLROWSELECT;
		m_listFileRemote.SetExtendedStyle(dwExtStyle);


		int nCol = 0;
		m_listFileRemote.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 300);
		m_listFileRemote.InsertColumn(nCol++, _T("�޸�����"), LVCFMT_LEFT, 100);
		m_listFileRemote.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 100);
		m_listFileRemote.InsertColumn(nCol++, _T("��С"), LVCFMT_LEFT, 100);
		SHFILEINFO sfi;
		HIMAGELIST hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileRemote.ModifyStyle(0, LVS_SMALLICON);
		m_listFileRemote.SetImageList(m_pImageList, LVSIL_SMALL);
	}


	//��������
	{
		m_comboDisplayTypeLocal.AddString(_T("Сͼ��"));
		m_comboDisplayTypeLocal.AddString(_T("��ͼ��"));
		m_comboDisplayTypeLocal.AddString(_T("��ϸ��Ϣ"));
		m_comboDisplayTypeLocal.SetCurSel(0);

		//��ȡ��ǰList�ķ��״̬
		DWORD dwExtStyle = m_listFileLocal.GetExtendedStyle();
		//����ѡ��ʱΪ����ѡ��
		dwExtStyle |= LVS_EX_FULLROWSELECT;
		m_listFileLocal.SetExtendedStyle(dwExtStyle);


		int nCol = 0;
		m_listFileLocal.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 300);
		m_listFileLocal.InsertColumn(nCol++, _T("�޸�����"), LVCFMT_LEFT, 100);
		m_listFileLocal.InsertColumn(nCol++, _T("����"), LVCFMT_LEFT, 100);
		m_listFileLocal.InsertColumn(nCol++, _T("��С"), LVCFMT_LEFT, 100);
		SHFILEINFO sfi;
		HIMAGELIST hIconLst = (HIMAGELIST)::SHGetFileInfo(
			_T("C:\\"), //ϵͳ�̷�
			FILE_ATTRIBUTE_NORMAL,
			&sfi,
			sizeof(SHFILEINFO),
			SHGFI_SYSICONINDEX | SHGFI_SMALLICON);//��ȡСͼ����ͼ������
		m_pImageList = CImageList::FromHandle(hIconLst);
		m_listFileLocal.ModifyStyle(0, LVS_SMALLICON);
		m_listFileLocal.SetImageList(m_pImageList, LVSIL_SMALL);

		FindLocalFile();
		FillLocalFileList();
	}
	//TODO ����Ϣ��������
	return TRUE;  // return TRUE unless you set the focus to a control
				  // �쳣: OCX ����ҳӦ���� FALSE
}

void RemoteFileDialog::FillRemoteFileList(wchar_t* data)
{
	//struct MessageInfo
	//{
	//	CString filename;
	//	CString iconIndex;
	//	CString fileTypeName;
	//};
	auto vecData = split(data, L"?");
	m_vecFilenameRemote.resize(vecData.size() / 3);
	for (size_t i = 0; i < vecData.size() - 1; i += 3)
	{
		m_vecFilenameRemote[i / 3] = vecData[i];
	}
	m_listFileRemote.DeleteAllItems();
	int nCount = m_vecFilenameRemote.size();
	for (size_t i = 0; i < vecData.size() - 1; i += 3)
	{
		m_listFileRemote.InsertItem(i / 3, m_vecFilenameRemote[i / 3], _wtoi(vecData[i + 1]));
		m_listFileRemote.SetItemText(i / 3, 2, vecData[i + 2]);
	}
	m_editDirPathRemote.SetWindowText(vecData.back());
	m_strDirPathRemote = vecData.back();
}


void RemoteFileDialog::OnBnClickedButtonDownload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UINT uSelectedCount = m_listFileRemote.GetSelectedCount();
	if (uSelectedCount == 0)
	{
		return;
	}
	int  nItem = -1;
	nItem = m_listFileRemote.GetNextItem(nItem, LVNI_SELECTED);


	CString remoteFileName = m_strDirPathRemote + m_vecFilenameRemote[nItem];
	CString LocalPathFileName = m_strDirPathLocal + m_vecFilenameRemote[nItem];
	m_socketClient->Send(Socket::MessageType::FileDownloadRequset, (remoteFileName + "?" + LocalPathFileName).GetBuffer());
}


void RemoteFileDialog::OnBnClickedButtonUpload()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString remoteFileName = m_strDirPathRemote + m_vecFilenameRemote[m_listFileRemote.GetSelectedColumn()];
	CString LocalPathFileName = m_strDirPathLocal + m_vecFilenameRemote[m_listFileRemote.GetSelectedColumn()];
}

void RemoteFileDialog::FindLocalFile()
{
	if (m_vecPathLocal.size() > 0)
	{
		CFileFind finder;
		m_vecFilenameLocal.clear();
		m_strDirPathLocal = _T("");
		// build a string with wildcards
		for (size_t i = 0; i < m_vecPathLocal.size(); i++)
			m_strDirPathLocal += m_vecPathLocal[i] + _T("\\");
		m_strDirPathLocal += _T("*.*");
		m_vecFilenameLocal.push_back(_T(".."));
		// start working for files
		BOOL bWorking = finder.FindFile(m_strDirPathLocal);

		while (bWorking)
		{
			bWorking = finder.FindNextFile();

			// skip . and .. files; otherwise, we'd
			// recur infinitely!

			if (finder.IsDots())
				continue;

				// if it's a directory, recursively search it

			else if (finder.IsDirectory())
			{
				CString str = finder.GetFileName();
				m_vecFilenameLocal.push_back(str);
			}
			else
			{
				CString str = finder.GetFileName();
				m_vecFilenameLocal.push_back(str);
			}
		}
		m_strDirPathLocal = finder.GetRoot();
		finder.Close();
		//m_strDirPath.Delete(m_strDirPath.GetLength() - 4, 4);
	}
	else
	{
		DWORD dwBuffer = 512;
		TCHAR szBuffer[512];
		memset(szBuffer, 0, dwBuffer);
		int dSize = GetLogicalDriveStrings(dwBuffer, szBuffer); //print szBuffer for detail
		if (dSize == 0)
		{
			//no drive found
			return;
		}
		m_vecFilenameLocal.clear();
		TCHAR szTmp[4];
		memset(szTmp, 0, 4);

		for (int i = 0; i < dSize; i += 4)
		{
			if (szBuffer[i] != '\0')
			{
				wmemcpy(szTmp, &szBuffer[i], 4);

				CString strTmp(szTmp, 2);
				m_vecFilenameLocal.push_back(strTmp);
			}
		}
		m_strDirPathLocal = _T("�˵���");
	}
	UpdateData(FALSE);
}

void RemoteFileDialog::FillLocalFileList()
{
	m_listFileLocal.DeleteAllItems();
	CString PathName = _T("");
	for (size_t i = 0; i < m_vecPathLocal.size(); i++)
		PathName += m_vecPathLocal[i] + _T("\\");
	int nCount = m_vecFilenameLocal.size();
	for (int i = 0; i < nCount; i++)
	{
		m_listFileLocal.InsertItem(i, m_vecFilenameLocal[i], GetIconIndex(PathName + m_vecFilenameLocal[i]));
		m_listFileLocal.SetItemText(i, 2, GetFileTypeName(PathName + m_vecFilenameLocal[i]));
	}
}

void RemoteFileDialog::SaveRemoteFile(char* data, size_t length)
{
	CString filePathName = (wchar_t*)data;
	size_t fileLength = length - (filePathName.GetLength() + 1) * sizeof TCHAR;
	char* buffer = data + (filePathName.GetLength() + 1) * sizeof TCHAR;
	HANDLE hFile = CreateFile(filePathName, GENERIC_WRITE, 0, NULL, CREATE_NEW, FILE_ATTRIBUTE_NORMAL , NULL);
	if (hFile == INVALID_HANDLE_VALUE)
	{
		MessageBox(L"�ļ��Ѵ��ڻ�·����Ч");
		return;
	}
	DWORD hasWrite;
	WriteFile(hFile, buffer, fileLength, &hasWrite, NULL);
	CloseHandle(hFile);
	MessageBox(L"�ļ��������", 0, MB_OK);
}
