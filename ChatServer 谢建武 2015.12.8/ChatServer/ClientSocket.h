#pragma once
#include "afxsock.h"
#include "stdafx.h"


//˵�����������ںͿͻ��˽���ͨ�ŵ�Socket

class ClientSocket : public CSocket
{
public:
	ClientSocket(CPtrList* pList);
	virtual ~ClientSocket();
public:
	CPtrList* m_pList;//���������Socket��List�Ķ���
	CString m_strName; //��������
public:
	virtual void OnClose(int nErrorCode);
	virtual void OnReceive(int nErrorCode); //�յ�
	void OnLogoIN(char* buff, int nlen, char from_user[20]);  //�����¼��Ϣ  
	void OnMSGTranslate(char* buff, int nlen, char to_user[20], char from_user[20]); //ת����Ϣ����������Ⱥ 
	CString UpdateServerLog();//�������˸��¡���¼��־ 
	void UpdateAllUser(CString strUserInfo, char from_user[20]);//���·������˵�������Ա�б�  
private:
	BOOL WChar2MByte(LPCWSTR srcBuff, LPSTR destBuff, int nlen);//���ֽڵ�ת��  
};

