#pragma once
#include "afxsock.h"
#include "ClientSocket.h"

class ServerSocket : public CSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();

public:
	CPtrList m_listSockets;//用来保存服务器与所有客户端连接成功后的Socket

public:
	virtual void OnAccept(int nErrorCode); //接受
};

