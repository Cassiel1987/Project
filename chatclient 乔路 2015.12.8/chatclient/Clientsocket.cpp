#include "stdafx.h"
#include "Clientsocket.h"
#include "Header.h"
#include "chatclient.h"
#include "chatclientDlg.h"

Clientsocket::Clientsocket():m_strUserName(_T(""))
{
}


Clientsocket::~Clientsocket()
{
}

void Clientsocket::OnReceive(int nErrorCode)
{
	//首先接受head头
	HEADER head;
	char* pHead = NULL;
	pHead = new char[sizeof(head)];
	memset(pHead, 0, sizeof(head));
	Receive(pHead, sizeof(head));

	head.type = ((LPHEADER)pHead)->type;
	head.nContentLen = ((LPHEADER)pHead)->nContentLen;
	strcpy_s(head.from_user, ((LPHEADER)pHead)->from_user);
	delete pHead;
	pHead = NULL;
		
	char* pBuff = NULL;
	pBuff = new char[head.nContentLen];
	if (!pBuff)
	{
		AfxMessageBox(_T("内存不足！"));
		return;
	}
	memset(pBuff, 0, sizeof(char)*head.nContentLen);
	if (head.nContentLen != Receive(pBuff, head.nContentLen))
	{
		AfxMessageBox(_T("收到数据有误！"));
		delete pBuff;
		return;
	}
	CString strText(pBuff);
	switch (head.type)
	{
	case MSG_UPDATE:
	{
		CString strText(pBuff);
		((CchatclientDlg*)(AfxGetApp()->GetMainWnd()))->UpdateUserInfo(strText);
	}
	break;
	case MSG_SEND:
	{
		//显示接收到的消息
		CString str(pBuff);
		((CchatclientDlg*)(AfxGetApp()->GetMainWnd()))->UpdateText(str);
		break;
	}
	default: break;
	}

	delete pBuff;
	CSocket::OnReceive(nErrorCode);
}

BOOL Clientsocket::SendMSG(LPSTR lpBuff, int nlen, char to_user[20], char from_user[20])
{
	//生成协议头
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = nlen;
	strcpy_s(head.to_user, to_user);
	strcpy_s(head.from_user, from_user);

	int i = Send(&head, sizeof(HEADER));
	if (i == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！"));
		return FALSE;
	};
	if (Send(lpBuff, nlen) == SOCKET_ERROR)
	{
		AfxMessageBox(_T("发送错误！"));
		return FALSE;
	};

	return  TRUE;
}
//用户登陆
BOOL Clientsocket::LogoIn(LPSTR lpBuff, int nlen, char from_user[20])
{
	HEADER _head;
	_head.type = MSG_LOGOIN;  //头部类型
	_head.nContentLen = nlen; //长度
	memset(_head.to_user, 0, 20);
	memset(_head.from_user, 0, 20);
	strcpy_s(_head.from_user, from_user);

	//_head.to_user = "";
	int _nSnd = 0;
	if ((_nSnd = Send((char*)&_head, sizeof(_head))) == SOCKET_ERROR)  //将头部发送过去
		return false;
	if ((_nSnd = Send(lpBuff, nlen)) == SOCKET_ERROR)  //头部内存空间，和长度发送过去
		return false;

	return TRUE;
}