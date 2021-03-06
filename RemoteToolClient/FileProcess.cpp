// FileProcess.cpp: 实现文件
//

#include "stdafx.h"
#include "RemoteToolClient.h"
#include "FileProcess.h"
#include <iostream>
#include <stack>

bool MyCreateDirectory(CString dirPath)
{
	std::stack<CString> vecDir;
	CString dir = dirPath;
	while (!CreateDirectory(dir, NULL) && GetLastError() == ERROR_PATH_NOT_FOUND)
	{
		vecDir.push(dir);
		dir = dir.Left(dir.ReverseFind('\\'));
	}
	while (!vecDir.empty())
	{
		if (!CreateDirectory(vecDir.top(), NULL))
			return false;
		vecDir.pop();
	}
	return true;
}
void FindAllFile(CString pstr, std::vector<CString>& allFilePath)
{
	;
	CFileFind finder;

	// build a string with wildcards
	CString strWildcard(pstr);
	strWildcard += _T("\\*.*");

	// start working for files
	BOOL bWorking = finder.FindFile(strWildcard);

	while (bWorking)
	{
		bWorking = finder.FindNextFile();

		// skip . and .. files; otherwise, we'd
		// recur infinitely!

		if (finder.IsDots())
			continue;

		// if it's a directory, recursively search it

		if (finder.IsDirectory())
		{
			CString str = finder.GetFilePath();
			FindAllFile(str, allFilePath);
		}
		else
		{
			allFilePath.push_back(finder.GetFilePath());
		}
	}

	finder.Close();
}



std::vector<CString> split(std::string str, std::string pattern)
{
	std::string::size_type pos;
	std::vector<CString> result;
	str += pattern;//扩展字符串以方便操作
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

static std::vector<CString> split(std::wstring str, std::wstring pattern)
{
	std::wstring::size_type pos;
	std::vector<CString> result;
	str += pattern;//扩展字符串以方便操作
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
// FileProcess

FileProcess::FileProcess()
{
	CoInitialize(NULL);
}

FileProcess::~FileProcess()
{
	CoUninitialize();
}


// FileProcess 成员函数
int GetIconIndex(CString strFilePath)
{
	//获取文件属性根据路径
	DWORD dwAttribute = GetFileAttributes(strFilePath);

	SHFILEINFO sfi = { 0 };
	SHGetFileInfo(strFilePath,//文件路径
		dwAttribute,//文件属性
		&sfi,//输出参数结构体
		sizeof(SHFILEINFO),//结构体Size
						   //获取文件的图标和属性
		SHGFI_SYSICONINDEX | SHGFI_USEFILEATTRIBUTES | SHGFI_TYPENAME);
	return sfi.iIcon;

}
CString GetFileTypeName(CString strFilePath)
{
	//获取文件属性根据路径
	DWORD dwAttribute = GetFileAttributes(strFilePath);

	SHFILEINFO sfi = { 0 };
	SHGetFileInfo(strFilePath,//文件路径
		dwAttribute,//文件属性
		&sfi,//输出参数结构体
		sizeof(SHFILEINFO),//结构体Size
						   //获取文件的图标和属性
		SHGFI_TYPENAME);
	return sfi.szTypeName;

}


void FileProcess::FindFile()
{
	if (m_vecPath.size() > 0)
	{
		CFileFind finder;
		m_vecFilename.clear();
		m_strDirPath = _T("");
		// build a string with wildcards
		for (size_t i = 0; i < m_vecPath.size(); i++)
			m_strDirPath += m_vecPath[i] + _T("\\");
		m_strDirPath += _T("*.*");
		m_vecFilename.push_back(_T(".."));
		// start working for files
		BOOL bWorking = finder.FindFile(m_strDirPath);
		if (bWorking)
		{
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
					m_vecFilename.push_back(str);
				}
				else
				{
					CString str = finder.GetFileName();
					m_vecFilename.push_back(str);
				}
			}
			m_strDirPath = finder.GetRoot();
		}
		finder.Close();
		//m_strDirPath.Delete(m_strDirPath.GetLength() - 4, 4);

	}
	else
	{
		DWORD dwBuffer = 512;
		TCHAR szBuffer[512];
		memset(szBuffer, 0, dwBuffer);
		int dSize = GetLogicalDriveStrings(dwBuffer, szBuffer);//print szBuffer for detail
		if (dSize == 0) {//no drive found
			return;
		}
		m_vecFilename.clear();
		TCHAR szTmp[4];
		memset(szTmp, 0, 4);

		for (int i = 0; i < dSize; i += 4)
		{
			if (szBuffer[i] != '\0')
			{
				wmemcpy(szTmp, &szBuffer[i], 4);

				CString strTmp(szTmp, 2);
				m_vecFilename.push_back(strTmp);
			}
		}
		m_strDirPath = _T("此电脑");
	}
}

void FileProcess::FillFileList()
{
	CString PathName = _T("");
	for (size_t i = 0; i < m_vecPath.size(); i++)
		PathName += m_vecPath[i] + _T("\\");
	int nCount = m_vecFilename.size();
	//struct MessageInfo
	//{
	//	CString filename;
	//	CString iconIndex;
	//	CString fileTypeName;
	//};
	CString strMessage;
	for (int i = 0; i < nCount; i++)
	{
		strMessage.Append(m_vecFilename[i]);
		strMessage.Append(L"?");
		strMessage.AppendFormat(L"%d?", GetIconIndex(PathName + m_vecFilename[i]));
		strMessage.Append(GetFileTypeName(PathName + m_vecFilename[i]));
		strMessage.Append(L"?");
	}
	strMessage.Append(m_strDirPath);
	strMessage.Append(L"?");
	m_socketClient->Send(Socket::MessageType::FileQueryReply, strMessage.GetBuffer());

}

void FileProcess::OnUploadQuere(std::wstring path)
{
	m_vecPath = split(path, L"?");
	FindFile();
	FillFileList();
}

void FileProcess::OnDownloadQuere(std::wstring path, size_t offsetRemote)
{
	auto vecPath = split(path, L"?");
	CString LocalFilePathName = vecPath[0];
	CString RemoteFilePathName = vecPath[1];
	if (PathIsDirectory(LocalFilePathName))
	{
		std::vector<CString> allLocalPathName;
		std::vector<CString> allRemotePathName;
		FindAllFile(LocalFilePathName, allLocalPathName);
		allRemotePathName.resize(allLocalPathName.size());
		for (size_t i = 0; i < allLocalPathName.size(); i++)
		{
			allRemotePathName[i] = RemoteFilePathName;
			allRemotePathName[i].Append(allLocalPathName[i].Right(allLocalPathName[i].GetLength() - LocalFilePathName.GetLength()));
		}
		std::wstring param;
		for (size_t i = 0; i < allLocalPathName.size(); i++)
		{
			param += allLocalPathName[i] + '?' + allRemotePathName[i] + '?';
		}
		m_socketClient->Send(Socket::MessageType::FileDownloadReplyIsDir, param);
	}
	else
	{
		HANDLE hFile = CreateFile(LocalFilePathName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		DWORD fileSize = GetFileSize(hFile, NULL);
		OVERLAPPED ol = { 0 };
		size_t blockSize = 1024 * 1024;
		size_t offset = offsetRemote;
		if (offset < fileSize)
		{
			DWORD readSize = (blockSize < fileSize - offset) ? blockSize : (fileSize - offset);
			Socket::MessageData message;
			message.type = Socket::MessageType::FileDownloadReply;
			message.length = (path.length() + 1) * sizeof(wchar_t) + readSize + 2 * sizeof(DWORD);
			message.data = new char[message.length];
			//远程文件名
			memcpy_s(message.data,
				(path.length() + 1) * sizeof(wchar_t),
				path.data(),
				(path.length() + 1) * sizeof(wchar_t));
			//文件大小
			memcpy_s(message.data + (path.length() + 1) * sizeof(wchar_t),
				sizeof(DWORD),
				&fileSize,
				sizeof(DWORD));
			//文件偏移
			memcpy_s(message.data + (path.length() + 1) * sizeof(wchar_t) + sizeof(DWORD),
				sizeof(DWORD),
				&offset,
				sizeof(DWORD));

			char* fileBuffer = message.data + (path.length() + 1) * sizeof(wchar_t) + 2 * sizeof(DWORD);
			long hignOffset = 0;
			DWORD dwPtr = SetFilePointer(hFile,
				offset,
				&hignOffset,
				FILE_BEGIN);
			DWORD szRead = 0;
			ReadFile(hFile, fileBuffer, readSize, &szRead, NULL);

			m_socketClient->Send(message);
		}

		CloseHandle(hFile);
	}

}

void FileProcess::SaveRemoteFile(char* data, size_t length)
{
	std::wstring str = (wchar_t*)data;
	auto vecData = split(str, L"?");
	CString filePathName = vecData[1];
	CString remotePathName = vecData[0];
	DWORD fileLength = *(int*)(data + (str.length() + 1) * sizeof TCHAR);
	DWORD offset = *(int*)(data + sizeof(DWORD) + (str.length() + 1) * sizeof TCHAR);
	DWORD bufferSize = length - (2 * sizeof(DWORD) + (str.length() + 1) * sizeof TCHAR);
	char* buffer = data + (2 * sizeof(DWORD) + (str.length() + 1) * sizeof TCHAR);

	CString dir = filePathName.Left(filePathName.ReverseFind('\\'));

	MyCreateDirectory(dir);
	HANDLE hFile = CreateFile(filePathName, GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFile == INVALID_HANDLE_VALUE)
	{
		TRACE(L"文件无法访问");
		return;
	}
	DWORD hasWrite;
	long hignOffset = 0;
	DWORD dwPtr = SetFilePointer(hFile,
		offset,
		&hignOffset,
		FILE_BEGIN);

	WriteFile(hFile, buffer, bufferSize, &hasWrite, NULL);
	DWORD endOffset = offset + hasWrite;
	if (endOffset == fileLength)
	{
		DeleteFile(filePathName + ".infoxxx");
		CloseHandle(hFile);
	}
	else
	{
		HANDLE hFileInfo = CreateFile(filePathName + ".infoxxx", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		WriteFile(hFileInfo, &endOffset, sizeof(DWORD), &hasWrite, NULL);
		CloseHandle(hFileInfo);
		CloseHandle(hFile);
		m_socketClient->Send(Socket::MessageType::FileUploadRequset, endOffset, (remotePathName + "?" + filePathName).GetBuffer());
	}


	//MessageBox(L"文件传输完毕", 0, MB_OK);
}

void FileProcess::SendDownloadRequset(CString remoteFileName, CString LocalPathFileName)
{
	HANDLE hFileInfo = CreateFile(LocalPathFileName + ".infoxxx", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_WRITE | FILE_SHARE_READ | FILE_SHARE_DELETE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileInfo == INVALID_HANDLE_VALUE && PathFileExists(LocalPathFileName))
	{

		m_socketClient->Send(Socket::MessageType::FileDownloadRequset, 0, (remoteFileName + "?" + LocalPathFileName).GetBuffer());
	}
	else
	{
		DWORD offset = 0;
		DWORD readNums = 0;
		ReadFile(hFileInfo, &offset, sizeof(DWORD), &readNums, NULL);
		m_socketClient->Send(Socket::MessageType::FileDownloadRequset, offset, (remoteFileName + "?" + LocalPathFileName).GetBuffer());
	}
	CloseHandle(hFileInfo);
}