#pragma once
#include "afxsock.h"
class Clientsocket :
	public CSocket
{
public:
	Clientsocket();
	virtual ~Clientsocket();
public:
	virtual void OnReceive(int nErrorCode);//�ͻ��˽�����Ϣ  
	BOOL SendMSG(LPSTR lpBuff, int nlen, char to_user[20], char from_user[20]);//�ͻ��˷�����Ϣ  
	BOOL LogoIn(LPSTR lpBuff, int nlen, char from_user[20]);//�ͻ��˵�¼  
	CString m_strUserName;
};

