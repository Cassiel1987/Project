#include "stdafx.h"
#include "ClientSocket.h"
#include "ChatServer.h"
#include "Header.h"
#include "ChatServerDlg.h"

ClientSocket::ClientSocket(CPtrList* pList):m_pList(pList),m_strName(_T(""))
{
}


ClientSocket::~ClientSocket()
{
}


void ClientSocket::OnReceive(int nErrorCode)
{
	//有消息接收
	//先得到信息头
	HEADER head;    //定义客户端发送的过来的一样的结构体
	int nlen = sizeof HEADER;  //计算结构体大小
	char *pHead = NULL;  //用于接受的结构体
	pHead = new char[nlen]; //申请和结构体一样大小的内存空间
	if (!pHead)
	{
		TRACE0("CClientSocket::OnReceive 内存不足！");
		return;
	}
	memset(pHead, 0, sizeof(char)*nlen);  //初始化
	Receive(pHead, nlen);   //收到内容，并赋值到pHead中，指定接受的空间大小
							//以下是将接收大结构体进行强制转换成我们的结构体，
	head.type = ((LPHEADER)pHead)->type;
	head.nContentLen = ((LPHEADER)pHead)->nContentLen;
	//head.to_user 是char[]类型，如果不进行初始化，可能会有乱码出现
	memset(head.to_user, 0, sizeof(head.to_user));
	//讲接受的数据转换过后并赋值到head.to_user，以下同
	strcpy_s(head.to_user, ((LPHEADER)pHead)->to_user);
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy_s(head.from_user, ((LPHEADER)pHead)->from_user);


	delete pHead; //使用完毕，指针变量的清除
	pHead = NULL;

	//再次接收,这次是接受正式数据内容
	//这个就是，头部接受到的内容长度，这样能对应的申请内容空间
	pHead = new char[head.nContentLen];
	if (!pHead)
	{
		TRACE0("CClientSocket::OnRecive 内存不足！");
		return;
	}
	//这里是一个验证，防止内存错误。和申请的空间进行对比，如果接受到数据大小
	//和头部的内容大小不一样，则数据有问题，不给予接受
	if (Receive(pHead, head.nContentLen) != head.nContentLen)
	{
		AfxMessageBox(_T("接收数据有误！"));
		delete pHead;
		return;
	}

	////////////根据消息类型，处理数据,这个也是和客户端进行对应的。。下面的MSG_LOGOIN，MSG_SEND是定义好的常量，可以F12看看////////////////////
	switch (head.type)
	{
	case MSG_LOGOIN: //登陆消息

		OnLogoIN(pHead, head.nContentLen, head.from_user);
		break;
	case MSG_SEND: //发送消息
		OnMSGTranslate(pHead, head.nContentLen, head.to_user, head.from_user);
		break;
	default: break;
	}

	delete pHead;
	CSocket::OnReceive(nErrorCode);
}


//关闭连接

void ClientSocket::OnClose(int nErrorCode)
{
	CTime time;
	time = CTime::GetCurrentTime();
	CString strTime = time.Format("%Y-%m-%d  %H:%M:%S  ");
	strTime = strTime + this->m_strName + _T("  离开...\r\n");
	((CChatServerDlg*)theApp.GetMainWnd())->DisplayLog(strTime);
	m_pList->RemoveAt(m_pList->Find(this));
	//更改服务器在线名单
	CString str1 = this->UpdateServerLog();
	//通知客户端刷新在线名单
	this->UpdateAllUser(str1, "0");
	this->Close();
	//销毁该套接字
	delete this;
	CSocket::OnClose(nErrorCode);
}

//登录
void ClientSocket::OnLogoIN(char* buff, int nlen, char from_user[20])
{
	//对得接收到的用户信息进行验证
	//... （为了简化这步省略）
	//登录成功
	CTime time;
	time = CTime::GetCurrentTime();  //获取现在时间
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S  ");

	CString strTemp(buff);
	strTime = strTime + strTemp + _T("  登录...\r\n");
	//记录日志
	//将内容在NetChatServerDlg里的控件显示
	((CChatServerDlg*)theApp.GetMainWnd())->DisplayLog(strTime);
	m_strName = strTemp;
	//更新服务列表，这个是更新服务器端的在线名单 
	//str1 返回的是所有用户字符串
	CString str1 = this->UpdateServerLog();
	//更新在线所有客服端，from_user 是为了不更新自己的在线列表，
	//自己跟自己聊天没多大意思吧，其实更自己聊也问题不大，我只是为了学习，加了这么一个工程
	this->UpdateAllUser(str1, from_user);
}

//转发消息
void ClientSocket::OnMSGTranslate(char* buff, int nlen, char to_user[20], char from_user[20])
{
	//建立头部信息，准备发送
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = nlen;
	strcpy_s(head.to_user, to_user);
	strcpy_s(head.from_user, from_user);

	POSITION ps = m_pList->GetHeadPosition();  //取得，所有用户的队列
	CString str(buff);
	int i = strcmp(head.to_user, "群聊");
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps);
		//只发送2个人， 一个是发送聊天消息的人和接收聊天消息的人。
		//如果，接收聊天消息的人是“群聊”那么就发送所有用户，实现群聊和一对一关键就在于此
		if (pTemp->m_strName == head.to_user || pTemp->m_strName == head.from_user || i == 0)
		{
			pTemp->Send(&head, sizeof(HEADER));  //先发送头部
			pTemp->Send(buff, nlen);			//然后发布内容
		}
	}
}


BOOL ClientSocket::WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen)
{
	int n = 0;
	n = WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, 0, 0, FALSE);
	if (n<nlen)
		return FALSE;

	WideCharToMultiByte(CP_OEMCP, 0, srcBuff, -1, destBuff, nlen, 0, FALSE);

	return TRUE;
}

//更新所有在线用户
void ClientSocket::UpdateAllUser(CString strUserInfo, char from_user[20])
{
	HEADER _head;
	_head.type = MSG_UPDATE;
	_head.nContentLen = strUserInfo.GetLength() + 1;
	memset(_head.from_user, 0, sizeof(_head.from_user));
	strcpy_s(_head.from_user, from_user);
	char *pSend = new char[_head.nContentLen];
	memset(pSend, 0, _head.nContentLen*sizeof(char));
	//因为我用的是vs2015 所以是用unicode字符，转换的时候很多
	//网站上转换方法不行，必须用WideCharToMultiByte准换
	if (!WChar2MByte(strUserInfo.GetBuffer(0), pSend, _head.nContentLen))
	{
		AfxMessageBox(_T("字符转换失败"));
		delete pSend;
		return;
	}
	POSITION ps = m_pList->GetHeadPosition();  //循环对客户端发送消息
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps);
		//发送协议头
		pTemp->Send((char*)&_head, sizeof(_head));
		pTemp->Send(pSend, _head.nContentLen);

	}
	delete pSend;
}

//跟新服务器在线名单  
// 返回在线用户列表的String
CString ClientSocket::UpdateServerLog()
{
	CString strUserInfo = _T("");
	POSITION ps = m_pList->GetHeadPosition();  //返回的是链表头元素的位置
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps); //指向下一个元素
		strUserInfo += pTemp->m_strName + _T("#");  //每一次用#结束
	}
	((CChatServerDlg*)theApp.GetMainWnd())->UpdateUserInfo(strUserInfo);   //更新服务器显示

	return strUserInfo;
}