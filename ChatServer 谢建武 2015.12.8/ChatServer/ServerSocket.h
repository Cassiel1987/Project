#pragma once
#include "afxsock.h"
#include "ClientSocket.h"

class ServerSocket : public CSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();

public:
	CPtrList m_listSockets;//������������������пͻ������ӳɹ����Socket

public:
	virtual void OnAccept(int nErrorCode); //����
};

