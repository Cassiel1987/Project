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
	//����Ϣ����
	//�ȵõ���Ϣͷ
	HEADER head;    //����ͻ��˷��͵Ĺ�����һ���Ľṹ��
	int nlen = sizeof HEADER;  //����ṹ���С
	char *pHead = NULL;  //���ڽ��ܵĽṹ��
	pHead = new char[nlen]; //����ͽṹ��һ����С���ڴ�ռ�
	if (!pHead)
	{
		TRACE0("CClientSocket::OnReceive �ڴ治�㣡");
		return;
	}
	memset(pHead, 0, sizeof(char)*nlen);  //��ʼ��
	Receive(pHead, nlen);   //�յ����ݣ�����ֵ��pHead�У�ָ�����ܵĿռ��С
							//�����ǽ����մ�ṹ�����ǿ��ת�������ǵĽṹ�壬
	head.type = ((LPHEADER)pHead)->type;
	head.nContentLen = ((LPHEADER)pHead)->nContentLen;
	//head.to_user ��char[]���ͣ���������г�ʼ�������ܻ����������
	memset(head.to_user, 0, sizeof(head.to_user));
	//�����ܵ�����ת�����󲢸�ֵ��head.to_user������ͬ
	strcpy_s(head.to_user, ((LPHEADER)pHead)->to_user);
	memset(head.from_user, 0, sizeof(head.from_user));
	strcpy_s(head.from_user, ((LPHEADER)pHead)->from_user);


	delete pHead; //ʹ����ϣ�ָ����������
	pHead = NULL;

	//�ٴν���,����ǽ�����ʽ��������
	//������ǣ�ͷ�����ܵ������ݳ��ȣ������ܶ�Ӧ���������ݿռ�
	pHead = new char[head.nContentLen];
	if (!pHead)
	{
		TRACE0("CClientSocket::OnRecive �ڴ治�㣡");
		return;
	}
	//������һ����֤����ֹ�ڴ���󡣺�����Ŀռ���жԱȣ�������ܵ����ݴ�С
	//��ͷ�������ݴ�С��һ���������������⣬���������
	if (Receive(pHead, head.nContentLen) != head.nContentLen)
	{
		AfxMessageBox(_T("������������"));
		delete pHead;
		return;
	}

	////////////������Ϣ���ͣ���������,���Ҳ�ǺͿͻ��˽��ж�Ӧ�ġ��������MSG_LOGOIN��MSG_SEND�Ƕ���õĳ���������F12����////////////////////
	switch (head.type)
	{
	case MSG_LOGOIN: //��½��Ϣ

		OnLogoIN(pHead, head.nContentLen, head.from_user);
		break;
	case MSG_SEND: //������Ϣ
		OnMSGTranslate(pHead, head.nContentLen, head.to_user, head.from_user);
		break;
	default: break;
	}

	delete pHead;
	CSocket::OnReceive(nErrorCode);
}


//�ر�����

void ClientSocket::OnClose(int nErrorCode)
{
	CTime time;
	time = CTime::GetCurrentTime();
	CString strTime = time.Format("%Y-%m-%d  %H:%M:%S  ");
	strTime = strTime + this->m_strName + _T("  �뿪...\r\n");
	((CChatServerDlg*)theApp.GetMainWnd())->DisplayLog(strTime);
	m_pList->RemoveAt(m_pList->Find(this));
	//���ķ�������������
	CString str1 = this->UpdateServerLog();
	//֪ͨ�ͻ���ˢ����������
	this->UpdateAllUser(str1, "0");
	this->Close();
	//���ٸ��׽���
	delete this;
	CSocket::OnClose(nErrorCode);
}

//��¼
void ClientSocket::OnLogoIN(char* buff, int nlen, char from_user[20])
{
	//�Եý��յ����û���Ϣ������֤
	//... ��Ϊ�˼��ⲽʡ�ԣ�
	//��¼�ɹ�
	CTime time;
	time = CTime::GetCurrentTime();  //��ȡ����ʱ��
	CString strTime = time.Format("%Y-%m-%d %H:%M:%S  ");

	CString strTemp(buff);
	strTime = strTime + strTemp + _T("  ��¼...\r\n");
	//��¼��־
	//��������NetChatServerDlg��Ŀؼ���ʾ
	((CChatServerDlg*)theApp.GetMainWnd())->DisplayLog(strTime);
	m_strName = strTemp;
	//���·����б�����Ǹ��·������˵��������� 
	//str1 ���ص��������û��ַ���
	CString str1 = this->UpdateServerLog();
	//�����������пͷ��ˣ�from_user ��Ϊ�˲������Լ��������б�
	//�Լ����Լ�����û�����˼�ɣ���ʵ���Լ���Ҳ���ⲻ����ֻ��Ϊ��ѧϰ��������ôһ������
	this->UpdateAllUser(str1, from_user);
}

//ת����Ϣ
void ClientSocket::OnMSGTranslate(char* buff, int nlen, char to_user[20], char from_user[20])
{
	//����ͷ����Ϣ��׼������
	HEADER head;
	head.type = MSG_SEND;
	head.nContentLen = nlen;
	strcpy_s(head.to_user, to_user);
	strcpy_s(head.from_user, from_user);

	POSITION ps = m_pList->GetHeadPosition();  //ȡ�ã������û��Ķ���
	CString str(buff);
	int i = strcmp(head.to_user, "Ⱥ��");
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps);
		//ֻ����2���ˣ� һ���Ƿ���������Ϣ���˺ͽ���������Ϣ���ˡ�
		//���������������Ϣ�����ǡ�Ⱥ�ġ���ô�ͷ��������û���ʵ��Ⱥ�ĺ�һ��һ�ؼ������ڴ�
		if (pTemp->m_strName == head.to_user || pTemp->m_strName == head.from_user || i == 0)
		{
			pTemp->Send(&head, sizeof(HEADER));  //�ȷ���ͷ��
			pTemp->Send(buff, nlen);			//Ȼ�󷢲�����
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

//�������������û�
void ClientSocket::UpdateAllUser(CString strUserInfo, char from_user[20])
{
	HEADER _head;
	_head.type = MSG_UPDATE;
	_head.nContentLen = strUserInfo.GetLength() + 1;
	memset(_head.from_user, 0, sizeof(_head.from_user));
	strcpy_s(_head.from_user, from_user);
	char *pSend = new char[_head.nContentLen];
	memset(pSend, 0, _head.nContentLen*sizeof(char));
	//��Ϊ���õ���vs2015 ��������unicode�ַ���ת����ʱ��ܶ�
	//��վ��ת���������У�������WideCharToMultiByte׼��
	if (!WChar2MByte(strUserInfo.GetBuffer(0), pSend, _head.nContentLen))
	{
		AfxMessageBox(_T("�ַ�ת��ʧ��"));
		delete pSend;
		return;
	}
	POSITION ps = m_pList->GetHeadPosition();  //ѭ���Կͻ��˷�����Ϣ
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps);
		//����Э��ͷ
		pTemp->Send((char*)&_head, sizeof(_head));
		pTemp->Send(pSend, _head.nContentLen);

	}
	delete pSend;
}

//���·�������������  
// ���������û��б��String
CString ClientSocket::UpdateServerLog()
{
	CString strUserInfo = _T("");
	POSITION ps = m_pList->GetHeadPosition();  //���ص�������ͷԪ�ص�λ��
	while (ps != NULL)
	{
		ClientSocket* pTemp = (ClientSocket*)m_pList->GetNext(ps); //ָ����һ��Ԫ��
		strUserInfo += pTemp->m_strName + _T("#");  //ÿһ����#����
	}
	((CChatServerDlg*)theApp.GetMainWnd())->UpdateUserInfo(strUserInfo);   //���·�������ʾ

	return strUserInfo;
}