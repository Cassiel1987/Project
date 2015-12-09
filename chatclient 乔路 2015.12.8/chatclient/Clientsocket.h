#pragma once
#include "afxsock.h"
class Clientsocket :
	public CSocket
{
public:
	Clientsocket();
	virtual ~Clientsocket();
public:
	virtual void OnReceive(int nErrorCode);//客户端接收消息  
	BOOL SendMSG(LPSTR lpBuff, int nlen, char to_user[20], char from_user[20]);//客户端发送消息  
	BOOL LogoIn(LPSTR lpBuff, int nlen, char from_user[20]);//客户端登录  
	CString m_strUserName;
};

