#include "stdafx.h"
#include "ServerSocket.h"
#include "ChatServer.h"


ServerSocket::ServerSocket()
{
}


ServerSocket::~ServerSocket()
{
}


void ServerSocket::OnAccept(int nErrorCode)
{
	//���ܵ�һ����������
	ClientSocket* theClientSock(0);
	//��ʼ���ڳ�ʼ�����m_listSockets��ֵ��m_pList��
	theClientSock = new ClientSocket(&m_listSockets);
	if (!theClientSock)
	{
		AfxMessageBox(_T("�ڴ治��,�ͻ����ӷ�����ʧ�ܣ�"));
		return;
	}
	Accept(*theClientSock); //����
							//����list�б��ڹ���
	m_listSockets.AddTail(theClientSock);
	CSocket::OnAccept(nErrorCode);
}