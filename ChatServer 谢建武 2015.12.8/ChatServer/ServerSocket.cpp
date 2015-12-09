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
	//接受到一个连接请求
	ClientSocket* theClientSock(0);
	//初始化在初始化里把m_listSockets赋值到m_pList里
	theClientSock = new ClientSocket(&m_listSockets);
	if (!theClientSock)
	{
		AfxMessageBox(_T("内存不足,客户连接服务器失败！"));
		return;
	}
	Accept(*theClientSock); //接受
							//加入list中便于管理
	m_listSockets.AddTail(theClientSock);
	CSocket::OnAccept(nErrorCode);
}