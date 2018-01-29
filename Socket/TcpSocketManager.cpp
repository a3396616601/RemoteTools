#include "stdafx.h"
#include "TcpSocketManager.h"

std::vector<WSAEVENT*> TcpSocketManager::m_vecHEventAry;
std::vector<SOCKET*> TcpSocketManager::m_vecHSocketAry;
std::vector<Socket**> TcpSocketManager::m_vecClientsAry;
std::vector<size_t> TcpSocketManager::m_vecSize;

DWORD TcpSocketManager::ThreadFunc(LPVOID lparam)
{
	int threadID = (int)lparam;
	size_t& m_nCount = m_vecSize[threadID];


	WSAEVENT* m_hEventAry = m_vecHEventAry[threadID];
	SOCKET* m_hSocketAry = m_vecHSocketAry[threadID];
	Socket** m_hClientAry = m_vecClientsAry[threadID];
	//ͨ���ȴ��¼������������жϺʹ���socket��Ϣ
	char szBuf[MAXBYTE] = { 0 };
	while (true)
	{

		DWORD  dwRet = WSAWaitForMultipleEvents(
			m_nCount,    //EventAry�ĸ���
			m_hEventAry, //EventAry����ָ��
			FALSE,       //�Ƿ�ȴ������¼����󱻴���
			INFINITE,    //�ȴ��¼�
			FALSE);

		if (dwRet == WSA_WAIT_FAILED)
		{
			//�ͷ���Դ
			return 0;
		}

		int nIndex = dwRet - WSA_WAIT_EVENT_0;

		for (int i = nIndex; i < m_nCount; i++)
		{
			dwRet = WSAWaitForMultipleEvents(
				1,    //EventAry�ĸ���
				(void *const *)m_hEventAry, //EventAry����ָ��
				FALSE,       //�Ƿ�ȴ������¼����󱻴���
				0,    //�ȴ��¼�
				FALSE);

			WSANETWORKEVENTS wsaNetworkEvents;
			int nRet = WSAEnumNetworkEvents(m_hSocketAry[i],
				m_hEventAry[i],
				&wsaNetworkEvents);

			//��ʾ�ɹ�
			if (nRet == 0)
			{

				if (wsaNetworkEvents.lNetworkEvents &FD_ACCEPT)
				{
					if (wsaNetworkEvents.iErrorCode[FD_ACCEPT_BIT] == 0)
					{
						//TODO: �������Խ�� m_hSocketAry m_hEventAry
						m_hClientAry[m_nCount] = ((TcpServer*)m_hClientAry[i])->Accept();
						m_hSocketAry[m_nCount] = *m_hClientAry[m_nCount];
						m_hEventAry[m_nCount] = WSACreateEvent();
						nRet = WSAEventSelect(m_hSocketAry[m_nCount],
							m_hEventAry[m_nCount],
							FD_READ | FD_WRITE | FD_CLOSE);

						if (SOCKET_ERROR == nRet)
						{
							//�ͷ���Դ
							// WSACloseEvent
							return 0;
						}
						m_nCount++;
					}
					else
					{
						//���ݴ����봦�����
					}
				}
				else if (wsaNetworkEvents.lNetworkEvents &FD_READ)
				{
					if (wsaNetworkEvents.iErrorCode[FD_READ_BIT] == 0)
					{
						((TcpClient*)m_hClientAry[i])->Receive();
					}
					else
					{
						//���ݴ����봦�����
					}
				}
				else if (wsaNetworkEvents.lNetworkEvents &FD_WRITE)
				{
					if (wsaNetworkEvents.iErrorCode[FD_WRITE_BIT] == 0)
					{
						((TcpClient*)m_hClientAry[i])->Send();
					}
					else
					{
						//���ݴ����봦�����
					}
				}
			}

		}
	}
	return 0;
}