#pragma once
#include "afxsock.h"
#include "stdafx.h"


//说明，该类用于和客户端建立通信的Socket

class ClientSocket : public CSocket
{
public:
	ClientSocket(CPtrList* pList);
	virtual ~ClientSocket();
public:
	CPtrList* m_pList;//保存服务器Socket中List的东西
	CString m_strName; //连接名称
public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode); //收到
	void OnLogoIN(char* buff, int nlen, char from_user[20]);  //处理登录消息  
	void OnMSGTranslate(char* buff, int nlen, char to_user[20], char from_user[20]); //转发消息给其他聊天群 
	CString UpdateServerLog();//服务器端更新、记录日志 
	void UpdateAllUser(CString strUserInfo, char from_user[20]);//更新服务器端的在线人员列表  
private:
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);//多字节的转换  
};

